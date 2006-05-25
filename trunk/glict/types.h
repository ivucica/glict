#ifndef _GLIT_TYPES_H
#define _GLIT_TYPES_H
typedef struct {
    union {
        int x,w;
    };
    union {
        int y,h;
    };
} glitPos; 
typedef glitPos glitSize;
typedef struct {
    float r, g, b, a;
} glitColor;
enum glitEvents {
    GLIT_MOUSECLICK = 0
};
#endif
