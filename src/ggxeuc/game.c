/* Guilty Gear X US (SLUS-202.87): 5 functions ported from the X Plus
   decomp (ggxeuc/game.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/game.h"
void LinkAddrTb(u_int *addr);
void ObjLinkSub(u_int *p, int pid, int cnt);
void ObjFileLink(int addr, int use, int pid);
void ForeStellPlayer(void);
void GameInit(void);
void GameLoad_Skill(void);
void GameLoad(void);
void set_noskill(void);
void Pause_Execute(void);
int pause_normal(int pos);
int pause_gallery(int pos);
int pause_training(int pos);
int pause_training_common(HEADWORK *hw, PARTS *pt, int pos);
void pause_training_common_init(HEADWORK *hw, int flg);
int pause_training_enemy(HEADWORK *hw, PARTS *pt, int pos);
int chk_selectok(int no);
void pause_training_enemy_init(HEADWORK *hw, int flg);
int pause_training_practice(HEADWORK *hw, PARTS *pt, int pos);
void pause_training_practice_init(HEADWORK *hw, int flg);
void disp_enemylevel(HEADWORK *hw);
void disp_cmnguard(HEADWORK *hw);
void GameKill(void);
void ExitDemo_Main(void);
void SwDataConv(void);
void GameMain(void);
void g_main_End(void);
void GameEnd(void);
extern char *Player1File;
extern char *Player2File;
extern char *Player1Skill;
extern char *Player2Skill;
extern int GlobalHitStop;
extern int ExitDemoMode;
extern int BattleStatus;
extern int AfterBattleCounter;
static const short idtb[] = { 1, 2, 3 };
static int an_id[] = { 0xd, 0x4c, 0x4e, 0x53, -1 };
static int ax_id[] = { 5, 0x4d, 0x51, -1 };
static int bk_id[] = { 9, 0x39, 0x3a, -1 };
static int ch_id[] = { 7, 0x45, 0x3b, 0x28, 0x3c, -1 };
static int dz_id[] = { 0x10, 0x24, 0x20, 0x43, -1 };
static int fa_id[] = { 0xa, 0x2b, 0x40, 0x4a, -1 };
static int jm_id[] = { 0xc, 0x2a, 0x26, 0x25, 0x2f, 0x30, 0x27, -1 };
static int jy_id[] = { 0xe, 0x47, 0x3f, 0x55, -1 };
static int ky_id[] = { 2, 0x29, 0x56, -1 };
static int ml_id[] = { 4, 0x34, 0x21, 0x2e, -1 };
static int my_id[] = { 3, 0x3e, 0x48, 0x37, 0x44, 0x46, 0x4b, -1 };
static int po_id[] = { 6, 0x49, 0x3d, -1 };
static int sl_id[] = { 1, 0x2c, 0x42, -1 };
static int ts_id[] = { 0xb, 0x50, 0x4f, 0x54, 0x52, -1 };
static int ve_id[] = { 0xf, 0x38, 0x2d, 0x22, 0x31, 0x35, 0x32, 0x36, -1 };
static int zt_id[] = { 8, 0x41, 0x23, 0x33, -1 };
static int kr_id[] = { 0x11, 0x57, -1 };
static int js_id[] = { 0x12, 0x59, 0x60, -1 };
static int *eff_id[] = { sl_id, ky_id, my_id, ml_id, ax_id, po_id, ch_id, zt_id, bk_id, fa_id, ts_id, jm_id, an_id, jy_id, ve_id, dz_id, kr_id, js_id };
static int effcmn[] = { 0x69, 0x64, 0x68, 0x6a, 0x6b, 0x66, 0x6d, 0x6f, 0x6c, 0x67, 0x61, 0x6e, 0x62, 0x7e, 0x63, 0x65, 0x7f, -1 };
void LoadFrameBufferObject(short int idno);
int SetCellTopAndGclTop(short int side, u_short idno, u_int *cell, u_int *patt);
int SetInstTb(short int side, u_short idno, u_int *inst);
int SetPalTop(short int side, u_short idno, u_int *pal);
void set_PlayDat(int idno, short int *p);
extern unsigned int DebugFlag;
extern unsigned int Pause;
extern GAME_INFOMATION g_info;
extern short int UseCharacter1p2p[];
extern USER_SETTING userinfo;
extern TRAINING_SETTING *tr_info;
extern SURVIVAL_INFOMATION *sv_info;
extern SYS_COND sc;
extern u_int TotalCounter;
extern char *EffectCmnFile;
int IST_Load(int *texadr, int sprid, int palid);
void JobNext(void);
void FontDraw(char *str, int x, int y, float z, int alfa);
extern PAD pads[2];
extern int BattleStatus;
extern CHARANGE_CTRL *gl_info;
void FontDrawDC(char *str, float x, float y, float z, int alpha, int id);
void InstDisp(int sw);
void JobChange(int job);
void Opt_KeyConfig(int sw);
int iodat_chk(int stat);
void nlSetBackColor(int c1, int c2, int c3);
int sound_request(int no);
void debugf(char *format, ...);
int chk_selectok(int no);
int pause_training_common(HEADWORK *hw, PARTS *pt, int pos);
int pause_training_enemy(HEADWORK *hw, PARTS *pt, int pos);
int pause_training_practice(HEADWORK *hw, PARTS *pt, int pos);
void pause_training_common_init(HEADWORK *hw, int flg);
void pause_training_enemy_init(HEADWORK *hw, int flg);
void pause_training_practice_init(HEADWORK *hw, int flg);
void disp_cmnguard(HEADWORK *hw);
void disp_enemylevel(HEADWORK *hw);
extern CHARACTER_WORK *PlayerWork;
void polydraw_FBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);

void LinkAddrTb(u_int *addr)
{
    u_int offset;

    offset = (u_int)addr;
    if (*addr < offset) {
        if (*addr == 0xFFFFFFFF) {
            *addr = 0;
            return;
        }
        while (*addr != 0xFFFFFFFF) {
            *addr += offset;
            addr++;
        }
        *addr = 0;
    }
}




void ObjLinkSub(u_int *p, int pid, int cnt)
{
    u_int *pal;
    u_int *inst;

    LinkAddrTb(p);
    if (p[0]) {
        pal = (u_int *)p[3];
        inst = (u_int *)p[2];
        SetCellTopAndGclTop(pid, cnt, (u_int *)p[1], (u_int *)p[0]);
        if (pal) SetPalTop(pid, cnt, pal);
        SetInstTb(pid, cnt, inst);
        if (pid != -2) {
            if (pid == -1) {
                set_PlayDat(0, (short *)inst);
                set_PlayDat(1, (short *)inst);
            } else {
                set_PlayDat(pid, (short *)inst);
            }
        }
        switch (cnt) {
        case 0x21:
        case 0x22:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
        case 0x29:
        case 0x2a:
        case 0x2b:
        case 0x2c:
        case 0x2d:
        case 0x2e:
        case 0x2f:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x38:
        case 0x39:
        case 0x3a:
        case 0x3b:
        case 0x3c:
        case 0x3d:
        case 0x3e:
        case 0x3f:
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x4c:
        case 0x4d:
        case 0x4e:
        case 0x4f:
        case 0x50:
        case 0x51:
        case 0x53:
        case 0x55:
        case 0x56:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6a:
        case 0x6b:
        case 0x6c:
        case 0x6d:
        case 0x6e:
        case 0x6f:
        case 0x7e:
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
            LoadFrameBufferObject(cnt);
            break;
        }
    }
}


void ObjFileLink(int addr, int use, int pid)
{
    u_int *p;
    u_int *IDTb;
    u_int *idlist;

    IDTb = (u_int *)addr;
    if (use >= 0) {
        idlist = (u_int *)eff_id[use];
    } else {
        idlist = (u_int *)effcmn;
    }
    LinkAddrTb((u_int *)addr);
    p = (u_int *)*(u_int *)addr;
    addr = (int)idlist;
    while (p) {
        ObjLinkSub(p, pid, *(u_int *)addr);
        IDTb++;
        addr += 4;
        p = (u_int *)*IDTb;
    }
}

char *FileReadSelect(int fid, int chan);
void Read_Playdat(int size);
void debugf(char *format, ...);
extern short int UseCharacter1p2p[];
extern USER_SETTING userinfo;

void AllBgWork_Clear(void);
int AllCharacterWorkInitial(void);
void CockpitInit(void);
void CockpitLoadInit(void);
void FadeInit(void);
void ForeStellBG(void);
void ForeStellVoiceLoad(void);
void InstTbInit(void);
void JobNext(void);
void PadInitActuator(int pad);
void Screen_Init(void);
void dcfont_init(void);
void heap_clear(void);
void set_random_seed(void);

int PXF_GetSysSprite(void);
extern DCFONT_INFO fi;

void BG_Init(void);
void ChaseInit(void);
void EnterFight_Init(void);
void FadeInit(void);
int FileReadCheck(void);
void G_GraphicInit(void);
void InitExCollisionWork(void);
void KakuseiFinishInit(void);
void PlayerInit(void);
void Poly_Effect_Initial(void);
void ResultInitial(void);
void Rq_Fadein(void);
void dcfont_init(void);
void init_davol(void);
void set_activebtn(int padid);
void set_sevol(void);
void sv_save_enemystatus(int side);
void sv_set_enemy_status(int side);

int Cr_IsPlaying(int side);
void polydraw_FBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
int sound_request(int no);
extern story_t story;

void FontDrawDC(char *str, float x, float y, float z, int alpha, int id);
void InstDisp(int sw);
void JobChange(int job);
void Opt_KeyConfig(int sw);
int iodat_chk(int stat);
void nlSetBackColor(int c1, int c2, int c3);
int sound_request(int no);

void Tranning_init_prac(void);

void set_random_seed_abs(void);


void func_00196B58(int size);


void func_0011B208(void)
{
    int f1id;
    int f2id;

    func_00196B58(0);
    f1id = (UseCharacter1p2p[0] - 1) * 2 + 0x43;
    f2id = (UseCharacter1p2p[1] - 1) * 2 + 0x43;
    if (userinfo.gguse[0]) f1id--;
    if (userinfo.gguse[1]) f2id--;
    debugf("Player Load %d %d\n", UseCharacter1p2p[0], UseCharacter1p2p[1]);
    if (f2id < f1id) {
        Player2File = FileReadSelect(f2id, 1);
        Player1File = FileReadSelect(f1id, 0);
    } else {
        Player1File = FileReadSelect(f1id, 0);
        Player2File = FileReadSelect(f2id, 1);
    }
}




void GameInit(void)
{
    heap_clear();
    PadInitActuator(0);
    PadInitActuator(1);
    DebugFlag = 0;
    Pause = 0;
    GlobalHitStop = 0;
    if (g_info.mode & 0x4000) set_random_seed_abs();
    ForeStellPlayer();
    ForeStellBG();
    AllCharacterWorkInitial();
    AllBgWork_Clear();
    Screen_Init();
    ExitDemoMode = 0;
    ForeStellVoiceLoad();
    InstTbInit();
    CockpitLoadInit();
    CockpitInit();
    dcfont_init();
    FadeInit();
    JobNext();
}



void GameLoad(void)
{
    if (!FileReadCheck()) {
        if (TotalCounter & 0x20) {
            FontDraw("NOW LOADING", 0xFE, 0xF0, 200.0f, 0xFF);
        }
    } else {
        ObjFileLink((int)Player1File, UseCharacter1p2p[0] - 1, 0);
        ObjFileLink((int)Player2File, UseCharacter1p2p[1] - 1, 1);
        ObjFileLink((int)EffectCmnFile, -1, -2);
        BG_Init();
        PlayerInit();
        set_activebtn(0);
        set_activebtn(1);
        Poly_Effect_Initial();
        FadeInit();
        ChaseInit();
        ResultInitial();
        InitExCollisionWork();
        G_GraphicInit();
        KakuseiFinishInit();
        dcfont_init();
        set_sevol();
        init_davol();
        Rq_Fadein();
        BattleStatus = 1;
        EnterFight_Init();
        sc.rannyu = 0;
        sc.pause_clear = 0;
        if (g_info.mode & 0x200) {
            sv_save_enemystatus(sv_info->ply == 0);
            sv_set_enemy_status(sv_info->ply == 0);
        }
        JobNext();
    }
}


void set_noskill(void)
{
    pads[0].trg = 0;
    pads[1].trg = 0;
    if (g_info.mode & 0x100) {
        tr_info->iobkup[0] = 0x50;
        tr_info->iobkup[2] = 0x50;
        tr_info->iobkup[1] = 0x50;
    }
}


int func_0011BAA8(int pos);


void func_0011B530(void)
{
    static int pos;
    static int alpha = 0xFF;

    if (g_info.Event) return;
    if (g_info.mode & 0x4000) return;
    if (Pause == 0 && sc.rannyu == 0 && BattleStatus == 2
        && (((pads[0].trg & 0x800) && Cr_IsPlaying(0)) || ((pads[1].trg & 0x800) && Cr_IsPlaying(1)))) {
        sound_request(0x39);
        Pause = 1;
        pos = 0;
        pads[0].trg = 0;
        pads[1].trg = 0;
        sc.pause_clear = 0;
    } else if (sc.pause_clear & 0xEF) {
        Pause = 1;
    }
    if (Pause) {
        if (sc.pause_clear) return;
        if (alpha < 0xD0) alpha += 0x20;
        if (alpha > 0xD0) alpha = 0xD0;
        polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 3.0f, alpha << 24);
        if (g_info.mode & 0x100) {
            pos = func_0011BAA8(pos);
        } else {
            pos = pause_normal(pos);
        }
        if (Pause == 0) {
            sc.pause_clear = 0;
            set_noskill();
        }
    } else {
        alpha = 0;
        DebugFlag &= ~8;
        DebugFlag &= ~0x40;
        DebugFlag &= ~0x100;
    }
}

void func_0015DF98(int sw);
void func_0016CF90(int sw);


int pause_normal(int pos)
{
    HEADWORK hw[4] = {
        { "RETURN", 256.0f, 208.0f, 0, 0, 0 },
        { "KEY CONFIG", 256.0f, 240.0f, 0, 0, 0 },
        { "COMMAND LIST", 256.0f, 272.0f, 0, 0, 0 },
        { "TITLE", 256.0f, 304.0f, 0, 0, 0 },
    };
    int i;
    int alpha;
    int pad;

    if (sc.grpvar[0] == 0) {
        if ((pad = iodat_chk(0x1000)) != 0) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                pos = pos ? pos - 1 : 3;
            }
            sc.cnt[pad - 1]++;
        } else if ((pad = iodat_chk(0x4000)) != 0) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                pos = (pos != 3) ? pos + 1 : 0;
            }
            sc.cnt[pad - 1]++;
        } else {
            sc.cnt[0] = 0;
            sc.cnt[1] = 0;
        }
        pad = pads[0].trg | pads[1].trg;
        if (pad & 0x810) {
            sound_request(0x3B);
            Pause = 0;
        } else if (pad & 0x40) {
            sound_request(0x39);
            switch (pos) {
            case 0:
                Pause = 0;
                break;
            case 1:
                func_0015DF98(0);
                sc.grpvar[0] = pos;
                break;
            case 2:
                if ((pads[0].trg | pads[1].trg) & 0x40) {
                    if (pads[0].trg & 0x40) {
                        sc.grpvar[0] = 1;
                    } else {
                        sc.grpvar[0] = pos;
                    }
                    func_0016CF90(0);
                }
                break;
            case 3:
                if (userinfo.autosave && !(g_info.mode & 0x200)) {
                    sc.grpvar[1] = -1;
                    JobChange(0x2A);
                } else {
                    JobChange(3);
                }
                break;
            }
        }
        for (i = 0; i < 4; i++) {
            alpha = (i == pos) ? 1 : 0xA0;
            FontDrawDC(hw[i].head, hw[i].xp, hw[i].yp, 2.0f, alpha, 1);
        }
    } else if (pos == 1) {
        func_0015DF98(1);
    } else if (pos == 2) {
        func_0016CF90(1);
    }
    return pos;
}



int func_0011BAA8(int pos)
{
    int (*pause_training_sub[2])() = { pause_training_common, pause_training_enemy };
    int i;
    int pad;
    int modeid[2][2] = { { 0, 8 }, { 0, 8 } };
    PARTS mp[2] = {
        { "CONFIG", 32.0f, 104.0f, 0 },
        { "ENEMY SETTING", 216.0f, 104.0f, 0 },
    };
    static HEADWORK hw[2][9] = {
        {
            { "RETURN", 64.0f, 144.0f, 0, 0, 0 },
            { "TENSION MAX", 64.0f, 176.0f, 0, 4, 0 },
            { "GUARD LEVEL", 64.0f, 208.0f, 0, -1, 0 },
            { "DAMAGE DISP", 64.0f, 240.0f, 0, 2, 0 },
            { "CHARACTER SELECT", 64.0f, 272.0f, 0, 0, 0 },
            { "KEY CONFIG", 64.0f, 304.0f, 0, 0, 0 },
            { "COMMAND LIST", 64.0f, 336.0f, 0, 0, 0 },
            { "DEFAULT", 64.0f, 368.0f, 0, 0, 0 },
            { "TITLE", 64.0f, 400.0f, 0, 0, 0 },
        },
        {
            { "RETURN", 64.0f, 144.0f, 0, 0, 0 },
            { "MODE", 64.0f, 176.0f, 0, 2, 0 },
            { "ENEMY STATE", 64.0f, 208.0f, 0, 3, 0 },
            { "GUARD", 64.0f, 240.0f, 0, 2, 0 },
            { "RECOVERY", 64.0f, 272.0f, 0, 5, 0 },
            { "ATTACK MEMORY", 64.0f, 304.0f, 0, 2, 0 },
            { "ENEMY LEVEL", 64.0f, 336.0f, 10, 33, 0 },
            { "LIFE REGAIN", 64.0f, 368.0f, 1, 2, 0 },
            { "DEFAULT", 64.0f, 400.0f, 0, 0, 0 },
        },
    };
    PARTS pt[18] = {
        { "STAND", 272.0f, 10.0f, 0 },
        { "CROUCH", 352.0f, 10.0f, 0 },
        { "JUMP", 444.0f, 10.0f, 0 },
        { "OFF", 272.0f, 10.0f, 0 },
        { "ON", 328.0f, 10.0f, 0 },
        { "NORMAL", 272.0f, 10.0f, 0 },
        { "MAX", 376.0f, 10.0f, 0 },
        { "OFF", 272.0f, 10.0f, 0 },
        { "FT", 336.0f, 10.0f, 0 },
        { "NT", 384.0f, 10.0f, 0 },
        { "BK", 432.0f, 10.0f, 0 },
        { "RND", 480.0f, 10.0f, 0 },
        { "FREE", 272.0f, 10.0f, 0 },
        { "VSCPU", 344.0f, 10.0f, 0 },
        { "OFF", 272.0f, 10.0f, 0 },
        { "PLAYER", 326.0f, 10.0f, 0 },
        { "ENEMY", 412.0f, 10.0f, 0 },
        { "BOTH", 484.0f, 10.0f, 0 },
    };
    int alpha;

    if (tr_info->clear == 0) {
        pause_training_common_init(hw[0], 1);
        pause_training_enemy_init(hw[1], 1);
        tr_info->clear = 1;
    }
    if (sc.grpvar[0] == 0) {
        FontDrawDC("TRAINING MENU", 32.0f, 40.0f, 2.0f, 1, 0);
        if (Pause == 1) {
            Pause = 2;
            hw[1][5].pos = 0;
        }
        pos = pause_training_sub[tr_info->cnfside](hw[tr_info->cnfside], pt, pos);
        for (i = 0; i < 2; i++) {
            alpha = (tr_info->cnfside == i) ? 1 : 0xA0;
            FontDrawDC(mp[i].head, mp[i].xp, mp[i].yp, 2.0f, alpha, 1);
        }
        pad = pads[0].trg | pads[1].trg;
        if ((pad & 0xC) || (pos == 0 && (pad & 0xA000))) {
            sound_request(0x37);
            sc.grpvar[0] = 0;
            if (tr_info->cnfside) {
                tr_info->cnfside = 0;
            } else {
                tr_info->cnfside = 1;
            }
            pos = 0;
        }
        if ((pad = iodat_chk(0x1000)) != 0) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                do {
                    pos = (pos == 0) ? modeid[tr_info->cnfside][1] : pos - 1;
                } while (chk_selectok(pos));
            }
            sc.cnt[pad - 1]++;
        } else if ((pad = iodat_chk(0x4000)) != 0) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                do {
                    pos = (pos == modeid[tr_info->cnfside][1]) ? modeid[tr_info->cnfside][0] : pos + 1;
                } while (chk_selectok(pos));
            }
            sc.cnt[pad - 1]++;
        } else if ((pad = iodat_chk(0x8000)) != 0) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                if (hw[tr_info->cnfside][pos].maxhead > 0) {
                    int ret;

                    if (tr_info->cnfside == 1 && pos == 6) {
                        ret = 0;
                        sound_request(6);
                    } else {
                        ret = 1;
                        sound_request(0x37);
                    }
                    hw[tr_info->cnfside][pos].pos--;
                    if (hw[tr_info->cnfside][pos].pos < 0) {
                        if (ret == 0) {
                            hw[tr_info->cnfside][pos].pos = 0;
                        } else {
                            hw[tr_info->cnfside][pos].pos = hw[tr_info->cnfside][pos].maxhead - 1;
                        }
                    }
                } else if (hw[tr_info->cnfside][pos].maxhead == -1) {
                    int num;

                    num = ((pads[0].data | pads[1].data) & 0x40) ? 10 : 1;
                    sound_request(6);
                    hw[tr_info->cnfside][pos].pos -= num;
                    if (hw[tr_info->cnfside][pos].pos < -128) hw[tr_info->cnfside][pos].pos = -128;
                }
            }
            sc.cnt[pad - 1]++;
        } else if ((pad = iodat_chk(0x2000)) != 0) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                if (hw[tr_info->cnfside][pos].maxhead > 0) {
                    int ret;

                    if (tr_info->cnfside == 1 && pos == 6) {
                        sound_request(6);
                        ret = 0;
                    } else {
                        sound_request(0x37);
                        ret = 1;
                    }
                    hw[tr_info->cnfside][pos].pos++;
                    if (hw[tr_info->cnfside][pos].pos >= hw[tr_info->cnfside][pos].maxhead) {
                        if (ret == 0) {
                            hw[tr_info->cnfside][pos].pos = hw[tr_info->cnfside][pos].maxhead - 1;
                        } else {
                            hw[tr_info->cnfside][pos].pos = 0;
                        }
                    }
                } else if (hw[tr_info->cnfside][pos].maxhead == -1) {
                    int num;

                    num = ((pads[0].data | pads[1].data) & 0x40) ? 10 : 1;
                    sound_request(6);
                    hw[tr_info->cnfside][pos].pos += num;
                    if (hw[tr_info->cnfside][pos].pos > 128) hw[tr_info->cnfside][pos].pos = 128;
                }
            }
            sc.cnt[pad - 1]++;
        } else {
            sc.cnt[0] = 0;
            sc.cnt[1] = 0;
        }
        if ((pads[0].trg | pads[1].trg) & 0x810) {
            sound_request(0x3B);
            Pause = 0;
            sc.grpvar[0] = 0;
        }
    } else if (pos == 5) {
        func_0015DF98(1);
    } else if (pos == 6) {
        func_0016CF90(1);
    }
    return pos;
}




int pause_training_common(HEADWORK *hw, PARTS *pt, int pos)
{
    int i;
    int alpha;
    int j;

    hw[1].p = pt + 14;
    hw[3].p = pt + 3;
    hw[0].p = 0;
    hw[2].p = 0;
    hw[4].p = 0;
    hw[5].p = 0;
    hw[6].p = 0;
    hw[7].p = 0;
    hw[8].p = 0;
    for (i = 0; i < 9; i++) {
        alpha = (i == pos) ? 1 : 0xA0;
        FontDrawDC(hw[i].head, hw[i].xp, hw[i].yp, 2.0f, alpha, 1);
        if (hw[i].p) {
            for (j = 0; j < hw[i].maxhead; j++) {
                alpha = (hw[i].pos == j) ? 0xFF : 0x50;
                FontDraw(hw[i].p[j].head, hw[i].xp + hw[i].p[j].xp, hw[i].yp + hw[i].p[j].yp, 2.0f, alpha);
            }
        }
    }
    switch (pos) {
    case 0:
        if ((pads[0].trg | pads[1].trg) & 0x40) {
            sound_request(0x39);
            Pause = 0;
        }
        break;
    case 1:
        tr_info->tcom.tension = hw[pos].pos;
        if (tr_info->tcom.tension == 0) {
            PlayerWork[0].ply->Tension = 0;
            PlayerWork[1].ply->Tension = 0;
        }
        break;
    case 2:
        tr_info->tcom.ggage = hw[pos].pos;
        break;
    case 3:
        tr_info->tcom.damage_disp = hw[pos].pos;
        break;
    case 4:
        if ((pads[0].trg | pads[1].trg) & 0x40) {
            sound_request(0x39);
            tr_info->stat = 1;
            Pause = 0;
        }
        break;
    case 5:
        if ((pads[0].trg | pads[1].trg) & 0x40) {
            sound_request(0x39);
            sc.grpvar[0] = 1;
            Opt_KeyConfig(0);
        }
        break;
    case 6:
        if ((pads[0].trg | pads[1].trg) & 0x40) {
            sound_request(0x39);
            sc.grpvar[0] = 1;
            InstDisp(0);
        }
        break;
    case 7:
        if ((pads[0].trg | pads[1].trg) & 0x40) {
            sound_request(0x39);
            pause_training_common_init(hw, 0);
        }
        break;
    case 8:
        if ((pads[0].trg | pads[1].trg) & 0x40) {
            sound_request(0x39);
            tr_info->stat = 3;
            Pause = 0;
        }
        break;
    }
    disp_cmnguard(hw + 2);
    return pos;
}






void pause_training_common_init(HEADWORK *hw, int flg)
{
    if (flg == 0) {
        hw[0].pos = 0;
        hw[1].pos = 0;
        hw[2].pos = 0;
        hw[3].pos = 0;
        hw[4].pos = 0;
        hw[5].pos = 0;
        hw[8].pos = 0;
        tr_info->tcom.tension = hw[1].pos;
        tr_info->tcom.ggage = hw[2].pos;
        tr_info->tcom.damage_disp = hw[3].pos;
    } else {
        hw[2].pos = tr_info->tcom.ggage;
        hw[1].pos = tr_info->tcom.tension;
        hw[3].pos = tr_info->tcom.damage_disp;
    }
}

/* chk_selectok's local flg[3][9] initializer sits in .rodata after data of functions not in this
   file yet: copy it from the original's bytes */
