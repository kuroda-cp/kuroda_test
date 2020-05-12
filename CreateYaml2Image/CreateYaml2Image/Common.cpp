#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "Common.h"

char * _readFileBase(char *fname, int *rsize)
{
	FILE *fp;
	char *buf;
	int size;

	fopen_s(&fp, fname, "rb");
	if (fp == NULL) return NULL;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buf = (char *)malloc(size);
	if (buf == NULL){
		fclose(fp);
		return NULL;
	}
	memset(buf, 0, size);
	fread(buf, 1, size, fp);
	fclose(fp);

	*rsize = size;
	return buf;
}

int _getKeyValue(char *line, int lcnt)
{
	char wknum[64];
	int i, j, val;

	j = 0;
	memset(wknum, 0, 64);
	for (i = 0; i < lcnt; i++){
		if (line[i] == ':'){
			i++;
			break;
		}
	}
	if (i == lcnt) return 0;
	for (; i < lcnt; i++){
		if (line[i] == 0x20) continue;
		if (line[i] == 0x0a) break;
		wknum[j++] = line[i];
	}
	val = atoi(wknum);
	return val;
}

int _getKeyName(char *line, int lcnt, char *keybuf)
{
	int i, j;

	j = 0;
	for (i = 0; i < lcnt; i++){
		if (line[i] == 0x20) continue;
		if (line[i] == '-') continue;
		if (line[i] == 0x0a) break;
		if (line[i] == ':') break;
		keybuf[j++] = line[i];
		if (j == 128) break;
	}
	return j;
}

int _getOneLine(char *buf, int size, int spos, char *line)
{
	int i, j;

	j = 0;
	for (i = spos; i < size; i++){
		line[j++] = buf[i];
		if (buf[i] == 0x0a) break;
	}
	return j;
}

void dbg_createBMPFileCbuf(char *cbuf, int ww, int hh, char *fname, int ymillor)
{
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	int wbyte, bufsize;
	FILE *ofp;
	char *revbuf;
	int x, y;
	char val;

	if (cbuf == NULL) return;
	if ((ww < 0) || (hh < 0)) return;
	wbyte = ww * 3;
	if (wbyte % 4 != 0) wbyte += (4 - (wbyte % 4));
	bufsize = wbyte * hh;

	revbuf = (char *)malloc(bufsize);
	if (revbuf == NULL) return;
	memset(revbuf, 0, bufsize);
	for (y = 0; y < hh; y++){
		for (x = 0; x < ww; x++){
			val = cbuf[(y * ww) + x];
			if (val == 0x00) continue;
			revbuf[(y * wbyte) + (x * 3)] = 0;
			revbuf[(y * wbyte) + (x * 3) + 1] = 0;
			revbuf[(y * wbyte) + (x * 3) + 2] = 0;
		}
	}

	memset(&bf, 0, sizeof(BITMAPFILEHEADER));
	memset(&bi, 0, sizeof(BITMAPINFOHEADER));

	bf.bfType = 0x4d42; //BM

	bi.biSize = 40;
	bi.biWidth = ww;
	if (ymillor == 1){
		bi.biHeight = hh * -1;
	}
	else{
		bi.biHeight = hh;
	}
	bi.biBitCount = 24;
	bi.biPlanes = 1;
	bi.biCompression = 0;

	bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bufsize;
	bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	fopen_s(&ofp, fname, "wb");
	if (ofp == NULL){
		free(revbuf);
		return;
	}

	fwrite(&bf, 1, sizeof(BITMAPFILEHEADER), ofp);
	fwrite(&bi, 1, sizeof(BITMAPINFOHEADER), ofp);
	fwrite(revbuf, 1, bufsize, ofp);

	free(revbuf);

	fclose(ofp);
	return;
}