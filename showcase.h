#ifndef SHOWCASE_H
#define SHOWCASE_H

/* =========================================================
   showcase.h  –  Stage 3: Final Showcase declarations
   ========================================================= */

#include "assets.h"

void showcase_draw(void);
void showcase_update(float dt);
void showcase_mouse_move(int x, int y);
void showcase_mouse_click(int button, int state, int x, int y);
void showcase_init(void);
void dressup_init(void);
#endif /* SHOWCASE_H */