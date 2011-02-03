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
 * $URL$
 * $Id$
 *
 */

#ifndef RING_SOUND_H
#define RING_SOUND_H

#include "ring/shared.h"

namespace Ring {

class Application;


//////////////////////////////////////////////////////////////////////////
// Engine sound
//////////////////////////////////////////////////////////////////////////
class SoundEntry : public BaseObject {
public:
	SoundEntry(Id soundId, SoundType type, Common::String name, byte a5);
	~SoundEntry();

	SoundType getType() { return _type; }

	void setField125(uint32 val) { _field_125 = val; }

private:
	SoundType      _type;
	Common::String _name;
	uint32         _field_10C;
	byte           _field_110;
	uint32         _field_111;
	uint32         _field_115;
	uint32         _field_119;
	uint32         _field_11D;
	LoadFrom       _loadFrom;
	uint32         _field_125;
};


class SoundManager {
public:
	SoundManager(Application *application);
	~SoundManager();

	SoundEntry *getSoundEntry(Id soundId);

private:
	Application *_application;

	AssociativeArray<SoundEntry *> _entries;
};

//////////////////////////////////////////////////////////////////////////
// Game classes
//////////////////////////////////////////////////////////////////////////
class SoundItem : public BaseObject {
public:
	SoundItem(Id id);
	~SoundItem();

	void init(SoundEntry *entry, uint32 a2, uint32 a3, byte a4, uint32 fadeFrames, uint32 a6, uint32 a7);
    void init(SoundEntry *entry, uint32 a2, uint32 a3, byte a4, uint32 a5, uint32 a6, uint32 fadeFrames, float a8, int a9);

	void setField1D(int32 val);
	void setAngle(float angle);

private:
	SoundEntry *_entry;
	uint32      _field_8;
	uint32      _field_C;
	uint32      _field_10;
	uint32      _field_14;
	byte        _field_18;
	uint32      _field_19;
	uint32      _field_1D;
	float       _angle;
	uint32      _field_25;
	uint32      _field_29;
	uint32      _field_2D;
	uint32      _field_31;
	uint32      _field_35;
	uint32      _field_39;
	uint32      _field_3D;
};

class SoundHandler {
public:
	SoundHandler();
	~SoundHandler();

	void setReverseStereo(int32 reverseStereo);
	void reset();

	float getDirection() { return _direction; }

private:
	// Data
	byte _field_0;
	uint32 _field_1;
	uint32 _field_5;
	Common::Array<SoundItem *> _soundItems1;
	Common::Array<SoundItem *> _soundItems2;
	Common::Array<SoundItem *> _soundItems3;
	Common::Array<SoundItem *> _soundItems4;

	// Sound direction? (-1.0 or 1.0)
	float _direction;
};

} // End of namespace Ring

#endif // RING_SOUND_H
