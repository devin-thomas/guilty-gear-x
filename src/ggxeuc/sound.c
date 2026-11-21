/* Guilty Gear X US (SLUS-202.87): 19 functions ported from the X Plus
   decomp (ggxeuc/sound.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/sound.h"

static const short idtb[] = { 1, 2, 3 };

extern SONG_CONTL sngctl;
extern USER_SETTING userinfo;
int SnDrvBgmCheck(void);
int GetFileTop(int fid);
int GetFileSize(int fid);
void SnDrvBgmLoadCD(int top, int size);
void SnDrvBgmStart(int vol, int loop);
/* .sdata string after SEFileRead's and the TU's initialized sdata variables */
extern char D_5CC068[]; /* RODATA_EXTERN 0x5cc068: "%d %d\n" */

extern short int UseCharacter1p2p[];
extern GAME_INFOMATION g_info;
void SEFileRead(int fid, int num);

void FileRead(int fid, void *adr);
char *FileReadSelect(int fid, int chan);
void *mmalloc(int size, char *name);
static int chantbl[6] = { -1, -1, 3, -1, 4, -1 };
static int se_max_tbl[72] = {
    31, 31, 31, 8,
    42, 42, 42, 7,
    44, 44, 44, 7,
    29, 29, 29, 5,
    42, 42, 42, 6,
    38, 38, 38, 9,
    41, 41, 41, 8,
    39, 39, 39, 8,
    45, 45, 45, 7,
    44, 44, 44, 9,
    33, 33, 33, 9,
    48, 48, 48, 10,
    35, 35, 35, 8,
    40, 40, 40, 7,
    32, 32, 32, 8,
    26, 26, 26, 9,
    29, 29, 29, 6,
    0, 0, 0, 6,
};


extern USER_SETTING userinfo;
int GetFileSize(int fid);
void SnDrvBgmKeep(int vol, int loop, int size);


void SnDrvSeVReq(int channel, int fid, int num);
void SnDrvSeTrans(char *top, int size, int chan);
void SnDrvStop(int chan);

void sound_rec_clear(void);
void func_001061A8(int fid, void *adr);


void func_001942A0(int fid, int num)
{
    char *adr;

    sound_rec_clear();
    if (num < 2) {
        adr = mmalloc(GetFileSize(fid), "SEFile");
        SEFile[num] = adr;
        func_001061A8(fid, adr);
    } else if (num == 3) {
        func_001061A8(fid, (void *)1);
    } else if (num == 5) {
        func_001061A8(fid, (void *)2);
    } else {
        SEFile[num] = FileReadSelect(fid, chantbl[num]);
    }
}


void VoiceLoad(int chan) {
    pack_allfree();
    FileidVoice[chan] = (UseCharacter1p2p[chan] - 1) * 4 + 1 + g_info.PlayerVoice[chan];
    FileidSE[chan] = (UseCharacter1p2p[chan] - 1) * 4 + 4;
    if (chan == 0) {
        SEFileRead(FileidVoice[0] + 0x6B, 2);
        SEFileRead(FileidSE[0] + 0x6B, 3);
    } else {
        SEFileRead(FileidVoice[1] + 0x6B, 4);
        SEFileRead(FileidSE[1] + 0x6B, 5);
    }
}




void sound_load_Common(void) {
    if (common_loaded == 0) {
        SEFileRead(0x27, 0);
        SEFileRead(0x29, 1);
        common_loaded = 1;
    }
}





void BGM_Request(int no, int loop) {
    int top;

    debugf("BGM Request %d\n", no);
    if (IsDemoBattle() && IsAdvertiseOff()) {
        return;
    }
    if (current_bgm == no && SnDrvBgmCheck()) {
        debugf("Request Canceled2\n");
        return;
    }
    sngctl.flag = 0;
    userinfo.vol_bgm = userinfo.vol_bgm_mst;
    set_bgmvol();
    debugf(D_5CC068, userinfo.vol_bgm, userinfo.vol_bgm_mst);
    top = GetFileTop(no - 1);
    SnDrvBgmLoadCD(top, GetFileSize(no - 1) / 2048);
    SnDrvBgmStart(userinfo.vol_bgm << 10, loop);
    current_bgm = no;
}



void BGM_KeepRequest(int no, int loop) {
    int vol;

    debugf("BGM Keep Request %d\n", no);
    if (IsDemoBattle() && IsAdvertiseOff()) {
        return;
    }
    no--;
    vol = userinfo.vol_bgm << 10;
    current_bgm = -1;
    SnDrvBgmKeep(vol, loop, GetFileSize(no));
    debugf("Keep Size %x\n", GetFileSize(no));
}

int func_001946B0(int no, int type);







