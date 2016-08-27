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

#ifndef TITANIC_PHONOGRAPH_CYLINDER_H
#define TITANIC_PHONOGRAPH_CYLINDER_H

#include "titanic/carry/carry.h"

namespace Titanic {

class CPhonographCylinder : public CCarry {
	DECLARE_MESSAGE_MAP;
	bool UseWithOtherMsg(CUseWithOtherMsg *msg);
	bool QueryCylinderMsg(CQueryCylinderMsg *msg);
	bool RecordOntoCylinderMsg(CRecordOntoCylinderMsg *msg);
	bool SetMusicControlsMsg(CSetMusicControlsMsg *msg);
	bool ErasePhonographCylinderMsg(CErasePhonographCylinderMsg *msg);
private:
	CString _itemName;
	bool _bellsMuteControl;
	bool _bellsPitchControl;
	bool _bellsSpeedControl;
	bool _bellsDirectionControl;
	bool _bellsInversionControl;
	bool _snakeMuteControl;
	bool _snakeSpeedControl;
	bool _snakePitchControl;
	bool _snakeInversionControl;
	bool _snakeDirectionControl;
	bool _pianoMuteControl;
	bool _pianoSpeedControl;
	bool _pianoPitchControl;
	bool _pianoInversionControl;
	bool _pianoDirectionControl;
	bool _bassMuteControl;
	bool _bassSpeedControl;
	bool _bassPitchControl;
	bool _bassInversionControl;
	bool _bassDirectionControl;
public:
	CLASSDEF;
	CPhonographCylinder();

	/**
	 * Save the data for the class to file
	 */
	virtual void save(SimpleFile *file, int indent);

	/**
	 * Load the data for the class from file
	 */
	virtual void load(SimpleFile *file);
};

} // End of namespace Titanic

#endif /* TITANIC_PHONOGRAPH_CYLINDER_H */
