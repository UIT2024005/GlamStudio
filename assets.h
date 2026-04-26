#ifndef ASSETS_H
#define ASSETS_H

/* =========================================================
   assets.h  –  Global constants, colour palettes, enums
   ========================================================= */

/* ── Window ─────────────────────────────────────────────── */
#define WIN_W  900
#define WIN_H  650
#define WIN_TITLE "✨ Glam Studio – Makeup & Dress-Up Game"

/* ── Stages ─────────────────────────────────────────────── */
typedef enum {
    STAGE_MAKEUP  = 0,
    STAGE_DRESSUP = 1,
    STAGE_SHOWCASE= 2
} Stage;

/* ── Makeup choices ─────────────────────────────────────── */
typedef enum {
    LIP_NONE = 0,
    LIP_RED,
    LIP_PINK,
    LIP_NUDE,
    LIP_BERRY,
    LIP_CORAL,
    LIP_WINE
} LipColor;

typedef enum {
    EYE_NONE = 0,
    EYE_BLUE,
    EYE_PURPLE,
    EYE_GOLD,
    EYE_GREEN,
    EYE_TEAL,
    EYE_SILVER
} EyeShadow;

typedef enum {
    BLUSH_NONE = 0,
    BLUSH_ROSE,
    BLUSH_PEACH,
    BLUSH_PINK
} BlushState;

/* ── Dress-up choices ───────────────────────────────────── */
typedef enum {
    DRESS_NONE = 0,
    DRESS_GOWN,      /* elegant ball gown  */
    DRESS_CASUAL,    /* casual sundress    */
    DRESS_GLAMOUR    /* glam sequin dress  */
} DressStyle;

typedef enum {
    HAIR_NONE = 0,
    HAIR_LONG,       /* long wavy          */
    HAIR_BUN,        /* classic bun        */
    HAIR_CURLY       /* voluminous curls   */
} HairStyle;

typedef enum {
    ACC_NONE     = 0,
    ACC_EARRINGS = 1,
    ACC_NECKLACE = 2,
    ACC_BOTH     = 3
} Accessories;

/* ── Global game state ──────────────────────────────────── */
typedef struct {
    Stage stage;

    /* makeup */
    LipColor lip;
    EyeShadow eye;
    BlushState blush;
    float lipR, lipG, lipB;
    float eyeR, eyeG, eyeB;

    /* dress-up */
    DressStyle dress;
    HairStyle hair;
    Accessories accessories;

    /* animation */
    float animTime;
    float dressFade;

    float blinkTimer;

    /* NEW BLINK SYSTEM */
    float blinkHold;
    float blinkInterval;
    int isBlinking;
float poseCycle;
    /* BREATHING */
    float breathPhase;

    /* POSE SYSTEM (NEW) */
    float posePhase;

    /* transition */
    float transAlpha;
    int transitioning;

    /* old (you can keep or remove later) */
    int poseActive;
    float poseTimer;

} GameState;
extern GameState G;   /* defined in main.c */

/* ── Colour helpers ─────────────────────────────────────── */
/* Named palette so we can reference colours by name */
#define COL_BG_MAKEUP   0.99f, 0.95f, 0.96f   /* soft blush-white     */
#define COL_BG_DRESSUP  0.96f, 0.96f, 0.99f   /* lavender-white       */
#define COL_BG_SHOWCASE 0.10f, 0.06f, 0.14f   /* deep plum stage      */

#define COL_SKIN        0.96f, 0.82f, 0.72f
#define COL_SKIN_DARK   0.89f, 0.72f, 0.60f
#define COL_HAIR_DARK   0.18f, 0.10f, 0.06f
#define COL_HAIR_MED    0.35f, 0.20f, 0.10f
#define COL_HAIR_LIGHT  0.75f, 0.55f, 0.30f

#define COL_BLUSH_ROSE   0.95f, 0.50f, 0.55f
#define COL_BLUSH_PEACH  0.98f, 0.65f, 0.45f
#define COL_BLUSH_PINK   0.95f, 0.60f, 0.75f

#define COL_LIP_RED     0.90f, 0.10f, 0.15f
#define COL_LIP_PINK    0.95f, 0.45f, 0.60f
#define COL_LIP_NUDE    0.85f, 0.62f, 0.55f
#define COL_LIP_DEFAULT 0.80f, 0.55f, 0.50f
/* NEW */
#define COL_LIP_BERRY   0.65f, 0.05f, 0.25f
#define COL_LIP_CORAL   0.98f, 0.45f, 0.35f
#define COL_LIP_WINE    0.45f, 0.05f, 0.10f

#define COL_EYE_BLUE    0.35f, 0.55f, 0.90f
#define COL_EYE_PURPLE  0.60f, 0.20f, 0.80f
#define COL_EYE_GOLD    0.85f, 0.68f, 0.10f
/* NEW */
#define COL_EYE_GREEN   0.20f, 0.75f, 0.45f
#define COL_EYE_TEAL    0.15f, 0.65f, 0.70f
#define COL_EYE_SILVER  0.75f, 0.75f, 0.80f
#define COL_EYE_DEFAULT 0.50f, 0.50f, 0.50f

#define COL_BLUSH       0.95f, 0.60f, 0.65f

#define COL_DRESS_GOWN    0.55f, 0.20f, 0.55f
#define COL_DRESS_CASUAL  0.30f, 0.65f, 0.85f
#define COL_DRESS_GLAMOUR 0.80f, 0.20f, 0.45f

#define COL_BTN_BASE    0.88f, 0.70f, 0.80f
#define COL_BTN_HOVER   0.70f, 0.30f, 0.55f
#define COL_BTN_TEXT    1.00f, 1.00f, 1.00f

/* ── Math helpers ───────────────────────────────────────── */
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define DEG2RAD(d) ((d) * M_PI / 180.0)
#define COL_BTN_BASE_R 0.8f
#define COL_BTN_BASE_G 0.6f
#define COL_BTN_BASE_B 0.7f

#define COL_BTN_TEXT_R 1.0f
#define COL_BTN_TEXT_G 1.0f
#define COL_BTN_TEXT_B 1.0f
/* Lerp a→b by t */
static inline float lerpf(float a, float b, float t) {
    return a + (b - a) * t;
}

/* Clamp x into [lo, hi] */
static inline float clampf(float x, float lo, float hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}

#endif /* ASSETS_H */