int func_001946B0(int no, int type)
{
    char *top;
    int size;

    sndrequested[channel] = type;
    if (type == 0) {
        no = soundtb_cmn[no];
    }
    if (type == 3) {
        SnDrvSeVReq(channel, 0, no);
    } else if (type == 5) {
        SnDrvSeVReq(channel, 1, no);
    } else {
        top = SEFile[type];
        size = ((int *)top)[no + 1] - ((int *)top)[no];
        top += ((int *)top)[no] / 4 * 4;
        if ((unsigned int)size <= 0x3FFFF) {
            SnDrvSeTrans(top, size, channel);
        }
    }
    channel++;
    if (channel >= 0x11) {
        channel = 1;
    }
    sndrequested[channel] = -1;
    SnDrvStop(channel);
    return 0;
}



int func_001947D8(int no)
{
    if (no != 0) {
        if (no < 0x3C) {
            func_001946B0(no, 0);
        } else if (no < 0x61) {
            func_001946B0(no - 0x40, 1);
        }
    }
    return 0;
}





void sound_setforce(void) {
}

void sound_alloff(void) {
}



int voice_request(CHARACTER_WORK *offset, int no) {
    int ret;

    if (offset->padid == 0) {
        ret = sound_request_main(no, 2);
    } else {
        ret = sound_request_main(no, 4);
    }
    return ret;
}




int privatese_request(CHARACTER_WORK *offset, int no) {
    int ret;

    if (offset->padid == 0) {
        ret = sound_request_main(no, 3);
    } else {
        ret = sound_request_main(no, 5);
    }
    return ret;
}



int SongIn(void) {
    int ret;

    if (++sngctl.val >= sngctl.intime) {
        sngctl.val = 0;
        userinfo.vol_bgm++;
    }
    ret = userinfo.vol_bgm == userinfo.vol_bgm_mst;
    SnDrvBgmVol(userinfo.vol_bgm << 10);
    return ret;
}



int SongOut(void) {
    int ret;

    ret = 0;
    if (++sngctl.val >= sngctl.outtime) {
        sngctl.val = 0;
        userinfo.vol_bgm--;
        ret = userinfo.vol_bgm <= sngctl.mval;
    }
    SnDrvBgmVol(userinfo.vol_bgm << 10);
    return ret;
}



void set_bgmvol(void) {
    userinfo.vol_bgm = userinfo.vol_bgm_mst;
    SnDrvBgmVol(userinfo.vol_bgm << 10);
}





int SongContExe(void)
{
    SONG_CONTL *p;

    p = &sngctl;
    switch (p->flag) {
    case 0:
        break;
    case 1:
        if (SongOut()) {
            p->flag = 3;
        }
        break;
    case 2:
        if (SongIn()) {
            p->flag = 0;
        }
        break;
    case 3:
        if (--p->mutetime == 0xFFFF) {
            p->flag = 2;
        }
        break;
    case 4:
        if (SongOut()) {
            p->flag = 6;
        }
        break;
    case 5:
        if (SongIn()) {
            p->flag = 6;
        }
        break;
    case 6:
        break;
    }
    return p->flag;
}



int ReqSongMute(int outtime, int intime, int mutetime, int mval) {
    int ret;

    ret = sngctl.flag;
    if (userinfo.vol_bgm <= mval) {
        return 0;
    }
    sngctl.flag = 1;
    sngctl.outtime = outtime;
    sngctl.intime = intime;
    sngctl.mutetime = mutetime;
    sngctl.mval = mval;
    sngctl.val = 0;
    return ret;
}



void InitSongCtrl(void) {
}

void func_001942A0(int fid, int num);


void stest_packload(int no, int no2)
{
    if (no2 == -1) {
        if (no < 17) {
            FileidSE[0] = (no - 1) * 4 + 4;
            func_001942A0((no - 1) * 4 + 0x6F, 3);
        }
    } else {
        FileidVoice[0] = (no - 1) * 4 + 1 + no2;
        func_001942A0((no - 1) * 4 + 0x6C + no2, 2);
    }
    set_sevol();
}




void kakusi_packload(void) {
    if (userinfo.phase < 2) {
        sc.faust = 0;
        SEFileRead(0x94, 2);
        SEFileRead(0xa8, 4);
    } else {
        sc.faust = 1;
        SEFileRead(0x90, 2);
    }
}


void set_sevol(void) {
}

void pack_allfree(void) {
}


int FTO_Exe(int cmd, int no) {
    return 0;
}

int GetSoundVolume(int flag) {
    return 0;
}

int IsAdvertiseOff(void) {
    return 0;
}

int SetSoundVolume(int vol, int flag) {
    return 0;
}

void SnDrvReqClear(void);

