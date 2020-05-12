
typedef struct{
	int kind; //1:pitch(OTAMA) 2:rest
	int sx;
	int sy;
	int ex;
	int ey;
	int type; //1:全音符、2:2分音符、3:４分音符　4:8分音符・・・
	int futen;
	int pitch; //音程ポジション第1線のミが0
	int UpDown;
	int Chord;
	int Tied;
	int Tuplet;
	int actualNotes;
	int normalNotes;
}YAML_NOTE_HEAD;

typedef struct{
	int ID;
	int sx;
	int sy;
	int bokoStartX;
	int numerator;
	int denominator;
	int SharpCount;
	int FlatCount;
	int ToHe; //1:To 2:He
	int note_cnt;
	int pitch_cnt;
	int rest_cnt;
	int chord_cnt;
	int real_note_cnt;
	YAML_NOTE_HEAD *note;
	YAML_NOTE_HEAD *chord_note;
}YAML_MEASURE_HEAD;

typedef struct{
	int sx;
	int ex;
	int sy;
	int ey;
	int interval;
	int Lirycs_StartY;
	int Lirycs_EndY;
	int measure_cnt;
	int *posx;
	YAML_MEASURE_HEAD *mbox;
}YAML_FIVELINE_HEAD;

typedef struct{
	int fivelinecnt;
	YAML_FIVELINE_HEAD *fiveline;
}YAML_PART_HEAD;

typedef struct{
	int imgw;
	int imgh;
	int bpm;
	int part_cnt;
	YAML_PART_HEAD *part;
}YAML_PAGE_HEAD;

typedef struct{
	int pagecnt;
	YAML_PAGE_HEAD *page;
}YAML_ORIGINAL_HEAD;

char * _readFileBase(char *fname, int *rsize);
int _getKeyValue(char *line, int lcnt);
int _getKeyName(char *line, int lcnt, char *keybuf);
int _getOneLine(char *buf, int size, int spos, char *line);


//YamlReadSet
YAML_ORIGINAL_HEAD *_createYAMLoriginalInfo(char *buf, int size);

//YAML情報から指定開始位置の次のKEY（コロン含む）の値を取得する
int _getYamlValue(char *buf, int size, int spos, char *key, char *stopkey, int *rpos);
void _freeYAMLoriginal(YAML_ORIGINAL_HEAD *yaml);
//YAMLバッファを指定KEYの先頭まで移動
int _moveYamlPosition(char *buf, int size, int spos, char *key);
//ストップキーまでに当該Keyがあればそのポジションを返却
int _existYamlKey(char *buf, int size, int spos, char *key, char *stopkey);
int _getNextOtamaRest(char *buf, int size, int spos, int *rkind);
//YAML情報から指定開始位置の次のKEY（コロン含む）の値を取得するオタマ、REST専用
int _getYamlValueOtamaRest(char *buf, int size, int spos, char *key, int *rpos);

int _createYAMLpageInfo(char *buf, int size, YAML_ORIGINAL_HEAD *yaml);
int _createYAMLpartInfo(char *buf, int size, int spos, YAML_PAGE_HEAD *page);
int _createYAMLfivelineInfo(char *buf, int size, int spos, YAML_PART_HEAD *part);
int _createYAMLmeasureInfo(char *buf, int size, int spos, YAML_FIVELINE_HEAD *fiveline);
int _createYAMLnoteInfo(char *buf, int size, int spos, YAML_MEASURE_HEAD *mbox);
