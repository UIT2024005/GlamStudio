/* =========================================================
   showcase.c  –  Stage 3: Final Showcase Stage
   Character on left; decorative stage on right.
   Animations: blinking, breathing, hair sway, sparkles.
   ========================================================= */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "showcase.h"
#include "dressup.h"
#include "ui.h"
#include "assets.h"

extern GameState G;
#define dress_fade 1.0f

/* ── Character position (left of screen) ────────────────── */
#define SCX  220.f
#define SCY  305.f
#define BCX SCX
#define BCY SCY

static int sc_inited = 0;
static void sc_init(void)
{
    G.poseActive = 1;

}

/* ── STAGE BACKGROUND ────────────────────────────────────── */
static void draw_sc_bg(void)
{
    /* Deep plum backdrop */
    glBegin(GL_QUADS);
      glColor3f(0.08f, 0.04f, 0.12f);
      glVertex2f(0,     0);
      glVertex2f(WIN_W, 0);
      glColor3f(0.15f, 0.08f, 0.22f);
      glVertex2f(WIN_W, WIN_H);
      glVertex2f(0,     WIN_H);
    glEnd();

    /* Spotlight cone from top-left onto character */
    glBegin(GL_TRIANGLE_FAN);
      glColor4f(1.0f, 0.95f, 0.80f, 0.12f);
      glVertex2f(SCX, WIN_H+60);
      glColor4f(1.0f, 0.95f, 0.80f, 0.00f);
      glVertex2f(SCX-200, 0);
      glVertex2f(SCX+200, 0);
    glEnd();

    /* Second spotlight (wider, softer) */
    glBegin(GL_TRIANGLE_FAN);
      glColor4f(0.85f, 0.70f, 1.00f, 0.07f);
      glVertex2f(SCX+100, WIN_H+60);
      glColor4f(0.85f, 0.70f, 1.00f, 0.00f);
      glVertex2f(SCX-100, 0);
      glVertex2f(SCX+300, 0);
    glEnd();

    /* Stage floor */
    glBegin(GL_QUADS);
      glColor4f(0.20f, 0.10f, 0.30f, 1.0f);
      glVertex2f(0,     0);
      glVertex2f(WIN_W, 0);
      glColor4f(0.12f, 0.06f, 0.20f, 1.0f);
      glVertex2f(WIN_W, 80);
      glVertex2f(0,     80);
    glEnd();

    /* Floor reflection */
    glBegin(GL_QUADS);
      glColor4f(0.80f, 0.60f, 1.00f, 0.15f);
      glVertex2f(SCX-90, 80);
      glVertex2f(SCX+90, 80);
      glColor4f(0.80f, 0.60f, 1.00f, 0.00f);
      glVertex2f(SCX+110, 0);
      glVertex2f(SCX-110, 0);
    glEnd();

    /* Curtain panels */
    /* Left curtain */
    glBegin(GL_QUADS);
      glColor4f(0.50f, 0.05f, 0.25f, 0.85f);
      glVertex2f(0,    0);
      glVertex2f(60,   0);
      glColor4f(0.40f, 0.02f, 0.18f, 0.85f);
      glVertex2f(80,   WIN_H);
      glVertex2f(0,    WIN_H);
    glEnd();
    /* Right curtain */
    glBegin(GL_QUADS);
      glColor4f(0.50f, 0.05f, 0.25f, 0.85f);
      glVertex2f(WIN_W,    0);
      glVertex2f(WIN_W-60, 0);
      glColor4f(0.40f, 0.02f, 0.18f, 0.85f);
      glVertex2f(WIN_W-80, WIN_H);
      glVertex2f(WIN_W,    WIN_H);
    glEnd();

    /* Gold curtain trim */
    glColor4f(0.85f, 0.70f, 0.15f, 0.8f);
    glLineWidth(3.f);
    glBegin(GL_LINES);
      glVertex2f(60,       0);
      glVertex2f(80,       WIN_H);
      glVertex2f(WIN_W-60, 0);
      glVertex2f(WIN_W-80, WIN_H);
    glEnd();
    glLineWidth(1.f);

    /* Animated sparkle stars */
    float t = G.animTime;
    for (int i = 0; i < 28; i++) {
        float sx = 80.f  + (float)(i*31 % 760);
        float sy = 110.f + (float)(i*47 % 520);
        float pulse = sinf(t * 2.5f + i * 0.7f);
        float alpha = 0.3f + pulse * 0.3f;
        float sz    = 1.5f + pulse * 1.5f;
        /* Star shape: 4 lines */
        glColor4f(1.0f, 0.95f, 0.70f, alpha);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
          glVertex2f(sx-sz*2, sy); glVertex2f(sx+sz*2, sy);
          glVertex2f(sx, sy-sz*2); glVertex2f(sx, sy+sz*2);
          glVertex2f(sx-sz, sy-sz); glVertex2f(sx+sz, sy+sz);
          glVertex2f(sx+sz, sy-sz); glVertex2f(sx-sz, sy+sz);
        glEnd();
        glLineWidth(1.f);
    }

    /* Info panel on the right */
    draw_rounded_rect(470, 80, 390, WIN_H-100, 18,
                      0.18f, 0.08f, 0.28f, 0.85f, 1);
    draw_rounded_rect(470, 80, 390, WIN_H-100, 18,
                      0.75f, 0.50f, 0.90f, 0.4f, 0);
}

