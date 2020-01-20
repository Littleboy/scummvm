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

#include "ultima/ultima1/u1dialogs/grocery.h"
#include "ultima/ultima1/game.h"
#include "ultima/ultima1/core/resources.h"
#include "ultima/shared/engine/messages.h"

namespace Ultima {
namespace Ultima1 {
namespace U1Dialogs {

EMPTY_MESSAGE_MAP(Grocery, BuySellDialog);

Grocery::Grocery(Ultima1Game *game, BuySell buySell, int groceryNum, uint costPerPack) :
		BuySellDialog(game, buySell, game->_res->GROCERY_NAMES[groceryNum]) {
	assert(buySell == SELL || costPerPack > 0);
}

void Grocery::draw() {
	BuySellDialog::draw();
	Shared::Gfx::VisualSurface s = getSurface();
	Ultima1Game *game = getGame();

	switch (_buySell) {
	case BUY:
		s.writeString(game->_res->GROCERY_PACKS1, TextPoint(5, 5));
		s.writeString(game->_res->GROCERY_PACKS2, TextPoint(5, 5));
		s.writeString(game->_res->GROCERY_PACKS3, TextPoint(5, 5));
		break;

	case SELL:
		s.writeString(game->_res->GROCERY_SELL, TextPoint(5, 5));
		break;

	default:
		break;
	}
}

} // End of namespace U1Dialogs
} // End of namespace Ultima1
} // End of namespace Ultima