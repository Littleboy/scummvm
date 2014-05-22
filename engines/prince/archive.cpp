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
 *
 */

#include "prince/archive.h"
#include "prince/decompress.h"

#include "common/stream.h"
#include "common/debug.h"
#include "common/memstream.h"

namespace Prince {

PtcArchive::PtcArchive() : _stream(NULL) {
}

PtcArchive::~PtcArchive() {
	close();
}

static void decrypt(byte *buffer, uint32 size) {
	uint32 key = 0xDEADF00D;
	while (size--) {
		*buffer++ += key & 0xFF;
		key ^= 0x2E84299A;
		key += 0x424C4148;
		key = ((key & 1) << 31) | (key >> 1);
	}
}

bool PtcArchive::open(const Common::String &filename) {
	_stream = SearchMan.createReadStreamForMember(filename);
	if (!_stream)
		return false;

	_stream->readUint32LE(); // magic
	uint32 fileTableOffset = _stream->readUint32LE() ^ 0x4D4F4B2D; // MOK-
	uint32 fileTableSize = _stream->readUint32LE() ^ 0x534F4654; // SOFT

	//debug("fileTableOffset : %08X", fileTableOffset);
	//debug("fileTableSize: %08X", fileTableSize);

	_stream->seek(fileTableOffset);

	byte *fileTable = new byte[fileTableSize];
	byte *fileTableEnd = fileTable + fileTableSize;
	_stream->read(fileTable, fileTableSize);
	decrypt(fileTable, fileTableSize);
	
	for (byte *fileItem = fileTable; fileItem < fileTableEnd; fileItem += 32) {
		FileEntry item;
		Common::String name = (const char*)fileItem;
		item._offset = READ_LE_UINT32(fileItem + 24);
		item._size = READ_LE_UINT32(fileItem + 28);
		//debug("%12s %8X %d", name.c_str(), item._offset, item._size);
		_items[name] = item;
	}
	
	delete[] fileTable;

	return true;
}

void PtcArchive::close() {
	delete _stream;
	_stream = NULL;
	_items.clear();
}

bool PtcArchive::hasFile(const Common::String &name) const {
	// TODO: check if path matching should be added
	return _items.contains(name);
}

int PtcArchive::listMembers(Common::ArchiveMemberList &list) const {
	int matches = 0;

	for (FileMap::const_iterator it = _items.begin(); it != _items.end(); ++it) {
		list.push_back(Common::ArchiveMemberList::value_type(new Common::GenericArchiveMember(it->_key, this)));
		matches++;
	}

	return matches;
}

const Common::ArchiveMemberPtr PtcArchive::getMember(const Common::String &name) const {
	if (!_items.contains(name)) {
		Common::ArchiveMemberPtr();
	}
	return Common::ArchiveMemberList::value_type(new Common::GenericArchiveMember(name, this));
}

Common::SeekableReadStream *PtcArchive::createReadStreamForMember(const Common::String &name) const {
	if (!_items.contains(name)) {
		return 0;
	}

	const FileEntry &entryHeader = _items[name];

	if (entryHeader._size < 4)
		return 0;

	uint32 size = entryHeader._size;

	_stream->seek(entryHeader._offset);

	// This *HAS* to be malloc (not new[]) because MemoryReadStream uses free() to free the memory
	byte* buffer = (byte *)malloc(size);
	_stream->read(buffer, size);

	if (READ_BE_UINT32(buffer) == 0x4D41534D) {
		Decompressor dec;
		uint32 decompLen = READ_BE_UINT32(buffer + 14);
		byte *decompData = (byte*)malloc(decompLen);
		dec.decompress(buffer + 18, decompData, decompLen);
		free(buffer);
		size = decompLen;
		buffer = decompData;
	}

	//debug("PtcArchive::createReadStreamForMember name %s", name.c_str());

	return new Common::MemoryReadStream(buffer, size, DisposeAfterUse::YES);
}

}

/* vim: set tabstop=4 noexpandtab: */
