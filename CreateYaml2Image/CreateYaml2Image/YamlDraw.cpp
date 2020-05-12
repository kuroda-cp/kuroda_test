#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Common.h"

void yd_drawLine(char *cbuf, int ww, int hh, int sx, int sy, int ex, int ey, int lw)
{
	int ssx, ssy, eex, eey;
	int x, y;

	if (sx == ex){
		ssx = sx - (lw / 2);
		if (lw % 2 == 0) ssx += 1;
		eex = sx + (lw / 2);
		for (y = sy; y <= ey; y++){
			for (x = ssx; x <= eex; x++){
				if (x < 0) continue;
				if (x >= ww) break;
				cbuf[(y * ww) + x] = 0x01;
			}
		}
	}
	if (sy == ey){
		ssy = sy - (lw / 2);
		if (lw % 2 == 0) ssy += 1;
		eey = sy + (lw / 2);
		for (y = ssy; y <= eey; y++){
			if (y < 0) continue;
			if (y >= hh) break;
			for (x = sx; x <= ex; x++){
				cbuf[(y * ww) + x] = 0x01;
			}
		}
	}
}