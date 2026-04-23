#ifndef DRESSUP_H
#define DRESSUP_H

/* =========================================================
   dressup.h  –  Stage 2: Dress-Up declarations
   ========================================================= */

#include "assets.h"

/* ── Body drawing ───────────────────────────────────────── */
void dressup_draw_body(void);        /* full body skeleton      */
void dressup_draw_dress(void);       /* dress overlay           */
void dressup_draw_hair(void);        /* chosen hairstyle        */
void dressup_draw_accessories(void); /* earrings / necklace     */
void dressup_draw_face(void);        /* simplified face         */

/* ── Stage display ──────────────────────────────────────── */
void dressup_draw(void);
void dressup_update(float dt);

/* ── Input ──────────────────────────────────────────────── */
void dressup_mouse_move(int x, int y);
void dressup_mouse_click(int button, int state, int x, int y);

#endif /* DRESSUP_H */