typedef struct {
    int flg[3][9];
} chk_selectok_flgtb;
extern chk_selectok_flgtb D_5BB158;

unsigned int gStripGetFadeColor(unsigned int color);
void polydraw_FLine(float x1, float y1, float x2, float y2, float z, unsigned int color1);
void polydraw_LBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
void set_char2num(int num, char *block, int id);



int pause_training_enemy(HEADWORK *hw, PARTS *pt, int pos)
{
    int i;
    int alpha;
    int j;

    hw[1].p = pt + 12;
    hw[0].p = 0;
    hw[6].p = 0;
    hw[2].p = pt;
    hw[3].p = pt + 3;
    hw[4].p = pt + 7;
    hw[7].p = pt + 3;
    hw[5].p = pt + 3;
    hw[8].p = 0;
    for (i = 0; i < 9; i++) {
        alpha = (i == pos) ? 1 : 0xA0;
        alpha = chk_selectok(i) ? 0xF0 : alpha;
        FontDrawDC(hw[i].head, hw[i].xp, hw[i].yp, 2.0f, alpha, 1);
        if (hw[i].p) {
            for (j = 0; j < hw[i].maxhead; j++) {
                alpha = (hw[i].pos == j) ? 0xFF : 0x50;
                alpha = chk_selectok(i) ? 0x10 : alpha;
                FontDraw(hw[i].p[j].head, hw[i].xp + hw[i].p[j].xp, hw[i].yp + hw[i].p[j].yp, 2.0f, alpha);
            }
        }
    }
    switch (pos) {
    case 0:
        if ((pads[0].trg | pads[1].trg) & 0x40) {
            sound_request(0x39);
            Pause = 0;
        }
        break;
    case 1:
        tr_info->tene.mode = hw[pos].pos;
        break;
    case 6:
        tr_info->tene.level = hw[pos].pos;
        break;
    case 2:
        tr_info->tene.type = hw[pos].pos;
        break;
    case 3:
        tr_info->tene.guard = hw[pos].pos;
        break;
    case 4:
        tr_info->tene.ukemi = hw[pos].pos;
        break;
    case 7:
        tr_info->tene.rejene = hw[pos].pos;
        break;
    case 5:
        tr_info->tene.memflg = hw[pos].pos;
        tr_info->tene.resettim = 0;
        break;
    case 8:
        if ((pads[0].trg | pads[1].trg) & 0x40) {
            pause_training_enemy_init(hw, 0);
        }
        break;
    }
    disp_enemylevel(&hw[6]);
    return pos;
}