/* ── SHOWCASE HAIR ───────────────────────────────────────── */
static void sc_draw_hair(void)
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

/* ── SHOWCASE FACE ───────────────────────────────────────── */
static void sc_draw_face(void)
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
float blink = sinf(G.animTime * 3.0f);  
int eyesClosed = (blink > 0.95f);  // blink moment

if (eyesClosed)
{
    /* CLOSED EYES */
    glColor3f(0.2f,0.1f,0.1f);

    glBegin(GL_LINES);
        glVertex2f(fcx-30, fcy+18);
        glVertex2f(fcx-10, fcy+18);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(fcx+10, fcy+18);
        glVertex2f(fcx+30, fcy+18);
    glEnd();
}
else
{
    /* OPEN EYES (your existing code) */
    draw_ellipse(fcx-20, fcy+18, 11, 7, 0.97f,0.97f,0.97f, 1.f, 1);
    draw_circle (fcx-20, fcy+18, 5,  0.15f,0.08f,0.04f, 1.f, 1);
    draw_circle (fcx-20, fcy+18, 2.5f, 0.02f,0.01f,0.01f, 1.f, 1);
    draw_circle (fcx-17, fcy+21, 1.5f, 1,1,1, 0.9f, 1);

    draw_ellipse(fcx+20, fcy+18, 11, 7, 0.97f,0.97f,0.97f, 1.f, 1);
    draw_circle (fcx+20, fcy+18, 5,  0.15f,0.08f,0.04f, 1.f, 1);
    draw_circle (fcx+20, fcy+18, 2.5f, 0.02f,0.01f,0.01f, 1.f, 1);
    draw_circle (fcx+23, fcy+21, 1.5f, 1,1,1, 0.9f, 1);
}

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

