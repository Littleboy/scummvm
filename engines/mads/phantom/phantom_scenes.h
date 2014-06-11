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

#ifndef MADS_PHANTOM_SCENES_H
#define MADS_PHANTOM_SCENES_H

#include "common/scummsys.h"
#include "mads/game.h"
#include "mads/scene.h"
#include "mads/phantom/game_phantom.h"
//#include "mads/phantom/globals_phantom.h"


namespace MADS {

namespace Phantom {

enum Noun {
	NOUN_GAME = 0x1,
	NOUN_QSAVE = 0x2,
	NOUN_LOOK = 0x3,
	NOUN_TAKE = 0x4,
	NOUN_PUSH = 0x5,
	NOUN_OPEN = 0x6,
	NOUN_PUT = 0x7,
	NOUN_TALK_TO = 0x8,
	NOUN_GIVE = 0x9,
	NOUN_PULL = 0xA,
	NOUN_CLOSE = 0xB,
	NOUN_THROW = 0xC,
	NOUN_WALK_TO = 0xD,
	NOUN_ = 0xE,
	NOUN_IN_ONE = 0xF,
	NOUN_IN_TWO = 0x10,
	NOUN_ACT_CURTAIN = 0x11,
	NOUN_AISLE = 0x12,
	NOUN_APRON = 0x13,
	NOUN_ATTACK = 0x14,
	NOUN_BACKSTAGE = 0x15,
	NOUN_BEAR_PROP = 0x16,
	NOUN_BLUE_FRAME = 0x17,
	NOUN_BOOK = 0x18,
	NOUN_BUST = 0x19,
	NOUN_CABLE = 0x1A,
	NOUN_CARPET = 0x1B,
	NOUN_CARTON = 0x1C,
	NOUN_CARTONS = 0x1D,
	NOUN_CEILING = 0x1E,
	NOUN_CHAIR = 0x1F,
	NOUN_CIRCULAR_STAIRCASE = 0x20,
	NOUN_CLIMB_DOWN = 0x21,
	NOUN_CLIMB_INTO = 0x22,
	NOUN_CLIMB_THROUGH = 0x23,
	NOUN_COLUMN_PROP = 0x24,
	NOUN_CONDUCTORS_STAND = 0x25,
	NOUN_CORRIDOR = 0x26,
	NOUN_COUCH = 0x27,
	NOUN_COUNTERWEIGHT_SYSTEM = 0x28,
	NOUN_CRATE = 0x29,
	NOUN_CRATES = 0x2A,
	NOUN_CRUMPLED_NOTE = 0x2B,
	NOUN_CYCLORAMA = 0x2C,
	NOUN_CYLINDER = 0x2D,
	NOUN_DOOR = 0x2E,
	NOUN_DRESSING_ROOM_DOOR = 0x2F,
	NOUN_DRESSING_SCREEN = 0x30,
	NOUN_DRESSING_TABLE = 0x31,
	NOUN_ELEPHANT_PROP = 0x32,
	NOUN_ENVELOPE = 0x33,
	NOUN_EXIT = 0x34,
	NOUN_EXIT_DOWN = 0x35,
	NOUN_EXIT_SIGN = 0x36,
	NOUN_EXIT_TO = 0x37,
	NOUN_EXIT_TO_BACKSTAGE = 0x38,
	NOUN_EXIT_TO_CELLAR = 0x39,
	NOUN_EXIT_TO_CORRIDOR = 0x3A,
	NOUN_EXIT_TO_DRESSING_RMS = 0x3B,
	NOUN_EXIT_TO_LEFT_WING = 0x3C,
	NOUN_EXIT_TO_PIT = 0x3D,
	NOUN_EXIT_TO_RIGHT_WING = 0x3E,
	NOUN_EXIT_TO_STAGE = 0x3F,
	NOUN_EXIT_TO_STAIRWELL = 0x40,
	NOUN_EXIT_TO_TRAP_ROOM = 0x41,
	NOUN_EXIT_UP = 0x42,
	NOUN_EXPOSED_BRICK = 0x43,
	NOUN_FAN = 0x44,
	NOUN_FIRE_AXE = 0x45,
	NOUN_FL = 0x46,
	NOUN_FLAT = 0x47,
	NOUN_FLATS = 0x48,
	NOUN_FLOOR = 0x49,
	NOUN_FOLDING_CHAIRS = 0x4A,
	NOUN_GARBAGE_CAN = 0x4B,
	NOUN_GRAFFITI = 0x4C,
	NOUN_GREEN_FRAME = 0x4D,
	NOUN_HEMP = 0x4E,
	NOUN_HOLE = 0x4F,
	NOUN_HOUSE = 0x50,
	NOUN_IN_ONE2 = 0x51,
	NOUN_IN_TWO2 = 0x52,
	NOUN_JUMP_INTO = 0x53,
	NOUN_JUNK = 0x54,
	NOUN_KEY = 0x55,
	NOUN_LAMP = 0x56,
	NOUN_LANTERN = 0x57,
	NOUN_LARGE_NOTE = 0x58,
	NOUN_LASSO = 0x59,
	NOUN_LEG = 0x5A,
	NOUN_LETTER = 0x5B,
	NOUN_LIGHT_FIXTURE = 0x5C,
	NOUN_LOCK = 0x5D,
	NOUN_LOCKING_RAIL = 0x5E,
	NOUN_LOCKRAIL = 0x5F,
	NOUN_LOOK_AT = 0x60,
	NOUN_LOOK_THROUGH = 0x61,
	NOUN_MANNEQUINS = 0x62,
	NOUN_MIRROR = 0x63,
	NOUN_MUMMY_PROP = 0x64,
	NOUN_MURAL = 0x65,
	NOUN_MUSIC_SCORE = 0x66,
	NOUN_MUSIC_STAND = 0x67,
	NOUN_MUSIC_STANDS = 0x68,
	NOUN_NOTHING = 0x69,
	NOUN_NOTICE = 0x6A,
	NOUN_ORCHESTRA_DOOR = 0x6B,
	NOUN_ORCHESTRA_PIT = 0x6C,
	NOUN_PAINTING = 0x6D,
	NOUN_PARCHMENT = 0x6E,
	NOUN_PIECE_OF_PAPER = 0x6F,
	NOUN_PIPE = 0x70,
	NOUN_PIT = 0x71,
	NOUN_PLANT = 0x72,
	NOUN_POSTER = 0x73,
	NOUN_PROMPTERS_BOX = 0x74,
	NOUN_PROP_TABLE = 0x75,
	NOUN_PROPS = 0x76,
	NOUN_PROSCENIUM_ARCH = 0x77,
	NOUN_PURCHASE_LINES = 0x78,
	NOUN_RAILING = 0x79,
	NOUN_READ = 0x7A,
	NOUN_RED_FRAME = 0x7B,
	NOUN_REMOVE = 0x7C,
	NOUN_ROPE = 0x7D,
	NOUN_RUG = 0x7E,
	NOUN_SANDBAG = 0x7F,
	NOUN_SCAFFOLDING = 0x80,
	NOUN_SEATS = 0x81,
	NOUN_SIDE_WALL = 0x82,
	NOUN_SMALL_NOTE = 0x83,
	NOUN_STAGE = 0x84,
	NOUN_STAGE_EXIT = 0x85,
	NOUN_STAGE_LEFT = 0x86,
	NOUN_STAGE_RIGHT = 0x87,
	NOUN_STAGEMANAGERS_POST = 0x88,
	NOUN_STAIR_UNIT = 0x89,
	NOUN_STAIRCASE = 0x8A,
	NOUN_STAIRWELL = 0x8B,
	NOUN_STOOL = 0x8C,
	NOUN_STRIKE = 0x8D,
	NOUN_SWORD = 0x8E,
	NOUN_TABLE = 0x8F,
	NOUN_THE_HOUSE = 0x90,
	NOUN_THUNDER_MACHINE = 0x91,
	NOUN_TICKET = 0x92,
	NOUN_TRAP_CEILING = 0x93,
	NOUN_TRAP_DOOR = 0x94,
	NOUN_TURN_OFF = 0x95,
	NOUN_TURN_ON = 0x96,
	NOUN_UNLOCK = 0x97,
	NOUN_URN = 0x98,
	NOUN_WALK_ACROSS = 0x99,
	NOUN_WALK_DOWN = 0x9A,
	NOUN_WALK_THROUGH = 0x9B,
	NOUN_WALK_UP = 0x9C,
	NOUN_WALL = 0x9D,
	NOUN_WARDROBE = 0x9E,
	NOUN_WASTE_BASKET = 0x9F,
	NOUN_WATER_PIPE = 0xA0,
	NOUN_WEAR = 0xA1,
	NOUN_WEDDING_RING = 0xA2,
	NOUN_YELLOW_FRAME = 0xA3,
	NOUN_PROP = 0xA4,
	NOUN_CLIMB_UP = 0xA5,
	NOUN_WALK_ONTO = 0xA6,
	NOUN_WALK = 0xA7,
	NOUN_LEFT_DOOR = 0xA8,
	NOUN_RIGHT_DOOR = 0xA9,
	NOUN_DOOR_TO_PIT = 0xAA,
	NOUN_HEADPHONES = 0xAB,
	NOUN_BOXES = 0xAC,
	NOUN_MUG = 0xAD,
	NOUN_DINETTE_SET = 0xAE,
	NOUN_BOX = 0xAF,
	NOUN_CASES = 0xB0,
	NOUN_TRASH_BUCKET = 0xB1,
	NOUN_CORK_BOARD = 0xB2,
	NOUN_HEADSET = 0xB3,
	NOUN_GRAND_FOYER = 0xB4,
	NOUN_BACK_WALL = 0xB5,
	NOUN_BALLET_BAR = 0xB6,
	NOUN_THROW_RUGS = 0xB7,
	NOUN_COSTUME_RACK = 0xB8,
	NOUN_COAT_RACK = 0xB9,
	NOUN_PAINTINGS = 0xBA,
	NOUN_UMBRELLA = 0xBB,
	NOUN_SHELF = 0xBC,
	NOUN_CONTAINER = 0xBD,
	NOUN_TORN_POSTER = 0xBE,
	NOUN_REVIEW = 0xBF,
	NOUN_REVIEWS = 0xC0,
	NOUN_STAGE_RIGHT_WING = 0xC1,
	NOUN_STAGE_LEFT_WING = 0xC2,
	NOUN_PEDESTAL = 0xC3,
	NOUN_PLANT_PROP = 0xC4,
	NOUN_STATUE = 0xC5,
	NOUN_BATTEN = 0xC6,
	NOUN_BIG_PROP = 0xC7,
	NOUN_VENTILATION_DUCT = 0xC8,
	NOUN_CHANDELIER = 0xC9,
	NOUN_BARRIER = 0xCA,
	NOUN_PLACARD = 0xCB,
	NOUN_TICKET_WINDOW = 0xCC,
	NOUN_ARCHWAY = 0xCD,
	NOUN_COLUMN = 0xCE,
	NOUN_RAIL = 0xCF,
	NOUN_SEAT = 0xD0,
	NOUN_LOGE_CORRIDOR = 0xD1,
	NOUN_HOUSE_LIGHT = 0xD2,
	NOUN_FLOV = 0xD3,
	NOUN_LEFT_COLUMN = 0xD4,
	NOUN_RIGHT_COLUMN = 0xD5,
	NOUN_BOOKCASE = 0xD6,
	NOUN_DOORWAY = 0xD7,
	NOUN_COMFY_CHAIR = 0xD8,
	NOUN_DESK = 0xD9,
	NOUN_MANAGERS_CHAIR = 0xDA,
	NOUN_DESK_LAMP = 0xDB,
	NOUN_WINDOW = 0xDC,
	NOUN_SHEERS = 0xDD,
	NOUN_TAPESTRY = 0xDE,
	NOUN_OVERDOOR_MEDALLION = 0xDF,
	NOUN_LATTICEWORK = 0xE0,
	NOUN_DECORATIVE_MOLDING = 0xE1,
	NOUN_LEFT_DOORWAY = 0xE2,
	NOUN_LEFT_ARCHWAY = 0xE3,
	NOUN_RIGHT_DOORWAY = 0xE4,
	NOUN_RIGHT_ARCHWAY = 0xE5,
	NOUN_SOFA = 0xE6,
	NOUN_END_TABLE = 0xE7,
	NOUN_COFFEE_TABLE = 0xE8,
	NOUN_DECORATIVE_VASE = 0xE9,
	NOUN_MARBLE_COLUMN = 0xEA,
	NOUN_BOX_FIVE = 0xEB,
	NOUN_ENTER = 0xEC,
	NOUN_BOX_SIX = 0xED,
	NOUN_BOX_SEVEN = 0xEE,
	NOUN_BOX_EIGHT = 0xEF,
	NOUN_BOX_NINE = 0xF0,
	NOUN_STEP = 0xF1,
	NOUN_PANEL = 0xF2,
	NOUN_WALK_BEHIND = 0xF3,
	NOUN_MIDDLE_DOORWAY = 0xF4,
	NOUN_LIGHT = 0xF5,
	NOUN_CANDLE = 0xF6,
	NOUN_CASE = 0xF7,
	NOUN_HANDLE = 0xF8,
	NOUN_AXE = 0xF9,
	NOUN_DOOR_CHUNKS = 0xFA,
	NOUN_FLO = 0xFB,
	NOUN_BULLETIN_BOARD = 0xFC,
	NOUN_JULIE = 0xFD,
	NOUN_GLASS_CASE = 0xFE,
	NOUN_KEYHOLE = 0xFF,
	NOUN_MIDDLE_DOOR = 0x100,
	NOUN_DRESSING_GOWN = 0x101,
	NOUN_MONSIEUR_BRIE = 0x102,
	NOUN_CATWALK = 0x103,
	NOUN_GRID = 0x104,
	NOUN_GIRDER = 0x105,
	NOUN_GRIDWORK = 0x106,
	NOUN_DUCTWORK = 0x107,
	NOUN_OPENING = 0x108,
	NOUN_DOME = 0x109,
	NOUN_ALCOVE = 0x10A,
	NOUN_CHRISTINE_DAAE = 0x10B,
	NOUN_CHRISTINE = 0x10C,
	NOUN_WOMAN = 0x10D,
	NOUN_PROMPTERS_STAND = 0x10E,
	NOUN_SUPPORT = 0x10F,
	NOUN_OTHER_CATWALK = 0x110,
	NOUN_SLOT = 0x111,
	NOUN_BEAM_POSITION = 0x112,
	NOUN_LIGHTING_INSTRUMENT = 0x113,
	NOUN_TARP = 0x114,
	NOUN_FACE = 0x115,
	NOUN_CATWALK_OVER_HOUSE = 0x116,
	NOUN_STAIRCASE_POST = 0x117,
	NOUN_JACQUES = 0x118,
	NOUN_GENTLEMAN = 0x119,
	NOUN_BODY = 0x11A,
	NOUN_HOLLOW_COLUMN = 0x11B,
	NOUN_UPPER_LEVEL = 0x11C,
	NOUN_MIDDLE_LEVEL = 0x11D,
	NOUN_LOWER_LEVEL = 0x11E,
	NOUN_LADDER = 0x11F,
	NOUN_CLIMB = 0x120,
	NOUN_CHANDELIER_TRAP = 0x121,
	NOUN_PIECE_OF_WOOD = 0x122,
	NOUN_CUT_HEMP = 0x123,
	NOUN_STONE_WALL = 0x124,
	NOUN_LAKE = 0x125,
	NOUN_STONE_COLUMN = 0x126,
	NOUN_EXIT_THROUGH = 0x127,
	NOUN_STONE_FLOOR = 0x128,
	NOUN_STONE_ARCHWAY = 0x129,
	NOUN_CHARLES = 0x12A,
	NOUN_SWITCH = 0x12B,
	NOUN_PROMPTERS_SEAT = 0x12C,
	NOUN_LEVER = 0x12D,
	NOUN_MONSIEUR_RICHARD = 0x12E,
	NOUN_JULIE2 = 0x12F,
	NOUN_CABLE_HOOK = 0x130,
	NOUN_ATTACH = 0x131,
	NOUN_ROPE_WITH_HOOK = 0x132,
	NOUN_GRAPPLE = 0x133,
	NOUN_OAR = 0x134,
	NOUN_ORGAN = 0x135,
	NOUN_SIT_AT = 0x136,
	NOUN_ORGAN_BENCH = 0x137,
	NOUN_SIT_ON = 0x138,
	NOUN_LARGE_CHAIR = 0x139,
	NOUN_SIT_IN = 0x13A,
	NOUN_SARCOPHAGUS = 0x13B,
	NOUN_SKULL = 0x13C,
	NOUN_SKULLS = 0x13D,
	NOUN_TOTEM = 0x13E,
	NOUN_POLE = 0x13F,
	NOUN_CURTAIN = 0x140,
	NOUN_TORCH = 0x141,
	NOUN_RAMP = 0x142,
	NOUN_MADAME_GIRY = 0x143,
	NOUN_PANELS = 0x144,
	NOUN_MORE_CATACOMBS = 0x145,
	NOUN_BLOCKED_ARCHWAY = 0x146,
	NOUN_GRATE = 0x147,
	NOUN_CATACOMBS = 0x148,
	NOUN_TICKET_SELLER = 0x149,
	NOUN_USHER = 0x14A,
	NOUN_UNLUCKY_ADVENTURER = 0x14B,
	NOUN_SWITCH_PANEL = 0x14C,
	NOUN_SKULL_SWITCH = 0x14D,
	NOUN_TOGGLE = 0x14E,
	NOUN_CATACOMB_ROOM = 0x14F,
	NOUN_BOX_TEN = 0x150,
	NOUN_FOYER = 0x151,
	NOUN_WALK_DOWN_STAIRCASE = 0x152,
	NOUN_WALK_DOWN_STAIRS_TO = 0x153,
	NOUN_HAT_RACK = 0x154,
	NOUN_VASE = 0x155,
	NOUN_CLOTHES_DUMMY = 0x156,
	NOUN_NOTICES = 0x157,
	NOUN_ARCHWAY_TO_NORTH = 0x158,
	NOUN_ARCHWAY_TO_WEST = 0x159,
	NOUN_ARCHWAY_TO_EAST = 0x15A,
	NOUN_GATE = 0x15B,
	NOUN_NEST = 0x15C,
	NOUN_POT = 0x15D,
	NOUN_PUDDLE = 0x15E,
	NOUN_WEB = 0x15F,
	NOUN_PLANK = 0x160,
	NOUN_BLOCK = 0x161,
	NOUN_RATS_NEST = 0x162,
	NOUN_BROKEN_POT = 0x163,
	NOUN_STONE = 0x164,
	NOUN_DRAIN = 0x165,
	NOUN_FATE = 0x166,
	NOUN_SKULL_SWITCH_1 = 0x167,
	NOUN_SKULL_SWITCH_2 = 0x168,
	NOUN_SKULL_SWITCH_3 = 0x169,
	NOUN_SKULL_SWITCH_4 = 0x16A,
	NOUN_SKULL_SWITCH_5 = 0x16B,
	NOUN_SKULL_SWITCH_6 = 0x16C,
	NOUN_SKULL_SWITCH_7 = 0x16D,
	NOUN_SKULL_SWITCH_8 = 0x16E,
	NOUN_SKULL_SWITCH_9 = 0x16F,
	NOUN_SKULL_SWITCH_10 = 0x170,
	NOUN_SKULL_SWITCH_11 = 0x171,
	NOUN_SKULL_SWITCH_12 = 0x172,
	NOUN_SKULL_SWITCH_13 = 0x173,
	NOUN_SKULL_SWITCH_14 = 0x174,
	NOUN_SKULL_SWITCH_15 = 0x175,
	NOUN_SKULL_SWITCH_16 = 0x176,
	NOUN_SKULL_SWITCH_17 = 0x177,
	NOUN_SKULL_SWITCH_18 = 0x178,
	NOUN_SKULL_SWITCH_19 = 0x179,
	NOUN_SKULL_SWITCH_20 = 0x17A,
	NOUN_SKULL_SWITCH_21 = 0x17B,
	NOUN_SKULL_SWITCH_22 = 0x17C,
	NOUN_SKULL_SWITCH_23 = 0x17D,
	NOUN_SKULL_SWITCH_24 = 0x17E,
	NOUN_SKULL_SWITCH_25 = 0x17F,
	NOUN_SKULL_SWITCH_26 = 0x180,
	NOUN_EDGAR_DEGAS = 0x181,
	NOUN_CHANDELIER_CABLE = 0x182,
	NOUN_COB_WEB = 0x183,
	NOUN_SKULL_FACE = 0x184,
	NOUN_BOAT = 0x185,
	NOUN_HOOK = 0x186,
	NOUN_AROUND = 0x187,
	NOUN_CANE = 0x188,
	NOUN_MASK = 0x189,
	NOUN_COVER = 0x18A,
	NOUN_PADLOCK = 0x18B,
	NOUN_LID = 0x18C,
	NOUN_COBWEB = 0x18D,
	NOUN_PHANTOM = 0x18E,
	NOUN_PAPER = 0x18F
};

class SceneFactory {
public:
	static SceneLogic *createScene(MADSEngine *vm);
};

/**
 * Specialized base class for Dragonsphere game scenes
 */
class PhantomScene : public SceneLogic {
protected:
	PhantomGlobals &_globals;
	GamePhantom &_game;
	MADSAction &_action;

