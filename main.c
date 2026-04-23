/* =========================================================
   main.c  –  Entry point & GLUT callback wiring
   =========================================================
   Build:
     gcc main.c makeup.c dressup.c showcase.c ui.c \
         -lGL -lGLU -lglut -lm -o makeup_game
   ========================================================= */
/* =========================================================
   main.c  –  Entry point & GLUT callback wiring
   =========================================================
   Build:
     gcc main.c makeup.c dressup.c showcase.c ui.c \
         -lGL -lGLU -lglut -lm -o makeup_game
   ========================================================= */

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "assets.h"
#include "ui.h"
#include "makeup.h"
#include "dressup.h"
#include "showcase.h"

/* ── Global game state (declared extern everywhere else) ── */
GameState G;

/* ── Timing ──────────────────────────────────────────────── */
static int   last_time_ms = 0;
static float delta_time   = 0.f;

/* ── Transition state ────────────────────────────────────── */
#define TRANS_SPEED 2.5f   /* fade in/out speed */
static int   pending_stage = -1;  /* stage to switch to after fade */

/* ── Forward declarations ────────────────────────────────── */
static void display(void);
static void idle(void);
static void reshape(int w, int h);
static void mouse_move(int x, int y);
static void mouse_click(int button, int state, int x, int y);
static void keyboard(unsigned char key, int x, int y);

/* ── Stage management ────────────────────────────────────── */

/* Called by makeup to advance to dressup, by dressup to advance to showcase */
void stage_next(void)
{
    if (G.transitioning) return;
    G.transitioning = 1;
    G.transAlpha    = 0.f;
    pending_stage   = (int)G.stage + 1;
}

/* Called by showcase restart button */
void stage_restart(void)
{
    if (G.transitioning) return;
    /* Reset all selections */
    G.lip         = LIP_NONE;
    G.eye         = EYE_NONE;
    G.blush       = BLUSH_NONE;
    G.dress       = DRESS_NONE;
    G.hair        = HAIR_NONE;
    G.accessories = ACC_NONE;
    G.lipR = 0.80f; G.lipG = 0.55f; G.lipB = 0.50f;
    G.eyeR = 0.50f; G.eyeG = 0.50f; G.eyeB = 0.50f;
    G.dressFade   = 1.f;
    G.breathPhase = 0.f;
    G.blinkTimer  = 0.f;
    G.eyeOpen     = 1;
    G.hairSway    = 0.f;
    G.transitioning = 1;
    G.transAlpha    = 0.f;
    pending_stage   = STAGE_MAKEUP;
}

/* ── OpenGL setup ────────────────────────────────────────── */
static void setup_projection(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* ── display callback ────────────────────────────────────── */
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    /* Draw the current stage */
    switch (G.stage) {
        case STAGE_MAKEUP:   makeup_draw();   break;
        case STAGE_DRESSUP:  dressup_draw();  break;
        case STAGE_SHOWCASE: showcase_draw(); break;
    }

    /* Stage transition overlay */
    if (G.transitioning && G.transAlpha > 0.f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ui_draw_overlay(G.transAlpha);
        glDisable(GL_BLEND);
    }

    glutSwapBuffers();
}

/* ── idle (animation tick) ───────────────────────────────── */
static void idle(void)
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    delta_time   = (now - last_time_ms) / 1000.f;
    last_time_ms = now;
    /* Cap dt to avoid spiral of death on slow frames */
    if (delta_time > 0.1f) delta_time = 0.1f;

    G.animTime += delta_time;

    /* Run per-stage update */
    switch (G.stage) {
        case STAGE_MAKEUP:   makeup_update(delta_time);   break;
        case STAGE_DRESSUP:  dressup_update(delta_time);  break;
        case STAGE_SHOWCASE: showcase_update(delta_time); break;
    }

    /* Handle fade transition */
    if (G.transitioning) {
        G.transAlpha += TRANS_SPEED * delta_time;
        if (G.transAlpha >= 1.f) {
            /* Fully black – switch stage now */
            if (pending_stage >= 0) {
                G.stage = (Stage)pending_stage;
                pending_stage = -1;
                G.animTime = 0.f;
            }
            G.transAlpha    = 1.f;
            G.transitioning = 2; /* fading back in */
        }
    } else if (G.transitioning == 2) {
        G.transAlpha -= TRANS_SPEED * delta_time;
        if (G.transAlpha <= 0.f) {
            G.transAlpha    = 0.f;
            G.transitioning = 0;
        }
    }

    glutPostRedisplay();
}

/* ── reshape ─────────────────────────────────────────────── */
static void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    setup_projection();
}

/* ── mouse motion ────────────────────────────────────────── */
static void mouse_move(int x, int y)
{
    switch (G.stage) {
        case STAGE_MAKEUP:   makeup_mouse_move(x, y);   break;
        case STAGE_DRESSUP:  dressup_mouse_move(x, y);  break;
        case STAGE_SHOWCASE: showcase_mouse_move(x, y); break;
    }
    glutPostRedisplay();
}

/* ── mouse click ─────────────────────────────────────────── */
static void mouse_click(int button, int state, int x, int y)
{
    if (G.transitioning) return; /* ignore clicks during fade */
    switch (G.stage) {
        case STAGE_MAKEUP:   makeup_mouse_click(button, state, x, y);   break;
        case STAGE_DRESSUP:  dressup_mouse_click(button, state, x, y);  break;
        case STAGE_SHOWCASE: showcase_mouse_click(button, state, x, y); break;
    }
    glutPostRedisplay();
}

/* ── keyboard ────────────────────────────────────────────── */
static void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    if (key == 27) /* ESC */ exit(0);
    /* Quick stage skips for testing */
    if (key == '1') { G.stage = STAGE_MAKEUP;   G.animTime=0; }
    if (key == '2') { G.stage = STAGE_DRESSUP;  G.animTime=0; }
    if (key == '3') { G.stage = STAGE_SHOWCASE; G.animTime=0; }
    glutPostRedisplay();
}

/* ── main ────────────────────────────────────────────────── */
int main(int argc, char **argv)
{
    /* Initialise game state */
    memset(&G, 0, sizeof(G));
    G.stage       = STAGE_MAKEUP;
    G.lipR = 0.80f; G.lipG = 0.55f; G.lipB = 0.50f;
    G.eyeR = 0.50f; G.eyeG = 0.50f; G.eyeB = 0.50f;
    G.dressFade   = 1.f;
    G.eyeOpen     = 1;
    G.transAlpha  = 0.f;

    /* GLUT init */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 80);
    glutCreateWindow(WIN_TITLE);

    /* OpenGL state */
    glClearColor(0.05f, 0.02f, 0.08f, 1.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    setup_projection();

    /* Callbacks */
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouse_move);
    glutMotionFunc(mouse_move);
    glutMouseFunc(mouse_click);
    glutKeyboardFunc(keyboard);

    last_time_ms = glutGet(GLUT_ELAPSED_TIME);

    printf("===========================================\n");
    printf(" Glam Studio – Makeup & Dress-Up Game\n");
    printf("===========================================\n");
    printf(" Stage 1: Apply makeup to the face\n");
    printf(" Stage 2: Choose dress, hair & accessories\n");
    printf(" Stage 3: See your final glamorous look!\n");
    printf("-------------------------------------------\n");
    printf(" Keys: 1/2/3 = jump to stage | ESC = quit\n");
    printf("===========================================\n");

    glutMainLoop();
    return 0;
}