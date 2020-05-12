#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Common.h"

YAML_ORIGINAL_HEAD *_createYAMLoriginalInfo(char *buf, int size)
{
	YAML_ORIGINAL_HEAD *yaml;
	YAML_PAGE_HEAD *page;

	int img_cnt;
	int rpos;
	int ret;

	img_cnt = _getYamlValue(buf, size, 0, "ImageCount:", "Images:", &rpos);
	if (img_cnt == -1) return NULL;

	yaml = (YAML_ORIGINAL_HEAD *)malloc(sizeof(YAML_ORIGINAL_HEAD));
	if (yaml == NULL) return NULL;
	memset(yaml, 0, sizeof(YAML_ORIGINAL_HEAD));

	page = (YAML_PAGE_HEAD *)malloc(sizeof(YAML_PAGE_HEAD) * img_cnt);
	if (page == NULL){
		free(yaml);
		return NULL;
	}
	memset(page, 0, sizeof(YAML_PAGE_HEAD) * img_cnt);
	yaml->pagecnt = img_cnt;
	yaml->page = page;

	ret = _createYAMLpageInfo(buf, size, yaml);
	if (ret == -1){
		//free
		_freeYAMLoriginal(yaml);
		return NULL;
	}

	return yaml;
}

int _createYAMLpageInfo(char *buf, int size, YAML_ORIGINAL_HEAD *yaml)
{
	int pos, rpos;
	int cnt;
	int i;
	YAML_PART_HEAD *part;
	int ret;

	pos = 0;
	for (i = 0; i < yaml->pagecnt; i++){
		yaml->page[i].imgw = _getYamlValue(buf, size, pos, "ImageWidth:", "Parts:", &rpos);
		yaml->page[i].imgh = _getYamlValue(buf, size, pos, "ImageHeight:", "Parts:", &rpos);
		yaml->page[i].bpm = _getYamlValue(buf, size, pos, "Bpm:", "Parts:", &rpos);
		cnt = _getYamlValue(buf, size, pos, "PartCount:", "Parts:", &rpos);
		part = (YAML_PART_HEAD *)malloc(sizeof(YAML_PART_HEAD) * cnt);
		if (part == NULL){
			return -1;
		}
		memset(part, 0, sizeof(YAML_PART_HEAD) * cnt);
		yaml->page[i].part_cnt = cnt;
		yaml->page[i].part = part;
		//set part Under
		ret = _createYAMLpartInfo(buf, size, rpos, &yaml->page[i]);
		if (ret == -1){
			return ret;
		}

		//next PAGE //image
		pos = _moveYamlPosition(buf, size, rpos, "ImageWidth:");
	}
	return 0;
}

int _createYAMLpartInfo(char *buf, int size, int spos, YAML_PAGE_HEAD *page)
{
	int i;
	int cnt;
	int pos, rpos;
	int ret;
	YAML_FIVELINE_HEAD *fiveline;

	pos = spos;
	for (i = 0; i < page->part_cnt; i++){
		cnt = _getYamlValue(buf, size, pos, "FiveLineInfoCount:", "FiveLine:", &rpos);
		fiveline = (YAML_FIVELINE_HEAD *)malloc(sizeof(YAML_FIVELINE_HEAD) * cnt);
		if (fiveline == NULL) return -1;
		memset(fiveline, 0, sizeof(YAML_FIVELINE_HEAD) * cnt);
		page->part[i].fivelinecnt = cnt;
		page->part[i].fiveline = fiveline;
		//set fiveline Under
		ret = _createYAMLfivelineInfo(buf, size, rpos, &page->part[i]);
		if (ret == -1) return ret;

		pos = _moveYamlPosition(buf, size, rpos, "Part:");
	}
	return 0;
}

