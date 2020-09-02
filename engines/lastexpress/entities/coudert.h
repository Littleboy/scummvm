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

#ifndef LASTEXPRESS_COUDERT_H
#define LASTEXPRESS_COUDERT_H

#include "lastexpress/entities/entity.h"
namespace LastExpress {

class LastExpressEngine;

class Coudert : public Entity {
public:
	Coudert(LastExpressEngine *engine);
	~Coudert() override {}

	/**
	 * Resets the entity
	 */
	DECLARE_FUNCTION(reset)

	/**
	 * Handle meeting Coudert with the blooded jacket
	 *
	 * @param sequence The sequence to draw
	 */
	DECLARE_FUNCTION_1(bloodJacket, const char *sequence)

	/**
	 * Handles entering/exiting a compartment.
	 *
	 * @param sequence    The sequence to draw
	 * @param compartment The compartment
	 */
	DECLARE_VFUNCTION_2(enterExitCompartment, const char *sequence, ObjectIndex compartment)

	/**
	 * Process callback action when the entity direction is not kDirectionRight
	 */
	DECLARE_FUNCTION(callbackActionOnDirection)

	/**
	* Handles entering/exiting a compartment.
	*
	* @param sequence        The sequence to draw
	* @param compartment     The compartment
	* @param entityPosition1 The entity position 1
	* @param entityPosition2 The entity position 2
	*/
	DECLARE_FUNCTION_4(enterExitCompartment2, const char *sequence, ObjectIndex compartment, EntityPosition entityPosition1, EntityPosition entityPosition2)

	/**
	 * Plays sound
	 *
	 * @param filename The sound filename
	 */
	DECLARE_VFUNCTION_1(playSound, const char *filename)

	/**
	 * Plays sound
	 *
	 * @param savepoint The savepoint
	 *                    - the sound filename
	 */
	DECLARE_FUNCTION_NOSETUP(playSound16)

	/**
	 * Saves the game
	 *
	 * @param savegameType The type of the savegame
	 * @param param        The param for the savegame (EventIndex or TimeValue)
	 */
	DECLARE_VFUNCTION_2(savegame, SavegameType savegameType, uint32 param)

	/**
	 * Updates the entity
	 *
	 * @param car            The car
	 * @param entityPosition The entity position
	 */
	DECLARE_VFUNCTION_2(updateEntity, CarIndex car, EntityPosition entityPosition)

	/**
	 * Updates parameter 2 using time value
	 *
	 * @param time The time to add
	 */
	DECLARE_FUNCTION_1(updateFromTime, uint32 time)

	/**
	 * Updates parameter 2 using ticks value
	 *
	 * @param ticks The number of ticks to add
	 */
	DECLARE_FUNCTION_1(updateFromTicks, uint32 ticks)

	DECLARE_FUNCTION_1(excuseMe, EntityIndex entity)
	DECLARE_FUNCTION_2(passing, bool, EntityIndex entity)
	DECLARE_FUNCTION_1(listen, EntityIndex entity)
	DECLARE_FUNCTION_1(tatianaLockUnlockMyCompartment, bool)
	DECLARE_FUNCTION(getUp)
	DECLARE_FUNCTION_1(getUpListen, bool)
	DECLARE_FUNCTION(sitDown)
	DECLARE_FUNCTION_1(sitDownFast, bool)
	DECLARE_FUNCTION_2(makeBed, ObjectIndex object1, ObjectIndex object2)
	DECLARE_FUNCTION(makeBedIvo)
	DECLARE_FUNCTION(makeBedMilos)
	DECLARE_FUNCTION(tryMakeAnna)
	DECLARE_FUNCTION(makeBedAnna)
	DECLARE_FUNCTION(makeBedRebecca)
	DECLARE_FUNCTION(makeBedMadame)
	DECLARE_FUNCTION(makeBedMonsieur)
	DECLARE_FUNCTION(makeBedTatiana)
	DECLARE_FUNCTION(makeBedVassili)
	DECLARE_FUNCTION_1(cathBuzzing, ObjectIndex compartment)
	DECLARE_FUNCTION_1(cathRattling, uint32)
	DECLARE_FUNCTION(bathroomTrip)
	DECLARE_FUNCTION(doPending)
	DECLARE_FUNCTION_1(takeMaxBack, bool)
	DECLARE_FUNCTION_1(returnMax, bool)

	/**
	 * Setup Chapter 1
	 */
	DECLARE_VFUNCTION(chapter1)
    
	DECLARE_FUNCTION(respondVassili)
	DECLARE_FUNCTION(atSeizure)
	DECLARE_FUNCTION(afterPolice)
	DECLARE_FUNCTION(sittingChapter1)
	DECLARE_FUNCTION(makeAllBeds)

	/**
	 * Setup Chapter 2
	 */
	DECLARE_VFUNCTION(chapter2)

	DECLARE_FUNCTION(sittingChapter2)

	/**
	 * Setup Chapter 3
	 */
	DECLARE_VFUNCTION(chapter3)

	DECLARE_FUNCTION(sittingChapter3)
	DECLARE_FUNCTION(annaTakeMax)
	DECLARE_FUNCTION_1(annaLockMe, bool)
	DECLARE_FUNCTION(annaUnlockMe)
	DECLARE_FUNCTION(inviteTatiana)
	DECLARE_FUNCTION(inviteRebecca)
	DECLARE_FUNCTION(tatianaComeHere)

	/**
	 * Setup Chapter 4
	 */
	DECLARE_VFUNCTION(chapter4)

	DECLARE_FUNCTION(onDuty4)
	DECLARE_FUNCTION(waitBombDefused)
	DECLARE_FUNCTION(serviceAnna)
	DECLARE_FUNCTION(makeAllBedsChapter4)

	/**
	 * Setup Chapter 5
	 */
	DECLARE_VFUNCTION(chapter5)

	DECLARE_FUNCTION(chapter5Handler)
	DECLARE_FUNCTION(returnSeat)
	DECLARE_FUNCTION(waitingChapter5)
	DECLARE_FUNCTION(makeRounds)
	DECLARE_FUNCTION(inCompartmentH)

	DECLARE_NULL_FUNCTION()

private:
	void visitCompartment(const SavePoint &savepoint, EntityPosition position, const char *seq1, ObjectIndex compartment, const char *seq2, const char *seq3, EntityPosition sittingPosition, ObjectIndex object, const char *seq4);
};

} // End of namespace LastExpress

#endif // LASTEXPRESS_COUDERT_H
