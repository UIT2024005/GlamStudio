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
extern void stage_restart(void);

/* ── Character position (left of screen) ────────────────── */
#define SCX  220.f
#define SCY  305.f

/* ── Restart button ──────────────────────────────────────── */
static Button btn_restart;
static int sc_inited = 0;

static void sc_init(void)
{
    if (sc_inited) return;
    sc_inited = 1;
    btn_restart.x = 600;
    btn_restart.y = 42;
    btn_restart.w = 200;
    btn_restart.h = 46;
    btn_restart.label = "Restart Game";
    btn_restart.r = 0.30f;
    btn_restart.g = 0.10f;
    btn_restart.b = 0.45f;
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
    float bob   = sinf(G.breathPhase) * 2.f;
    float sway  = G.hairSway;   /* subtle left-right */
    float fcy   = SCY + 220.f + bob;

    switch (G.hair) {
        case HAIR_LONG:
            draw_ellipse(SCX+sway*0.4f, fcy+10, 58, 90,
                         COL_HAIR_DARK, 1.f, 1);
            draw_ellipse(SCX-54+sway, fcy-30, 16, 80,
                         COL_HAIR_DARK, 1.f, 1);
            draw_ellipse(SCX+54+sway, fcy-30, 16, 80,
                         COL_HAIR_DARK, 1.f, 1);
            /* highlight */
            draw_ellipse(SCX-10+sway, fcy+30, 20, 12,
                         COL_HAIR_MED, 0.4f, 1);
            break;

        case HAIR_BUN:
            draw_ellipse(SCX+sway*0.3f, fcy+15, 50, 55,
                         COL_HAIR_MED, 1.f, 1);
            draw_circle(SCX+sway*0.2f, fcy+50, 20,
                        COL_HAIR_MED, 1.f, 1);
            draw_circle(SCX+sway*0.2f, fcy+52, 16,
                        0.38f,0.22f,0.12f, 1.f, 1);
            draw_ellipse(SCX-45+sway, fcy-5, 13, 36,
                         COL_HAIR_MED, 1.f, 1);
            draw_ellipse(SCX+45+sway, fcy-5, 13, 36,
                         COL_HAIR_MED, 1.f, 1);
            break;

        case HAIR_CURLY:
            draw_ellipse(SCX+sway*0.5f, fcy+10, 70, 75,
                         COL_HAIR_LIGHT, 1.f, 1);
            for (int i = 0; i < 8; i++) {
                float ang = i*(float)(M_PI/4.0);
                float cx2 = SCX + cosf(ang)*62.f + sway*0.6f;
                float cy2 = fcy + sinf(ang)*48.f;
                draw_circle(cx2, cy2, 20, COL_HAIR_LIGHT, 1.f, 1);
            }
            draw_ellipse(SCX-16+sway, fcy+25, 26, 18,
                         COL_HAIR_LIGHT, 0.38f, 1);
            break;

        default:
            draw_ellipse(SCX+sway*0.3f, fcy+5, 56, 70,
                         COL_HAIR_DARK, 1.f, 1);
            break;
    }
}