/* ── SHOWCASE BODY ───────────────────────────────────────── */
static void sc_draw_body(void)
{
    /* breathing bob */
    float bob = (float)sin(G.breathPhase) * 3.f;

    /* POSE FACTOR */
float t = G.poseCycle;
float pose = 0.0f;

/* 0 - 1s: straight → arms start lifting */
if (t < 1.0f)
    pose = t / 1.0f;

/* 1 - 2s: full hands on waist */
else if (t < 2.5f)
    pose = 1.0f;

/* 2.5 - 4s: go back down */
else
    pose = 1.0f - ((t - 2.5f) / 1.5f);

    /* Neck */
    glColor3f(COL_SKIN);
    glBegin(GL_QUADS);
      glVertex2f(BCX-14, BCY+165+bob + pose*6);
      glVertex2f(BCX+14, BCY+165+bob + pose*6);
      glVertex2f(BCX+18, BCY+140+bob + pose*6);
      glVertex2f(BCX-18, BCY+140+bob + pose*6);
    glEnd();

    /* Upper torso */
    draw_ellipse(BCX, BCY+110 + pose*5, 38, 60, COL_SKIN, 1.f, 1);

    /* waist */
    draw_ellipse(BCX, BCY+75 + pose*3, 18, 10, COL_SKIN, 1.f, 1);

    /* ===== SHOULDERS ===== */
    draw_ellipse(BCX-36, BCY+120+bob + pose*3, 20, 14, COL_SKIN, 1.f, 1);
    draw_ellipse(BCX+36, BCY+120+bob + pose*3, 20, 14, COL_SKIN, 1.f, 1);

    /* ===== UPPER ARMS (pose lift effect) ===== */
     draw_ellipse(BCX-48, BCY+90+bob + pose*8, 14, 55, COL_SKIN, 1.f, 1); 
     draw_ellipse(BCX+48, BCY+90+bob + pose*8, 14, 55, COL_SKIN, 1.f, 1);
      /* ===== ELBOW JOINT ===== */ 
      draw_circle(BCX-46, BCY+55+bob + pose*8, 6.5f, COL_SKIN, 1.f, 1); 
      draw_circle(BCX+46, BCY+55+bob + pose*8, 6.5f, COL_SKIN, 1.f, 1); 
    /* ===== LOWER ARMS ===== */
     draw_ellipse(BCX-44, BCY+25+bob + pose*5, 12, 28, COL_SKIN, 1.f, 1); 
     draw_ellipse(BCX+44, BCY+25+bob + pose*5, 12, 28, COL_SKIN, 1.f, 1);

    /* ===== HIP TRANSITION ===== */
    draw_ellipse(BCX, BCY+20 + pose*2, 30, 35, COL_SKIN, 1.f, 1);

    /* ===== UPPER THIGHS ===== */
    draw_ellipse(BCX-14, BCY-40 + pose*2, 16, 70, COL_SKIN, 1.f, 1);
    draw_ellipse(BCX+14, BCY-40 + pose*2, 16, 70, COL_SKIN, 1.f, 1);

    /* ===== LOWER LEGS ===== */
    draw_ellipse(BCX-16, BCY-110, 14, 75, COL_SKIN, 1.f, 1);
    draw_ellipse(BCX+16, BCY-110, 14, 75, COL_SKIN, 1.f, 1);

    /* ===== FEET ===== */
    draw_ellipse(BCX-16, BCY-175, 18, 10, COL_SKIN_DARK, 1.f, 1);
    draw_ellipse(BCX+16, BCY-175, 18, 10, COL_SKIN_DARK, 1.f, 1);
}

