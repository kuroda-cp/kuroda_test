#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Common.h"

void yd_drawLine(char *cbuf, int ww, int hh, int sx, int sy, int ex, int ey, int lw)
{
	int ssx, ssy, eex, eey;
	int x, y;
	double kata, b;
	double dx, dy;
	int ix, iy;

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
	//ŽÎ‚ßü
	kata = (double)(ex - sx) / (double)(ey - sy);

	if (abs(ey - sy) > abs(ex - sx)){
		//y Trace
		ssx = sx - (lw / 2);
		if (lw % 2 == 0) ssx += 1;
		eex = sx + (lw / 2);
		for (y = sy; y <= ey; y++){
			for (x = ssx; x <= eex; x++){
				b = (double)y - (double)x * kata;
				dx = ((double)y - b) / kata;
				ix = (int)dx + 0.5;
				if (ix < 0) continue;
				if (ix >= ww) continue;
				cbuf[(y * ww) + ix] = 0x01;
			}
		}
	}
	else{
		//x Trace
		ssy = sy - (lw / 2);
		if (lw % 2 == 0) ssy += 1;
		eey = sy + (lw / 2);
		for (x = sx; x <= ex; x++){
			for (y = ssy; y <= eey; y++){
				b = (double)y - (double)x * kata;
				dy = (double)x * kata + b;
				iy = (int)(dy + 0.5);
				if (iy < 0) continue;
				if (iy >= hh) continue;
			}
		}
	}

}