/* ── SHOWCASE FACE ───────────────────────────────────────── */
static void sc_draw_face(void)
{
    float bob = sinf(G.breathPhase) * 2.f;
    float fcx = SCX;
    float fcy = SCY + 220.f + bob;

    /* Face oval */
    draw_ellipse(fcx, fcy, 48, 60, COL_SKIN, 1.f, 1);
    draw_ellipse(fcx, fcy-20, 38, 36, COL_SKIN, 1.f, 1);

    /* Eye shadow */
    if (G.eye != EYE_NONE) {
        draw_ellipse(fcx-22, fcy+22, 18, 10,
                     G.eyeR, G.eyeG, G.eyeB, 0.75f, 1);
        draw_ellipse(fcx+22, fcy+22, 18, 10,
                     G.eyeR, G.eyeG, G.eyeB, 0.75f, 1);
    }

    /* ── Eyes with blink ── */
    if (G.eyeOpen) {
        /* open */
        draw_ellipse(fcx-22, fcy+19, 13, 8,
                     0.97f, 0.97f, 0.97f, 1.f, 1);
        draw_circle (fcx-22, fcy+19, 6,
                     0.18f, 0.10f, 0.05f, 1.f, 1);
        draw_circle (fcx-22, fcy+19, 3,
                     0.04f, 0.02f, 0.01f, 1.f, 1);
        draw_circle (fcx-18, fcy+22, 2.f, 1,1,1, 0.9f, 1);

        draw_ellipse(fcx+22, fcy+19, 13, 8,
                     0.97f, 0.97f, 0.97f, 1.f, 1);
        draw_circle (fcx+22, fcy+19, 6,
                     0.18f, 0.10f, 0.05f, 1.f, 1);
        draw_circle (fcx+22, fcy+19, 3,
                     0.04f, 0.02f, 0.01f, 1.f, 1);
        draw_circle (fcx+26, fcy+22, 2.f, 1,1,1, 0.9f, 1);

        /* Lashes */
        glColor3f(0.04f,0.02f,0.01f);
        glLineWidth(2.f);
        glBegin(GL_LINE_STRIP);
          glVertex2f(fcx-38, fcy+21);
          glVertex2f(fcx-22, fcy+29);
          glVertex2f(fcx-6,  fcy+21);
        glEnd();
        glBegin(GL_LINE_STRIP);
          glVertex2f(fcx+38, fcy+21);
          glVertex2f(fcx+22, fcy+29);
          glVertex2f(fcx+6,  fcy+21);
        glEnd();
        glLineWidth(1.f);
    } else {
        /* closed – single curved line */
        glColor3f(COL_HAIR_DARK);
        glLineWidth(3.f);
        glBegin(GL_LINE_STRIP);
          glVertex2f(fcx-35, fcy+21);
          glVertex2f(fcx-22, fcy+27);
          glVertex2f(fcx-9,  fcy+21);
        glEnd();
        glBegin(GL_LINE_STRIP);
          glVertex2f(fcx+35, fcy+21);
          glVertex2f(fcx+22, fcy+27);
          glVertex2f(fcx+9,  fcy+21);
        glEnd();
        glLineWidth(1.f);
    }

    /* Eyebrows */
    glColor3f(COL_HAIR_DARK);
    glLineWidth(3.f);
    glBegin(GL_LINE_STRIP);
      glVertex2f(fcx-38, fcy+38);
      glVertex2f(fcx-22, fcy+46);
      glVertex2f(fcx-6,  fcy+40);
    glEnd();
    glBegin(GL_LINE_STRIP);
      glVertex2f(fcx+38, fcy+38);
      glVertex2f(fcx+22, fcy+46);
      glVertex2f(fcx+6,  fcy+40);
    glEnd();
    glLineWidth(1.f);

    /* Nose */
    draw_ellipse(fcx, fcy+8, 7, 9, COL_SKIN_DARK, 0.22f, 1);
    glColor4f(COL_SKIN_DARK, 0.45f);
    glLineWidth(1.8f);
    glBegin(GL_LINE_STRIP);
      glVertex2f(fcx-9,  fcy+22);
      glVertex2f(fcx-13, fcy+2);
      glVertex2f(fcx,    fcy+6);
      glVertex2f(fcx+13, fcy+2);
      glVertex2f(fcx+9,  fcy+22);
    glEnd();
    glLineWidth(1.f);

    /* Blush */
    if (G.blush) {
        draw_ellipse(fcx-36, fcy+12, 14, 8, COL_BLUSH, 0.45f, 1);
        draw_ellipse(fcx+36, fcy+12, 14, 8, COL_BLUSH, 0.45f, 1);
    }

    /* Lips */
    draw_ellipse(fcx,     fcy-12, 16,  7,  G.lipR, G.lipG, G.lipB, 1.f, 1);
    draw_ellipse(fcx-7,   fcy-6,  10,  6,  G.lipR, G.lipG, G.lipB, 1.f, 1);
    draw_ellipse(fcx+7,   fcy-6,  10,  6,  G.lipR, G.lipG, G.lipB, 1.f, 1);
    draw_ellipse(fcx,     fcy-12, 8,   3,
                 clampf(G.lipR+0.2f,0,1),
                 clampf(G.lipG+0.2f,0,1),
                 clampf(G.lipB+0.2f,0,1), 0.55f, 1);

    /* Neck */
    glColor3f(COL_SKIN);
    glBegin(GL_QUADS);
      glVertex2f(fcx-14, fcy-30);
      glVertex2f(fcx+14, fcy-30);
      glVertex2f(fcx+18, fcy-55);
      glVertex2f(fcx-18, fcy-55);
    glEnd();
}

/* ── SHOWCASE BODY ───────────────────────────────────────── */
static void sc_draw_body(void)
{
    float bob = sinf(G.breathPhase) * 3.f;

    /* Torso */
    draw_ellipse(SCX, SCY+90+bob, 52, 78, COL_SKIN, 1.f, 1);
    draw_ellipse(SCX, SCY+68+bob, 36, 18, COL_SKIN, 1.f, 1);

    /* Arms – gentle pose with slight curve */
    /* Left arm – slightly out */
    draw_ellipse(SCX-64, SCY+88+bob, 15, 58, COL_SKIN, 1.f, 1);
    draw_ellipse(SCX-68, SCY+32+bob, 13, 22, COL_SKIN, 1.f, 1);

    /* Right arm – hand-on-hip pose */
    draw_ellipse(SCX+64, SCY+92+bob, 15, 54, COL_SKIN, 1.f, 1);
    draw_ellipse(SCX+68, SCY+40+bob, 13, 22, COL_SKIN, 1.f, 1);

    /* Legs */
    draw_ellipse(SCX-18, SCY-95, 17, 88, COL_SKIN, 1.f, 1);
    draw_ellipse(SCX+18, SCY-95, 17, 88, COL_SKIN, 1.f, 1);
    /* Feet */
    draw_ellipse(SCX-18, SCY-178, 19, 10, COL_SKIN_DARK, 1.f, 1);
    draw_ellipse(SCX+18, SCY-178, 19, 10, COL_SKIN_DARK, 1.f, 1);
}

