/* =========================================================
   ui.c  –  Button / label / primitive drawing routines
   ========================================================= */

#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include "ui.h"
#include "assets.h"

/* ── draw_circle ─────────────────────────────────────────── */
void draw_circle(float cx, float cy, float radius,
                 float r, float g, float b, float a, int filled)
{
    int   segments = 64;
    float step = (float)(2.0 * M_PI / segments);
    glColor4f(r, g, b, a);
    glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
    if (filled) glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float ang = i * step;
        glVertex2f(cx + cosf(ang) * radius,
                   cy + sinf(ang) * radius);
    }
    glEnd();
}

/* ── draw_ellipse ────────────────────────────────────────── */
void draw_ellipse(float cx, float cy, float rx, float ry,
                  float r, float g, float b, float a, int filled)
{
    int   segments = 64;
    float step = (float)(2.0 * M_PI / segments);
    glColor4f(r, g, b, a);
    glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
    if (filled) glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float ang = i * step;
        glVertex2f(cx + cosf(ang) * rx,
                   cy + sinf(ang) * ry);
    }
    glEnd();
}

/* ── draw_rounded_rect ───────────────────────────────────── */
void draw_rounded_rect(float x, float y, float w, float h,
                       float radius,
                       float r, float g, float b, float a, int filled)
{
    int   corner_segs = 16;
    float step = (float)(M_PI / 2.0 / corner_segs);
    glColor4f(r, g, b, a);
    if (filled) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x + w / 2.f, y + h / 2.f);
    } else {
        glBegin(GL_LINE_LOOP);
    }

    /* bottom-left corner */
    for (int i = 0; i <= corner_segs; i++) {
        float ang = (float)(M_PI) + i * step;
        glVertex2f(x + radius + cosf(ang)*radius,
                   y + radius + sinf(ang)*radius);
    }

    /* bottom-right */
    for (int i = 0; i <= corner_segs; i++) {
        float ang = (float)(3.0*M_PI/2.0) + i * step;
        glVertex2f(x + w - radius + cosf(ang)*radius,
                   y + radius     + sinf(ang)*radius);
    }

    /* top-right */
    for (int i = 0; i <= corner_segs; i++) {
        float ang = 0.f + i * step;
        glVertex2f(x + w - radius + cosf(ang)*radius,
                   y + h - radius + sinf(ang)*radius);
    }

    /* top-left */
    for (int i = 0; i <= corner_segs; i++) {
        float ang = (float)(M_PI/2.0) + i * step;
        glVertex2f(x + radius     + cosf(ang)*radius,
                   y + h - radius + sinf(ang)*radius);
    }

    glEnd();
}

/* ── ui_draw_button ──────────────────────────────────────── */
void ui_draw_button(const Button *btn)
{
    float br = COL_BTN_BASE_R;   /* FIXED: was COL_BTN_BASE */

    float bg_r, bg_g, bg_b;

    if (btn->r > 0.f || btn->g > 0.f || btn->b > 0.f) {
        bg_r = btn->r; bg_g = btn->g; bg_b = btn->b;
    } else {
        bg_r = br; bg_g = 0.70f; bg_b = 0.80f;
    }

    if (btn->active) {
        bg_r = clampf(bg_r - 0.15f, 0, 1);
        bg_g = clampf(bg_g - 0.10f, 0, 1);
        bg_b = clampf(bg_b - 0.05f, 0, 1);
    } else if (btn->hovered) {
        bg_r = clampf(bg_r + 0.08f, 0, 1);
        bg_g = clampf(bg_g - 0.05f, 0, 1);
        bg_b = clampf(bg_b + 0.05f, 0, 1);
    }

    draw_rounded_rect(btn->x+3, btn->y-3, btn->w, btn->h, 6,
                      0.3f, 0.1f, 0.2f, 0.25f, 1);

    draw_rounded_rect(btn->x, btn->y, btn->w, btn->h, 6,
                      bg_r, bg_g, bg_b, 1.f, 1);

    glLineWidth(btn->active ? 2.5f : 1.5f);
    draw_rounded_rect(btn->x, btn->y, btn->w, btn->h, 6,
                      0.55f, 0.20f, 0.40f, 0.8f, 0);
    glLineWidth(1.0f);

    /* label */
    if (btn->label) {
        float tw = (float)strlen(btn->label) * 8.f;
        float tx = btn->x + (btn->w - tw) / 2.f;
        float ty = btn->y + btn->h / 2.f - 5.f;

        ui_draw_text(tx, ty, btn->label,
                     COL_BTN_TEXT_R,
                     COL_BTN_TEXT_G,
                     COL_BTN_TEXT_B,
                     GLUT_BITMAP_8_BY_13);
    }
}

