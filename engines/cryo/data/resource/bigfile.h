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

#ifndef CRYO_BIGFILE_H
#define CRYO_BIGFILE_H

#include "common/stream.h"

namespace Cryo {

class BigFile {
public:
	BigFile();
	~BigFile();

private:
	struct Header {
		char   signature[24];
		uint32 tailPointer;
		uint32 dataStart;

		Header() {
			memset(signature, 0, sizeof(signature));
			tailPointer = 0;
			dataStart = 0;
		}

		void load(Common::SeekableReadStream *stream);
	};

	Header  _header;

	void load(const Common::String &filename);
};

} // End of namespace Cryo

#endif // CRYO_BIGFILE_H