void sound_rec_clear(void) {
    SnDrvReqClear();
}

void VoiceLoad(int chan);

void ForeStellVoiceLoad(void) {
    VoiceLoad(0);
    VoiceLoad(1);
}

extern SONG_CONTL sngctl;
extern USER_SETTING userinfo;

extern USER_SETTING userinfo;
void SnDrvBgmVol(int vol);

extern USER_SETTING userinfo;
int SnDrvGetBgmMode(void);

extern GAME_INFOMATION g_info;
extern SYS_COND sc;
extern short int UseCharacter1p2p[];
int IsDemoBattle(void);
int get_replay_sound(void);
void set_replay_sound(int no);


void SnDrvBgmStop(void);



void init_davol(void) {
    sngctl.flag = 0;
    userinfo.vol_bgm = userinfo.vol_bgm_mst;
    set_bgmvol();
}


void BGM_Request(int no, int loop);


void func_00194D80(void)
{
    int bgmno[16] = { 1, 0x10, 2, 0x12, 4, 5, 0x11, 3, 0x13, 6, 0xB, 0xA, 8, 7, 9, 0xC };
    int no;

    IsDemoBattle();
    if ((UseCharacter1p2p[0] == 1 && UseCharacter1p2p[1] == 2) || (UseCharacter1p2p[1] == 1 && UseCharacter1p2p[0] == 2)) {
        no = 0xD;
    } else if ((UseCharacter1p2p[0] == 8 && UseCharacter1p2p[1] == 4) || (UseCharacter1p2p[1] == 8 && UseCharacter1p2p[0] == 4)) {
        no = 0xF;
    } else if (UseCharacter1p2p[0] == UseCharacter1p2p[1]) {
        no = 0xE;
    } else {
        no = bgmno[UseCharacter1p2p[g_info.Player ? 0 : 1] - 1];
    }
    if (g_info.mode & 0x4000) {
        if (sc.replay == 1) {
            set_replay_sound(no);
        } else if (sc.replay == 2) {
            no = get_replay_sound();
        }
    }
    BGM_Request(no, 1);
}



void BGM_Free(void) {
    debugf("BGM Free\n");
    SnDrvBgmStop();
    current_bgm = -1;
}

extern USER_SETTING userinfo;
void SnDrvBgmMode(int mode);

extern int common_loaded;
void SEFileRead(int fid, int num);

void dfree(void *mem);

void VoiceFree(void) {
    int i;

    for (i = 2; i < 6; i++) {
        if (SEFile[i]) {
            dfree(SEFile[i]);
            SEFile[i] = 0;
        }
    }
}

extern USER_SETTING userinfo;
extern SYS_COND sc;
void SEFileRead(int fid, int num);

void SnDrvStop(int chan);

void voice_off(CHARACTER_WORK *offset) {
    int i;
    int type;

    type = offset->padid ? 4 : 2;
    for (i = 0; i < 24; i++) {
        if (sndrequested[i] == type) {
            SnDrvStop(i);
            sndrequested[i] = -1;
        }
    }
}

extern SONG_CONTL sngctl;

int ReqSongIn(int intime, int mval) {
    sngctl.flag = 5;
    sngctl.intime = intime;
    sngctl.outtime = 0;
    sngctl.val = 0;
    return 0;
}

extern SONG_CONTL sngctl;

int ReqSongOut(int outtime, int mval) {
    sngctl.flag = 4;
    sngctl.outtime = outtime;
    sngctl.mval = mval;
    sngctl.intime = 0;
    sngctl.val = 0;
    return 0;
}

int sound_request_main(int no, int type);

int sound_request_main(int no, int type);

int sound_request_main(int no, int type);

extern SONG_CONTL sngctl;
extern USER_SETTING userinfo;

void SnDrvInit(void);

void sound_Init(void) {
    int i;
    int v;

    channel = 1;
    v = -1;
    for (i = 23; i >= 0; i--) {
        sndrequested[i] = v;
    }
    SnDrvInit();
}

void SnDrvRequestClear(void);

void sound_Request_Init(void) {
    int i;
    int v;

    channel = 1;
    v = -1;
    for (i = 23; i >= 0; i--) {
        sndrequested[i] = v;
    }
    SnDrvRequestClear();
    BGM_Free();
}

extern SONG_CONTL sngctl;
extern USER_SETTING userinfo;
void SnDrvBgmVol(int vol);

extern SONG_CONTL sngctl;
extern USER_SETTING userinfo;
void SnDrvBgmVol(int vol);



void set_soundmode(int mode) {
    SnDrvBgmMode(mode);
    userinfo.sound_mode = mode;
}




void get_soundmode(void) {
    userinfo.sound_mode = SnDrvGetBgmMode();
}