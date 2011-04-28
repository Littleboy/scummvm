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

#if defined(USE_UPDATES)

#include "backends/updates/default-updates.h"

#include "base/version.h"

Common::String UpdateManager::getAppcastUrl() {
	// TODO replace by proper URL

	Common::String url = "http://littleboy.github.com/appcasts/scummvm/release.xml";

	// Check if running beta or rc version and adjust appcast url
	if (Common::String(gScummVMVersion).contains("git"))
		url = "http://littleboy.github.com/appcasts/scummvm/beta.xml";

	return url;
}

#endif