/* ── ui_draw_swatch ──────────────────────────────────────── */
void ui_draw_swatch(float x, float y, float size,
                    float r, float g, float b,
                    int hovered, int active)
{
    if (hovered || active) {
        draw_rounded_rect(x-4, y-4, size+8, size+8, 4,
                          1,1,1, active ? 0.9f : 0.5f, 1);
    }

    draw_rounded_rect(x, y, size, size, 4, r, g, b, 1.f, 1);

    glLineWidth(active ? 3.0f : 1.5f);
    draw_rounded_rect(x, y, size, size, 4,
                      active ? 0.9f : 0.4f,
                      active ? 0.7f : 0.2f,
                      active ? 0.8f : 0.3f, 1.f, 0);
    glLineWidth(1.0f);

    if (active) {
        glColor3f(1,1,1);
        glLineWidth(2.5f);
        glBegin(GL_LINE_STRIP);
          glVertex2f(x + size*0.22f, y + size*0.50f);
          glVertex2f(x + size*0.44f, y + size*0.28f);
          glVertex2f(x + size*0.80f, y + size*0.72f);
        glEnd();
        glLineWidth(1.0f);
    }
}

/* ── ui_draw_text ────────────────────────────────────────── */
void ui_draw_text(float x, float y, const char *str,
                  float r, float g, float b, void *font)
{
    if (!font) font = GLUT_BITMAP_8_BY_13;
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char *c = str; *c; c++)
        glutBitmapCharacter(font, (int)*c);
}

/* ── ui_draw_title ───────────────────────────────────────── */
void ui_draw_title(float cx, float y, const char *str,
                   float r, float g, float b)
{
    int len = (int)strlen(str);
    float tw = (float)len * 10.f;
    float x = cx - tw / 2.f;

//   ui_draw_text(x+4, y-4, str, 0.2f,0.05f,0.15f,
//              GLUT_BITMAP_HELVETICA_18);

    ui_draw_text(x+4, y-4, str, r, g, b,
                 GLUT_BITMAP_HELVETICA_18);
}

/* ── ui_hit ──────────────────────────────────────────────── */
int ui_hit(const Button *btn, int mx, int my)
{
    float fy = (float)(WIN_H - my);
    return (mx >= btn->x && mx <= btn->x + btn->w &&
            fy >= btn->y && fy <= btn->y + btn->h);
}

/* ── ui_draw_overlay ─────────────────────────────────────── */
void ui_draw_overlay(float alpha)
{
    glColor4f(0.05f, 0.02f, 0.08f, alpha);
    glBegin(GL_QUADS);
      glVertex2f(0, 0);
      glVertex2f(WIN_W, 0);
      glVertex2f(WIN_W, WIN_H);
      glVertex2f(0, WIN_H);
    glEnd();
}

/* ── ui_draw_divider ─────────────────────────────────────── */
void ui_draw_divider(float x1, float y1, float x2, float y2,
                     float r, float g, float b, float alpha)
{
    glColor4f(r, g, b, alpha);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
      glVertex2f(x1, y1);
      glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1.0f);
}

/* ── draw_bezier ─────────────────────────────────────────── */
void draw_bezier(const float *pts, int n, float thick,
                 float r, float g, float b)
{
    glColor3f(r, g, b);
    glLineWidth(thick);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++)
        glVertex2f(pts[i*2], pts[i*2+1]);
    glEnd();
    glLineWidth(1.0f);
}