int _createYAMLfivelineInfo(char *buf, int size, int spos, YAML_PART_HEAD *part)
{
	int pos, rpos, wkpos;
	int i, j;
	int cnt, ret;
	int *posx;
	YAML_MEASURE_HEAD *mbox;

	pos = spos;
	for (i = 0; i < part->fivelinecnt; i++){
		part->fiveline[i].sx = _getYamlValue(buf, size, pos, "StartX:", "Boxes:", &rpos);
		part->fiveline[i].sy = _getYamlValue(buf, size, pos, "StartY:", "Boxes:", &rpos);
		part->fiveline[i].ex = _getYamlValue(buf, size, pos, "EndX:", "Boxes:", &rpos);
		part->fiveline[i].ey = _getYamlValue(buf, size, pos, "EndY:", "Boxes:", &rpos);
		//search Lirycs
		wkpos = _existYamlKey(buf, size, pos, "Lirycs:", "Boxes:");
		if (wkpos > 0){
			part->fiveline[i].Lirycs_StartY = _getYamlValue(buf, size, wkpos, "StartY:", "Boxes:", &rpos);
			part->fiveline[i].Lirycs_EndY = _getYamlValue(buf, size, wkpos, "EndY:", "Boxes:", &rpos);
		}
		part->fiveline[i].interval = _getYamlValue(buf, size, pos, "Interval:", "Boxes:", &rpos);
		//search measure
		wkpos = _existYamlKey(buf, size, pos, "Measure:", "Boxes:");
		if (wkpos > 0){
			cnt = _getYamlValue(buf, size, wkpos, "Count:", "Boxes:", &rpos);
			posx = (int *)malloc(sizeof(int) * cnt);
			if (posx == NULL) return -1;
			memset(posx, 0, sizeof(int) *cnt);
			for (j = 0; j < cnt; j++){
				posx[j] = _getYamlValue(buf, size, wkpos, "POSX:", "Boxes:", &rpos);
				wkpos = rpos;
			}
			cnt -= 1; //YAMLは先頭ラインから終端ラインの数をCountで記録しているため
			mbox = (YAML_MEASURE_HEAD *)malloc(sizeof(YAML_MEASURE_HEAD) * cnt);
			if (mbox == NULL) return -1;
			memset(mbox, 0, sizeof(YAML_MEASURE_HEAD) * cnt);
			part->fiveline[i].measure_cnt = cnt;
			part->fiveline[i].mbox = mbox;
		}
		//set measure
		ret = _createYAMLmeasureInfo(buf, size, rpos, &part->fiveline[i]);
		if (ret == -1)return ret;

		pos = _moveYamlPosition(buf, size, rpos, "FiveLine:");
	}
	return 0;
}

int _createYAMLmeasureInfo(char *buf, int size, int spos, YAML_FIVELINE_HEAD *fiveline)
{
	int pos, rpos;
	int i, ret;
	int pcnt, rcnt, ncnt;
	YAML_NOTE_HEAD *note;

	pos = spos;
	for (i = 0; i < fiveline->measure_cnt; i++){
		fiveline->mbox[i].ID = _getYamlValue(buf, size, pos, "ID:", "Notes:", &rpos);
		fiveline->mbox[i].sx = _getYamlValue(buf, size, pos, "SX:", "Notes:", &rpos);
		fiveline->mbox[i].sy = _getYamlValue(buf, size, pos, "SY:", "Notes:", &rpos);
		fiveline->mbox[i].bokoStartX = _getYamlValue(buf, size, pos, "BokoStartX:", "Notes:", &rpos);
		fiveline->mbox[i].SharpCount = _getYamlValue(buf, size, pos, "SharpCount:", "Notes:", &rpos);
		fiveline->mbox[i].FlatCount = _getYamlValue(buf, size, pos, "FlatCount:", "Notes:", &rpos);
		fiveline->mbox[i].numerator = _getYamlValue(buf, size, pos, "numerator:", "Notes:", &rpos);
		fiveline->mbox[i].denominator = _getYamlValue(buf, size, pos, "denominator:", "Notes:", &rpos);
		fiveline->mbox[i].ToHe = _getYamlValue(buf, size, pos, "ToHe:", "Notes:", &rpos);
		pcnt = _getYamlValue(buf, size, pos, "PitchCount:", "Notes:", &rpos);
		rcnt = _getYamlValue(buf, size, pos, "RestCount:", "Notes:", &rpos);
		ncnt = pcnt + rcnt;
		fiveline->mbox[i].pitch_cnt = pcnt;
		fiveline->mbox[i].rest_cnt = rcnt;
		fiveline->mbox[i].note_cnt = ncnt;
		note = (YAML_NOTE_HEAD *)malloc(sizeof(YAML_NOTE_HEAD) * ncnt);
		if (note == NULL) return -1;
		memset(note, 0, sizeof(YAML_NOTE_HEAD) * ncnt);
		fiveline->mbox[i].note = note;
		//set note
		ret = _createYAMLnoteInfo(buf, size, rpos, &fiveline->mbox[i]);
		if (ret == -1) return ret;

		pos = _moveYamlPosition(buf, size, rpos, "Box:");
	}
	return 0;
}

