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

#include "cryo/debug.h"

#include "cryo/data/graphics/sprite.h"

#include "common/debug-channels.h"

namespace Cryo {

Debugger::Debugger(CryoEngine *engine) : _engine(engine) {

	//////////////////////////////////////////////////////////////////////////
	// Register the debugger commands

	// General
	DCmd_Register("help",      WRAP_METHOD(Debugger, cmdHelp));

	DCmd_Register("sprite",    WRAP_METHOD(Debugger, cmdSprite));
}

Debugger::~Debugger() {
	DebugMan.clearAllDebugChannels();

	// Zero passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Debugger commands
//////////////////////////////////////////////////////////////////////////
bool Debugger::cmdHelp(int, const char **) {
	DebugPrintf("Debug flags\n");
	DebugPrintf("-----------\n");
	DebugPrintf(" debugflag_list - Lists the available debug flags and their status\n");
	DebugPrintf(" debugflag_enable - Enables a debug flag\n");
	DebugPrintf(" debugflag_disable - Disables a debug flag\n");
	DebugPrintf("\n");
	DebugPrintf("Commands\n");
	DebugPrintf("--------\n");
	DebugPrintf("list - List files inside an archive\n");
	DebugPrintf("music - Play music\n");
	DebugPrintf("sound - Play sound\n");
	DebugPrintf("sprite - Show a sprite\n");
	DebugPrintf("\n");
	return true;
}

bool Debugger::cmdSprite(int argc, const char **argv) {
	if (argc == 2) {

		// Get sprite name
		Common::String name(const_cast<char *>(argv[1]));
		if (!name.contains('.'))
			name += ".SPR";

		Sprite *sprite = new Sprite(name);
		//sprite->draw();

		delete sprite;

	} else {
		DebugPrintf("Syntax: sprite <filename>\n");
	}
	return true;
}

} // End of namespace Cryo
