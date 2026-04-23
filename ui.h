#ifndef UI_H
#define UI_H

/* =========================================================
   ui.h  –  Button / label primitives
   ========================================================= */

#include "assets.h"

/* A simple rectangular button */
typedef struct {
    float x, y, w, h;       /* position (bottom-left) and size */
    const char *label;
    int   hovered;
    int   active;            /* is this the currently selected option? */
    float r, g, b;           /* optional tint colour (0 = use default) */
} Button;

/* Draw a rounded-rect button; highlight if hovered/active */
void ui_draw_button(const Button *btn);

/* Draw a colour-swatch button (filled square with outline) */
void ui_draw_swatch(float x, float y, float size,
                    float r, float g, float b,
                    int hovered, int active);

/* Draw a string using GLUT bitmap font at (x,y) in window coords */
void ui_draw_text(float x, float y, const char *str,
                  float r, float g, float b, void *font);

/* Draw a centred title string */
void ui_draw_title(float cx, float y, const char *str,
                   float r, float g, float b);

/* Hit-test a button against mouse coords */
int ui_hit(const Button *btn, int mx, int my);

/* Draw a semi-transparent dark overlay (for stage transition) */
void ui_draw_overlay(float alpha);

/* Draw a decorative divider line */
void ui_draw_divider(float x1, float y1, float x2, float y2,
                     float r, float g, float b, float alpha);

/* Draw a filled circle (utility used by many draw routines) */
void draw_circle(float cx, float cy, float radius,
                 float r, float g, float b, float a, int filled);

/* Draw a filled ellipse */
void draw_ellipse(float cx, float cy, float rx, float ry,
                  float r, float g, float b, float a, int filled);

/* Draw a rounded rectangle */
void draw_rounded_rect(float x, float y, float w, float h,
                       float radius,
                       float r, float g, float b, float a, int filled);

/* Draw a bezier-curve approximation as a polyline
   pts: array of 2*(n) floats [x0,y0, x1,y1, …]  */
void draw_bezier(const float *pts, int n, float thick,
                 float r, float g, float b);

#endif /* UI_H */