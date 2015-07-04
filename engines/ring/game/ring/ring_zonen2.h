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
 */

#ifndef RING_RING_ZONE_N2_H
#define RING_RING_ZONE_N2_H

#include "ring/base/event.h"

#include "ring/shared.h"

namespace Ring {

class ApplicationRing;

class ZoneN2Ring : public EventHandlerZone {
public:
	ZoneN2Ring(ApplicationRing *application);
	~ZoneN2Ring();

	void onInit() override;
	void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) override;

	void onSetup(SetupType type) override;
	void onSwitch(uint32 type) override;
	void onTimer(TimerId id) override;
	void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) override;
	void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) override;
	void onSound(Id id, SoundType type, uint32 a3, bool process) override;
	void onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) override;
	void onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) override;
	void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) override;
	void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) override;

private:
	ApplicationRing *_app;

	// Bag
	int32        _presentationIndexN2;
	int32        _distN2;

	void sub_433FA0();
	void sub_4340C0();
};

} // End of namespace Ring

#endif // RING_RING_ZONE_N2_H
