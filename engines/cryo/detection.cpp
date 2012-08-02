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

#include "cryo/cryo.h"

#include "engines/advancedDetector.h"

namespace Cryo {

static const PlainGameDescriptor cryoGames[] = {
	// Games
	{"atlantis", "Atlantis: The Lost Tales"},
	{"atlantis2", "Atlantis II: Beyond Atlantis"},
	{"atlantis3", "Atlantis III: The New World"},
	{"atlantis4", "Atlantis Evolution"},
	{"atlantis5", "The Secrets of Atlantis: The Sacred Legacy"},
	{"aztec", "Aztec: The Curse in the Heart of the City of Gold"}, // also known as "The Sacred Amulet"
	{"china", "China: The Forbidden City"},
	{"egypt", "Egypt 1156 B.C.: Tomb of the Pharaoh"},
	{"egypt2", "Egypt II: The Heliopolis Prophecy"},
	{"salammbo", "Salammbo: Battle for Carthage"},
	{"versailles", "Versailles 1685"},
	{"versailles2", "Versailles II: Testament of the King"},
	{"zone", "Zero Zone"},
	{0, 0}
};

static const CryoGameDescription gameDescriptions[] = {

	{
		{
			"china",
			"",
			{
				{"CHINE.EXE", 0, "8850a946310e8e10208576af34b919f2", 573952},
				{"CD.HNM",    0, "1aec4b0148683c2776cc00f523ea8a43", 45628},
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_UNSTABLE,
			GUIO1(GUIO_NOASPECT)
		},
		GameTypeChina,
		0
	},

	{
		{
			"egypt",
			"",
			{
				{"EGYPTE.EXE", 0, "cce60d7469fbf587f652d756957457d6", 375808},
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_UNSTABLE,
			GUIO1(GUIO_NOASPECT)
		},
		GameTypeEgypt,
		0
	},

	{AD_TABLE_END_MARKER, 0, 0}
};


class CryoMetaEngine : public AdvancedMetaEngine {
public:
	CryoMetaEngine() : AdvancedMetaEngine(gameDescriptions, sizeof(Cryo::CryoGameDescription), cryoGames) {
		_singleid = "cryo";
		_guioptions = GUIO2(GUIO_NOSUBTITLES, GUIO_NOSFX);
	}

	const char *getName() const {
		return "Cryo";
	}

	const char *getOriginalCopyright() const {
		return "Cryo game Engine (C) 1997-2002 Cryo Interactive";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual int getMaximumSaveSlot() const { return 99; }

protected:
	bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const;
};

bool CryoMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const {
	if (gd) {
		*engine = new CryoEngine(syst, (const CryoGameDescription *)gd);
	}
	return gd != 0;
}

bool CryoMetaEngine::hasFeature(MetaEngineFeature f) const {
	return (f == kSupportsListSaves) ||
	       (f == kSupportsDeleteSave) ||
	       (f == kSavesSupportMetaInfo) ||
	       (f == kSavesSupportThumbnail) ||
	       (f == kSavesSupportCreationDate) ||
	       (f == kSavesSupportPlayTime);
}

} // End of namespace Cryo

#if PLUGIN_ENABLED_DYNAMIC(CRYO)
	REGISTER_PLUGIN_DYNAMIC(CRYO, PLUGIN_TYPE_ENGINE, Cryo::CryoMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(CRYO, PLUGIN_TYPE_ENGINE, Cryo::CryoMetaEngine);
#endif