	/**
	 * Forms an animation resource name
	 */
	Common::String formAnimName(char sepChar, int suffixNum);

	/**
	 * Plays appropriate sound for entering varous rooms
	 */
	void lowRoomsEntrySound();
public:
	/**
	 * Constructor
	 */
	PhantomScene(MADSEngine *vm);

	void sub7178C();
};

class SceneInfoPhantom : public SceneInfo {
	friend class SceneInfo;
protected:
	virtual void loadCodes(MSurface &depthSurface, int variant);

	virtual void loadCodes(MSurface &depthSurface, Common::SeekableReadStream *stream);

	/**
	* Constructor
	*/
	SceneInfoPhantom(MADSEngine *vm) : SceneInfo(vm) {}
};

// TODO: Temporary, remove once implemented properly
class Scene1xx : public PhantomScene {
protected:
	/**
	 * Plays an appropriate sound when entering a scene
	 */
	void sceneEntrySound() {}

	/**
	 *Sets the AA file to use for the scene
	 */
	void setAAName() {}

	/**
	 * Updates the prefix used for getting player sprites for the scene
	 */
	void setPlayerSpritesPrefix() {}
public:
	Scene1xx(MADSEngine *vm) : PhantomScene(vm) {}
};

// TODO: Temporary, remove once implemented properly
class DummyScene : public PhantomScene {
public:
	DummyScene(MADSEngine *vm) : PhantomScene(vm) {
		warning("Unimplemented scene");
	}

	virtual void setup() {}
	virtual void enter() {}
	virtual void actions() {}
};

} // End of namespace Phantom

} // End of namespace MADS

#endif /* MADS_PHANTOM_SCENES_H */
