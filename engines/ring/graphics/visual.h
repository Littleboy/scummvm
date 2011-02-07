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

#ifndef RING_VISUAL_H
#define RING_VISUAL_H

#include "ring/shared.h"

namespace Ring {

class Hotspot;
class ImageHandle;
class Object;
class Text;

class Visual : public BaseObject {
public:
	Visual(Id id) : BaseObject(id) {}
	~Visual() {}
};

class VisualElement : public Visual {
public:
	VisualElement(Id id);
	~VisualElement();

	void init(uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8);

	// Accessors
	void setField8(uint32 val) { _field_8 = val; }
	void setFieldC(uint32 val) { _field_C = val; }

private:
	uint32 _field_8;
	byte _field_C;
	uint32 _field_D;
	uint32 _field_11;
	uint32 _field_15;
	uint32 _field_19;
	uint32 _field_1D;
	uint32 _field_21;
	uint32 _field_25;
	uint32 _field_29;
	byte _field_2D;
	uint32 _field_2E;
	uint32 _field_32;
	uint32 _field_36;
	uint32 _field_3A;
};

class VisualObjectList : public Visual {
public:
	VisualObjectList(Id id);
	~VisualObjectList();

	// Init
	void init(uint32 a1, Common::String imagePath, Common::String iconPath, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10, Common::String filename11, Common::String filename12, Common::String filename13, uint32 a15, ArchiveType archiveType);
	void sub_46DCF0(uint32 a1, uint32 a2);
	void sub_46DD11(uint32 a1, uint32 a2);
	void sub_46DD30(uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6);
	void sub_46DD60(uint32 a1, uint32 a2);
	void sub_46DD80(uint32 a1, uint32 a2);
	void sub_46DDA0(uint32 a1, uint32 a2, uint32 a3, uint32 a4);
	void sub_46DDD0(uint32 a1, uint32 a2, uint32 a3, uint32 a4);
	void sub_46DE00(uint32 a1, uint32 a2, uint32 a3, uint32 a4);
	void sub_46DE30(uint32 a1, uint32 a2);
	void initHotspots();
	void sub_46E330(uint32 a1);
	void sub_46E340(uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6);
	void sub_46E3F0(int32 a1, int32 a2, int32 a3);
	void setFontId(FontId fontId);

	// Management
	void add(ObjectId objectId);
	void remove(ObjectId objectId, bool removeObject);
	void removeAll(bool removeObject);

	// Accessors
	void setField8(uint32 val) { _field_8 = val; }
	void setFieldC(uint32 val) { _field_C = val; }
	uint32 getItemCount() { return _itemCount; }

private:
	uint32 _field_8;
	uint32 _field_C;
	Common::String _iconPath;
	AssociativeArray<Object *> _objects;
	Common::Array<uint32 *> _field_15;
	Common::Array<Hotspot *> _hotspots;
	ImageHandle *_backgroundImage;
	ImageHandle *_upGun;
	ImageHandle *_upGur;
	ImageHandle *_upGus;
	ImageHandle *_upGua;
	ImageHandle *_downGun;
	ImageHandle *_downGur;
	ImageHandle *_downGus;
	ImageHandle *_downGua;
	ImageHandle *_cliImageP;
	ImageHandle *_cliImageA;
	uint32 _field_49;
	uint32 _field_4D;
	uint32 _field_51;
	uint32 _field_55;
	uint32 _field_59;
	uint32 _field_5D;
	uint32 _field_61;
	uint32 _field_65;
	uint32 _field_69;
	uint32 _field_6D;
	uint32 _field_71;
	uint32 _field_75;
	uint32 _field_79;
	uint32 _field_7D;
	uint32 _field_81;
	uint32 _field_85;
	uint32 _field_89;
	uint32 _field_8D;
	uint32 _field_91;
	uint32 _field_95;
	uint32 _field_99;
	uint32 _field_9D;
	uint32 _field_A1;
	uint32 _field_A5;
	uint32 _field_A9;
	uint32 _field_AD;
	uint32 _field_B1;
	uint32 _field_B5;
	uint32 _field_B9;
	uint32 _field_BD;
	uint32 _field_C1;
	uint32 _itemCount;
	uint32 _field_C9;
	int32 _field_CD;
	int32 _field_D1;
	ArchiveType _archiveType;
	Text *_text1;
	Text *_text2;
	FontId _fontId;
	uint32 _field_E2;
	uint32 _field_E6;
	uint32 _field_EA;
	uint32 _field_EE;
	uint32 _field_F2;
	uint32 _field_F6;
	uint32 _field_FA;
	uint32 _field_FE;
	uint32 _field_102;
	uint32 _field_106;
};

} // End of namespace Ring

#endif // RING_VISUAL_H
