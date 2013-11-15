// point.c
// COMP20003 proj2
// 2012s2

#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "point.h"

void
point2string(char *s, point p)
{
	sprintf(s,"%20.10Lf  %20.10Lf",p.x,p.y);
}


ld square(ld x);

// actually, its the distance squared
// (to save execution time)
ld
dist(point p, point q)
{
	return square(p.x - q.x) + square(p.y - q.y);
}

/*ld
dist2(point p, point q)
{
	return (ld) 0;
}*/

ld
square(ld x)
{
	return x*x;
}

void
add_to(point *p, point q)
{
	p->x += q.x;
	p->y += q.y;
}

void
divide(point *p, int z)
{
	p->x /= z;
	p->y /= z;
}

void
initialize(point *p) {
	p->x = 0;
	p->y = 0;
}