int chk_selectok(int no)
{
    int flg[2][9] = {
        { -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, 0, 0, 0, 0, 1, -1, -1 },
    };

    if (flg[tr_info->cnfside][no] == -1 || flg[tr_info->cnfside][no] == tr_info->tene.mode) return 0;
    return 1;
}


void pause_training_enemy_init(HEADWORK *hw, int flg)
{
    if (flg == 0) {
        hw[6].pos = 10;
        hw[7].pos = 1;
        hw[0].pos = 0;
        hw[2].pos = 0;
        hw[3].pos = 0;
        hw[4].pos = 0;
        hw[5].pos = 0;
        tr_info->tene.level = hw[6].pos;
        tr_info->tene.type = hw[2].pos;
        tr_info->tene.guard = hw[3].pos;
        tr_info->tene.ukemi = hw[4].pos;
        tr_info->tene.rejene = hw[7].pos;
        tr_info->tene.memflg = hw[5].pos;
        tr_info->tene.resettim = 0;
    } else {
        hw[1].pos = tr_info->tene.mode;
        hw[6].pos = tr_info->tene.level;
        hw[2].pos = tr_info->tene.type;
        hw[3].pos = tr_info->tene.guard;
        hw[4].pos = tr_info->tene.ukemi;
        hw[7].pos = tr_info->tene.rejene;
    }
}

