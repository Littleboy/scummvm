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

#include "liath/data/archive.h"

#include "liath/debug.h"

#include "common/debug.h"
#include "common/file.h"
#include "common/substream.h"

namespace Liath {

MultiArchive::MultiArchive(const Common::String &path) {
	_filename = path;

	// Open a stream to the archive
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember(_filename);
	if (!archive) {
		debugC(2, kLiathDebugResource, "Error opening file: %s", path.c_str());
		return;
	}

	debugC(2, kLiathDebugResource, "Opened archive: %s", path.c_str());

	// Skip signature, compiled file name and other data
	archive->seek(336, SEEK_SET);

	// TODO what is that field for?
	uint32 arg = archive->readUint32LE();
	if (arg >= 500)
		return;

	// Go to start of file entries
	archive->seek(4, SEEK_CUR);

	do {
		char name[20];
		FileEntry entry;

		archive->read(&name, sizeof(char) * 20);

		Common::String filename(name);
		filename.trim();

		if (filename == "")
			break;

		archive->seek(12, SEEK_CUR);
		entry.size = archive->readUint32LE();
		archive->seek(4, SEEK_CUR);
		entry.offset = archive->readUint32LE();
		archive->seek(8, SEEK_CUR);

		// Add to file map
		_files[filename] = entry;

		if (_files.size() >= 500) {
			debugC(2, kLiathDebugResource, "ERROR: MultiArchive: read behind the end of file map (%s)!", path.c_str());
			break;
		}
	} while (!archive->eos() && !archive->err());

	// Close stream
	delete archive;
}

bool MultiArchive::hasFile(const Common::String &name) const {
	return (_files.find(name) != _files.end());
}

int MultiArchive::listMembers(Common::ArchiveMemberList &list) const {
	int numMembers = 0;

	for (FileMap::const_iterator i = _files.begin(); i != _files.end(); ++i) {
		list.push_back(Common::ArchiveMemberList::value_type(new Common::GenericArchiveMember(i->_key, this)));
		numMembers++;
	}

	return numMembers;
}

const Common::ArchiveMemberPtr MultiArchive::getMember(const Common::String &name) const {
	if (!hasFile(name))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(name, this));
}

Common::SeekableReadStream *MultiArchive::createReadStreamForMember(const Common::String &name) const {
	FileMap::const_iterator fDesc = _files.find(name);
	if (fDesc == _files.end())
		return nullptr;

	Common::File *archive = new Common::File();
	if (!archive->open(_filename)) {
		delete archive;
		return nullptr;
	}

	return new Common::SeekableSubReadStream(archive, fDesc->_value.offset, fDesc->_value.offset + fDesc->_value.size, DisposeAfterUse::YES);
}

} // End of namespace Liath
