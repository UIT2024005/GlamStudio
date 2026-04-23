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
#define PANEL_W2  330.f

/* Swatch size */
#define SW2  38.f
#define GAP2 12.f

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
    sy = 460.f;
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
    float bcy = BCY + (float)sin(G.breathPhase) * 2.5f;

    switch (G.hair) {
        case HAIR_LONG:
            /* Long wavy – wide side panels */
            draw_ellipse(BCX, bcy+235, 62, 100, COL_HAIR_DARK, 1.f, 1);
            /* waves left */
            draw_ellipse(BCX-58, bcy+130, 18, 90, COL_HAIR_DARK, 1.f, 1);
            draw_ellipse(BCX-62, bcy+60,  16, 60, COL_HAIR_DARK, 0.8f, 1);
            /* waves right */
            draw_ellipse(BCX+58, bcy+130, 18, 90, COL_HAIR_DARK, 1.f, 1);
            draw_ellipse(BCX+62, bcy+60,  16, 60, COL_HAIR_DARK, 0.8f, 1);
            break;

        case HAIR_BUN:
            /* Bun on top */
            draw_ellipse(BCX, bcy+262, 55, 55, COL_HAIR_MED, 1.f, 1);
            /* bun knot */
            draw_circle(BCX, bcy+298, 22, COL_HAIR_MED, 1.f, 1);
            draw_circle(BCX, bcy+300, 18,
                        0.40f,0.24f,0.12f, 1.f, 1);
            /* side wisps */
            draw_ellipse(BCX-50, bcy+225, 14, 40, COL_HAIR_MED, 1.f, 1);
            draw_ellipse(BCX+50, bcy+225, 14, 40, COL_HAIR_MED, 1.f, 1);
            break;

        case HAIR_CURLY:
            /* Voluminous curly mass */
            draw_ellipse(BCX, bcy+250, 80, 85, COL_HAIR_LIGHT, 1.f, 1);
            /* curl clusters */
            for (int i = 0; i < 8; i++) {
                float ang = i * (float)(M_PI/4.0);
                float cx2 = BCX + cosf(ang)*68.f;
                float cy2 = bcy+240 + sinf(ang)*52.f;
                draw_circle(cx2, cy2, 22,
                            COL_HAIR_LIGHT, 1.f, 1);
            }
            /* highlight */
            draw_ellipse(BCX-20, bcy+280, 30, 20, COL_HAIR_LIGHT,0.4f, 1);
            break;

        default:
            /* No hair – plain dark */
            draw_ellipse(BCX, bcy+240, 62, 80, COL_HAIR_DARK, 1.f, 1);
            break;
    }
}