void disp_enemylevel(HEADWORK *hw)
{
    int col;
    int alpha;
    char buf[8];

    alpha = (tr_info->tene.mode == 1) ? 0xFF : 0x10;
    set_char2num(hw->pos, buf, 0);
    FontDraw(buf, hw->xp + 272.0f + 256.0f + 8.0f, hw->yp + 10.0f, 2.0f, alpha);
    if (tr_info->tene.mode != 1) {
        polydraw_FBox(hw->xp + 272.0f, hw->yp + 10.0f, hw->xp + 272.0f + 256.0f, hw->yp + 24.0f, 1.0f, gStripGetFadeColor(0xC0000010));
    }
    polydraw_LBox(hw->xp + 272.0f, hw->yp + 10.0f, hw->xp + 272.0f + 256.0f, hw->yp + 24.0f, 1.0f, 0xFFFFFFFF);
    col = tr_info->tene.level * 8;
    if (col >= 0xFF) col = 0xFF;
    if (col <= 0x20) col = 0x20;
    col = (col << 16) + col / 4;
    col |= 0xFF000000;
    if (tr_info->tene.level) {
        polydraw_FBox(hw->xp + 272.0f, hw->yp + 10.0f, hw->xp + 272.0f + (float)(tr_info->tene.level * 8) - 1.0f, hw->yp + 24.0f, 2.0f, col);
        polydraw_FLine(hw->xp + 272.0f + (float)(tr_info->tene.level * 8) - 1.0f, hw->yp + 10.0f, hw->xp + 272.0f + (float)(tr_info->tene.level * 8) - 1.0f, hw->yp + 24.0f, 1.0f, 0xFFFF0000);
    }
}

