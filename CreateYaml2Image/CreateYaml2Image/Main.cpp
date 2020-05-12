#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Common.h"

char gName[256];

int main(int argc, char *argv[])
{
	char *yamlname;
	char *buf;
	int size;
	int len;
	int i, j;
	YAML_ORIGINAL_HEAD *yaml;

	if (argc < 2) return -1;
	yamlname = argv[1];
	buf = _readFileBase(yamlname, &size);
	if (buf == NULL){
		return -1;
	}

	len = (int)strlen(yamlname);
	memset(gName, 0, 256);
	for (i = len - 1; i >= 0; i--){
		if (yamlname[i] == '\\'){
			break;
		}
	}
	i++;
	j = 0;
	for (; i < len - 5; i++){
		gName[j++] = yamlname[i];
	}

	yaml = _createYAMLoriginalInfo(buf, size);

	free(buf);

	if (yaml == NULL) return -1;

	for (i = 0; i < yaml->pagecnt; i++){

	}

	_freeYAMLoriginal(yaml);

	return 0;
}

void _createOnePageImage(int pno, YAML_PAGE_HEAD *page, char *outbase)
{
	int ww, hh;
	char *cbuf;

	ww = page->imgw;
	hh = page->imgh;

	cbuf = (char *)malloc(ww * hh);
	if (cbuf == NULL) return;
	memset(cbuf, 0, ww * hh);

}

