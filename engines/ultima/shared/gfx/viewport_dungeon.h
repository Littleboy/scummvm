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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef ULTIMA_SHARED_GFX_VIEWPORT_DUNGEON_H
#define ULTIMA_SHARED_GFX_VIEWPORT_DUNGEON_H

#include "ultima/shared/gfx/visual_item.h"

namespace Ultima {
namespace Shared {

class ViewportDungeon : public Gfx::VisualItem {
	DECLARE_MESSAGE_MAP;
public:
	CLASSDEF;
	ViewportDungeon(TreeItem *parent) : Gfx::VisualItem("ViewportDungeon", Common::Rect(8, 8, 312, 168), parent) {}
	virtual ~ViewportDungeon() {}
};

} // End of namespace Shared
} // End of namespace Xeen

#endif