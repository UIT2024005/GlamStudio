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
static Button swatch_lip[3]; /* RED, PINK, NUDE */
static float lip_cols[3][3] = {
    {COL_LIP_RED},
    {COL_LIP_PINK},
    {COL_LIP_NUDE}
};
static const char *lip_names[] = { "Red", "Pink", "Nude" };

/* Eye-shadow swatches */
static Button swatch_eye[3]; /* BLUE, PURPLE, GOLD */
static float eye_cols[3][3] = {
    {COL_EYE_BLUE},
    {COL_EYE_PURPLE},
    {COL_EYE_GOLD}
};
static const char *eye_names[] = { "Blue", "Purple", "Gold" };

/* Blush toggle */
static Button btn_blush;

/* ── Colour interpolation targets ────────────────────────── */
static float lip_target[3];
static float eye_target[3];

/* ── Init ────────────────────────────────────────────────── */
static void makeup_init_buttons(void)
{
    /* Lipstick swatches – row at y=440 */
    float sx = PANEL_X + 20.f;
    float sy = 440.f;
    for (int i = 0; i < 3; i++) {
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
    for (int i = 0; i < 3; i++) {
        swatch_eye[i].x = sx + i*(SW_SZ+SW_GAP);
        swatch_eye[i].y = sy;
        swatch_eye[i].w = SW_SZ;
        swatch_eye[i].h = SW_SZ;
        swatch_eye[i].label  = eye_names[i];
        swatch_eye[i].r = eye_cols[i][0];
        swatch_eye[i].g = eye_cols[i][1];
        swatch_eye[i].b = eye_cols[i][2];
    }

    /* Blush toggle */
    btn_blush.x = PANEL_X + 20.f;
    btn_blush.y = 220.f;
    btn_blush.w = 130.f;
    btn_blush.h = 35.f;
    btn_blush.label = "Blush  ON/OFF";

    /* Next button */
    btn_next.x = PANEL_X + 60.f;
    btn_next.y = 40.f;
    btn_next.w = 160.f;
    btn_next.h = 42.f;
    btn_next.label = "Next: Dress Up >";
    btn_next.r = 0.65f; btn_next.g = 0.15f; btn_next.b = 0.45f;
}

/* ── FACE DRAWING ────────────────────────────────────────── */

/* Hair – drawn first (behind face) */
void makeup_draw_hair(void)
{
    /* Main hair silhouette: dark blob behind head */
    draw_ellipse(FCX, FCY+30, 105, 125, COL_HAIR_DARK, 1.f, 1);

    /* Side strands left */
    draw_ellipse(FCX-100, FCY-60, 22, 80, COL_HAIR_DARK, 1.f, 1);
    /* Side strands right */
    draw_ellipse(FCX+100, FCY-60, 22, 80, COL_HAIR_DARK, 1.f, 1);

    /* Hair top highlight */
    draw_ellipse(FCX, FCY+100, 65, 35, COL_HAIR_MED, 0.45f, 1);
}

/* Neck + shoulders */
void makeup_draw_neck_shoulders(void)
{
    /* Neck */
    glColor3f(COL_SKIN);
    glBegin(GL_QUADS);
      glVertex2f(FCX-22, FCY-95);
      glVertex2f(FCX+22, FCY-95);
      glVertex2f(FCX+28, FCY-145);
      glVertex2f(FCX-28, FCY-145);
    glEnd();

    /* Shoulders */
    draw_ellipse(FCX, FCY-160, 110, 38, COL_SKIN, 1.f, 1);

    /* Collarbone shadow */
    draw_ellipse(FCX, FCY-150, 70, 8, COL_SKIN_DARK, 0.4f, 1);
}

/* Skin base face */
void makeup_draw_base(void)
{
    /* Main face oval */
    draw_ellipse(FCX, FCY, 95, 118, COL_SKIN, 1.f, 1);

    /* Jaw / chin – slightly narrower ellipse at bottom */
    draw_ellipse(FCX, FCY-45, 72, 72, COL_SKIN, 1.f, 1);

    /* Subtle cheek depth */
    draw_ellipse(FCX-80, FCY+10, 25, 30, COL_SKIN_DARK, 0.18f, 1);
    draw_ellipse(FCX+80, FCY+10, 25, 30, COL_SKIN_DARK, 0.18f, 1);
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
void makeup_draw_eyes(void)
{
    float er = G.eyeR, eg = G.eyeG, eb = G.eyeB;

    /* ── Left eye ── */
    /* Shadow lid */
    if (G.eye != EYE_NONE)
        draw_ellipse(FCX-48, FCY+38, 26, 14, er, eg, eb, 0.70f, 1);

    /* White */
    draw_ellipse(FCX-48, FCY+35, 22, 13, 0.97f, 0.97f, 0.97f, 1.f, 1);
    /* Iris */
    draw_circle  (FCX-48, FCY+35, 9,  0.25f, 0.15f, 0.08f, 1.f, 1);
    /* Pupil */
    draw_circle  (FCX-48, FCY+35, 5,  0.05f, 0.02f, 0.02f, 1.f, 1);
    /* Highlight */
    draw_circle  (FCX-44, FCY+38, 2.5f, 1,1,1, 0.9f, 1);

    /* Lashes top – left eye */
    glColor3f(0.05f, 0.02f, 0.02f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
      glVertex2f(FCX-70, FCY+37);
      glVertex2f(FCX-48, FCY+49);
      glVertex2f(FCX-26, FCY+37);
    glEnd();
    /* Lash tips */
    for (int i = 0; i < 5; i++) {
        float tx = (FCX-70) + i*11.f;
        float ty = FCY+37 + sinf((float)i*0.7f)*6.f + 12.f;
        glBegin(GL_LINES);
          glVertex2f(tx, ty-4);
          glVertex2f(tx-2, ty+6);
        glEnd();
    }
    glLineWidth(1.0f);

    /* ── Right eye ── (mirrored) */
    if (G.eye != EYE_NONE)
        draw_ellipse(FCX+48, FCY+38, 26, 14, er, eg, eb, 0.70f, 1);

    draw_ellipse(FCX+48, FCY+35, 22, 13, 0.97f, 0.97f, 0.97f, 1.f, 1);
    draw_circle  (FCX+48, FCY+35, 9,  0.25f, 0.15f, 0.08f, 1.f, 1);
    draw_circle  (FCX+48, FCY+35, 5,  0.05f, 0.02f, 0.02f, 1.f, 1);
    draw_circle  (FCX+52, FCY+38, 2.5f, 1,1,1, 0.9f, 1);

    glColor3f(0.05f, 0.02f, 0.02f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
      glVertex2f(FCX+70, FCY+37);
      glVertex2f(FCX+48, FCY+49);
      glVertex2f(FCX+26, FCY+37);
    glEnd();
    for (int i = 0; i < 5; i++) {
        float tx = (FCX+26) + i*11.f;
        float ty = FCY+37 + sinf((float)i*0.7f)*6.f + 12.f;
        glBegin(GL_LINES);
          glVertex2f(tx, ty-4);
          glVertex2f(tx+2, ty+6);
        glEnd();
    }
    glLineWidth(1.0f);
}

/* Nose */
void makeup_draw_nose(void)
{
    /* Simple soft nose with two nostril dots */
    draw_ellipse(FCX, FCY+8, 12, 16, COL_SKIN_DARK, 0.25f, 1);

    glColor4f(COL_SKIN_DARK, 0.55f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
      glVertex2f(FCX-8,  FCY+20);
      glVertex2f(FCX-12, FCY+0);
      glVertex2f(FCX,    FCY+4);
      glVertex2f(FCX+12, FCY+0);
      glVertex2f(FCX+8,  FCY+20);
    glEnd();
    glLineWidth(1.0f);

    /* Nostril dots */
    draw_circle(FCX-10, FCY+18, 3.5f, COL_SKIN_DARK, 0.4f, 1);
    draw_circle(FCX+10, FCY+18, 3.5f, COL_SKIN_DARK, 0.4f, 1);
}

/* Blush */
void makeup_draw_blush(void)
{
    if (G.blush == BLUSH_NONE) return;
    draw_ellipse(FCX-70, FCY+10, 28, 16, COL_BLUSH, 0.40f, 1);
    draw_ellipse(FCX+70, FCY+10, 28, 16, COL_BLUSH, 0.40f, 1);
}

/* Lips */
void makeup_draw_lips(void)
{
    float lr = G.lipR, lg = G.lipG, lb = G.lipB;

    /* Lower lip */
    draw_ellipse(FCX, FCY-30, 30, 13, lr, lg, lb, 1.f, 1);

    /* Upper lip – two lobes */
    /* left lobe */
    draw_ellipse(FCX-14, FCY-20, 16, 9, lr, lg, lb, 1.f, 1);
    /* right lobe */
    draw_ellipse(FCX+14, FCY-20, 16, 9, lr, lg, lb, 1.f, 1);
    /* centre bridge fill */
    draw_ellipse(FCX, FCY-20, 8, 6, lr, lg, lb, 1.f, 1);

    /* Lip highlight */
    draw_ellipse(FCX, FCY-30, 14, 4,
                 clampf(lr+0.25f,0,1),
                 clampf(lg+0.25f,0,1),
                 clampf(lb+0.25f,0,1), 0.55f, 1);

    /* Lip outline */
    glColor4f(clampf(lr-0.15f,0,1),
              clampf(lg-0.15f,0,1),
              clampf(lb-0.15f,0,1), 0.7f);
    glLineWidth(1.5f);
    /* lower outline */
    draw_ellipse(FCX, FCY-30, 30, 13, 0,0,0, 0, 0);
    glLineWidth(1.0f);
}

/* Composite face */
void makeup_draw_face(void)
{
    makeup_draw_hair();
    makeup_draw_neck_shoulders();
    makeup_draw_base();
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

    /* Title */
    ui_draw_title(PANEL_X + (PANEL_W-20)/2.f, WIN_H-25,
                  "Makeup Studio",
                  0.55f, 0.15f, 0.35f);

    /* ── Lipstick section ── */
    ui_draw_text(PANEL_X+18, 510, "Lipstick",
                 0.45f, 0.15f, 0.30f, GLUT_BITMAP_HELVETICA_18);
    ui_draw_divider(PANEL_X+18, 505, PANEL_X+PANEL_W-38, 505,
                    0.70f, 0.40f, 0.55f, 0.5f);

    for (int i = 0; i < 3; i++) {
        int active  = (G.lip == (LipColor)(i+1));
        ui_draw_swatch(swatch_lip[i].x, swatch_lip[i].y, SW_SZ,
                       lip_cols[i][0], lip_cols[i][1], lip_cols[i][2],
                       swatch_lip[i].hovered, active);
        /* small label below swatch */
        ui_draw_text(swatch_lip[i].x+4, swatch_lip[i].y-14,
                     lip_names[i], 0.50f,0.20f,0.35f,
                     GLUT_BITMAP_8_BY_13);
    }

    /* ── Eye Shadow section ── */
    ui_draw_text(PANEL_X+18, 396, "Eye Shadow",
                 0.30f, 0.10f, 0.55f, GLUT_BITMAP_HELVETICA_18);
    ui_draw_divider(PANEL_X+18, 391, PANEL_X+PANEL_W-38, 391,
                    0.60f, 0.30f, 0.75f, 0.5f);

    for (int i = 0; i < 3; i++) {
        int active = (G.eye == (EyeShadow)(i+1));
        ui_draw_swatch(swatch_eye[i].x, swatch_eye[i].y, SW_SZ,
                       eye_cols[i][0], eye_cols[i][1], eye_cols[i][2],
                       swatch_eye[i].hovered, active);
        ui_draw_text(swatch_eye[i].x+2, swatch_eye[i].y-14,
                     eye_names[i], 0.30f,0.10f,0.50f,
                     GLUT_BITMAP_8_BY_13);
    }

    /* ── Blush section ── */
    ui_draw_text(PANEL_X+18, 284, "Blush",
                 0.70f, 0.30f, 0.45f, GLUT_BITMAP_HELVETICA_18);
    ui_draw_divider(PANEL_X+18, 279, PANEL_X+PANEL_W-38, 279,
                    0.80f, 0.45f, 0.60f, 0.5f);
    btn_blush.active = (G.blush == BLUSH_ON);
    ui_draw_button(&btn_blush);

    /* ── Next button ── */
    ui_draw_button(&btn_next);

    /* Hint text */
    ui_draw_text(PANEL_X+18, 24,
                 "Click colours to apply makeup",
                 0.55f, 0.35f, 0.45f, GLUT_BITMAP_8_BY_13);
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
        case LIP_RED:  lip_target[0]=COL_LIP_RED;   break;
        case LIP_PINK: lip_target[0]=COL_LIP_PINK;  break;
        case LIP_NUDE: lip_target[0]=COL_LIP_NUDE;  break;
        default:       lip_target[0]=COL_LIP_DEFAULT; break;
    }
    /* Smooth transition (lerp) */
    float speed = 4.f * dt;
    G.lipR = lerpf(G.lipR, lip_target[0], clampf(speed,0,1));
    G.lipG = lerpf(G.lipG, lip_target[1], clampf(speed,0,1));
    G.lipB = lerpf(G.lipB, lip_target[2], clampf(speed,0,1));

    /* Eye shadow targets */
    switch (G.eye) {
        case EYE_BLUE:   eye_target[0]=COL_EYE_BLUE;    break;
        case EYE_PURPLE: eye_target[0]=COL_EYE_PURPLE;  break;
        case EYE_GOLD:   eye_target[0]=COL_EYE_GOLD;    break;
        default:         eye_target[0]=COL_EYE_DEFAULT; break;
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
    for (int i=0;i<3;i++) swatch_lip[i].hovered = ui_hit(&swatch_lip[i],x,y);
    for (int i=0;i<3;i++) swatch_eye[i].hovered = ui_hit(&swatch_eye[i],x,y);
    btn_blush.hovered = ui_hit(&btn_blush,x,y);
    btn_next.hovered  = ui_hit(&btn_next, x,y);
}

void makeup_mouse_click(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    makeup_ensure_init();

    /* Lipstick */
    for (int i=0;i<3;i++) {
        if (ui_hit(&swatch_lip[i],x,y)) {
            G.lip = (LipColor)(i+1);
            lip_target[0]=lip_cols[i][0];
            lip_target[1]=lip_cols[i][1];
            lip_target[2]=lip_cols[i][2];
        }
    }
    /* Eye shadow */
    for (int i=0;i<3;i++) {
        if (ui_hit(&swatch_eye[i],x,y)) {
            G.eye = (EyeShadow)(i+1);
            eye_target[0]=eye_cols[i][0];
            eye_target[1]=eye_cols[i][1];
            eye_target[2]=eye_cols[i][2];
        }
    }
    /* Blush */
    if (ui_hit(&btn_blush,x,y))
        G.blush = (G.blush == BLUSH_ON) ? BLUSH_NONE : BLUSH_ON;

    /* Next */
    if (ui_hit(&btn_next,x,y))
        stage_next();
}