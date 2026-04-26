/* =========================================================
   dressup.c  –  Stage 2: Dress-Up Stage
   Full body character; dress / hair / accessory selection.
   ========================================================= */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "dressup.h"
#include "makeup.h"
#include "ui.h"
#include "assets.h"

extern GameState G;
extern void stage_next(void);

/* ── Body / character centre ─────────────────────────────── */
#define BCX  270.f   /* body centre x */
#define BCY  320.f   /* body mid y    */

/* ── UI panel ────────────────────────────────────────────── */
#define PANEL_X2  550.f
#define PANEL_W2  380.f

/* Swatch size */
#define SW2  42.f
#define GAP2 40.f

/* ── Buttons ─────────────────────────────────────────────── */
static Button btn_finish;

/* Dress buttons */
static Button btn_dress[3];
static const char *dress_labels[] = { "Ball Gown", "Sundress", "Glam" };
static float dress_cols[3][3] = {
    {COL_DRESS_GOWN},
    {COL_DRESS_CASUAL},
    {COL_DRESS_GLAMOUR}
};

/* Hair buttons */
static Button btn_hair[3];
static const char *hair_labels[] = { "Long Wavy", "Bun", "Curly" };
static float hair_cols[3][3] = {
    {COL_HAIR_DARK},
    {COL_HAIR_MED},
    {COL_HAIR_LIGHT}
};

/* Accessory toggle buttons */
static Button btn_earrings;
static Button btn_necklace;

/* Dress fade animation */
static float dress_fade  = 1.f;
static DressStyle last_dress = DRESS_NONE;

void dressup_init(void)
{
    float sx, sy;

    /* Dress buttons */
  sy = 470.f;  // Dresses (higher)
    for (int i = 0; i < 3; i++) {
        btn_dress[i].x = PANEL_X2 + 18 + i*(SW2+GAP2);
        btn_dress[i].y = sy;
        btn_dress[i].w = SW2;
        btn_dress[i].h = SW2;
        btn_dress[i].label = dress_labels[i];
        btn_dress[i].r = dress_cols[i][0];
        btn_dress[i].g = dress_cols[i][1];
        btn_dress[i].b = dress_cols[i][2];
    }

    /* Hair buttons */
    sy = 340.f;
    for (int i = 0; i < 3; i++) {
        btn_hair[i].x = PANEL_X2 + 18 + i*(SW2+GAP2);
        btn_hair[i].y = sy;
        btn_hair[i].w = SW2;
        btn_hair[i].h = SW2;
        btn_hair[i].label = hair_labels[i];
        btn_hair[i].r = hair_cols[i][0];
        btn_hair[i].g = hair_cols[i][1];
        btn_hair[i].b = hair_cols[i][2];
    }

    /* Accessory toggles */
    btn_earrings.x = PANEL_X2+18;  btn_earrings.y = 220;
    btn_earrings.w = 120; btn_earrings.h = 34;
    btn_earrings.label = "Earrings";

    btn_necklace.x = PANEL_X2+158; btn_necklace.y = 220;
    btn_necklace.w = 120; btn_necklace.h = 34;
    btn_necklace.label = "Necklace";

    /* Finish button */
    btn_finish.x = PANEL_X2 + 60;
    btn_finish.y = 40;
    btn_finish.w = 170;
    btn_finish.h = 44;
    btn_finish.label = "Finish: Showcase >";
    btn_finish.r = 0.50f; btn_finish.g = 0.10f; btn_finish.b = 0.55f;
}

