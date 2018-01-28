static const u8 RED_UP_MSPRITE[] = {
    -8, -8, 0xA0, 0,
     0, -8, 0xA1, 0,
    -8,  0, 0xB0, 0,
     0,  0, 0xB1, 0,
    MSPRITE_END
};

static const u8 RED_DOWN_MSPRITE[] = {
    -8, -8, 0xB0, SPR_FLIP_Y,
     0, -8, 0xB1, SPR_FLIP_Y,
    -8,  0, 0xA0, SPR_FLIP_Y,
     0,  0, 0xA1, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 RED_RIGHT_MSPRITE[] = {
    -8, -8, 0xA2, 0,
     0, -8, 0xA3, 0,
    -8,  0, 0xB2, 0,
     0,  0, 0xB3, 0,
    MSPRITE_END
};

static const u8 RED_LEFT_MSPRITE[] = {
    -8, -8, 0xA3, SPR_FLIP_X,
     0, -8, 0xA2, SPR_FLIP_X,
    -8,  0, 0xB3, SPR_FLIP_X,
     0,  0, 0xB2, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 RED_RIGHT_UP_MSPRITE[] = {
    -8, -8, 0xA4, 0,
     0, -8, 0xA5, 0,
    -8,  0, 0xB4, 0,
     0,  0, 0xB5, 0,
    MSPRITE_END
};

static const u8 RED_RIGHT_DOWN_MSPRITE[] = {
    -8, -8, 0xB4, SPR_FLIP_Y,
     0, -8, 0xB5, SPR_FLIP_Y,
    -8,  0, 0xA4, SPR_FLIP_Y,
     0,  0, 0xA5, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 RED_LEFT_UP_MSPRITE[] = {
    -8, -8, 0xA5, SPR_FLIP_X,
     0, -8, 0xA4, SPR_FLIP_X,
    -8,  0, 0xB5, SPR_FLIP_X,
     0,  0, 0xB4, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 RED_LEFT_DOWN_MSPRITE[] = {
    -8, -8, 0xB5, SPR_FLIP_X | SPR_FLIP_Y,
     0, -8, 0xB4, SPR_FLIP_X | SPR_FLIP_Y,
    -8,  0, 0xA5, SPR_FLIP_X | SPR_FLIP_Y,
     0,  0, 0xA4, SPR_FLIP_X | SPR_FLIP_Y,
    MSPRITE_END
};

// Map ship directions directly to the right, left, down, up input bits.
// NULL if conflict. (ex: right and left)
static const u8 * const RED_DIRECTIONS[] = {
    NULL,
    RED_UP_MSPRITE,
    RED_DOWN_MSPRITE,
    NULL,
    RED_LEFT_MSPRITE,
    RED_LEFT_UP_MSPRITE,
    RED_LEFT_DOWN_MSPRITE,
    RED_LEFT_MSPRITE,
    RED_RIGHT_MSPRITE,
    RED_RIGHT_UP_MSPRITE,
    RED_RIGHT_DOWN_MSPRITE,
    RED_RIGHT_MSPRITE,
    NULL,
    RED_UP_MSPRITE,
    RED_DOWN_MSPRITE,
    NULL,
};