/* Simplified face for body view */
void dressup_draw_face(void)
{
    float bcy = BCY + (float)sin(G.breathPhase) * 2.5f;
    float fcx = BCX, fcy = bcy + 220.f;

    /* Face oval */
    draw_ellipse(fcx, fcy, 46, 58, COL_SKIN, 1.f, 1);

    /* Eye shadow */
    if (G.eye != EYE_NONE)
        draw_ellipse(fcx-20, fcy+20, 14, 7, G.eyeR,G.eyeG,G.eyeB, 0.7f, 1);
    if (G.eye != EYE_NONE)
        draw_ellipse(fcx+20, fcy+20, 14, 7, G.eyeR,G.eyeG,G.eyeB, 0.7f, 1);

    /* Eyes */
    draw_ellipse(fcx-20, fcy+18, 11, 7, 0.97f,0.97f,0.97f, 1.f, 1);
    draw_circle (fcx-20, fcy+18, 5,  0.15f,0.08f,0.04f, 1.f, 1);
    draw_circle (fcx-20, fcy+18, 2.5f, 0.02f,0.01f,0.01f, 1.f, 1);
    draw_circle (fcx-17, fcy+21, 1.5f, 1,1,1, 0.9f, 1);

    draw_ellipse(fcx+20, fcy+18, 11, 7, 0.97f,0.97f,0.97f, 1.f, 1);
    draw_circle (fcx+20, fcy+18, 5,  0.15f,0.08f,0.04f, 1.f, 1);
    draw_circle (fcx+20, fcy+18, 2.5f, 0.02f,0.01f,0.01f, 1.f, 1);
    draw_circle (fcx+23, fcy+21, 1.5f, 1,1,1, 0.9f, 1);

    /* Eyebrows */
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

    /* Nose */
    draw_ellipse(fcx, fcy+4, 6, 8, COL_SKIN_DARK, 0.25f, 1);

    /* Blush */
    if (G.blush) {
        draw_ellipse(fcx-34, fcy+8, 12, 7, COL_BLUSH, 0.4f, 1);
        draw_ellipse(fcx+34, fcy+8, 12, 7, COL_BLUSH, 0.4f, 1);
    }

    /* Lips */
    draw_ellipse(fcx, fcy-14, 14, 6, G.lipR,G.lipG,G.lipB, 1.f, 1);
    draw_ellipse(fcx-6, fcy-8, 8, 5, G.lipR,G.lipG,G.lipB, 1.f, 1);
    draw_ellipse(fcx+6, fcy-8, 8, 5, G.lipR,G.lipG,G.lipB, 1.f, 1);
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

    /* Torso */
    draw_ellipse(BCX, BCY+90, 55, 80, COL_SKIN, 1.f, 1);
    /* Waist indent */
    draw_ellipse(BCX, BCY+70, 38, 20, COL_SKIN, 1.f, 1);

    /* Arms */
    /* Left arm */
    draw_ellipse(BCX-65, BCY+90+bob, 16, 60, COL_SKIN, 1.f, 1);
    draw_ellipse(BCX-68, BCY+30+bob, 14, 24, COL_SKIN, 1.f, 1);
    /* Right arm */
    draw_ellipse(BCX+65, BCY+90+bob, 16, 60, COL_SKIN, 1.f, 1);
    draw_ellipse(BCX+68, BCY+30+bob, 14, 24, COL_SKIN, 1.f, 1);

    /* Legs – visible below dress hem (drawn first, dress overlays) */
    draw_ellipse(BCX-20, BCY-95, 18, 90, COL_SKIN, 1.f, 1);
    draw_ellipse(BCX+20, BCY-95, 18, 90, COL_SKIN, 1.f, 1);
    /* Feet */
    draw_ellipse(BCX-20, BCY-180, 20, 10, COL_SKIN_DARK, 1.f, 1);
    draw_ellipse(BCX+20, BCY-180, 20, 10, COL_SKIN_DARK, 1.f, 1);
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
        default: return;
    }

    glColor4f(dr, dg, db, fade);

    switch (G.dress) {
        /* ── Ball Gown ── */
        case DRESS_GOWN: {
            /* Bodice */
            glBegin(GL_QUADS);
              glVertex2f(BCX-40, BCY+60+bob);
              glVertex2f(BCX+40, BCY+60+bob);
              glVertex2f(BCX+50, BCY+130+bob);
              glVertex2f(BCX-50, BCY+130+bob);
            glEnd();
            /* Skirt – wide bell shape */
            glBegin(GL_TRIANGLE_FAN);
              glVertex2f(BCX, BCY+55+bob);
              glVertex2f(BCX-130, BCY-160);
              glVertex2f(BCX-90,  BCY-170);
              glVertex2f(BCX-50,  BCY-175);
              glVertex2f(BCX,     BCY-180);
              glVertex2f(BCX+50,  BCY-175);
              glVertex2f(BCX+90,  BCY-170);
              glVertex2f(BCX+130, BCY-160);
            glEnd();
            /* Tulle layers */
            glColor4f(dr+0.15f, dg+0.10f, db+0.15f, fade*0.5f);
            glBegin(GL_TRIANGLE_FAN);
              glVertex2f(BCX, BCY+30+bob);
              glVertex2f(BCX-140, BCY-165);
              glVertex2f(BCX-100, BCY-175);
              glVertex2f(BCX,     BCY-185);
              glVertex2f(BCX+100, BCY-175);
              glVertex2f(BCX+140, BCY-165);
            glEnd();
            /* Waistband */
            glColor4f(dr-0.1f, dg, db-0.05f, fade);
            draw_ellipse(BCX, BCY+60+bob, 42, 10, dr-0.1f,dg,db-0.05f, fade, 1);
            break;
        }

        /* ── Casual Sundress ── */
        case DRESS_CASUAL: {
            /* Bodice */
            glColor4f(dr, dg, db, fade);
            glBegin(GL_QUADS);
              glVertex2f(BCX-36, BCY+65+bob);
              glVertex2f(BCX+36, BCY+65+bob);
              glVertex2f(BCX+44, BCY+135+bob);
              glVertex2f(BCX-44, BCY+135+bob);
            glEnd();
            /* A-line skirt */
            glBegin(GL_TRIANGLE_FAN);
              glVertex2f(BCX, BCY+60+bob);
              glVertex2f(BCX-72, BCY-80);
              glVertex2f(BCX-50, BCY-120);
              glVertex2f(BCX,    BCY-130);
              glVertex2f(BCX+50, BCY-120);
              glVertex2f(BCX+72, BCY-80);
            glEnd();
            /* Floral pattern dots */
            glColor4f(1.f, 0.85f, 0.90f, fade*0.7f);
            for (int i = 0; i < 10; i++) {
                float fx = BCX - 55.f + i*13.f;
                float fy = BCY - 20.f + (float)sin(i*1.2f)*30.f;
                draw_circle(fx, fy, 5, 1,0.8f,0.88f, fade*0.7f, 1);
            }
            /* Straps */
            glColor4f(dr-0.05f, dg+0.05f, db+0.10f, fade);
            glLineWidth(5.f);
            glBegin(GL_LINES);
              glVertex2f(BCX-22, BCY+135+bob);
              glVertex2f(BCX-30, BCY+165+bob);
              glVertex2f(BCX+22, BCY+135+bob);
              glVertex2f(BCX+30, BCY+165+bob);
            glEnd();
            glLineWidth(1.f);
            break;
        }

        /* ── Glamour Sequin Dress ── */
        case DRESS_GLAMOUR: {
            /* Sheath body */
            glColor4f(dr, dg, db, fade);
            glBegin(GL_QUADS);
              glVertex2f(BCX-40, BCY+65+bob);
              glVertex2f(BCX+40, BCY+65+bob);
              glVertex2f(BCX+32, BCY-90);
              glVertex2f(BCX-32, BCY-90);
            glEnd();
            /* Sequin sparkle overlay */
            for (int i = 0; i < 30; i++) {
                float sx2 = BCX - 35.f + (i % 7)*11.f;
                float sy2 = BCY + 60.f + bob - (i/7)*28.f;
                float bright = 0.7f + sinf(G.animTime*3.f+i)*0.3f;
                glColor4f(bright, bright*0.8f, bright*0.9f, fade*0.85f);
                draw_circle(sx2, sy2, 3.5f, bright,bright*0.8f,bright*0.9f, fade*0.85f, 1);
            }
            /* Slit */
            glColor4f(0.96f, 0.88f, 0.92f, fade * 0.6f);
            glBegin(GL_TRIANGLES);
              glVertex2f(BCX+10, BCY-40);
              glVertex2f(BCX+32, BCY-40);
              glVertex2f(BCX+32, BCY-90);
            glEnd();
            /* Halter neck strap */
            glColor4f(dr, dg, db, fade);
            glLineWidth(6.f);
            glBegin(GL_LINES);
              glVertex2f(BCX-10, BCY+65+bob);
              glVertex2f(BCX,    BCY+155+bob);
              glVertex2f(BCX+10, BCY+65+bob);
              glVertex2f(BCX,    BCY+155+bob);
            glEnd();
            glLineWidth(1.f);
            break;
        }
        default: break;
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
        /* Gold drop earrings */
        draw_circle(BCX-48, fcy-24, 5,  0.85f,0.70f,0.10f, 1.f, 1);
        draw_circle(BCX-48, fcy-34, 3,  0.85f,0.70f,0.10f, 1.f, 1);
        glColor3f(0.85f,0.70f,0.10f);
        glLineWidth(2.f);
        glBegin(GL_LINES);
          glVertex2f(BCX-48, fcy-29);
          glVertex2f(BCX-48, fcy-20);
        glEnd();
        /* Right */
        draw_circle(BCX+48, fcy-24, 5,  0.85f,0.70f,0.10f, 1.f, 1);
        draw_circle(BCX+48, fcy-34, 3,  0.85f,0.70f,0.10f, 1.f, 1);
        glBegin(GL_LINES);
          glVertex2f(BCX+48, fcy-29);
          glVertex2f(BCX+48, fcy-20);
        glEnd();
        glLineWidth(1.f);
    }

    if (has_necklace) {
        /* Pearl necklace at neck */
        float ny = BCY + 155.f + bob;
        glColor3f(0.95f, 0.92f, 0.85f);
        glLineWidth(2.f);
        /* arc of pearls */
        int np = 11;
        for (int i = 0; i < np; i++) {
            float ang = (float)M_PI * 0.15f +
                        (float)i / (np-1) * (float)M_PI * 0.70f;
            float px = BCX + cosf(ang)*32.f;
            float py = ny  - sinf(ang)*12.f;
            draw_circle(px, py, 4.5f, 0.95f,0.92f,0.85f, 1.f, 1);
            /* gold sheen */
            draw_circle(px+1, py+1, 1.8f, 1,0.95f,0.80f, 0.6f, 1);
        }
        glLineWidth(1.f);
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
    ui_draw_text(PANEL_X2+18, 518, "Dress Style",
                 0.40f, 0.10f, 0.55f, GLUT_BITMAP_HELVETICA_18);
    ui_draw_divider(PANEL_X2+18, 513, PANEL_X2+PANEL_W2-33, 513,
                    0.55f, 0.25f, 0.75f, 0.5f);
    for (int i = 0; i < 3; i++) {
        int active = (G.dress == (DressStyle)(i+1));
        ui_draw_swatch(btn_dress[i].x, btn_dress[i].y, SW2,
                       dress_cols[i][0], dress_cols[i][1], dress_cols[i][2],
                       btn_dress[i].hovered, active);
        ui_draw_text(btn_dress[i].x+1, btn_dress[i].y-14,
                     dress_labels[i], 0.35f,0.10f,0.50f, GLUT_BITMAP_8_BY_13);
    }

    /* ── Hairstyle ── */
    ui_draw_text(PANEL_X2+18, 398, "Hairstyle",
                 0.40f, 0.10f, 0.55f, GLUT_BITMAP_HELVETICA_18);
    ui_draw_divider(PANEL_X2+18, 393, PANEL_X2+PANEL_W2-33, 393,
                    0.55f, 0.25f, 0.75f, 0.5f);
    for (int i = 0; i < 3; i++) {
        int active = (G.hair == (HairStyle)(i+1));
        ui_draw_swatch(btn_hair[i].x, btn_hair[i].y, SW2,
                       hair_cols[i][0], hair_cols[i][1], hair_cols[i][2],
                       btn_hair[i].hovered, active);
        ui_draw_text(btn_hair[i].x+1, btn_hair[i].y-14,
                     hair_labels[i], 0.35f,0.10f,0.50f, GLUT_BITMAP_8_BY_13);
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