int _createYAMLnoteInfo(char *buf, int size, int spos, YAML_MEASURE_HEAD *mbox)
{
	int pos, rpos;
	int i;
	int nextpos, kind;
	int chord_cnt;
	int chord_val;
	int real_note_cnt;
	YAML_NOTE_HEAD *chord;
	int outhit;

	rpos = pos = spos;
	chord_cnt = 0;
	real_note_cnt = 0;
	//数だけ先に数える 2020.03.30
	while (1){
		nextpos = _getNextOtamaRest(buf, size, rpos, &kind);
		if (nextpos == -1) break;
		pos = nextpos;
		chord_val = _getYamlValueOtamaRest(buf, size, pos, "Chord:", &rpos);
		_getYamlValueOtamaRest(buf, size, pos, "Futen:", &rpos);
		if (chord_val == 0){
			real_note_cnt++;
		}
		else{
			chord_cnt++;
		}
	}
	rpos = pos = spos;
	outhit = 0;
	for (i = 0; i < mbox->note_cnt; i++){
		nextpos = _getNextOtamaRest(buf, size, rpos, &kind);
		if (nextpos == -1){
			break;
		}
		pos = nextpos;
		chord_val = _getYamlValueOtamaRest(buf, size, pos, "Chord:", &rpos);
		if (chord_val != 0){
			_getYamlValueOtamaRest(buf, size, pos, "Futen:", &rpos);
			i--;
			continue;
		}
		mbox->note[i].kind = kind;
		mbox->note[i].sx = _getYamlValueOtamaRest(buf, size, pos, "SX:", &rpos);
		mbox->note[i].sy = _getYamlValueOtamaRest(buf, size, pos, "SY:", &rpos);
		mbox->note[i].ex = _getYamlValueOtamaRest(buf, size, pos, "EX:", &rpos);
		mbox->note[i].ey = _getYamlValueOtamaRest(buf, size, pos, "EY:", &rpos);
		mbox->note[i].type = _getYamlValueOtamaRest(buf, size, pos, "Type:", &rpos);
		mbox->note[i].Tuplet = _getYamlValueOtamaRest(buf, size, pos, "Tuplet:", &rpos);
		mbox->note[i].actualNotes = _getYamlValueOtamaRest(buf, size, pos, "actualNotes:", &rpos);
		mbox->note[i].normalNotes = _getYamlValueOtamaRest(buf, size, pos, "normalNotes:", &rpos);
		mbox->note[i].futen = _getYamlValueOtamaRest(buf, size, pos, "Futen:", &rpos);

		if (kind == 1){
			mbox->note[i].pitch = _getYamlValueOtamaRest(buf, size, pos, "Pitch:", &rpos);
			mbox->note[i].Chord = _getYamlValueOtamaRest(buf, size, pos, "Chord:", &rpos);
			mbox->note[i].Tied = _getYamlValueOtamaRest(buf, size, pos, "Tied:", &rpos);
			mbox->note[i].UpDown = _getYamlValueOtamaRest(buf, size, pos, "UpDown:", &rpos);
		}

	}
	mbox->real_note_cnt = real_note_cnt;
	if (chord_cnt > 0){
		rpos = pos = spos;
		chord = (YAML_NOTE_HEAD *)malloc(sizeof(YAML_NOTE_HEAD) * chord_cnt);
		if (chord != NULL){
			memset(chord, 0, sizeof(YAML_NOTE_HEAD) * chord_cnt);
			mbox->chord_cnt = chord_cnt;
			mbox->chord_note = chord;
			for (i = 0; i < chord_cnt; i++){
				nextpos = _getNextOtamaRest(buf, size, rpos, &kind);
				if (nextpos == -1){
					mbox->real_note_cnt = real_note_cnt;
					break;
				}
				pos = nextpos;
				chord_val = _getYamlValueOtamaRest(buf, size, pos, "Chord:", &rpos);
				if (chord_val == 0){
					_getYamlValueOtamaRest(buf, size, pos, "Futen:", &rpos);
					i--;
					continue;
				}
				mbox->chord_note[i].kind = kind;
				mbox->chord_note[i].sx = _getYamlValueOtamaRest(buf, size, pos, "SX:", &rpos);
				mbox->chord_note[i].sy = _getYamlValueOtamaRest(buf, size, pos, "SY:", &rpos);
				mbox->chord_note[i].ex = _getYamlValueOtamaRest(buf, size, pos, "EX:", &rpos);
				mbox->chord_note[i].ey = _getYamlValueOtamaRest(buf, size, pos, "EY:", &rpos);
				mbox->chord_note[i].type = _getYamlValueOtamaRest(buf, size, pos, "Type:", &rpos);
				mbox->chord_note[i].Tuplet = _getYamlValueOtamaRest(buf, size, pos, "Tuplet:", &rpos);
				mbox->chord_note[i].actualNotes = _getYamlValueOtamaRest(buf, size, pos, "actualNotes:", &rpos);
				mbox->chord_note[i].normalNotes = _getYamlValueOtamaRest(buf, size, pos, "normalNotes:", &rpos);
				mbox->chord_note[i].futen = _getYamlValueOtamaRest(buf, size, pos, "Futen:", &rpos);

				if (kind == 1){
					mbox->chord_note[i].pitch = _getYamlValueOtamaRest(buf, size, pos, "Pitch:", &rpos);
					mbox->chord_note[i].Chord = _getYamlValueOtamaRest(buf, size, pos, "Chord:", &rpos);
					mbox->chord_note[i].Tied = _getYamlValueOtamaRest(buf, size, pos, "Tied:", &rpos);
					mbox->chord_note[i].UpDown = _getYamlValueOtamaRest(buf, size, pos, "UpDown:", &rpos);
				}
			}
		}
	}
	return 0;
}

