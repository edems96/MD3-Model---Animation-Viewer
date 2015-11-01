#ifndef TYPES_H
#define TYPES_H

typedef unsigned char	uchar;
typedef unsigned short  ushort;
typedef unsigned int	uint;
typedef long int		lint;

typedef enum bool {false, true} bool;

typedef enum MouseWheel {MOUSE_WHEEL_DOWN, MOUSE_WHEEL_UP} MouseWheel;

typedef struct vector {
	float x, y, z;
} vector;

typedef struct svector {
	short x, y, z;
} svector;

#endif