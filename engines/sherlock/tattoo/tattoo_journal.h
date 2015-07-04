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

#ifndef SHERLOCK_TATTOO_JOURNAL_H
#define SHERLOCK_TATTOO_JOURNAL_H

#include "sherlock/journal.h"
#include "sherlock/image_file.h"

namespace Sherlock {

namespace Tattoo {

class TattooJournal : public Journal {
private:
	ImageFile *_journalImages;
	int _selector, _oldSelector;
	bool _wait;
	bool _exitJournal;
	uint32 _scrollingTimer;
	int _savedIndex, _savedSub, _savedPage;

	/**
	 * Load the list of journal locations
	 */
	void loadLocations();

	/**
	 * Displays the controls used by the journal
	 * @param mode	0: Normal journal buttons, 1: Search interface
	 */
	void drawControls(int mode);

	/**
	 * Draw the journal controls used by the journal
	 */
	void highlightJournalControls(bool slamIt);

	/**
	 * Draw the journal controls used in search mode
	 */
	void highlightSearchControls(bool slamIt);

	void drawScrollBar();

	/**
	 * Check for and handle any pending keyboard events
	 */
	void handleKeyboardEvents();

	/**
	 * Handle mouse presses on interface buttons
	 */
	void handleButtons();

	/**
	 * Disable the journal controls
	 */
	void disableControls();

	/**
	 * Get in a name to search through the journal for
	 */
	int getFindName(bool printError);
public:
	TattooJournal(SherlockEngine *vm);
	virtual ~TattooJournal() {}

	/**
	 * Show the journal
	 */
	void show();
public:
	/**
	 * Draw the journal background, frame, and interface buttons
	 */
	virtual void drawFrame();

	/**
	 * Synchronize the data for a savegame
	 */
	virtual void synchronize(Serializer &s);
};

} // End of namespace Tattoo

} // End of namespace Sherlock

#endif
