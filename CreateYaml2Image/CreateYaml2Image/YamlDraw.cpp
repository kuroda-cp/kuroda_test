#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Common.h"

void yd_drawWhileOtama(char *cbuf, int ww, int hh, int sx, int sy, int ex, int ey)
{
	int cx, cy;
	int dh, dw;
	int x, y;
	double rate;
	int hcnt;
	int wcnt;
	int ssx, eex;

	cx = sx + (ex - sx) / 2;
	cy = sy + (ey - sy) / 2;
	dh = cy - sy + 1;
	dw = ex - sx + 1;

	rate = 1 / (double)dh;

	hcnt = 0;
	for (y = cy; y >= sy; y--){
		rate = (double)(dh - hcnt) / (double)dh;
		wcnt = (int)((double)dw * rate);
		ssx = cx - (wcnt / 2);
		eex = cx + (wcnt / 2);
		if (eex - ssx <= 4){
			for (x = ssx; x <= eex; x++){
				cbuf[(y * ww) + x] = 0x01;
			}
		}
		else{
			for (x = ssx; x <= ssx + 1; x++){
				cbuf[(y * ww) + x] = 0x01;
			}
			for (x = eex - 1; x <= eex; x++){
				cbuf[(y * ww) + x] = 0x01;
			}
		}
		hcnt++;
	}
	hcnt = 0;
	for (y = cy; y <= ey; y++){
		rate = (double)(dh - hcnt) / (double)dh;
		wcnt = (int)((double)dw * rate);
		ssx = cx - (wcnt / 2);
		eex = cx + (wcnt / 2);
		if (eex - ssx <= 4){
			for (x = ssx; x <= eex; x++){
				cbuf[(y * ww) + x] = 0x01;
			}
		}
		else{
			for (x = ssx; x <= ssx + 1; x++){
				cbuf[(y * ww) + x] = 0x01;
			}
			for (x = eex - 1; x <= eex; x++){
				cbuf[(y * ww) + x] = 0x01;
			}
		}
		hcnt++;
	}

}
void yd_drawBlackOtama(char *cbuf, int ww, int hh, int sx, int sy, int ex, int ey)
{
	int cx, cy;
	int dh, dw;
	int x, y;
	double rate;
	int hcnt;
	int wcnt;
	int ssx, eex;

	cx = sx + (ex - sx) / 2;
	cy = sy + (ey - sy) / 2;
	dh = cy - sy + 1;
	dw = ex - sx + 1;

	rate = 1 / (double)dh;

	hcnt = 0;
	for (y = cy; y >= sy; y--){
		rate = (double)(dh - hcnt) / (double)dh;
		wcnt = (int)((double)dw * rate);
		ssx = cx - (wcnt / 2);
		eex = cx + (wcnt / 2);
		for (x = ssx; x <= eex; x++){
			cbuf[(y * ww) + x] = 0x01;
		}
		hcnt++;
	}
	hcnt = 0;
	for (y = cy; y <= ey; y++){
		rate = (double)(dh - hcnt) / (double)dh;
		wcnt = (int)((double)dw * rate);
		ssx = cx - (wcnt / 2);
		eex = cx + (wcnt / 2);
		for (x = ssx; x <= eex; x++){
			cbuf[(y * ww) + x] = 0x01;
		}
		hcnt++;
	}
}

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