//YAML情報から指定開始位置の次のKEY（コロン含む）の値を取得するオタマ、REST専用
int _getYamlValueOtamaRest(char *buf, int size, int spos, char *key, int *rpos)
{
	char wknum[64];
	int i, j;
	int len;
	int pos, val;

	val = pos = -1;
	len = (int)strlen(key);
	for (i = spos; i < size; i++){
		if (key[0] == buf[i]){
			if (strncmp(&buf[i], key, len) == 0){
				i += len;
				memset(wknum, 0, 64);
				j = 0;
				for (; i < size; i++){
					if ((buf[i] >= 0x30) && (buf[i] <= 0x39)){
						wknum[j++] = buf[i];
					}
					if (buf[i] == 0x2d){
						wknum[j++] = buf[i];
					}
					if (buf[i] == 0x0a) break;
				}
				val = atoi(wknum);
				pos = i;
				break;
			}
		}
		if (i > spos + 1){
			if (buf[i] == 'O'){
				if (strncmp(&buf[i], "Otama:", 6) == 0) break;
			}
			if (buf[i] == 'R'){
				if (strncmp(&buf[i], "Rest:", 5) == 0) break;
			}
		}
	}
	if (pos == -1) val = 0;
	*rpos = pos;
	return val;
}

int _getNextOtamaRest(char *buf, int size, int spos, int *rkind)
{
	int i;

	for (i = spos; i < size; i++){
		if (buf[i] == 'O'){
			if (strncmp(&buf[i], "Otama:", 6) == 0){
				*rkind = 1;
				return i;
			}
		}
		if (buf[i] == 'R'){
			if (strncmp(&buf[i], "Rest:", 5) == 0){
				*rkind = 2;
				return i;
			}
		}
		if (buf[i] == 'B'){
			if (strncmp(&buf[i], "Box:", 4) == 0){
				return -1;
			}
		}
	}
	return -1;
}