void AllCharacterWorkRelease(void);
void BGM_Free(void);
void VoiceFree(void);
void dfree(void *mem);

void BGObjCreate(void);
void BG_Execute(void);
void ChaseEffectExe(void);
void ChaseFadeOutExe(void);
void CockpitExecute(void);
void CollisionExecute(void);
void CpObjExecute(void);
int DigestReloadStack(void);
void DrawDemoPlayandLogo(void);
void EffectExecute(void);
void EnterFight_Main(void);
void FlashExecute(void);
void GraphicExecuteRoot(void);
void HitStopTimeExeRoot(void);
int IsDemoBattle(void);
void JSTokorotenExecute(int flag);
void KakuseiBG_Draw(void);
void KakuseiBG_Execute(void);
int MeterColorChange(int col);
void ObjectExecute(void);
void PlayerExecute(void);
void PlayerInfoDisp(void);
void RefereeExe(void);
void ScreenExecute(void);
void TranningStat_chk(void);
void chk_boss_stat(void);
void chk_survival_stat(void);
void font_execute(int size);
void gallery_status_chk(void);
int get_player(void);
void ioexe_play(int side);
void replay_playback(void);
void replay_status_chk(void);
extern story_t story;
extern int JobMode;
extern int JSTokorotenFlag;
extern u_int TotalGameCounter;
extern int GameMain_SlowCount;