/* ── SHOWCASE DRESS ──────────────────────────────────────── */
static void sc_draw_dress(void)
{
     float bob  = (float)sin(G.breathPhase) * 3.f;
   float fade = G.dressFade;

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

/* ── SHOWCASE ACCESSORIES ────────────────────────────────── */
static void sc_draw_accessories(void)
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

/* ── SHOWCASE INFO PANEL ─────────────────────────────────── */
static void draw_info_panel(void)
{
    /* Main title */
    ui_draw_title(680, WIN_H-50,
                  "Your Final Look!",
                  0.95f, 0.85f, 1.00f);

    /* Sparkle divider */
    ui_draw_divider(490, WIN_H-68, 840, WIN_H-68,
                    0.80f, 0.60f, 1.00f, 0.6f);

    /* Makeup summary */
    ui_draw_text(495, WIN_H-100, "Makeup:",
                 0.85f, 0.70f, 1.00f, GLUT_BITMAP_HELVETICA_18);

    const char *lip_str =
        (G.lip==LIP_RED)  ? "Red Lipstick"  :
        (G.lip==LIP_PINK) ? "Pink Lipstick" :
        (G.lip==LIP_NUDE) ? "Nude Lipstick" : "No Lipstick";
    const char *eye_str =
        (G.eye==EYE_BLUE)   ? "Blue Eye Shadow"   :
        (G.eye==EYE_PURPLE) ? "Purple Eye Shadow" :
        (G.eye==EYE_GOLD)   ? "Gold Eye Shadow"   : "No Eye Shadow";

    ui_draw_text(510, WIN_H-128, lip_str,
                 1.0f, 0.90f, 0.95f, GLUT_BITMAP_8_BY_13);
    ui_draw_text(510, WIN_H-148, eye_str,
                 1.0f, 0.90f, 0.95f, GLUT_BITMAP_8_BY_13);
ui_draw_text(510, WIN_H-168,
    (G.blush==BLUSH_ROSE)  ? "Blush: Rose"  :
    (G.blush==BLUSH_PEACH) ? "Blush: Peach" :
    (G.blush==BLUSH_PINK)  ? "Blush: Pink"  :
    "Blush: None",
    1.0f, 0.90f, 0.95f, GLUT_BITMAP_8_BY_13);

    ui_draw_divider(490, WIN_H-185, 840, WIN_H-185,
                    0.70f, 0.50f, 0.90f, 0.4f);

    /* Dress summary */
    ui_draw_text(495, WIN_H-210, "Outfit:",
                 0.85f, 0.70f, 1.00f, GLUT_BITMAP_HELVETICA_18);

    const char *dress_str =
        (G.dress==DRESS_GOWN)    ? "Ball Gown"     :
        (G.dress==DRESS_CASUAL)  ? "Casual Sundress" :
        (G.dress==DRESS_GLAMOUR) ? "Glamour Dress" : "No Dress";
    const char *hair_str =
        (G.hair==HAIR_LONG)  ? "Long Wavy Hair"  :
        (G.hair==HAIR_BUN)   ? "Classic Bun"     :
        (G.hair==HAIR_CURLY) ? "Voluminous Curls" : "Default Hair";

    ui_draw_text(510, WIN_H-238, dress_str,
                 1.0f, 0.90f, 0.95f, GLUT_BITMAP_8_BY_13);
    ui_draw_text(510, WIN_H-258, hair_str,
                 1.0f, 0.90f, 0.95f, GLUT_BITMAP_8_BY_13);

    const char *acc_str =
        (G.accessories==ACC_BOTH)     ? "Earrings & Necklace" :
        (G.accessories==ACC_EARRINGS) ? "Earrings"           :
        (G.accessories==ACC_NECKLACE) ? "Necklace"           : "No Accessories";
    ui_draw_text(510, WIN_H-278, acc_str,
                 1.0f, 0.90f, 0.95f, GLUT_BITMAP_8_BY_13);

    ui_draw_divider(490, WIN_H-295, 840, WIN_H-295,
                    0.70f, 0.50f, 0.90f, 0.4f);

    /* Compliment message */
    float t = G.animTime;
    float pulse = 0.85f + sinf(t*2.0f)*0.15f;
    ui_draw_text(510, WIN_H-330,
                 "You look absolutely stunning!",
                 pulse, pulse*0.80f, 1.0f,
                 GLUT_BITMAP_HELVETICA_18);
    ui_draw_text(510, WIN_H-355,
                 "A true fashion icon!",
                 0.95f, 0.80f, 1.00f, GLUT_BITMAP_8_BY_13);

    /* Animated sparkle trail near text */
    for (int i = 0; i < 5; i++) {
        float sx = 490.f + i * 70.f;
        float sy = WIN_H - 380.f;
        float sz = 2.f + sinf(t*3.f + i*1.2f)*2.f;
        float al = 0.4f + sinf(t*2.5f + i*0.8f)*0.35f;
        glColor4f(0.90f, 0.75f, 1.00f, al);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
          glVertex2f(sx-sz, sy); glVertex2f(sx+sz, sy);
          glVertex2f(sx, sy-sz); glVertex2f(sx, sy+sz);
        glEnd();
        glLineWidth(1.f);
    }
}

/* ── PUBLIC API ──────────────────────────────────────────── */
void showcase_draw(void)
{
    sc_init();
    draw_sc_bg();
    sc_draw_hair();
    sc_draw_body();
    sc_draw_dress();
    sc_draw_face();
    sc_draw_accessories();
    draw_info_panel();
}

void showcase_update(float dt)
{
    G.poseCycle += dt;

    if (G.poseCycle > 3.0f)
        G.poseCycle = 0.0f;

    /* waving phase only */
    G.poseActive = 1;   // always enable pose now (we use cycle for motion)
}

void showcase_mouse_move(int x, int y)
{
    (void)x;
    (void)y;
}

void showcase_mouse_click(int button, int state, int x, int y)
{
    (void)button;
    (void)state;
    (void)x;
    (void)y;
}