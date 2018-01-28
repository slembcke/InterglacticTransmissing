static const u8 BLUE_UP_MSPRITE[] = {
    -8, -8, 0xC0, 0,
     0, -8, 0xC1, 0,
    -8,  0, 0xD0, 0,
     0,  0, 0xD1, 0,
    MSPRITE_END
};

static const u8 BLUE_DOWN_MSPRITE[] = {
    -8, -8, 0xD0, SPR_FLIP_Y,
     0, -8, 0xD1, SPR_FLIP_Y,
    -8,  0, 0xC0, SPR_FLIP_Y,
     0,  0, 0xC1, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 BLUE_RIGHT_MSPRITE[] = {
    -8, -8, 0xC2, 0,
     0, -8, 0xC3, 0,
    -8,  0, 0xD2, 0,
     0,  0, 0xD3, 0,
    MSPRITE_END
};

static const u8 BLUE_LEFT_MSPRITE[] = {
    -8, -8, 0xC3, SPR_FLIP_X,
     0, -8, 0xC2, SPR_FLIP_X,
    -8,  0, 0xD3, SPR_FLIP_X,
     0,  0, 0xD2, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 BLUE_RIGHT_UP_MSPRITE[] = {
    -8, -8, 0xC4, 0,
     0, -8, 0xC5, 0,
    -8,  0, 0xD4, 0,
     0,  0, 0xD5, 0,
    MSPRITE_END
};

static const u8 BLUE_RIGHT_DOWN_MSPRITE[] = {
    -8, -8, 0xD4, SPR_FLIP_Y,
     0, -8, 0xD5, SPR_FLIP_Y,
    -8,  0, 0xC4, SPR_FLIP_Y,
     0,  0, 0xC5, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 BLUE_LEFT_UP_MSPRITE[] = {
    -8, -8, 0xC5, SPR_FLIP_X,
     0, -8, 0xC4, SPR_FLIP_X,
    -8,  0, 0xD5, SPR_FLIP_X,
     0,  0, 0xD4, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 BLUE_LEFT_DOWN_MSPRITE[] = {
    -8, -8, 0xD5, SPR_FLIP_X | SPR_FLIP_Y,
     0, -8, 0xD4, SPR_FLIP_X | SPR_FLIP_Y,
    -8,  0, 0xC5, SPR_FLIP_X | SPR_FLIP_Y,
     0,  0, 0xC4, SPR_FLIP_X | SPR_FLIP_Y,
    MSPRITE_END
};

// Map ship directions directly to the right, left, down, up input bits.
// NULL if conflict. (ex: right and left)
static const u8 * const BLUE_DIRECTIONS[] = {
    NULL,
    BLUE_UP_MSPRITE,
    BLUE_DOWN_MSPRITE,
    NULL,
    BLUE_LEFT_MSPRITE,
    BLUE_LEFT_UP_MSPRITE,
    BLUE_LEFT_DOWN_MSPRITE,
    BLUE_LEFT_MSPRITE,
    BLUE_RIGHT_MSPRITE,
    BLUE_RIGHT_UP_MSPRITE,
    BLUE_RIGHT_DOWN_MSPRITE,
    BLUE_RIGHT_MSPRITE,
    NULL,
    BLUE_UP_MSPRITE,
    BLUE_DOWN_MSPRITE,
    NULL,
};