void BGM_Free(void);
void BG_End(void);
void ChaseEnd(void);
int FTO_Exe(int cmd, int no);
void KakuseiFinishEnd(void);
void VoiceFree(void);
void nlFreeCommonTextureAll(void);
void nlFreePaletteAll(void);
void sound_alloff(void);





void disp_cmnguard(HEADWORK *hw)
{
    int col;
    char buf[8];

    set_char2num(hw->pos, buf, 0);
    FontDraw(buf, hw->xp + 272.0f + 257.0f + 8.0f, hw->yp + 10.0f, 2.0f, 0xFF);
    col = (hw->pos < 0) ? 0xFF : 0x50;
    FontDraw("-", hw->xp + 272.0f + 48.0f, hw->yp + 10.0f, 2.0f, col);
    col = (hw->pos >= 0) ? 0xFF : 0x50;
    FontDraw("+", hw->xp + 272.0f + 257.0f - 56.0f, hw->yp + 10.0f, 2.0f, col);
    polydraw_LBox(hw->xp + 272.0f, hw->yp + 10.0f, hw->xp + 272.0f + 257.0f, hw->yp + 24.0f, 1.0f, 0xFFFFFFFF);
    polydraw_FLine(hw->xp + 272.0f + 128.0f, hw->yp + 10.0f, hw->xp + 272.0f + 128.0f, hw->yp + 24.0f, 1.0f, 0xFFFFFFFF);
    col = tr_info->tcom.ggage + 128;
    if (col >= 0xFF) col = 0xFF;
    if (col <= 0x20) col = 0x20;
    col = (col << 16) + col / 4;
    col |= 0xFF000000;
    polydraw_FBox(hw->xp + 272.0f, hw->yp + 10.0f, hw->xp + 272.0f + (float)(tr_info->tcom.ggage + 128), hw->yp + 24.0f, 3.0f, col);
    polydraw_FLine(hw->xp + 272.0f + (float)(tr_info->tcom.ggage + 128), hw->yp + 10.0f, hw->xp + 272.0f + (float)(tr_info->tcom.ggage + 128), hw->yp + 24.0f, 3.0f, 0xFFFF0000);
}


