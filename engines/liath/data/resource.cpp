/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "liath/data/resource.h"

#include "liath/data/archive.h"

#include "liath/debug.h"

#include "common/archive.h"
#include "common/file.h"

namespace Liath {

ResourceManager::ResourceManager() {
	_archiveCache = new ArchiveMap();
}

ResourceManager::~ResourceManager() {
	for (ArchiveMap::iterator it = _archiveCache->begin(); it != _archiveCache->end(); it++)
		delete it->_value;

	_archiveCache->clear();
	delete _archiveCache;

	_fileMap.clear();
}

//////////////////////////////////////////////////////////////////////////
// Archive
//////////////////////////////////////////////////////////////////////////

bool ResourceManager::hasFile(const Common::String &name) const {
	// Files can be either part of a MUL archive or in one of the data folders

	if (_fileMap.find(name) != _fileMap.end())
		return true;

	return SearchMan.hasFile(name);
}

int ResourceManager::listMembers(Common::ArchiveMemberList &list) const {
	int numMembers = 0;

	for (FileMap::const_iterator i = _fileMap.begin(); i != _fileMap.end(); ++i) {
		list.push_back(Common::ArchiveMemberList::value_type(new Common::GenericArchiveMember(i->_key, this)));
		numMembers++;
	}

	Common::ArchiveMemberList files;
	numMembers += SearchMan.listMembers(files);

	for (Common::ArchiveMemberList::iterator i = files.begin(); i != files.end(); ++i) {
		list.push_back(Common::ArchiveMemberList::value_type(new Common::GenericArchiveMember((*i)->getName(), this)));
	}

	return numMembers;
}

const Common::ArchiveMemberPtr ResourceManager::getMember(const Common::String &name) const {
	if (!hasFile(name))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(name, this));
}

Common::SeekableReadStream *ResourceManager::createReadStreamForMember(const Common::String &name) const {
	// Load a normal file
	//  - if the archive name is empty, also try to open the file directly
	if (_fileMap.find(name) == _fileMap.end() || _fileMap[name].archiveName.empty()) {
		Common::File *file = new Common::File();
		if (!file->open(name)) {
			delete file;
			return nullptr;
		}

		return file;
	}

	// Get the archive file from the cache
	Common::String archiveName = _fileMap[name].archiveName;
	if (!_archiveCache->contains(archiveName))
		_archiveCache->setVal(archiveName, (Common::Archive *)(new MultiArchive(archiveName)));

	// Load the file from the archive
	Common::Archive *archive = _archiveCache->getVal(archiveName);
	if (!archive->hasFile(name))
		return nullptr;

	Common::SeekableReadStream *stream = archive->createReadStreamForMember(name);

	return stream;
}

//////////////////////////////////////////////////////////////////////////
// Path data
//////////////////////////////////////////////////////////////////////////

/**
 * Reads the path file and initialize the path list
 *
 * Note: Using ConfigFile cannot work here, as it expects key/value pairs
 */
void ResourceManager::readPathFile() {
	// Open a stream to the path file
	Common::File *pathFile = new Common::File();
	if (!pathFile->open("liath.ini"))
		error("Could not open config file (liath.ini)");

	bool readingPaths = false;

	while (!pathFile->eos() && !pathFile->err()) {

		// Read a line
		Common::String line = pathFile->readLine();
		line.trim();

		if (line.size() == 0) {
			// Do nothing
		} else if (line.c_str()[0] == 26) { // There is a single SUB character at the end of the file
			// Do nothing
		} else if (line[0] == '[') {
			// Check if this is a Path section, and start reading the list of path if true
			readingPaths = (line == "[Path]" ? true : false);
		} else if (readingPaths) {
			// We are inside a Path section, separate elements and store into a FilePath object
			const char *t = line.c_str();

			// Split string at ' ' into 'name' and 'full path'. First, find the " " delimiter.
			const char *sep = strchr(t, ' ');
			if (!sep)
				error("ResourceManager::readPathFile: path file invalid: no valid separator between paths");

			// Now extract components from the full path
			Common::String fullpath(sep + 1);
			fullpath.trim();

			// Split string at ':' into 'media type' and 'directory'
			const char *p = fullpath.c_str();
			const char *sep2 = strchr(p, ':');

			// Extract the name/path pair
			FilePath path;
			Common::String filename(t, sep);
			filename.toLowercase();
			filename.trim();

			if (sep2 != nullptr) {
				path.folderName = Common::String(sep2 + 1);
				path.folderName.trim();
			}

			// Convert media type
			Common::String media(p, sep2);
			if (media == "cd")
				path.type = kMediaCd;
			else if (media == "hd")
				path.type = kMediaHd;
			else
				error("ResourceManager::readPathFile: Invalid media type (was:%s)", media.c_str());

			_fileMap[filename] = path;
		}
	}

	debugC(2, kLiathDebugResource, "Loaded %d file paths", _fileMap.size());

	delete pathFile;
}

/**
 * Reads the multigen.dat file. This contains the list of associations between archives and files
 */
void ResourceManager::readMultiData() {
	// Open a stream to the path file
	Common::File *multigenFile = new Common::File();
	if (!multigenFile->open("multigen.dat"))
		error("ResourceManager::readMultiData: Could not open multigen.dat file!");

	uint32 count = 0;

	while (!multigenFile->eos() && !multigenFile->err()) {

		char name[20];
		multigenFile->read(&name, sizeof(name));

		Common::String filename(name);
		filename.toLowercase();
		filename.trim();

		// Search for existing FilePath and update archive
		if (_fileMap.contains(filename)) {
			// Found an existing file with that name
			char archive[20];
			multigenFile->read(&archive, sizeof(archive));

			_fileMap[name].archiveName = Common::String(archive);
			_fileMap[name].archiveName.trim();
			_fileMap[name].cd = (CdNumber)multigenFile->readUint16LE();

			//debugC(kLiathDebugResource, "%s - %s", filename.c_str(), _files[name].archiveName.c_str());

			count++;
		}
	}

	debugC(2, kLiathDebugResource, "Updated %d file paths with archive name", count);

	delete multigenFile;
}

CdNumber ResourceManager::getCd(const Common::String &filename) {
	if (!_fileMap.contains(filename))
		return kCdNone;

	return _fileMap[filename].cd;
}

} // End of namespace Liath
