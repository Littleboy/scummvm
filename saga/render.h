/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004 The ScummVM project
 *
 * The ReInherit Engine is (C)2000-2003 by Daniel Balsom.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

// Main rendering loop - private header

#ifndef SAGA_RENDER_H_
#define SAGA_RENDER_H_

#include "saga/sprite.h"

namespace Saga {

#define R_PAUSEGAME_MSG "PAWS GAME"
enum RENDER_FLAGS {
	RF_SHOW_FPS = 0x01,
	RF_PALETTE_TEST = 0x02,
	RF_TEXT_TEST = 0x04,
	RF_OBJECTMAP_TEST = 0x08,
	RF_RENDERPAUSE = 0x10,
	RF_GAMEPAUSE = 0x20
};

struct R_BUFFER_INFO {
	byte *r_bg_buf;
	int r_bg_buf_w;
	int r_bg_buf_h;
	byte *r_tmp_buf;
	int r_tmp_buf_w;
	int r_tmp_buf_h;
};

class Render {
public:
	int reg(void);
	Render(SagaEngine *vm, OSystem *system, Gfx *gfx, ObjectMap *omap);
	~Render(void);
	bool initialized();
	int drawScene(void);
	unsigned int getFlags(void);
	void setFlag(unsigned int);
	void toggleFlag(unsigned int);
	unsigned int getFrameCount(void);
	unsigned int resetFrameCount(void);
	int getBufferInfo(R_BUFFER_INFO *);

private:
	static void fpsTimerCallback(void *refCon);
	void fpsTimer(void);

	SagaEngine *_vm;
	OSystem *_system;
	bool _initialized;
	Gfx *_gfx;
	ObjectMap *_omap;

	// Module data
	R_SURFACE *_backbuf_surface;

	byte *_bg_buf;
	int _bg_buf_w;
	int _bg_buf_h;
	byte *_tmp_buf;
	int _tmp_buf_w;
	int _tmp_buf_h;

	R_SPRITELIST *_test_sprite;

	unsigned int _fps;
	unsigned int _framecount;
	unsigned int _flags;
	int _mode;
};

} // End of namespace Saga

#endif
