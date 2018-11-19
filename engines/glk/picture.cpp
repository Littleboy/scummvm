/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software{} you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation{} either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program{} if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "glk/picture.h"
#include "common/file.h"
#include "image/jpeg.h"
#include "image/png.h"

namespace Glk {

void Pictures::clear() {
	for (uint idx = 0; idx < _store.size(); ++idx) {
		delete _store[idx]._picture;
		delete _store[idx]._scaled;
	}

	_store.clear();
}

void Pictures::increment() {
	++_refCount;
}

void Pictures::decrement() {
	if (_refCount > 0 && --_refCount == 0)
		clear();
}

PictureEntry *Pictures::search(uint id) {
	Picture *pic;

	for (uint idx = 0; idx < _store.size(); ++idx) {
		pic = _store[idx]._picture;

		if (pic && pic->_id == id)
			return &_store[idx];
	}

	return nullptr;
}

void Pictures::storeOriginal(Picture *pic) {
	PictureEntry newPic;
	newPic._picture = pic;

	_store.push_back(newPic);
}

void Pictures::storeScaled(Picture *pic) {
	PictureEntry *entry = search(pic->_id);
	if (!entry)
		return;

	delete entry->_scaled;
	entry->_scaled = pic;
}

void Pictures::store(Picture *pic) {
	if (!pic)
		return;

	if (!pic->_scaled)
		storeOriginal(pic);
	else
		storeScaled(pic);
}

Picture *Pictures::retrieve(uint id, bool scaled) {
	Picture *pic;

	for (uint idx = 0; idx < _store.size(); ++idx) {
		pic = scaled ? _store[idx]._scaled : _store[idx]._picture;

		if (pic && pic->_id == id)
			return pic;
	}

	return nullptr;
}

Picture *Pictures::load(uint32 id) {
	::Image::PNGDecoder png;
	::Image::JPEGDecoder jpg;
	const Graphics::Surface *img;
	Picture *pic;

	// Check if the picture is already in the store
    pic = retrieve(id, false);
    if (pic)
        return pic;

	Common::File f;
	if (f.open(Common::String::format("PIC%lu.png", id))) {
		png.loadStream(f);
		img = png.getSurface();
	} else if (f.open(Common::String::format("PIC%lu.jpg", id))) {
		jpg.loadStream(f);
		img = jpg.getSurface();
	}

	pic = new Picture();
	pic->_refCount = 1;
    pic->_id = id;
    pic->_scaled = false;

    store(pic);
    return pic;
}

/*--------------------------------------------------------------------------*/

void Picture::increment() {
	++_refCount;
}

void Picture::decrement() {
	if (_refCount > 0 && --_refCount == 0) {
		// No longer any references to this picture, so remove it
		delete this;
	}
}

Picture *Picture::scale(int sx, int sy) {
	// TODO: gli_picture_scale
	return nullptr;
}

void Picture::drawPicture(int x0, int y0, int dx0, int dy0, int dx1, int dy1) {
	// TODO: drawPicture
}


} // End of namespace Glk