//ストップキーまでに当該Keyがあればそのポジションを返却
int _existYamlKey(char *buf, int size, int spos, char *key, char *stopkey)
{
	int len, stlen;
	int i;

	len = (int)strlen(key);
	stlen = (int)strlen(stopkey);
	for (i = spos; i < size; i++){
		if (buf[i] == key[0]){
			if (strncmp(&buf[i], key, len) == 0){
				return i;
			}
		}
		if (buf[i] == stopkey[0]){
			if (strncmp(&buf[i], stopkey, stlen) == 0){
				return -1;
			}
		}
	}
	return -1;
}

//YAMLバッファを指定KEYの先頭まで移動
int _moveYamlPosition(char *buf, int size, int spos, char *key)
{
	int i, len;

	len = (int)strlen(key);
	for (i = spos; i < size; i++){
		if (buf[i] == key[0]){
			if (strncmp(&buf[i], key, len) == 0){
				return i;
			}
		}
	}
	return -1;
}

//YAML情報から指定開始位置の次のKEY（コロン含む）の値を取得する
int _getYamlValue(char *buf, int size, int spos, char *key, char *stopkey, int *rpos)
{
	char wknum[64];
	int i, j;
	int len;
	int pos, val;
	int stoplen;

	val = pos = -1;
	len = (int)strlen(key);
	stoplen = (int)strlen(stopkey);
	for (i = spos; i < size; i++){
		if (key[0] == buf[i]){
			if (strncmp(&buf[i], key, len) == 0){
				i += len;
				memset(wknum, 0, 64);
				j = 0;
				for (; i < size; i++){
					if ((buf[i] >= 0x30) && (buf[i] <= 0x39)){
						wknum[j++] = buf[i];
					}
					if (buf[i] == 0x0a) break;
				}
				val = atoi(wknum);
				pos = i;
				break;
			}
		}
		if (buf[i] == stopkey[0]){
			if (strncmp(&buf[i], stopkey, stoplen) == 0){
				break;
			}
		}
	}
	*rpos = pos;
	return val;
}

void _freeYAMLoriginal(YAML_ORIGINAL_HEAD *yaml)
{
	int page, p, fl, mb;
	YAML_MEASURE_HEAD *mbox;
	YAML_FIVELINE_HEAD *fiveline;
	YAML_PART_HEAD *part;
	if (yaml == NULL) return;

	for (page = 0; page < yaml->pagecnt; page++){
		for (p = 0; p < yaml->page[page].part_cnt; p++){
			part = &yaml->page[page].part[p];
			for (fl = 0; fl < part->fivelinecnt; fl++){
				fiveline = &part->fiveline[fl];
				for (mb = 0; mb < fiveline->measure_cnt; mb++){
					mbox = &fiveline->mbox[mb];
					if (mbox->note != NULL){
						free(mbox->note);
					}
					if (mbox->chord_note != NULL){
						free(mbox->chord_note);
					}
				}
				if (fiveline->mbox != NULL){
					free(fiveline->mbox);
				}
				if (fiveline->posx != NULL){
					free(fiveline->posx);
				}
			}
			if (part->fiveline != NULL){
				free(part->fiveline);
			}
		}
		if (yaml->page[page].part != NULL){
			free(yaml->page[page].part);
		}
	}
	if (yaml->page != NULL){
		free(yaml->page);
	}
	free(yaml);
}