void func_0011D028(void)
{
    AllCharacterWorkRelease();
    if (Player2File) {
        if (Player2File != Player1File) {
            dfree(Player2File);
        }
        Player2File = 0;
    }
    if (Player1File) {
        dfree(Player1File);
        Player1File = 0;
    }
    BGM_Free();
    VoiceFree();
}


int GetPassBattleTime(void);
int IsFadeEnd(void);
int ReqSongMute(int outtime, int intime, int mutetime, int mval);
void Rq_Fadeout(void);
void SetFadeSpeed(int frame);

extern int BootControl;
extern int PLAYER_HITPOINTMAX;



void ExitDemo_Main(void)
{
    if (ExitDemoMode == 0) {
        if (GetPassBattleTime() < 0x709) {
            if ((pads[0].trg | pads[1].trg) & 0x8FF) {
                ExitDemoMode = 1;
                SetFadeSpeed(20);
                Rq_Fadeout();
                ReqSongMute(1, 10, 240, 5);
            } else {
                if (!(PlayerWork[0].HitPoint < PLAYER_HITPOINTMAX / 4 || PlayerWork[1].HitPoint < PLAYER_HITPOINTMAX / 4)) {
                    return;
                }
                BootControl |= 2;
                ExitDemoMode = 1;
                SetFadeSpeed(20);
                Rq_Fadeout();
                ReqSongMute(1, 10, 240, 5);
            }
        } else {
            BootControl |= 2;
            ExitDemoMode = 1;
            SetFadeSpeed(20);
            Rq_Fadeout();
            ReqSongMute(1, 10, 240, 5);
        }
    } else if (IsFadeEnd()) {
        sound_alloff();
        GameKill();
        JobChange(3);
    }
}