/* ── BACKGROUND ──────────────────────────────────────────── */
static void draw_dressup_bg(void)
{
    /* Gradient */
    glBegin(GL_QUADS);
      glColor3f(0.96f, 0.94f, 0.99f);
      glVertex2f(0,     0);
      glVertex2f(WIN_W, 0);
      glColor3f(0.90f, 0.88f, 0.98f);
      glVertex2f(WIN_W, WIN_H);
      glVertex2f(0,     WIN_H);
    glEnd();

    /* Floor reflection */
    glBegin(GL_QUADS);
      glColor4f(0.75f, 0.65f, 0.85f, 0.30f);
      glVertex2f(50,  80);
      glVertex2f(490, 80);
      glColor4f(0.85f, 0.80f, 0.95f, 0.05f);
      glVertex2f(490, 0);
      glVertex2f(50,  0);
    glEnd();

    /* Wardrobe frame lines */
    glColor4f(0.60f, 0.45f, 0.70f, 0.35f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(40,  20);
      glVertex2f(520, 20);
      glVertex2f(520, WIN_H-20);
      glVertex2f(40,  WIN_H-20);
    glEnd();
    glLineWidth(1.0f);

    /* Sparkle dots */
    for (int i = 0; i < 12; i++) {
        float sx2 = 50.f + i*40.f;
        float sy2 = 580.f + sinf(i*1.3f)*15.f;
        float sz  = 3.f + sinf(i*0.8f+G.animTime)*2.f;
        draw_circle(sx2, sy2, sz, 0.80f,0.60f,0.95f,
                    0.5f + sinf(G.animTime+i)*0.3f, 1);
    }
}

/* ── CHARACTER BODY ──────────────────────────────────────── */
void dressup_draw_hair(void)
{
    float bcy = BCY + sin(G.breathPhase) * 2.5f;

    switch (G.hair)
    {
    /* ================= LONG WAVY (SOFT + FLOWY) ================= */
    case HAIR_LONG:
    {
        /* BACK VOLUME (soft, not hard oval) */
        draw_ellipse(BCX, bcy+240, 65, 85, COL_HAIR_DARK, 0.9f, 1);

        /* LEFT FLOW STRANDS */
        for (int i = 0; i < 5; i++) {
            float xoff = -35 - i*6;
            float yoff = bcy + 210 - i*35;

            draw_ellipse(BCX + xoff, yoff,
                         16 - i*1.5f, 40,
                         COL_HAIR_DARK, 0.9f - i*0.1f, 1);
        }

        /* RIGHT FLOW STRANDS */
        for (int i = 0; i < 5; i++) {
            float xoff = 35 + i*6;
            float yoff = bcy + 210 - i*35;

            draw_ellipse(BCX + xoff, yoff,
                         16 - i*1.5f, 40,
                         COL_HAIR_DARK, 0.9f - i*0.1f, 1);
        }

        /* FRONT SOFT STRANDS */
        draw_ellipse(BCX-30, bcy+215, 14, 45, COL_HAIR_DARK, 1.f, 1);
        draw_ellipse(BCX+30, bcy+215, 14, 45, COL_HAIR_DARK, 1.f, 1);

        /* FACE FRAMING STRANDS (important for realism) */
        draw_ellipse(BCX-20, bcy+180, 10, 35, COL_HAIR_DARK, 0.9f, 1);
        draw_ellipse(BCX+20, bcy+180, 10, 35, COL_HAIR_DARK, 0.9f, 1);

        /* TOP SHINE */
        draw_ellipse(BCX-15, bcy+275, 30, 18, 1,1,1, 0.12f, 1);

        break;
    }

    /* ================= BUN (CLEAN + ELEGANT) ================= */
case HAIR_BUN:
{
    /* ===== BASE (narrower, lifted) ===== */
    draw_ellipse(BCX, bcy+235, 46, 58, COL_HAIR_MED, 1.f, 1);

    /* ===== SMALLER, LESS ROUND BUN ===== */
    draw_ellipse(BCX+2, bcy+300, 20, 26, COL_HAIR_MED, 1.f, 1);

    /* ===== WRAPPED HAIR EFFECT (breaks perfect shape) ===== */
    draw_ellipse(BCX-6, bcy+305, 10, 16, COL_HAIR_MED, 0.8f, 1);
    draw_ellipse(BCX+8, bcy+292, 8, 14, COL_HAIR_MED, 0.75f, 1);

    /* ===== SLIGHT ASYMMETRY (natural look) ===== */
    draw_ellipse(BCX-3, bcy+298, 6, 10, COL_HAIR_MED, 0.7f, 1);

    /* ===== FACE-FRAMING STRANDS (slimming) ===== */
    draw_ellipse(BCX-22, bcy+185, 7, 52, COL_HAIR_MED, 0.9f, 1);
    draw_ellipse(BCX+20, bcy+185, 7, 52, COL_HAIR_MED, 0.9f, 1);

    /* inward curve (prettier jaw framing) */
    draw_ellipse(BCX-16, bcy+155, 5, 36, COL_HAIR_MED, 0.7f, 1);
    draw_ellipse(BCX+14, bcy+155, 5, 36, COL_HAIR_MED, 0.7f, 1);

    /* ===== BABY HAIRS (tiny but powerful detail) ===== */
    draw_ellipse(BCX-10, bcy+205, 4, 16, COL_HAIR_MED, 0.6f, 1);
    draw_ellipse(BCX+10, bcy+205, 4, 16, COL_HAIR_MED, 0.6f, 1);

    /* ===== SUBTLE TOP HIGHLIGHT ===== */
    draw_ellipse(BCX, bcy+308, 10, 6, 1,1,1, 0.08f, 1);

    break;
}

    /* ================= CURLY (BOUNCY + NATURAL) ================= */
    case HAIR_CURLY:
    {
        /* BASE SHAPE */
        draw_ellipse(BCX, bcy+240, 75, 80, COL_HAIR_LIGHT, 0.9f, 1);

        /* RANDOMIZED CURLS (less perfect circle = more real) */
        for (int i = 0; i < 16; i++) {
            float ang = i * (2*M_PI/16);

            float radius = 55 + sinf(i)*6;  // variation
            float cx = BCX + cosf(ang) * radius;
            float cy = bcy + 230 + sinf(ang) * (45 + cosf(i)*5);

            draw_circle(cx, cy, 14 + (i%3), COL_HAIR_LIGHT, 0.95f, 1);
        }

        /* INNER DEPTH CURLS */
        for (int i = 0; i < 8; i++) {
            float ang = i * (2*M_PI/8);
            float cx = BCX + cosf(ang) * 30.f;
            float cy = bcy + 235 + sinf(ang) * 25.f;

            draw_circle(cx, cy, 10, COL_HAIR_LIGHT, 0.85f, 1);
        }

        /* FACE FRAMING CURLS */
        draw_circle(BCX-28, bcy+190, 12, COL_HAIR_LIGHT, 1.f, 1);
        draw_circle(BCX+28, bcy+190, 12, COL_HAIR_LIGHT, 1.f, 1);

        /* SOFT HIGHLIGHT */
        draw_ellipse(BCX-10, bcy+270, 25, 15, 1,1,1, 0.10f, 1);

        break;
    }

    default:
        draw_ellipse(BCX, bcy+240, 60, 80, COL_HAIR_DARK, 1.f, 1);
        break;
    }
}

/* Simplified face for body view */
void dressup_draw_face(void)
{
    float bcy = BCY + (float)sin(G.breathPhase) * 2.5f;
    float fcx = BCX, fcy = bcy + 220.f;

    /* ================= FACE BASE (SLIMMER) ================= */
    draw_ellipse(fcx, fcy+4, 44, 54, COL_SKIN, 1.f, 1);

    /* ================= JAW OVERLAY (SHARPER) ================= */
    glColor3f(COL_SKIN);
    glBegin(GL_POLYGON);

        /* Chin (slightly higher + sharp) */
        glVertex2f(fcx,      fcy - 48);

        /* Right jaw (tight inward) */
        glVertex2f(fcx + 16, fcy - 30);
        glVertex2f(fcx + 24, fcy - 6);

        /* Blend to cheek */
        glVertex2f(fcx + 22, fcy + 18);

        /* Top */
        glVertex2f(fcx,      fcy + 48);

        /* Left mirror */
        glVertex2f(fcx - 22, fcy + 18);
        glVertex2f(fcx - 24, fcy - 6);
        glVertex2f(fcx - 16, fcy - 30);

    glEnd();

    /* ================= JAW SHADOW (TIGHTER) ================= */
/* ================= STABLE V CHIN LINE ================= */
float static_fcy = BCY + 220.f;   // remove breathing from chin

glDisable(GL_BLEND);              // avoid alpha flicker
glColor3f(0.75f, 0.62f, 0.58f);  // soft warm skin shadow

glLineWidth(1.4f);

glBegin(GL_LINES);
    /* left line */
    glVertex2f(fcx - 12, static_fcy - 32);
    glVertex2f(fcx,      static_fcy - 46);

    /* right line */
    glVertex2f(fcx,      static_fcy - 46);
    glVertex2f(fcx + 12, static_fcy - 32);
glEnd();

glLineWidth(1.f);
glEnable(GL_BLEND);   // re-enable if used elsewhere

    /* ================= EARS ================= */
    draw_ellipse(fcx-44, fcy+10, 7, 12, COL_SKIN, 1.f, 1);
    draw_ellipse(fcx+44, fcy+10, 7, 12, COL_SKIN, 1.f, 1);

    draw_ellipse(fcx-44, fcy+8, 4, 6, COL_SKIN_DARK, 0.25f, 1);
    draw_ellipse(fcx+44, fcy+8, 4, 6, COL_SKIN_DARK, 0.25f, 1);

    /* ================= EYE SHADOW ================= */
    if (G.eye != EYE_NONE) {
        draw_ellipse(fcx-20, fcy+20, 14, 7, G.eyeR,G.eyeG,G.eyeB, 0.45f, 1);
        draw_ellipse(fcx+20, fcy+20, 14, 7, G.eyeR,G.eyeG,G.eyeB, 0.45f, 1);
    }

    /* ================= EYES ================= */
    draw_ellipse(fcx-20, fcy+18, 11, 7, 0.97f,0.97f,0.97f, 1.f, 1);
    draw_circle (fcx-20, fcy+18, 5,  0.15f,0.08f,0.04f, 1.f, 1);
    draw_circle (fcx-20, fcy+18, 2.5f, 0.02f,0.01f,0.01f, 1.f, 1);
    draw_circle (fcx-17, fcy+21, 1.5f, 1,1,1, 0.9f, 1);

    draw_ellipse(fcx+20, fcy+18, 11, 7, 0.97f,0.97f,0.97f, 1.f, 1);
    draw_circle (fcx+20, fcy+18, 5,  0.15f,0.08f,0.04f, 1.f, 1);
    draw_circle (fcx+20, fcy+18, 2.5f, 0.02f,0.01f,0.01f, 1.f, 1);
    draw_circle (fcx+23, fcy+21, 1.5f, 1,1,1, 0.9f, 1);

    /* Eyelids */
    glColor3f(0.2f,0.1f,0.1f);
    glLineWidth(2.f);

    glBegin(GL_LINE_STRIP);
        glVertex2f(fcx-30, fcy+20);
        glVertex2f(fcx-20, fcy+23);
        glVertex2f(fcx-10, fcy+20);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(fcx+10, fcy+20);
        glVertex2f(fcx+20, fcy+23);
        glVertex2f(fcx+30, fcy+20);
    glEnd();

    glLineWidth(1.f);

    /* ================= EYEBROWS ================= */
    glColor3f(COL_HAIR_DARK);
    glLineWidth(3.f);

    glBegin(GL_LINE_STRIP);
        glVertex2f(fcx-34, fcy+34);
        glVertex2f(fcx-20, fcy+41);
        glVertex2f(fcx-6,  fcy+36);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(fcx+34, fcy+34);
        glVertex2f(fcx+20, fcy+41);
        glVertex2f(fcx+6,  fcy+36);
    glEnd();

    glLineWidth(1.f);

    /* ================= NOSE ================= */
    draw_ellipse(fcx, fcy+4, 6, 8, COL_SKIN_DARK, 0.20f, 1);

    /* ================= BLUSH (SOFTER) ================= */
    if (G.blush) {
        draw_ellipse(fcx-30, fcy+8, 10, 5, COL_BLUSH, 0.22f, 1);
        draw_ellipse(fcx+30, fcy+8, 10, 5, COL_BLUSH, 0.22f, 1);
    }

    /* ================= LIPS (MORE GAP FROM NOSE) ================= */
    draw_ellipse(fcx, fcy-20, 14, 6, G.lipR,G.lipG,G.lipB, 1.f, 1);
    draw_ellipse(fcx-6, fcy-14, 8, 5, G.lipR,G.lipG,G.lipB, 1.f, 1);
    draw_ellipse(fcx+6, fcy-14, 8, 5, G.lipR,G.lipG,G.lipB, 1.f, 1);

}

void dressup_draw_body(void)
{
    /* breathing bob */
    float bob = (float)sin(G.breathPhase) * 3.f;

    /* Neck */
    glColor3f(COL_SKIN);
    glBegin(GL_QUADS);
      glVertex2f(BCX-14, BCY+165+bob);
      glVertex2f(BCX+14, BCY+165+bob);
      glVertex2f(BCX+18, BCY+140+bob);
      glVertex2f(BCX-18, BCY+140+bob);
    glEnd();
/* Upper torso (visible part) */
draw_ellipse(BCX, BCY+110, 38, 60, COL_SKIN, 1.f, 1);

/* REMOVE or shrink waist completely */
draw_ellipse(BCX, BCY+75, 18, 10, COL_SKIN, 1.f, 1);

    /* Arms */
/* ===== SHOULDERS (balanced width) ===== */
draw_ellipse(BCX-36, BCY+120+bob, 20, 14, COL_SKIN, 1.f, 1);
draw_ellipse(BCX+36, BCY+120+bob, 20, 14, COL_SKIN, 1.f, 1);


/* ===== UPPER ARMS (moved outward slightly) ===== */
draw_ellipse(BCX-48, BCY+90+bob, 14, 55, COL_SKIN, 1.f, 1);
draw_ellipse(BCX+48, BCY+90+bob, 14, 55, COL_SKIN, 1.f, 1);


/* ===== ELBOW JOINT ===== */
draw_circle(BCX-46, BCY+55+bob, 6.5f, COL_SKIN, 1.f, 1);
draw_circle(BCX+46, BCY+55+bob, 6.5f, COL_SKIN, 1.f, 1);


/* ===== LOWER ARMS ===== */
draw_ellipse(BCX-44, BCY+25+bob, 12, 28, COL_SKIN, 1.f, 1);
draw_ellipse(BCX+44, BCY+25+bob, 12, 28, COL_SKIN, 1.f, 1);

  /* ===== HIP TRANSITION (VERY IMPORTANT FIX) ===== */
draw_ellipse(BCX, BCY+20, 30, 35, COL_SKIN, 1.f, 1);

/* ===== UPPER THIGHS (closer, slightly inward) ===== */
draw_ellipse(BCX-14, BCY-40, 16, 70, COL_SKIN, 1.f, 1);
draw_ellipse(BCX+14, BCY-40, 16, 70, COL_SKIN, 1.f, 1);

/* ===== LOWER LEGS ===== */
draw_ellipse(BCX-16, BCY-110, 14, 75, COL_SKIN, 1.f, 1);
draw_ellipse(BCX+16, BCY-110, 14, 75, COL_SKIN, 1.f, 1);

/* ===== FEET ===== */
draw_ellipse(BCX-16, BCY-175, 18, 10, COL_SKIN_DARK, 1.f, 1);
draw_ellipse(BCX+16, BCY-175, 18, 10, COL_SKIN_DARK, 1.f, 1);
}

/* Draw chosen dress */
void dressup_draw_dress(void)
{
    float bob  = (float)sin(G.breathPhase) * 3.f;
    float fade = dress_fade;

    float dr=0, dg=0, db=0;
    switch (G.dress) {
        case DRESS_GOWN:    dr=COL_DRESS_GOWN;    break;
        case DRESS_CASUAL:  dr=COL_DRESS_CASUAL;  break;
        case DRESS_GLAMOUR: dr=COL_DRESS_GLAMOUR; break;
        default: break;
    }

    glColor4f(dr, dg, db, fade);

    switch (G.dress)
    {
    /* ================= BALL GOWN ================= */
    case DRESS_GOWN:
    {
        /* LOWERED bodice → creates neck space */
        draw_ellipse(BCX, BCY+115+bob, 50, 60, dr,dg,db, fade, 1);

        /* neckline dip */
        draw_ellipse(BCX, BCY+155+bob, 28, 18, COL_SKIN, 1.f, 1);

        /* waist */
        draw_ellipse(BCX, BCY+70+bob, 38, 18, dr-0.1f,dg,db-0.05f, fade, 1);

        /* skirt */
        draw_ellipse(BCX, BCY-40, 140, 120, dr,dg,db, fade, 1);
        draw_ellipse(BCX, BCY-80, 160, 110, dr,dg,db, fade*0.8f, 1);

        break;
    }

    /* ================= BODYCON CASUAL ================= */
case DRESS_CASUAL:
{
    /* ===== MAIN BODY (STATIC — NO bob) ===== */
    draw_ellipse(BCX, BCY+30, 48, 140, dr,dg,db, fade, 1);

    /* ===== V NECK CUT (can slightly follow breathing) ===== */
    glColor3f(COL_SKIN);
    glBegin(GL_TRIANGLES);
        glVertex2f(BCX-18, BCY+150+bob);
        glVertex2f(BCX+18, BCY+150+bob);
        glVertex2f(BCX,    BCY+120+bob);
    glEnd();

    /* waist shaping (static) */
    draw_ellipse(BCX, BCY+60, 36, 38, dr-0.05f,dg,db, fade, 1);

    /* subtle shading */
    draw_ellipse(BCX+10, BCY+10, 10, 110, 0,0,0, 0.08f, 1);

    break;
}   

    /* ================= GLAM (NEW STYLE - NO SLIT) ================= */
 case DRESS_GLAMOUR:
{
    /* upper dress base */
    draw_ellipse(BCX, BCY+110+bob, 55, 60, dr,dg,db, fade, 1);

    /* ===== SWEETHEART NECK ===== */
    glColor3f(COL_SKIN);

    /* left curve */
    draw_ellipse(BCX-15, BCY+145+bob, 20, 15, COL_SKIN, 1.f, 1);

    /* right curve */
    draw_ellipse(BCX+15, BCY+145+bob, 20, 15, COL_SKIN, 1.f, 1);

    /* center dip */
    draw_ellipse(BCX, BCY+135+bob, 12, 10, COL_SKIN, 1.f, 1);

    /* fitted waist */
    draw_ellipse(BCX, BCY+50+bob, 45, 70, dr,dg,db, fade, 1);

    /* mermaid bottom */
    draw_ellipse(BCX, BCY-70, 70, 120, dr,dg,db, fade, 1);
    draw_ellipse(BCX, BCY-120, 95, 90, dr,dg,db, fade*0.85f, 1);

    /* sparkle */
    for (int i = 0; i < 25; i++) {
        float sx = BCX - 30.f + (i % 6)*12.f;
        float sy = BCY + 60.f - (i/6)*35.f;
        float glow = 0.7f + sinf(G.animTime*3+i)*0.3f;
        draw_circle(sx, sy, 3, glow, glow*0.8f, glow, fade, 1);
    }

    break;
}

    default:
{
    float bob = (float)sin(G.breathPhase) * 3.f;

    /* ===== SIMPLE TOP ===== */
    draw_ellipse(BCX, BCY+95+bob, 42, 32,
                 0.85f, 0.75f, 0.90f, 1.f, 1);

    /* slight highlight */
    draw_ellipse(BCX+5, BCY+88+bob, 12, 20,
                 1,1,1, 0.08f, 1);

    /* ===== SIMPLE SHORTS / PANTY STYLE ===== */
    draw_ellipse(BCX-12, BCY+40+bob, 20, 35,
                 0.75f, 0.60f, 0.85f, 1.f, 1);

    draw_ellipse(BCX+12, BCY+40+bob, 20, 35,
                 0.75f, 0.60f, 0.85f, 1.f, 1);

    /* center seam */
    glColor3f(0.6f, 0.45f, 0.75f);
    glLineWidth(2.f);
    glBegin(GL_LINES);
        glVertex2f(BCX, BCY+55+bob);
        glVertex2f(BCX, BCY+25+bob);
    glEnd();
    glLineWidth(1.f);

    break;
}
    }
}
/* Draw accessories */
void dressup_draw_accessories(void)
{
    float bob = (float)sin(G.breathPhase) * 2.5f;
    float fcy = BCY + 220.f + bob;  /* approx face Y */

    int has_earrings = (G.accessories == ACC_EARRINGS || G.accessories == ACC_BOTH);
    int has_necklace = (G.accessories == ACC_NECKLACE || G.accessories == ACC_BOTH);

if (has_earrings) {

    float ear_x_l = BCX - 44;
    float ear_x_r = BCX + 44;

    /* ear lobe reference (must match face ear position) */
    float ear_y = fcy -10;   // align with lower ear region

    /* LEFT EARRING (starts from ear end) */
    draw_circle(ear_x_l, ear_y, 4.5f, 0.85f,0.70f,0.10f, 1.f, 1);
    draw_circle(ear_x_l, ear_y - 10, 3.2f, 0.85f,0.70f,0.10f, 1.f, 1);

    glColor3f(0.85f,0.70f,0.10f);
    glBegin(GL_LINES);
        glVertex2f(ear_x_l, ear_y);
        glVertex2f(ear_x_l, ear_y + 10);
    glEnd();

    /* RIGHT EARRING */
    draw_circle(ear_x_r, ear_y, 4.5f, 0.85f,0.70f,0.10f, 1.f, 1);
    draw_circle(ear_x_r, ear_y - 10, 3.2f, 0.85f,0.70f,0.10f, 1.f, 1);

    glBegin(GL_LINES);
        glVertex2f(ear_x_r, ear_y);
        glVertex2f(ear_x_r, ear_y + 10);
    glEnd();
}

   if (has_necklace) {

    /* proper neck base */
    float neck_y = BCY + 150.f + bob;

    /* slightly curved collar line reference */
    float radius_x = 30.f;
    float radius_y = 10.f;

    glColor3f(0.95f, 0.92f, 0.85f);

    int np = 13;

    for (int i = 0; i < np; i++) {

        float t = (float)i / (np - 1);
        float ang = 3.1415f * (0.15f + t * 0.7f);

        float px = BCX + cosf(ang) * radius_x;
        float py = neck_y - sinf(ang) * radius_y;

        draw_circle(px, py, 4.2f, 0.95f,0.92f,0.85f, 1.f, 1);

        /* shine */
        draw_circle(px+1, py+1, 1.5f, 1,0.95f,0.8f, 0.5f, 1);
    }
}
}

/* ── PANEL UI ─────────────────────────────────────────────── */
static void draw_panel_ui2(void)
{
    /* Panel background */
    draw_rounded_rect(PANEL_X2, 20, PANEL_W2-15, WIN_H-40, 16,
                      0.96f, 0.94f, 0.99f, 0.93f, 1);

    ui_draw_title(PANEL_X2 + (PANEL_W2-15)/2.f, WIN_H-25,
                  "Dress-Up Room",
                  0.40f, 0.10f, 0.60f);

    /* ── Dresses ── */
    ui_draw_text(PANEL_X2+18, 540, "Dress Style",
                 0.40f, 0.10f, 0.55f, GLUT_BITMAP_HELVETICA_18);
    ui_draw_divider(PANEL_X2+18, 513, PANEL_X2+PANEL_W2-33, 513,
                    0.55f, 0.25f, 0.75f, 0.5f);
    for (int i = 0; i < 3; i++) {
        int active = (G.dress == (DressStyle)(i+1));
        ui_draw_swatch(btn_dress[i].x, btn_dress[i].y, SW2,
                       dress_cols[i][0], dress_cols[i][1], dress_cols[i][2],
                       btn_dress[i].hovered, active);
ui_draw_text(
    btn_dress[i].x+4,
    btn_dress[i].y - 30,
    dress_labels[i],
    0.35f,0.10f,0.50f,
    GLUT_BITMAP_8_BY_13
);
    }

    /* ── Hairstyle ── */
    ui_draw_text(PANEL_X2+18, 410, "Hairstyle",
                 0.40f, 0.10f, 0.55f, GLUT_BITMAP_HELVETICA_18);
    ui_draw_divider(PANEL_X2+18, 393, PANEL_X2+PANEL_W2-33, 393,
                    0.55f, 0.25f, 0.75f, 0.5f);
    for (int i = 0; i < 3; i++) {
        int active = (G.hair == (HairStyle)(i+1));
        ui_draw_swatch(btn_hair[i].x, btn_hair[i].y, SW2,
                       hair_cols[i][0], hair_cols[i][1], hair_cols[i][2],
                       btn_hair[i].hovered, active);
ui_draw_text(
    btn_hair[i].x + 4,
    btn_hair[i].y - 30,
    hair_labels[i],
    0.35f,0.10f,0.50f,
    GLUT_BITMAP_8_BY_13
);
    }

    /* ── Accessories ── */
    ui_draw_text(PANEL_X2+18, 278, "Accessories",
                 0.40f, 0.10f, 0.55f, GLUT_BITMAP_HELVETICA_18);
    ui_draw_divider(PANEL_X2+18, 273, PANEL_X2+PANEL_W2-33, 273,
                    0.55f, 0.25f, 0.75f, 0.5f);
    btn_earrings.active = (G.accessories==ACC_EARRINGS||G.accessories==ACC_BOTH);
    btn_necklace.active = (G.accessories==ACC_NECKLACE||G.accessories==ACC_BOTH);
    ui_draw_button(&btn_earrings);
    ui_draw_button(&btn_necklace);

    /* ── Finish button ── */
    ui_draw_button(&btn_finish);

    /* Hint */
    ui_draw_text(PANEL_X2+10, 24,
                 "Select dress, hair & accessories",
                 0.45f, 0.25f, 0.55f, GLUT_BITMAP_8_BY_13);
}

/* ── PUBLIC API ──────────────────────────────────────────── */
static int dressup_inited = 0;
static void ensure_init(void) {
    if (dressup_inited) return;
    dressup_inited = 1;
    dressup_init();
    last_dress = G.dress;
    dress_fade = 1.f;
}

void dressup_draw(void)
{
    ensure_init();
    draw_dressup_bg();
    dressup_draw_hair();
    dressup_draw_body();
    dressup_draw_dress();
    dressup_draw_face();
    dressup_draw_accessories();
    draw_panel_ui2();
}

void dressup_update(float dt)
{
    ensure_init();
    /* dress fade animation */
    if (G.dress != last_dress) {
        dress_fade = 0.f;
        last_dress = G.dress;
    }
    if (dress_fade < 1.f)
        dress_fade = clampf(dress_fade + dt * 3.5f, 0.f, 1.f);
    G.dressFade = dress_fade;

    /* Breathing */
    G.breathPhase += dt * 1.2f;
}

void dressup_mouse_move(int x, int y)
{
    ensure_init();
    for (int i=0;i<3;i++) btn_dress[i].hovered = ui_hit(&btn_dress[i],x,y);
    for (int i=0;i<3;i++) btn_hair[i].hovered  = ui_hit(&btn_hair[i], x,y);
    btn_earrings.hovered = ui_hit(&btn_earrings, x, y);
    btn_necklace.hovered = ui_hit(&btn_necklace, x, y);
    btn_finish.hovered   = ui_hit(&btn_finish,   x, y);
}

void dressup_mouse_click(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    ensure_init();

    /* Dress selection */
    for (int i=0;i<3;i++) {
        if (ui_hit(&btn_dress[i],x,y)) {
            G.dress = (DressStyle)(i+1);
            dress_fade = 0.f;
            last_dress = G.dress;
        }
    }
    /* Hair selection */
    for (int i=0;i<3;i++) {
        if (ui_hit(&btn_hair[i],x,y))
            G.hair = (HairStyle)(i+1);
    }
    /* Accessories */
    if (ui_hit(&btn_earrings,x,y)) {
        if (G.accessories == ACC_EARRINGS)     G.accessories = ACC_NONE;
        else if (G.accessories == ACC_BOTH)    G.accessories = ACC_NECKLACE;
        else if (G.accessories == ACC_NECKLACE) G.accessories = ACC_BOTH;
        else                                    G.accessories = ACC_EARRINGS;
    }
    if (ui_hit(&btn_necklace,x,y)) {
        if (G.accessories == ACC_NECKLACE)     G.accessories = ACC_NONE;
        else if (G.accessories == ACC_BOTH)    G.accessories = ACC_EARRINGS;
        else if (G.accessories == ACC_EARRINGS) G.accessories = ACC_BOTH;
        else                                    G.accessories = ACC_NECKLACE;
    }
    /* Finish */
    if (ui_hit(&btn_finish,x,y))
        stage_next();
}