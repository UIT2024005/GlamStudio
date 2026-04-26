/* =========================================================
   makeup.c  –  Stage 1: Makeup Stage
   Face drawn with OpenGL primitives; UI on the right panel.
   ========================================================= */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "makeup.h"
#include "ui.h"
#include "assets.h"

/* ── External game state ─────────────────────────────────── */
extern GameState G;
extern void stage_next(void);   /* defined in main.c */

/* ── Face centre / scale ─────────────────────────────────── */
#define FCX  310.f   /* face centre x  */
#define FCY  340.f   /* face centre y  */
#define FSC  1.0f    /* scale factor   */

/* ── UI panel layout ─────────────────────────────────────── */
#define PANEL_X  580.f
#define PANEL_W  300.f

/* Swatch grid helpers */
#define SW_SZ    40.f  /* swatch size */
#define SW_GAP   14.f  /* gap between */

/* ── Local button definitions ────────────────────────────── */
static Button btn_next;

/* Lipstick swatches */
static Button swatch_lip[6]; /* RED, PINK, NUDE, BERRY, CORAL, WINE */
static float lip_cols[6][3] = {
    {0.90f, 0.10f, 0.15f},  // RED
    {0.95f, 0.45f, 0.60f},  // PINK
    {0.85f, 0.62f, 0.55f},  // NUDE
    {0.65f, 0.05f, 0.25f},  // BERRY
    {0.98f, 0.45f, 0.35f},  // CORAL
    {0.45f, 0.05f, 0.10f}   // WINE
};

static const char *lip_names[] =
{ "Red", "Pink", "Nude", "Berry", "Coral", "Wine" };

/* Eye-shadow swatches */
static Button swatch_eye[6]; /* BLUE, PURPLE, GOLD, GREEN, TEAL, SILVER */
static float eye_cols[6][3] = {
    {0.35f, 0.55f, 0.90f},  // BLUE
    {0.60f, 0.20f, 0.80f},  // PURPLE
    {0.85f, 0.68f, 0.10f},  // GOLD
    {0.20f, 0.75f, 0.45f},  // GREEN
    {0.15f, 0.65f, 0.70f},  // TEAL
    {0.75f, 0.75f, 0.80f}   // SILVER
};
static const char *eye_names[] = { "Blue", "Purple", "Gold", "Green", "Teal", "Silver" };

/* Blush swatches */
static Button swatch_blush[3];
static float blush_cols[3][3] = {
    {0.95f, 0.50f, 0.55f},
    {0.98f, 0.65f, 0.45f},
    {0.95f, 0.60f, 0.75f}
};
static const char* blush_names[3] = {"Rose","Peach","Pink"};
/* ── Colour interpolation targets ────────────────────────── */
static float lip_target[3];
static float eye_target[3];

/* ── Init ────────────────────────────────────────────────── */
static void makeup_init_buttons(void)
{
    /* Lipstick swatches – row at y=440 */
    float sx = PANEL_X + 20.f;
    float sy = 440.f;
    for (int i = 0; i < 6; i++) {
        swatch_lip[i].x = sx + i*(SW_SZ+SW_GAP);
        swatch_lip[i].y = sy;
        swatch_lip[i].w = SW_SZ;
        swatch_lip[i].h = SW_SZ;
        swatch_lip[i].label  = lip_names[i];
        swatch_lip[i].r = lip_cols[i][0];
        swatch_lip[i].g = lip_cols[i][1];
        swatch_lip[i].b = lip_cols[i][2];
    }

    /* Eye-shadow swatches – row at y=330 */
    sy = 330.f;
    for (int i = 0; i < 6; i++) {
        swatch_eye[i].x = sx + i*(SW_SZ+SW_GAP);
        swatch_eye[i].y = sy;
        swatch_eye[i].w = SW_SZ;
        swatch_eye[i].h = SW_SZ;
        swatch_eye[i].label  = eye_names[i];
        swatch_eye[i].r = eye_cols[i][0];
        swatch_eye[i].g = eye_cols[i][1];
        swatch_eye[i].b = eye_cols[i][2];
    }
    /* Blush swatches – row at y=260 */
float bx = PANEL_X + 20.f;
float by = 230.f;

    for (int i = 0; i < 3; i++) {
        swatch_blush[i].x = bx + i*(SW_SZ+14);
        swatch_blush[i].y = by;
        swatch_blush[i].w = SW_SZ;
        swatch_blush[i].h = SW_SZ;
        swatch_blush[i].label = blush_names[i];
        swatch_blush[i].r = blush_cols[i][0];
        swatch_blush[i].g = blush_cols[i][1];
        swatch_blush[i].b = blush_cols[i][2];
    }

    /* Next button */
    btn_next.x = PANEL_X + 60.f;
    btn_next.y = 40.f;
    btn_next.w = 160.f;
    btn_next.h = 42.f;
    btn_next.label = "Next: Dress Up >";
    btn_next.r = 0.65f; btn_next.g = 0.15f; btn_next.b = 0.45f;
}