/* ── SHOWCASE DRESS ──────────────────────────────────────── */
static void sc_draw_dress(void)
{
    float bob = sinf(G.breathPhase) * 3.f;
    float dr=0, dg=0, db=0;

    switch (G.dress) {
        case DRESS_GOWN:    dr=COL_DRESS_GOWN;    break;
        case DRESS_CASUAL:  dr=COL_DRESS_CASUAL;  break;
        case DRESS_GLAMOUR: dr=COL_DRESS_GLAMOUR; break;
        default: return;
    }

    switch (G.dress) {
        case DRESS_GOWN: {
            glColor3f(dr, dg, db);
            glBegin(GL_QUADS);
              glVertex2f(SCX-38, SCY+58+bob);
              glVertex2f(SCX+38, SCY+58+bob);
              glVertex2f(SCX+48, SCY+128+bob);
              glVertex2f(SCX-48, SCY+128+bob);
            glEnd();
            glBegin(GL_TRIANGLE_FAN);
              glVertex2f(SCX, SCY+55+bob);
              glVertex2f(SCX-125, SCY-165);
              glVertex2f(SCX-85,  SCY-172);
              glVertex2f(SCX-45,  SCY-178);
              glVertex2f(SCX,     SCY-182);
              glVertex2f(SCX+45,  SCY-178);
              glVertex2f(SCX+85,  SCY-172);
              glVertex2f(SCX+125, SCY-165);
            glEnd();
            /* tulle shimmer */
            float t = G.animTime;
            for (int i = 0; i < 6; i++) {
                float shine = 0.55f + sinf(t*1.8f + i*0.9f)*0.15f;
                glColor4f(dr+0.15f, dg+shine*0.3f, db+0.20f, 0.25f + sinf(t+i)*0.1f);
                draw_ellipse(SCX - 80 + i*30.f, SCY - 100 - i*12.f,
                             22, 10, dr+0.15f, dg+0.15f, db+0.20f,
                             0.25f + sinf(t+i)*0.1f, 1);
            }
            /* waist band */
            draw_ellipse(SCX, SCY+58+bob, 40, 10, dr-0.10f, dg, db-0.05f, 1.f, 1);
            break;
        }
        case DRESS_CASUAL: {
            glColor3f(dr, dg, db);
            glBegin(GL_QUADS);
              glVertex2f(SCX-34, SCY+62+bob);
              glVertex2f(SCX+34, SCY+62+bob);
              glVertex2f(SCX+42, SCY+130+bob);
              glVertex2f(SCX-42, SCY+130+bob);
            glEnd();
            glBegin(GL_TRIANGLE_FAN);
              glVertex2f(SCX, SCY+58+bob);
              glVertex2f(SCX-70, SCY-82);
              glVertex2f(SCX-50, SCY-122);
              glVertex2f(SCX,    SCY-132);
              glVertex2f(SCX+50, SCY-122);
              glVertex2f(SCX+70, SCY-82);
            glEnd();
            /* polka dots */
            glColor4f(1.f, 0.88f, 0.92f, 0.65f);
            for (int i = 0; i < 9; i++) {
                float fx = SCX - 48.f + (i%3)*32.f;
                float fy = SCY + 48.f + bob - (i/3)*32.f;
                draw_circle(fx, fy, 5.5f, 1,0.85f,0.90f, 0.65f, 1);
            }
            glColor3f(dr-0.05f, dg+0.05f, db+0.10f);
            glLineWidth(5.f);
            glBegin(GL_LINES);
              glVertex2f(SCX-20, SCY+130+bob);
              glVertex2f(SCX-28, SCY+158+bob);
              glVertex2f(SCX+20, SCY+130+bob);
              glVertex2f(SCX+28, SCY+158+bob);
            glEnd();
            glLineWidth(1.f);
            break;
        }
        case DRESS_GLAMOUR: {
            glColor3f(dr, dg, db);
            glBegin(GL_QUADS);
              glVertex2f(SCX-38, SCY+62+bob);
              glVertex2f(SCX+38, SCY+62+bob);
              glVertex2f(SCX+30, SCY-92);
              glVertex2f(SCX-30, SCY-92);
            glEnd();
            /* Sequins */
            float t = G.animTime;
            for (int i = 0; i < 35; i++) {
                float sx2 = SCX - 28.f + (i%7)*9.f;
                float sy2 = SCY + 55.f + bob - (i/7)*24.f;
                float bright = 0.65f + sinf(t*3.2f + i*0.65f)*0.35f;
                glColor4f(bright, bright*0.75f, bright*0.88f, 0.88f);
                draw_circle(sx2, sy2, 3.2f,
                            bright, bright*0.75f, bright*0.88f, 0.88f, 1);
            }
            /* Slit */
            glColor4f(0.96f, 0.88f, 0.92f, 0.55f);
            glBegin(GL_TRIANGLES);
              glVertex2f(SCX+8,  SCY-40);
              glVertex2f(SCX+30, SCY-40);
              glVertex2f(SCX+30, SCY-92);
            glEnd();
            /* Halter */
            glColor3f(dr, dg, db);
            glLineWidth(6.f);
            glBegin(GL_LINES);
              glVertex2f(SCX-9, SCY+62+bob);
              glVertex2f(SCX,   SCY+150+bob);
              glVertex2f(SCX+9, SCY+62+bob);
              glVertex2f(SCX,   SCY+150+bob);
            glEnd();
            glLineWidth(1.f);
            break;
        }
        default: break;
    }
}

