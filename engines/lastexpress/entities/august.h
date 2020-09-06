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

#ifndef LASTEXPRESS_AUGUST_H
#define LASTEXPRESS_AUGUST_H

#include "lastexpress/entities/entity.h"

namespace LastExpress {

class LastExpressEngine;

class August : public Entity {
public:
	August(LastExpressEngine *engine);
	~August() override {}

	/**
	 * Resets the entity
	 */
	DECLARE_FUNCTION(reset)

	/**
	 * Updates parameter 2 using time value
	 *
	 * @param time The time to add
	 */
	DECLARE_FUNCTION_1(updateFromTime, uint32 time)

	/**
	 * Draws the entity
	 *
	 * @param sequence The sequence to draw
	 */
	DECLARE_FUNCTION_1(draw, const char *sequence)

	/**
	 * Updates the position
	 *
	 * @param sequence1      The sequence to draw
	 * @param car            The car
	 * @param position       The position
	 */
	DECLARE_FUNCTION_3(updatePosition, const char *sequence1, CarIndex car, Position position)

	/**
	 * Handles entering/exiting a compartment.
	 *
	 * @param sequence    The sequence to draw
	 * @param compartment The compartment
	 */
	DECLARE_VFUNCTION_2(enterExitCompartment, const char *sequence, ObjectIndex compartment)

	/**
	 * Handles entering/exiting a compartment and updates position/play animation
	 *
	 * @param sequence    The sequence to draw
	 * @param compartment The compartment
	 */
	DECLARE_FUNCTION_2(enterExitCompartment2, const char *sequence, ObjectIndex compartment)

	/**
	 * Handles entering/exiting a compartment.
	 *
	 * @param sequence    The sequence to draw
	 * @param compartment The compartment
	 */
	DECLARE_FUNCTION_2(enterExitCompartment3, const char *sequence, ObjectIndex compartment)

	/**
	 * Process callback action when the entity direction is not kDirectionRight
	 */
	DECLARE_FUNCTION(callbackActionOnDirection)

	/**
	 * Call a savepoint (or draw sequence in default case)
	 *
	 * @param sequence1   The sequence to draw in the default case
	 * @param entity      The entity
	 * @param action      The action
	 * @param sequence2   The sequence name for the savepoint
	 */
	DECLARE_FUNCTION_4(callSavepoint, const char *sequence1, EntityIndex entity, ActionIndex action, const char *sequence2)

	/**
	 * Call a savepoint
	 *
	 * @param param1 The entity
	 * @param param2 The action
	 * @param seq    The sequence name for the savepoint
	 */
	DECLARE_FUNCTION_3(callSavepointNoDrawing, EntityIndex entity, ActionIndex action, const char *sequence)

	/**
	 * Draws the entity along with another one
	 *
	 * @param sequence1   The sequence to draw
	 * @param sequence2   The sequence to draw for the second entity
	 * @param entity      The EntityIndex of the second entity
	 */
	DECLARE_FUNCTION_3(draw2, const char *sequence1, const char *sequence2, EntityIndex entity)

	/**
	 * Plays sound
	 *
	 * @param filename The sound filename
	 */
	DECLARE_VFUNCTION_1(playSound, const char *filename)

	/**
	 * Plays sound
	 *
	 * @param filename The sound filename
	 */
	DECLARE_FUNCTION_1(playSound16, const char *filename)

	/**
	 * Process callback action when somebody is standing in the restaurant or salon.
	 */
	DECLARE_FUNCTION(callbackActionRestaurantOrSalon)

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

	DECLARE_FUNCTION_1(lookingForCath, TimeValue timeValue)

	/**
	 * Updates the entity
	 *
	 * @param param1 The car
	 * @param param2 The entity position
	 */
	DECLARE_FUNCTION_2(updateEntity2, CarIndex car, EntityPosition entityPosition)

	DECLARE_FUNCTION_2(enterCompartment, bool, bool)

	DECLARE_FUNCTION_1(exitCompartment, bool)
	DECLARE_FUNCTION_1(compartmentLogic, TimeValue timeValue)

	/**
	 * Setup Chapter 1
	 */
	DECLARE_VFUNCTION(chapter1)

	DECLARE_FUNCTION_1(knockTyler, TimeValue timeValue)
	DECLARE_FUNCTION(dinner)
	DECLARE_FUNCTION(waitTyler)
	DECLARE_FUNCTION(seekTyler)
	DECLARE_FUNCTION(goToDinner)
	DECLARE_FUNCTION(orderDinner)
	DECLARE_FUNCTION(eatingDinner)
	DECLARE_FUNCTION(greatAnna)
	DECLARE_FUNCTION(returnFromDinner)
	DECLARE_FUNCTION(goSalon)
	DECLARE_FUNCTION(backFromSalon)
	DECLARE_FUNCTION(function34)

	/**
	 * Setup Chapter 2
	 */
	DECLARE_VFUNCTION(chapter2)

	DECLARE_FUNCTION(atBreakfast)
	DECLARE_FUNCTION(openCompartment)
	DECLARE_FUNCTION(inSalon2)
	DECLARE_FUNCTION(endChapter2)

	/**
	 * Setup Chapter 3
	 */
	DECLARE_VFUNCTION(chapter3)

	DECLARE_FUNCTION_2(function41, CarIndex car, EntityPosition entityPosition)
	DECLARE_FUNCTION_3(function42, CarIndex car, EntityPosition entityPosition, bool)
	DECLARE_FUNCTION(goLunch)
	DECLARE_FUNCTION(returnLunch)
	DECLARE_FUNCTION(function45)
	DECLARE_FUNCTION(reading)
	DECLARE_FUNCTION(bathroomTrip)
	DECLARE_FUNCTION(dressing)
	DECLARE_FUNCTION(goConcert)
	DECLARE_FUNCTION(function50)
	DECLARE_FUNCTION(stalkAnna)
	DECLARE_FUNCTION(afterConcert)
	DECLARE_FUNCTION(satisfied)
	DECLARE_FUNCTION(inSalon3)
	DECLARE_FUNCTION(leaveSalon)
	DECLARE_FUNCTION(beforeVienna)

	/**
	 * Setup Chapter 4
	 */
	DECLARE_VFUNCTION(chapter4)

	DECLARE_FUNCTION(goToDinner4)
	DECLARE_FUNCTION(function59)
	DECLARE_FUNCTION(dinner4)
	DECLARE_FUNCTION(returnCompartment4)
	DECLARE_FUNCTION(goSalon4)
	DECLARE_FUNCTION(drinking)
	DECLARE_FUNCTION(drunk)
	DECLARE_FUNCTION(function65)

	/**
	 * Setup Chapter 5
	 */
	DECLARE_VFUNCTION(chapter5)

	/**
	 * Handle Chapter 5 events
	 */
	DECLARE_FUNCTION(chapter5Handler)

	DECLARE_FUNCTION(hiding)
	DECLARE_FUNCTION(cutCarsLoose)

	DECLARE_NULL_FUNCTION()
};

} // End of namespace LastExpress

#endif // LASTEXPRESS_AUGUST_H