/* ── FACE DRAWING ────────────────────────────────────────── */
void makeup_draw_ears(void)
{
    float cx = FCX;
    float cy = FCY;

    /* Left ear */
    draw_ellipse(cx - 88, cy + 25, 14, 22,
                 COL_SKIN, 1.f, 1);

    draw_ellipse(cx - 90, cy + 25, 6, 12,
                 COL_SKIN_DARK, 0.25f, 1);

    /* Right ear */
    draw_ellipse(cx + 88, cy + 25, 14, 22,
                 COL_SKIN, 1.f, 1);

    draw_ellipse(cx + 90, cy + 25, 6, 12,
                 COL_SKIN_DARK, 0.25f, 1);
}
/* Hair – drawn first (behind face) */
void makeup_draw_hair(void)
{
    float cx = FCX;
    float cy = FCY;

    /* ── MAIN HAIR ── */
    draw_ellipse(cx, cy+40, 85, 105,
                 COL_HAIR_DARK, 1.f, 1);

    /* ── SOFT HAIRLINE ── */
    draw_ellipse(cx, cy+75, 60, 35,
                 COL_HAIR_DARK, 1.f, 1);

    /* =========================
       HAIRBAND (FOREHEAD LEVEL)
       ========================= */

    draw_ellipse(cx, cy+78, 78, 18,
                 0.85f, 0.25f, 0.45f, 1.f, 1);

    /* inner highlight (fabric depth) */
    draw_ellipse(cx, cy+78, 70, 10,
                 0.95f, 0.40f, 0.60f, 0.35f, 1);

    /* side knot */
    draw_circle(cx+75, cy+78, 6,
                0.85f, 0.25f, 0.45f, 1.f, 1);

    draw_circle(cx+82, cy+74, 4,
                0.75f, 0.20f, 0.40f, 1.f, 1);

    /* =========================
       LOW BUN (NATURAL LONG FORM)
       ========================= */

    /* main bun volume */
    draw_ellipse(cx, cy+122, 26, 34,
                 COL_HAIR_DARK, 1.f, 1);

    /* inner depth */
    draw_ellipse(cx, cy+122, 18, 24,
                 COL_HAIR_MED, 0.35f, 1);

    /* bottom taper (hair tie pull effect) */
    draw_circle(cx, cy+140, 10,
                COL_HAIR_DARK, 0.95f, 1);

    /* bun highlight */
    draw_circle(cx+6, cy+126, 7,
                COL_HAIR_MED, 0.45f, 1);
}
/* Neck + shoulders */
void makeup_draw_neck_shoulders(void)
{
    float cx = FCX;
    float cy = FCY;

    /* =========================
       NECK (aligned to jaw)
       ========================= */

    glColor3f(COL_SKIN);

    glBegin(GL_QUADS);

        /* left side of neck (follows jaw width) */
        glVertex2f(cx - 24, cy - 95);
        glVertex2f(cx + 24, cy - 95);

        /* taper down for natural neck shape */
        glVertex2f(cx + 20, cy - 145);
        glVertex2f(cx - 20, cy - 145);

    glEnd();

    /* neck shading (adds depth) */
    draw_ellipse(cx, cy - 115, 26, 55,
                 COL_SKIN_DARK, 0.18f, 1);

    /* subtle throat shadow */
    draw_ellipse(cx, cy - 110, 12, 35,
                 COL_SKIN_DARK, 0.25f, 1);

    /* collarbone hint */
    draw_ellipse(cx, cy - 150, 65, 10,
                 COL_SKIN_DARK, 0.35f, 1);


    /* =========================
       SHOULDERS (natural slope)
       ========================= */

    /* main shoulder base */
    draw_ellipse(cx, cy - 165, 125, 45,
                 COL_SKIN, 1.f, 1);

    /* left shoulder highlight */
    draw_ellipse(cx - 60, cy - 165, 70, 35,
                 COL_SKIN, 0.85f, 1);

    /* right shoulder highlight */
    draw_ellipse(cx + 60, cy - 165, 70, 35,
                 COL_SKIN, 0.85f, 1);

    /* shoulder shadow (adds depth curve) */
    draw_ellipse(cx, cy - 175, 140, 25,
                 COL_SKIN_DARK, 0.20f, 1);
}
void makeup_draw_base(void)
{
    float cx = FCX;
    float cy = FCY;

    /* Upper face (slightly reduced width) */
    draw_ellipse(cx, cy+10, 85, 98, COL_SKIN, 1.f, 1);

    /* Lower face with refined jaw */
    glColor3f(COL_SKIN);
    glBegin(GL_POLYGON);

        // Chin (sharp but not too long)
        glVertex2f(cx, cy - 100);

        // Jawline right (reduced width)
        glVertex2f(cx + 38, cy - 68);
        glVertex2f(cx + 58, cy - 18);
        glVertex2f(cx + 52, cy + 28);

        // Connect to upper face
        glVertex2f(cx + 32, cy + 72);

        // Top center
        glVertex2f(cx, cy + 90);

        // Left side (mirror)
        glVertex2f(cx - 32, cy + 72);
        glVertex2f(cx - 52, cy + 28);
        glVertex2f(cx - 58, cy - 18);
        glVertex2f(cx - 38, cy - 68);

    glEnd();

    /* Jawline shadow (more subtle) */
    glColor4f(0.65f, 0.50f, 0.45f, 0.22f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(cx - 35, cy - 68);
        glVertex2f(cx,      cy - 95);
        glVertex2f(cx + 35, cy - 68);
    glEnd();
    glLineWidth(1.0f);

    /* Cheek contour (moved inward) */
    draw_ellipse(cx-60, cy+10, 18, 24, COL_SKIN_DARK, 0.14f, 1);
    draw_ellipse(cx+60, cy+10, 18, 24, COL_SKIN_DARK, 0.14f, 1);
}

/* Eyebrows */
void makeup_draw_eyebrows(void)
{
    glColor3f(COL_HAIR_DARK);
    glLineWidth(4.0f);
    /* Left brow */
    glBegin(GL_LINE_STRIP);
      glVertex2f(FCX-72, FCY+55);
      glVertex2f(FCX-50, FCY+68);
      glVertex2f(FCX-28, FCY+62);
    glEnd();
    /* Right brow */
    glBegin(GL_LINE_STRIP);
      glVertex2f(FCX+72, FCY+55);
      glVertex2f(FCX+50, FCY+68);
      glVertex2f(FCX+28, FCY+62);
    glEnd();
    glLineWidth(1.0f);
}

/* Eyes with eyeshadow */
void draw_single_eye(float x, float y, float er, float eg, float eb)
{
    float w = 23;   /* reduced size */
    float h = 11;

    /* ================= EYELID SHADOW (SOFT DEPTH) ================= */
    draw_ellipse(x, y+4, w+5, h+4,
                 er*0.6f, eg*0.6f, eb*0.6f, 0.28f, 1);

    /* ================= EYE WHITE ================= */
    draw_ellipse(x, y, w, h,
                 0.97f, 0.97f, 0.97f, 1.f, 1);

    /* ================= DARK LIMBAL RING (REALISTIC EDGE) ================= */
    draw_circle(x, y, 7.4f,
                0.05f, 0.03f, 0.02f, 0.95f, 1);

    /* ================= IRIS (depth layers) ================= */
    draw_circle(x, y, 6.3f,
                0.22f, 0.12f, 0.06f, 1.f, 1);

    draw_circle(x, y, 5.2f,
                0.28f, 0.16f, 0.10f, 1.f, 1);

    /* ================= PUPIL (darker, more real) ================= */
    draw_circle(x, y, 3.8f,
                0.02f, 0.01f, 0.01f, 1.f, 1);

    /* ================= EYE GLOW HIGHLIGHT ================= */
    draw_circle(x+2.5f, y+2.5f, 2.0f,
                1, 1, 1, 0.85f, 1);

    /* ================= EYELASHES ================= */
    glColor3f(0.02f, 0.01f, 0.01f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);

    /* upper lashes */
    glVertex2f(x-13, y+6);  glVertex2f(x-15, y+12);
    glVertex2f(x-9,  y+7);  glVertex2f(x-10, y+13);
    glVertex2f(x-5,  y+8);  glVertex2f(x-5,  y+14);

    glVertex2f(x-1,  y+9);  glVertex2f(x-1,  y+15);
    glVertex2f(x+3,  y+9);  glVertex2f(x+4,  y+15);

    glVertex2f(x+7,  y+8);  glVertex2f(x+7,  y+14);
    glVertex2f(x+11, y+7);  glVertex2f(x+12, y+13);
    glVertex2f(x+14, y+6);  glVertex2f(x+15, y+12);

    /* lower lashes (soft) */
    glVertex2f(x-9, y-5); glVertex2f(x-10, y-7);
    glVertex2f(x-3, y-5); glVertex2f(x-3,  y-7);
    glVertex2f(x+3, y-5); glVertex2f(x+3,  y-7);
    glVertex2f(x+9, y-5); glVertex2f(x+10, y-7);

    glEnd();
}

void makeup_draw_eyes(void)
{
    float er = G.eyeR, eg = G.eyeG, eb = G.eyeB;

    float lx = FCX - 42;
    float rx = FCX + 42;
    float ey = FCY + 34;

    draw_single_eye(lx, ey, er, eg, eb);
    draw_single_eye(rx, ey, er, eg, eb);
}
/* Nose */
void makeup_draw_nose(void)
{
    float cx = FCX;
    float cy = FCY;

    /* soft nose bridge (IMPORTANT: higher opacity) */
    draw_ellipse(cx, cy+12, 11, 20,
                 COL_SKIN_DARK, 0.35f, 1);

    /* side shading (gives 3D feel) */
    draw_ellipse(cx-6, cy+10, 6, 16,
                 COL_SKIN_DARK, 0.18f, 1);

    draw_ellipse(cx+6, cy+10, 6, 16,
                 COL_SKIN_DARK, 0.18f, 1);

    /* nose tip highlight */
    draw_circle(cx, cy+8, 3.5f,
                1.0f, 0.85f, 0.80f, 0.35f, 1);

    /* nostrils (slightly darker + clearer) */
    draw_circle(cx-6, cy+20, 3.2f, COL_SKIN_DARK, 0.65f, 1);
    draw_circle(cx+6, cy+20, 3.2f, COL_SKIN_DARK, 0.65f, 1);

    /* soft base shadow */
    draw_ellipse(cx, cy+18, 18, 8,
                 COL_SKIN_DARK, 0.15f, 1);
}

/* Blush */
void makeup_draw_blush(void)
{
    float br = 0.0f, bg = 0.0f, bb = 0.0f;

    switch(G.blush) {
        case BLUSH_ROSE:
            br=0.95f; bg=0.50f; bb=0.55f;
            break;

        case BLUSH_PEACH:
            br=0.98f; bg=0.65f; bb=0.45f;
            break;

        case BLUSH_PINK:
            br=0.95f; bg=0.60f; bb=0.75f;
            break;

        default:
            return;
    }

   draw_ellipse(FCX-68, FCY+12, 18, 10, br, bg, bb, 0.40f, 1);
draw_ellipse(FCX+68, FCY+12, 18, 10, br, bg, bb, 0.40f, 1);
}

/* Lips */
void makeup_draw_lips(void)
{
    float lr = G.lipR, lg = G.lipG, lb = G.lipB;

    float cx = FCX;
    float cy = FCY - 32;

    /* lower lip (fuller, soft) */
    draw_ellipse(cx, cy, 26, 12, lr, lg, lb, 1.f, 1);

    /* upper lip (cupid bow effect) */
    draw_ellipse(cx-12, cy+10, 14, 8, lr, lg, lb, 1.f, 1);
    draw_ellipse(cx+12, cy+10, 14, 8, lr, lg, lb, 1.f, 1);
    draw_ellipse(cx, cy+8, 6, 4, lr, lg, lb, 1.f, 1);

    /* soft highlight (gloss look) */
    draw_ellipse(cx, cy-6, 12, 4,
                 clampf(lr+0.25f,0,1),
                 clampf(lg+0.25f,0,1),
                 clampf(lb+0.25f,0,1), 0.45f, 1);

    /* subtle outline */
    glColor4f(clampf(lr-0.2f,0,1),
              clampf(lg-0.2f,0,1),
              clampf(lb-0.2f,0,1), 0.5f);

    draw_ellipse(cx, cy, 26, 12, 0,0,0, 0, 0);
}

/* Composite face */
void makeup_draw_face(void)
{
    makeup_draw_hair();     // back layer
makeup_draw_base();     // face on top OR adjust overlap
makeup_draw_ears();
    makeup_draw_neck_shoulders();
    makeup_draw_eyebrows();
    makeup_draw_eyes();
    makeup_draw_nose();
    makeup_draw_blush();
    makeup_draw_lips();
}


/* ── PANEL BACKGROUND ────────────────────────────────────── */
static void draw_panel_bg(void)
{
    /* Panel card */
    draw_rounded_rect(PANEL_X, 20, PANEL_W-20, WIN_H-40, 16,
                      0.97f, 0.93f, 0.95f, 0.92f, 1);
    /* Panel shadow */
    draw_rounded_rect(PANEL_X+4, 16, PANEL_W-20, WIN_H-40, 16,
                      0.3f, 0.1f, 0.2f, 0.12f, 1);

    ui_draw_divider(PANEL_X+10, WIN_H-35,
                    PANEL_X+PANEL_W-30, WIN_H-35,
                    0.80f, 0.50f, 0.65f, 0.6f);
}

/* ── STAGE BACKGROUND ────────────────────────────────────── */
static void draw_bg(void)
{
    /* Gradient-like background using two quads */
    glBegin(GL_QUADS);
      glColor3f(0.99f, 0.92f, 0.95f);
      glVertex2f(0,     0);
      glVertex2f(WIN_W, 0);
      glColor3f(0.96f, 0.85f, 0.90f);
      glVertex2f(WIN_W, WIN_H);
      glVertex2f(0,     WIN_H);
    glEnd();

    /* Decorative circles */
    draw_circle( 80, WIN_H-80, 60, 0.95f,0.75f,0.85f, 0.30f, 1);
    draw_circle(180, WIN_H-30, 35, 0.90f,0.70f,0.80f, 0.20f, 1);
    draw_circle( 30, 200,      45, 0.92f,0.80f,0.88f, 0.18f, 1);

    /* Mirror / vanity frame */
    draw_rounded_rect(90, 100, 430, 490, 20,
                      0.78f, 0.55f, 0.65f, 0.55f, 0);
    glLineWidth(3.0f);
    draw_rounded_rect(90, 100, 430, 490, 20,
                      0.78f, 0.55f, 0.65f, 0.55f, 0);
    glLineWidth(1.0f);

    /* Vanity light bulbs */
    float bx = 90.f;
    for (int i = 0; i < 6; i++) {
        float bxp = bx + 40 + i*68.f;
        draw_circle(bxp, 590, 8, 1.0f,0.95f,0.7f, 0.95f, 1);
        draw_circle(bxp, 590, 12, 1.0f,0.95f,0.7f, 0.25f, 1);
    }
}

/* ── PANEL UI ─────────────────────────────────────────────── */
static void draw_panel_ui(void)
{
    draw_panel_bg();

    ui_draw_title(PANEL_X + (PANEL_W-20)/2.f, WIN_H-25,
                  "Makeup Studio",
                  0.55f, 0.15f, 0.35f);

    /* ───────── LIPSTICK ───────── */
    ui_draw_text(PANEL_X+18, 510, "Lipstick",
                 0.45f, 0.15f, 0.30f, GLUT_BITMAP_HELVETICA_18);

    ui_draw_divider(PANEL_X+18, 505, PANEL_X+PANEL_W-38, 505,
                    0.70f, 0.40f, 0.55f, 0.5f);

    for (int i = 0; i < 6; i++) {
        int active = (G.lip == (LipColor)(i+1));

        ui_draw_swatch(swatch_lip[i].x, swatch_lip[i].y, SW_SZ,
                       lip_cols[i][0], lip_cols[i][1], lip_cols[i][2],
                       swatch_lip[i].hovered, active);

        ui_draw_text(swatch_lip[i].x+4, swatch_lip[i].y-14,
                     lip_names[i],
                     0.50f,0.20f,0.35f,
                     GLUT_BITMAP_8_BY_13);
    }

    /* ───────── EYE SHADOW ───────── */
    ui_draw_text(PANEL_X+18, 396, "Eye Shadow",
                 0.30f, 0.10f, 0.55f, GLUT_BITMAP_HELVETICA_18);

    ui_draw_divider(PANEL_X+18, 391, PANEL_X+PANEL_W-38, 391,
                    0.60f, 0.30f, 0.75f, 0.5f);

    for (int i = 0; i < 6; i++) {
        int active = (G.eye == (EyeShadow)(i+1));

        ui_draw_swatch(swatch_eye[i].x, swatch_eye[i].y, SW_SZ,
                       eye_cols[i][0], eye_cols[i][1], eye_cols[i][2],
                       swatch_eye[i].hovered, active);

        ui_draw_text(swatch_eye[i].x+2, swatch_eye[i].y-14,
                     eye_names[i],
                     0.30f,0.10f,0.50f,
                     GLUT_BITMAP_8_BY_13);
    }

    /* ───────── BLUSH PLATTER (NEW STYLE) ───────── */
    ui_draw_text(PANEL_X+18, 284, "Blush Palette",
                 0.70f, 0.30f, 0.45f, GLUT_BITMAP_HELVETICA_18);

    ui_draw_divider(PANEL_X+18, 279, PANEL_X+PANEL_W-38, 279,
                    0.80f, 0.45f, 0.60f, 0.5f);

    /* draw blush swatches like palette */
    for (int i = 0; i < 3; i++) {
    int active = (G.blush == i+1);

    ui_draw_swatch(
        swatch_blush[i].x,
        swatch_blush[i].y,
        SW_SZ,
        blush_cols[i][0],
        blush_cols[i][1],
        blush_cols[i][2],
        0,
        active
    );

    ui_draw_text(
        swatch_blush[i].x,
        swatch_blush[i].y - 14,
        blush_names[i],
        0.5f,0.2f,0.35f,
        GLUT_BITMAP_8_BY_13
    );
}
    /* NEXT BUTTON */
    ui_draw_button(&btn_next);

    ui_draw_text(PANEL_X+18, 24,
                 "Click colours to apply makeup",
                 0.55f, 0.35f, 0.45f,
                 GLUT_BITMAP_8_BY_13);
}
/* ── PUBLIC API ──────────────────────────────────────────── */

void makeup_draw(void)
{
    draw_bg();
    makeup_draw_face();
    draw_panel_ui();
}

void makeup_update(float dt)
{
    (void)dt;
    /* Determine target lip colour */
   switch (G.lip) {
    case LIP_RED:
        lip_target[0]=0.90f; lip_target[1]=0.10f; lip_target[2]=0.15f;
        break;

    case LIP_PINK:
        lip_target[0]=0.95f; lip_target[1]=0.45f; lip_target[2]=0.60f;
        break;

    case LIP_NUDE:
        lip_target[0]=0.85f; lip_target[1]=0.62f; lip_target[2]=0.55f;
        break;

    case LIP_BERRY:
        lip_target[0]=0.65f; lip_target[1]=0.05f; lip_target[2]=0.25f;
        break;

    case LIP_CORAL:
        lip_target[0]=0.98f; lip_target[1]=0.45f; lip_target[2]=0.35f;
        break;

    case LIP_WINE:
        lip_target[0]=0.45f; lip_target[1]=0.05f; lip_target[2]=0.10f;
        break;

    default:
        lip_target[0]=0.80f; lip_target[1]=0.55f; lip_target[2]=0.50f;
        break;
}
    /* Smooth transition (lerp) */
    float speed = 4.f * dt;
    G.lipR = lerpf(G.lipR, lip_target[0], clampf(speed,0,1));
    G.lipG = lerpf(G.lipG, lip_target[1], clampf(speed,0,1));
    G.lipB = lerpf(G.lipB, lip_target[2], clampf(speed,0,1));

    /* Eye shadow targets */
   switch (G.eye) {
    case EYE_BLUE:
        eye_target[0]=0.35f;
        eye_target[1]=0.55f;
        eye_target[2]=0.90f;
        break;

    case EYE_PURPLE:
        eye_target[0]=0.60f;
        eye_target[1]=0.20f;
        eye_target[2]=0.80f;
        break;

    case EYE_GOLD:
        eye_target[0]=0.85f;
        eye_target[1]=0.68f;
        eye_target[2]=0.10f;
        break;

    case EYE_GREEN:
        eye_target[0]=0.20f;
        eye_target[1]=0.75f;
        eye_target[2]=0.45f;
        break;

    case EYE_TEAL:
        eye_target[0]=0.15f;
        eye_target[1]=0.65f;
        eye_target[2]=0.70f;
        break;

    case EYE_SILVER:
        eye_target[0]=0.75f;
        eye_target[1]=0.75f;
        eye_target[2]=0.80f;
        break;

    default:
        eye_target[0]=0.50f;
        eye_target[1]=0.50f;
        eye_target[2]=0.50f;
        break;
}
    G.eyeR = lerpf(G.eyeR, eye_target[0], clampf(speed,0,1));
    G.eyeG = lerpf(G.eyeG, eye_target[1], clampf(speed,0,1));
    G.eyeB = lerpf(G.eyeB, eye_target[2], clampf(speed,0,1));
}

/* Initialise targets on first entry */
static int makeup_initialised = 0;
static void makeup_ensure_init(void)
{
    if (makeup_initialised) return;
    makeup_initialised = 1;
    makeup_init_buttons();
    /* Default lip = nude tones */
    G.lipR = COL_LIP_DEFAULT; G.lipG = 0.55f; G.lipB = 0.50f;
    lip_target[0]=G.lipR; lip_target[1]=G.lipG; lip_target[2]=G.lipB;
    G.eyeR = COL_EYE_DEFAULT; G.eyeG = 0.50f; G.eyeB = 0.50f;
    eye_target[0]=G.eyeR; eye_target[1]=G.eyeG; eye_target[2]=G.eyeB;
}

void makeup_mouse_move(int x, int y)
{
    makeup_ensure_init();
    for (int i=0;i<6;i++) swatch_lip[i].hovered = ui_hit(&swatch_lip[i],x,y);
    for (int i=0;i<6;i++) swatch_eye[i].hovered = ui_hit(&swatch_eye[i],x,y);
    btn_next.hovered  = ui_hit(&btn_next, x,y);
}

void makeup_mouse_click(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    makeup_ensure_init();

    /* ───────── Lipstick ───────── */
    for (int i = 0; i < 6; i++) {
        if (ui_hit(&swatch_lip[i], x, y)) {
            G.lip = (LipColor)(i + 1);

            lip_target[0] = lip_cols[i][0];
            lip_target[1] = lip_cols[i][1];
            lip_target[2] = lip_cols[i][2];
        }
    }

    /* ───────── Eye Shadow ───────── */
    for (int i = 0; i < 6; i++) {
        if (ui_hit(&swatch_eye[i], x, y)) {
            G.eye = (EyeShadow)(i + 1);

            eye_target[0] = eye_cols[i][0];
            eye_target[1] = eye_cols[i][1];
            eye_target[2] = eye_cols[i][2];
        }
    }

    /* ───────── Blush Palette (FIXED) ───────── */
    for (int i = 0; i < 3; i++) {
    if (ui_hit(&swatch_blush[i], x, y)) {
        G.blush = (BlushState)(i + 1);
    }
}

    /* ───────── Next Button ───────── */
    if (ui_hit(&btn_next, x, y)) {
        stage_next();
    }
}