/* ── SHOWCASE ACCESSORIES ────────────────────────────────── */
static void sc_draw_accessories(void)
{
    float bob = sinf(G.breathPhase) * 2.f;
    float fcy = SCY + 220.f + bob;
    float t   = G.animTime;

    int has_e = (G.accessories==ACC_EARRINGS||G.accessories==ACC_BOTH);
    int has_n = (G.accessories==ACC_NECKLACE||G.accessories==ACC_BOTH);

    if (has_e) {
        float shine = 0.80f + sinf(t*2.f)*0.20f;
        /* Left */
        draw_circle(SCX-48, fcy-22, 5.5f, shine,shine*0.85f,0.10f, 1.f, 1);
        draw_circle(SCX-48, fcy-32, 3.5f, shine,shine*0.85f,0.10f, 1.f, 1);
        glColor3f(shine, shine*0.85f, 0.10f);
        glLineWidth(2.f);
        glBegin(GL_LINES);
          glVertex2f(SCX-48, fcy-27);
          glVertex2f(SCX-48, fcy-17);
        glEnd();
        /* Right */
        draw_circle(SCX+48, fcy-22, 5.5f, shine,shine*0.85f,0.10f, 1.f, 1);
        draw_circle(SCX+48, fcy-32, 3.5f, shine,shine*0.85f,0.10f, 1.f, 1);
        glBegin(GL_LINES);
          glVertex2f(SCX+48, fcy-27);
          glVertex2f(SCX+48, fcy-17);
        glEnd();
        glLineWidth(1.f);
    }

    if (has_n) {
        float ny = SCY + 153.f + bob;
        float shine = 0.90f + sinf(t*1.5f)*0.10f;
        int np = 13;
        for (int i = 0; i < np; i++) {
            float ang = (float)M_PI*0.12f +
                        (float)i/(np-1)*(float)M_PI*0.76f;
            float px = SCX + cosf(ang)*36.f;
            float py = ny  - sinf(ang)*14.f;
            draw_circle(px, py, 5.f, shine, shine*0.96f, shine*0.88f, 1.f, 1);
            draw_circle(px+1.5f, py+1.5f, 2.f, 1,0.98f,0.85f, 0.5f, 1);
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
                 (G.blush==BLUSH_ON) ? "Blush: ON" : "Blush: OFF",
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

    /* Restart button */
    ui_draw_button(&btn_restart);
    ui_draw_text(600, 30,
                 "Play again to try a new look!",
                 0.70f, 0.55f, 0.85f, GLUT_BITMAP_8_BY_13);
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
    sc_init();
    G.breathPhase += dt * 1.1f;

    /* Hair sway – slow sine */
    G.hairSway = sinf(G.animTime * 0.7f) * 5.f;

    /* Blink logic: open 3s, close 0.2s */
    G.blinkTimer += dt;
    if (G.eyeOpen && G.blinkTimer > 3.0f) {
        G.eyeOpen    = 0;
        G.blinkTimer = 0;
    } else if (!G.eyeOpen && G.blinkTimer > 0.18f) {
        G.eyeOpen    = 1;
        G.blinkTimer = 0;
    }
}

void showcase_mouse_move(int x, int y)
{
    sc_init();
    btn_restart.hovered = ui_hit(&btn_restart, x, y);
}

void showcase_mouse_click(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    sc_init();
    if (ui_hit(&btn_restart, x, y))
        stage_restart();
}