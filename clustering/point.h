//point.h
//COMP20003 proj1
// 2012s2

typedef struct{
	ld x;
	ld y;
} point;

void
point2string(char *s, point p);

ld
dist(point p, point q);

// add point q's coordinates
// to point p
// 	p += q
void
add_to(point *p, point q);

// divide point p's coordinates
// by integer z
// 	p /= z
void
divide(point *p, int z);

// initialize a point to (0,0)
void
initialize(point *p);
