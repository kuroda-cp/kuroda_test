#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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