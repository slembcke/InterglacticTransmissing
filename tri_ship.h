static const u8 TRI_UP_MSPRITE[] = {
    -8, -8, 0xE0, 0,
     0, -8, 0xE1, 0,
    -8,  0, 0xF0, 0,
     0,  0, 0xF1, 0,
    MSPRITE_END
};

static const u8 TRI_DOWN_MSPRITE[] = {
    -8, -8, 0xF0, SPR_FLIP_Y,
     0, -8, 0xF1, SPR_FLIP_Y,
    -8,  0, 0xE0, SPR_FLIP_Y,
     0,  0, 0xE1, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 TRI_RIGHT_MSPRITE[] = {
    -8, -8, 0xE2, 0,
     0, -8, 0xE3, 0,
    -8,  0, 0xF2, 0,
     0,  0, 0xF3, 0,
    MSPRITE_END
};

static const u8 TRI_LEFT_MSPRITE[] = {
    -8, -8, 0xE3, SPR_FLIP_X,
     0, -8, 0xE2, SPR_FLIP_X,
    -8,  0, 0xF3, SPR_FLIP_X,
     0,  0, 0xF2, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 TRI_RIGHT_UP_MSPRITE[] = {
    -8, -8, 0xE4, 0,
     0, -8, 0xE5, 0,
    -8,  0, 0xF4, 0,
     0,  0, 0xF5, 0,
    MSPRITE_END
};

static const u8 TRI_RIGHT_DOWN_MSPRITE[] = {
    -8, -8, 0xF4, SPR_FLIP_Y,
     0, -8, 0xF5, SPR_FLIP_Y,
    -8,  0, 0xE4, SPR_FLIP_Y,
     0,  0, 0xE5, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 TRI_LEFT_UP_MSPRITE[] = {
    -8, -8, 0xE5, SPR_FLIP_X,
     0, -8, 0xE4, SPR_FLIP_X,
    -8,  0, 0xF5, SPR_FLIP_X,
     0,  0, 0xF4, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 TRI_LEFT_DOWN_MSPRITE[] = {
    -8, -8, 0xF5, SPR_FLIP_X | SPR_FLIP_Y,
     0, -8, 0xF4, SPR_FLIP_X | SPR_FLIP_Y,
    -8,  0, 0xE5, SPR_FLIP_X | SPR_FLIP_Y,
     0,  0, 0xE4, SPR_FLIP_X | SPR_FLIP_Y,
    MSPRITE_END
};

// Map ship directions directly to the right, left, down, up input bits.
// NULL if conflict. (ex: right and left)
static const u8 * const TRI_DIRECTIONS[] = {
    TRI_LEFT_MSPRITE,
    TRI_UP_MSPRITE,
    TRI_RIGHT_MSPRITE,
    TRI_DOWN_MSPRITE,
};

