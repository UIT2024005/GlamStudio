#ifndef MAKEUP_H
#define MAKEUP_H

/* =========================================================
   makeup.h  –  Stage 1: Makeup face & UI declarations
   ========================================================= */

#include "assets.h"

/* ── Face drawing ───────────────────────────────────────── */
void makeup_draw_face(void);          /* full face composite    */
void makeup_draw_base(void);          /* skin / face shape      */
void makeup_draw_eyes(void);          /* eyes + eyeshadow       */
void makeup_draw_lips(void);          /* mouth + lipstick       */
void makeup_draw_blush(void);         /* cheek blush patches    */
void makeup_draw_nose(void);          /* simple nose            */
void makeup_draw_eyebrows(void);      /* eyebrows               */
void makeup_draw_hair(void);          /* simple hair backdrop   */
void makeup_draw_neck_shoulders(void);
void makeup_draw_ears(void);         /* ears drawn separately  */
void makeup_init(void);
/* ── Stage display ──────────────────────────────────────── */
void makeup_draw(void);               /* entire stage 1 scene   */
void makeup_update(float dt);         /* animation tick         */

/* ── Input handling ─────────────────────────────────────── */
void makeup_mouse_move(int x, int y);
void makeup_mouse_click(int button, int state, int x, int y);

#endif /* MAKEUP_H */