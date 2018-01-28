
static const u8 ANG_UP_MSPRITE[] = {
    -8, -8, 0x86, 0,
     0, -8, 0x87, 0,
    -8,  0, 0x96, 0,
     0,  0, 0x97, 0,
    MSPRITE_END
};

static const u8 ANG_DOWN_MSPRITE[] = {
    -8, -8, 0x96, SPR_FLIP_Y,
     0, -8, 0x97, SPR_FLIP_Y,
    -8,  0, 0x86, SPR_FLIP_Y,
     0,  0, 0x87, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 ANG_RIGHT_MSPRITE[] = {
    -8, -8, 0x88, 0,
     0, -8, 0x89, 0,
    -8,  0, 0x98, 0,
     0,  0, 0x99, 0,
    MSPRITE_END
};

static const u8 ANG_LEFT_MSPRITE[] = {
    -8, -8, 0x89, SPR_FLIP_X,
     0, -8, 0x88, SPR_FLIP_X,
    -8,  0, 0x99, SPR_FLIP_X,
     0,  0, 0x98, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 ANG_RIGHT_UP_MSPRITE[] = {
    -8, -8, 0x8A, 0,
     0, -8, 0x8B, 0,
    -8,  0, 0x9A, 0,
     0,  0, 0x9B, 0,
    MSPRITE_END
};

static const u8 ANG_RIGHT_DOWN_MSPRITE[] = {
    -8, -8, 0x9A, SPR_FLIP_Y,
     0, -8, 0x9B, SPR_FLIP_Y,
    -8,  0, 0x8A, SPR_FLIP_Y,
     0,  0, 0x8B, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 ANG_LEFT_UP_MSPRITE[] = {
    -8, -8, 0x8B, SPR_FLIP_X,
     0, -8, 0x8A, SPR_FLIP_X,
    -8,  0, 0x9B, SPR_FLIP_X,
     0,  0, 0x9A, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 ANG_LEFT_DOWN_MSPRITE[] = {
    -8, -8, 0x9B, SPR_FLIP_X | SPR_FLIP_Y,
     0, -8, 0x9A, SPR_FLIP_X | SPR_FLIP_Y,
    -8,  0, 0x8B, SPR_FLIP_X | SPR_FLIP_Y,
     0,  0, 0x8A, SPR_FLIP_X | SPR_FLIP_Y,
    MSPRITE_END
};

// Map ship directions directly to the right, left, down, up input bits.
// NULL if conflict. (ex: right and left)
static const u8 * const ANG_DIRECTIONS[] = {
    ANG_LEFT_MSPRITE,
    ANG_UP_MSPRITE,
    ANG_RIGHT_MSPRITE,
    ANG_DOWN_MSPRITE,
};