void func_0011B530(void);
void func_00157DB8(void);
void func_0012D3C8(void);
void func_0015ABD8(void);
void func_001102D8(void);
void func_0012CF00(void);


void func_0011D188(void)
{
    static int SlowCount = 0;
    int Doing;

    if (IsDemoBattle()) {
        ExitDemo_Main();
        if (JobMode != 0x1D) return;
        DrawDemoPlayandLogo();
    } else {
        func_0011B530();
    }
    if (Pause) {
        Doing = 0;
    } else if (DebugFlag & 1) {
        if (--SlowCount <= 0) {
            SlowCount = 4;
            Doing = 1;
        } else {
            Doing = 0;
        }
    } else {
        Doing = 1;
    }
    if (Doing) {
        if (GlobalHitStop <= 0) {
            TotalGameCounter++;
            ScreenExecute();
            PlayerExecute();
            BGObjCreate();
            ObjectExecute();
            EffectExecute();
            ChaseFadeOutExe();
            HitStopTimeExeRoot();
        } else {
            GlobalHitStop--;
        }
        KakuseiBG_Execute();
        CpObjExecute();
    }
    PlayerInfoDisp();
    DigestReloadStack();
    GraphicExecuteRoot();
    MeterColorChange(0xFFFF00);
    BG_Execute();
    MeterColorChange(0xFF);
    CockpitExecute();
    DigestReloadStack();
    FlashExecute();
    KakuseiBG_Draw();
    func_00157DB8();
    MeterColorChange(0);
    if (Doing) CollisionExecute();
    font_execute(1);
    if (Doing) func_0012D3C8();
    if (g_info.mode & 0x100) {
        func_0015ABD8();
    } else if (g_info.mode & 0x200) {
        func_001102D8();
    } else if (g_info.mode & 0x4000) {
        replay_status_chk();
    }
    sc.damage[0] = 0;
    sc.damage[1] = 0;
    func_0012CF00();
    if (JobMode != 0x1D) func_0011D028();
}



void g_main_End(void)
{
    sound_alloff();
    nlSetBackColor(0, 0, 0);
    nlFreeCommonTextureAll();
    nlFreePaletteAll();
    ChaseEnd();
    BG_End();
    FTO_Exe(7, 0);
    BGM_Free();
    VoiceFree();
    KakuseiFinishEnd();
}

extern int intrapt;
int Credit_Init(void);
int func_0010FBB8(void);
void func_0011D028(void);


void func_0011D3F8(void)
{
    GAME_INFOMATION *gp;

    gp = &g_info;
    g_main_End();
    if (intrapt) {
        JobChange(0xC);
        intrapt = 0;
    } else if (gp->ModeFlag) {
        if (gp->mode & 0x200) {
            if (sv_info->ply == gp->WinnerSide) {
                if (userinfo.quickconti) {
                    JobChange(0x1B);
                } else {
                    JobChange(0x15);
                }
                if (func_0010FBB8()) {
                    JobChange(0x23);
                }
            } else {
                JobChange(0x23);
            }
        } else if (gp->mode & 0x400) {
            JobChange(0x18);
        } else if (gp->mode & 0x4000) {
            Credit_Init();
            JobChange(0x19);
        } else {
            JobChange(0x25);
        }
    } else {
        JobChange(6);
    }
    func_0011D028();
}