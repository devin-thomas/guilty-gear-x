/* Guilty Gear X US (SLUS-202.87): 22 functions ported from the X Plus
   decomp (ggxeuc/training.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/training.h"

#define NULL 0
extern TRAINING_SETTING *tr_info;
extern USER_SETTING userinfo;
void MarkDraw(char *str, int x, int y, int z, int alpha);
extern PAD pads[2];
void *mmalloc(int size, char *name);
extern CHARACTER_WORK *PlayerWork;
extern CHARANGE_CTRL *gl_info;
extern PRACALL prac_all[];
extern GAME_INFOMATION g_info;
void g_main_End(void);
void JobChange(int job);
void nlSetBackColor(int c1, int c2, int c3);
extern SYS_COND sc;
extern unsigned int Pause;
void gSpriteDrawPS(NL_SPR_ARG *sprite);
extern DCFONT_INFO fi;
int IsHoldPunch(CHARACTER_WORK *offset);
int IsHoldKick(CHARACTER_WORK *offset);
int IsHoldSlash(CHARACTER_WORK *offset);
int IsHoldHighSlash(CHARACTER_WORK *offset);
int IsHoldFuckin(CHARACTER_WORK *offset);
int IsTrgPunch(CHARACTER_WORK *offset);
int IsTrgKick(CHARACTER_WORK *offset);
int IsTrgSlash(CHARACTER_WORK *offset);
int IsTrgHighSlash(CHARACTER_WORK *offset);
int IsTrgFuckin(CHARACTER_WORK *offset);
void InitScore(void);
void MayBack(int a);
extern int BattleStatus;
extern int StageNo;
void set_char2num(int num, char *block, int id);
void FontDrawDC(char *str, float x, float y, float z, int alpha, int id);
int GetBattleTimeStopFlag(int padid);
void FontDraw(char *str, int x, int y, float z, int alfa);
char *G_sprintf(char *buff, char *str, int *valtb);
extern short int UseCharacter1p2p[];
void ResetScreen(void);
void EliminateRoundEnd(void);
void PlayerParaInit(void);
void SetAnimeNo(CHARACTER_WORK *offset, int no);
void ResultInitial(void);
void effect_killall(void);
int SetActNo(CHARACTER_WORK *offset, unsigned int no);
void object_killall(void);
void font_entry(int side, char *str);
void FadeInit(void);
extern short int PDDef_Gravity[];
extern unsigned int ResultFlag;
unsigned int gStripGetFadeColor(unsigned int col);
void polydraw_FBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
extern char D_5CB910[]; /* RODATA_EXTERN 0x5cb910: "tr_info" (.sdata; TranningStat_chk strings follow) */
void InitGameinfo_ByGameOver(int side);
extern int PLAYER_HITPOINTMAX;

void mark_init_main(void);


void Tranning_init(void)
{
    int i;

    if (tr_info == NULL) {
        tr_info = mmalloc(0x3880, D_5CB910);
    }
    for (i = 0; i < 2; i++) {
        g_info.LoadPlayerID[i] = -1;
        UseCharacter1p2p[i] = 0;
        InitGameinfo_ByGameOver(i);
    }
    StageNo = 0;
    tr_info->cnfside = 0;
    tr_info->stat = 0;
    tr_info->itigeki = 0;
    tr_info->wait = 0;
    tr_info->damage_origin = 0;
    tr_info->damage = 0;
    tr_info->total_damage = 0;
    tr_info->max_damage = 0;
    tr_info->max_combo = 0;
    tr_info->iobkup[0] = 0;
    tr_info->iobkup[1] = 0;
    tr_info->iobkup[2] = 0;
    tr_info->iobkup[3] = 0;
    tr_info->quickselect = 0;
    tr_info->tene.memflg = 0;
    tr_info->tene.iomem[0][0] = -1;
    tr_info->tene.iomem[1][0] = -1;
    tr_info->tene.iomem[2][0] = -1;
    tr_info->tene.iomem[3][0] = -1;
    tr_info->tene.iomem[0][899] = -1;
    tr_info->tene.iomem[1][899] = -1;
    tr_info->tene.memcnt = 0;
    tr_info->tene.resettim = 0;
    tr_info->tene.iomem[2][899] = -1;
    tr_info->tene.iomem[3][899] = -1;
    tr_info->begin = 0;
    tr_info->clear = 0;
    tr_info->tcom.ggage = userinfo.tbkup.ggage;
    tr_info->tcom.tension = userinfo.tbkup.tension;
    tr_info->tcom.damage_disp = userinfo.tbkup.damage_disp;
    tr_info->tene.mode = userinfo.tbkup.mode;
    tr_info->tene.level = userinfo.tbkup.level;
    tr_info->tene.type = userinfo.tbkup.type;
    tr_info->tene.guard = userinfo.tbkup.guard;
    tr_info->tene.ukemi = userinfo.tbkup.ukemi;
    tr_info->tene.rejene = userinfo.tbkup.rejene;
    mark_init_main();
}

void bkup_trainin_setting(void);

void tranning_reset(void);

int func_0015B1D0(void);
void tranning_bkup_clr(void);
void func_0015B818(void);
void Training_DamageChk(void);
void Training_DamageDIsp(void);
void disp_memcnt(void);
void chk_itigeki(int id);
void func_0015BF70(void);


void func_0015ABD8(void)
{
    HEADWORK hw[4] = {
        { "", 0.0f, 0.0f, 0, 0, 0 },
        { "STANDBY", 257.0f, 96.0f, 0, 0, 0 },
        { "RECORDING", 236.0f, 96.0f, 0, 0, 0 },
        { "PLAYBACK", 250.0f, 96.0f, 0, 0, 0 },
    };
    int i;

    func_0015B818();
    Training_DamageChk();
    Training_DamageDIsp();
    if (tr_info->itigeki == 5) {
        tr_info->stat = 2;
    }
    if (func_0015B1D0()) {
        return;
    }
    if (!Pause) {
        func_0015BF70();
    }
    if (BattleStatus != 2) {
        return;
    }
    if (!Pause) {
        for (i = 0; i < 2; i++) {
            if ((tr_info->itigeki == 0 && tr_info->tene.rejene) || tr_info->ply == i) {
                if (PlayerWork[i].HitPoint == 0) {
                    PlayerWork[i].HitPoint = 1;
                }
                if (!(PlayerWork[i].actst & 0x20) && PlayerWork[i].HitPoint < PLAYER_HITPOINTMAX) {
                    PlayerWork[i].HitPoint++;
                }
            }
            chk_itigeki(i);
        }
    }
    if (tr_info->wait) {
        tr_info->wait--;
    } else {
        tr_info->wait = 0;
    }
    if (Pause == 0) {
        FontDrawDC(hw[tr_info->tene.memflg].head, hw[tr_info->tene.memflg].xp, hw[tr_info->tene.memflg].yp, 4.0f, 1, 1);
        if (pads[tr_info->ply].data == userinfo.key[tr_info->ply].memory && tr_info->tene.memflg != 1) {
            if (tr_info->tene.resettim < 60) {
                tr_info->tene.resettim++;
            } else {
                tr_info->tene.memcnt = 0;
                tr_info->tene.memflg = 1;
            }
        } else {
            tr_info->tene.resettim = 0;
        }
        if ((tr_info->iobkup[2] & userinfo.key[tr_info->ply].memory) && !tr_info->wait) {
            switch (tr_info->tene.memflg) {
            case 0:
                if (tr_info->tene.iomem[0][0] != -1) {
                    tr_info->tene.memflg = 3;
                    tr_info->tene.memcnt = 0;
                    tr_info->wait = 2;
                }
                break;
            case 1:
                disp_memcnt();
                tranning_bkup_clr();
                tr_info->tene.memcnt = 0;
                if (tr_info->tene.resettim == 0) {
                    tr_info->tene.memflg = 2;
                    tr_info->wait = 2;
                }
                break;
            case 2:
                tr_info->tene.memflg = 0;
                tr_info->tene.iomem[0][tr_info->tene.memcnt] = -1;
                tr_info->tene.iomem[1][tr_info->tene.memcnt] = -1;
                tr_info->tene.iomem[2][tr_info->tene.memcnt] = -1;
                tr_info->tene.iomem[3][tr_info->tene.memcnt] = -1;
                tr_info->wait = 2;
                break;
            case 3:
                tr_info->tene.memflg = 0;
                break;
            }
        } else {
            if (tr_info->tene.mode == 1) {
                tr_info->tene.memflg = 0;
            }
            switch (tr_info->tene.memflg) {
            case 2:
                disp_memcnt();
                if (tr_info->tene.memcnt >= 899 || BattleStatus != 2) {
                    tr_info->tene.iomem[0][tr_info->tene.memcnt] = -1;
                    tr_info->tene.iomem[1][tr_info->tene.memcnt] = -1;
                    tr_info->tene.iomem[2][tr_info->tene.memcnt] = -1;
                    tr_info->tene.iomem[3][tr_info->tene.memcnt] = -1;
                    tr_info->tene.memflg = 0;
                } else {
                    tr_info->tene.memcnt++;
                }
                break;
            case 3:
                if (tr_info->tene.iomem[1][tr_info->tene.memcnt] == -1) {
                    tr_info->tene.memflg = 0;
                } else {
                    tr_info->tene.memcnt++;
                }
                break;
            case 1:
                disp_memcnt();
                break;
            default:
                tr_info->tene.memcnt = 0;
                break;
            }
        }
        switch (tr_info->tcom.tension) {
        case 1:
            PlayerWork[tr_info->ply].ply->Tension = 10000;
            break;
        case 2:
            PlayerWork[tr_info->ply ? 0 : 1].ply->Tension = 10000;
            break;
        case 3:
            PlayerWork[0].ply->Tension = 10000;
            PlayerWork[1].ply->Tension = 10000;
            break;
        }
    } else {
        if (tr_info->tene.memflg == 2 || tr_info->tene.memflg == 3) {
            tr_info->tene.memflg = 0;
        }
    }
    if (tr_info->tene.mode == 0) {
        PlayerWork[tr_info->ply ? 0 : 1].ply->CPU = 0;
        g_info.isCPU[tr_info->ply == 0] = 0;
    } else if (tr_info->tene.mode == 1) {
        if (PlayerWork[tr_info->ply ? 0 : 1].ply->CPU == 0) {
            PlayerWork[tr_info->ply ? 0 : 1].ply->CPU = 1;
            g_info.isCPU[tr_info->ply == 0] = 1;
        }
    }
}



int func_0015B1D0(void)
{
    switch (tr_info->stat) {
    case 0:
        break;
    case 1:
        g_main_End();
        g_info.PlayerSelected[0] = 0;
        g_info.PlayerSelected[1] = 0;
        JobChange(0xC);
        return 1;
    case 2:
        tranning_reset();
        tr_info->stat = 0;
        break;
    case 3:
        g_main_End();
        bkup_trainin_setting();
        g_info.PlayerSelected[0] = 0;
        g_info.PlayerSelected[1] = 0;
        if (userinfo.autosave) {
            sc.grpvar[1] = -1;
            JobChange(0x2A);
        } else {
            JobChange(3);
        }
        return 1;
    }
    return 0;
}





void bkup_trainin_setting(void)
{
    userinfo.tbkup.ggage = tr_info->tcom.ggage;
    userinfo.tbkup.tension = tr_info->tcom.tension;
    userinfo.tbkup.damage_disp = tr_info->tcom.damage_disp;
    userinfo.tbkup.mode = tr_info->tene.mode;
    userinfo.tbkup.level = tr_info->tene.level;
    userinfo.tbkup.type = tr_info->tene.type;
    userinfo.tbkup.guard = tr_info->tene.guard;
    userinfo.tbkup.ukemi = tr_info->tene.ukemi;
    userinfo.tbkup.rejene = tr_info->tene.rejene;
}

void func_001208E8(void);
void func_00133478(void);
void func_00149FB8(void);


void func_0015B310(void)
{
    int i;

    EliminateRoundEnd();
    ResultInitial();
    FadeInit();
    font_entry(-1, NULL);
    func_001208E8();
    for (i = 0; i < 2; i++) {
        PlayerWork[i].parentx = 0;
        PlayerWork[i].parenty = 0;
        PlayerWork[i].actno = 0;
        PlayerWork[i].actnonext = -1;
        SetActNo(&PlayerWork[i], 0);
        SetAnimeNo(&PlayerWork[i], 0);
        if (UseCharacter1p2p[i] == 8) {
            PlayerWork[i].ply->PersonalWork[0].b[1] = 1;
            PlayerWork[i].ply->PersonalWork[1].w[0] = 600;
        }
    }
    ResetScreen();
    func_00133478();
    func_00149FB8();
    tr_info->itigeki = 0;
    tr_info->tene.memflg = 0;
    g_info.BattleControlFlag = 0;
    BattleStatus = 2;
    ResultFlag = 0;
}




void tranning_memorize_execute(int side)
{
    if ((tr_info->iobkup[2] & userinfo.key[side ? 0 : 1].memory) && tr_info->wait == 0) {
        tr_info->tene.iomem[0][tr_info->tene.memcnt] = -1;
        tr_info->tene.iomem[1][tr_info->tene.memcnt] = -1;
        tr_info->tene.iomem[2][tr_info->tene.memcnt] = -1;
        tr_info->tene.iomem[3][tr_info->tene.memcnt] = -1;
    } else {
        tr_info->tene.iomem[2][tr_info->tene.memcnt] = pads[side].trg;
        tr_info->tene.iomem[1][tr_info->tene.memcnt] = pads[side].data;
        tr_info->tene.iomem[0][tr_info->tene.memcnt] = pads[side].old;
        tr_info->tene.iomem[3][tr_info->tene.memcnt] = pads[side].offtrg;
    }
}



void tranning_execute_execute(int side)
{
    if (tr_info->tene.iomem[1][tr_info->tene.memcnt] != -1) {
        pads[side].trg = tr_info->tene.iomem[2][tr_info->tene.memcnt];
        pads[side].data = tr_info->tene.iomem[1][tr_info->tene.memcnt];
        pads[side].old = tr_info->tene.iomem[0][tr_info->tene.memcnt];
        pads[side].offtrg = tr_info->tene.iomem[3][tr_info->tene.memcnt];
    } else {
        pads[side].trg = 0;
        pads[side].data = 0;
    }
}

void tranning_plykey_save(int side)
{
    int key = pads[side].data;

    tr_info->iobkup[0] = tr_info->iobkup[1];
    tr_info->iobkup[2] = ~tr_info->iobkup[1] & key;
    tr_info->iobkup[3] = tr_info->iobkup[1] & ~key;
    tr_info->iobkup[1] = key;
    pads[side].old = tr_info->iobkup[0];
    pads[side].data = tr_info->iobkup[1];
    pads[side].trg = tr_info->iobkup[2];
    pads[side].offtrg = tr_info->iobkup[3];
}

void tranning_bkup_clr(void)
{
    tr_info->iobkup[0] = 0;
    tr_info->iobkup[1] = 0;
    tr_info->iobkup[2] = 0;
    tr_info->iobkup[3] = 0;
}

void tranning_keyset_deku(int side)
{
    pads[side].trg = change_iodat_plytodeku(side, tr_info->iobkup[2]);
    pads[side].data = change_iodat_plytodeku(side, tr_info->iobkup[1]);
    pads[side].old = change_iodat_plytodeku(side, tr_info->iobkup[0]);
    pads[side].offtrg = change_iodat_plytodeku(side, tr_info->iobkup[3]);
}

u_int change_iodat_plytodeku(int side, u_int dat)
{
    int tmp;

    tmp = 0;
    if (dat & 0x1000) {
        tmp |= 0x1000;
    }
    if (dat & 0x4000) {
        tmp |= 0x4000;
    }
    if (dat & 0x8000) {
        tmp |= 0x8000;
    }
    if (dat & 0x2000) {
        tmp |= 0x2000;
    }
    if (dat & PlayerWork[side ? 0 : 1].ply->PunchKey) {
        tmp |= PlayerWork[side].ply->PunchKey;
    }
    if (dat & PlayerWork[side ? 0 : 1].ply->KickKey) {
        tmp |= PlayerWork[side].ply->KickKey;
    }
    if (dat & PlayerWork[side ? 0 : 1].ply->SlashKey) {
        tmp |= PlayerWork[side].ply->SlashKey;
    }
    if (dat & PlayerWork[side ? 0 : 1].ply->HighSlashKey) {
        tmp |= PlayerWork[side].ply->HighSlashKey;
    }
    if (dat & PlayerWork[side ? 0 : 1].ply->Fuckin) {
        tmp |= PlayerWork[side].ply->Fuckin;
    }
    return tmp;
}





int get_dekustat(CHARACTER_WORK *offset)
{
    if ((g_info.mode & 0x100) && offset->padid != tr_info->ply && tr_info->tene.mode == 0 && tr_info->tene.memflg != 2 && tr_info->tene.memflg != 3) return 0;
    return 1;
}

void func_0015B818(void)
{
    int i;
    int flg;

    if (Pause) {
        return;
    }
    if (tr_info->itigeki == 3) {
        return;
    }
    if (BattleStatus != 2) {
        return;
    }
    flg = 0;
    for (i = 0; i < 2; i++) {
        if ((pads[i].data & PlayerWork[i].ply->Fuckin) && (pads[i].data & PlayerWork[i].ply->MemoryKey)) {
            flg = 1;
            if (tr_info->begin == 0) {
                tr_info->stat = 2;
                pads[0].trg = 0;
                pads[1].trg = 0;
                tr_info->begin = 1;
                InitScore();
                if (StageNo == 5) {
                    MayBack(1);
                }
                break;
            }
        }
    }
    if (!flg) {
        tr_info->begin = 0;
    }
}



void Training_DamageChk(void)
{
    if ((tr_info->tcom.damage_disp == 0 && Pause == 0) || tr_info->stat == 1) {
        tr_info->damage = 0;
        tr_info->total_damage = 0;
        tr_info->max_damage = 0;
        tr_info->max_combo = 0;
    }
    if (((PlayerWork[tr_info->ply ? 0 : 1].actst & 0x20) || sc.damage[tr_info->ply ? 0 : 1] != 0) && tr_info->itigeki == 0) {
        if (sc.damage[tr_info->ply ? 0 : 1]) {
            tr_info->damage = sc.damage[tr_info->ply ? 0 : 1];
            tr_info->total_damage = tr_info->damage_origin - PlayerWork[tr_info->ply ? 0 : 1].HitPoint;
            if (tr_info->max_damage < tr_info->total_damage) {
                tr_info->max_damage = tr_info->total_damage;
            }
            if (tr_info->max_combo < PlayerWork[tr_info->ply ? 0 : 1].ply->HitCount) {
                tr_info->max_combo = PlayerWork[tr_info->ply ? 0 : 1].ply->HitCount;
            }
        }
    } else {
        tr_info->damage_origin = PlayerWork[tr_info->ply ? 0 : 1].HitPoint;
    }
}

void Training_DamageDIsp(void)
{
    int cpsenterpos[2] = { 160, 480 };
    int valtb[2];

    if (tr_info->tcom.damage_disp && !Pause) {
        valtb[0] = tr_info->damage;
        FontDraw("      DAMAGE", cpsenterpos[tr_info->ply ? 0 : 1] - 118, 112, 265.0f, 0xFF);
        FontDraw(G_sprintf(NULL, "%3d", valtb), cpsenterpos[tr_info->ply ? 0 : 1] + 58, 112, 266.0f, 0xFF);
        valtb[0] = tr_info->total_damage;
        FontDraw("TOTAL DAMAGE", cpsenterpos[tr_info->ply ? 0 : 1] - 118, 136, 265.0f, 0xFF);
        FontDraw(G_sprintf(NULL, "%3d", valtb), cpsenterpos[tr_info->ply ? 0 : 1] + 58, 136, 266.0f, 0xFF);
        valtb[0] = tr_info->max_damage;
        FontDraw("  MAX DAMAGE", cpsenterpos[tr_info->ply ? 0 : 1] - 118, 160, 265.0f, 0xFF);
        FontDraw(G_sprintf(NULL, "%3d", valtb), cpsenterpos[tr_info->ply ? 0 : 1] + 58, 160, 266.0f, 0xFF);
        valtb[0] = tr_info->max_combo;
        FontDraw("  MAX  COMBO", cpsenterpos[tr_info->ply ? 0 : 1] - 118, 184, 265.0f, 0xFF);
        FontDraw(G_sprintf(NULL, "%3d", valtb), cpsenterpos[tr_info->ply ? 0 : 1] + 58, 184, 266.0f, 0xFF);
    }
}

void disp_memcnt(void)
{
    int cnt;
    int xp;
    char buf[4];

    cnt = 900 - tr_info->tene.memcnt;
    set_char2num(cnt / 60, buf, 0);
    xp = (cnt / 60 < 10) ? 0x128 : 0x114;
    FontDrawDC(buf, xp, 136.0f, 4.0f, 1, 2);
    FontDrawDC(":", 322.0f, 136.0f, 4.0f, 1, 2);
    set_char2num(cnt % 60, buf, 2);
    FontDrawDC(buf, 334.0f, 136.0f, 4.0f, 1, 2);
}




void chk_itigeki(int id)
{
    int i;
    int temp_no;
    int flag;
    int itigeki_table[18][6] = {
        { 0x7C, 0x7D, 0x7E, 0x7F, -1, -1 },
        { 0x60, -1, -1, -1, -1, -1 },
        { 0xA2, 0xA3, 0xA4, -1, -1, -1 },
        { 0x70, 0x71, 0x72, -1, -1, -1 },
        { 0x7F, 0x80, 0x81, -1, -1, -1 },
        { 0x96, 0x97, -1, -1, -1, -1 },
        { 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C },
        { 0x5D, 0x5E, 0x5F, 0x60, -1, -1 },
        { 0x65, 0x66, 0x67, -1, -1, -1 },
        { 0x64, 0x65, 0x66, 0x68, -1, -1 },
        { 0x96, 0x97, 0x5E, -1, -1, -1 },
        { 0x65, 0x66, 0x78, 0x79, 0x73, 0x89 },
        { 0x63, 0x64, 0x65, 0x6E, -1, -1 },
        { 0x7C, 0x7E, 0x7F, -1, -1, -1 },
        { 0x6A, 0x6B, 0x6C, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1 },
    };

    if (PlayerWork[id].ply->OverKillFlag == 2) {
        flag = 0;
        for (i = 0; i < 6; i++) {
            temp_no = itigeki_table[PlayerWork[id].idno - 1][i];
            if (temp_no != -1 && temp_no == PlayerWork[id].actno) {
                flag++;
            }
        }
        if (flag == 0) {
            PlayerWork[id].ply->OverKillFlag = 0;
            tr_info->itigeki = 0;
        } else if (GetBattleTimeStopFlag(id)) {
            tr_info->itigeki = 3;
        } else if (tr_info->itigeki != 3) {
            tr_info->itigeki = 2;
        }
    }
}

int mark_making(void);
void mark_disp(void);
void mark_init_sort(void);
void mark_init_clear(void);
void mark_oldkey_clr(u_int key);


void func_0015BF70(void)
{
    int ret;

    minfo->key = pads[tr_info->ply].data;
    minfo->trg = pads[tr_info->ply].trg;
    mark_oldkey_clr(minfo->key);
    ret = mark_making();
    mark_disp();
    if (--minfo->renewal_time <= 0) {
        minfo->renewal_time = 0;
    }
    if (ret == 0) {
        minfo->disp_time--;
    }
    if (minfo->disp_time <= 0) {
        mark_init_clear();
        mark_init_sort();
        markbuf[0] = mdata;
        minfo->disp_time = 0;
        markbuf[1] = NULL;
    }
}



int mark_making(void)
{
    int ret;

    ret = mark_make_key(minfo->key);
    trg_wait_frame(minfo->trg);
    if (minfo->trg_time >= 3) {
        ret |= mark_make_button(minfo->trg_douji);
    }
    if (ret) {
        if (minfo->renewal_time == 0) {
            minfo->renewal_time = 60;
        } else {
            minfo->renewal_time += 15;
        }
        if (minfo->renewal_time > 60) {
            minfo->renewal_time = 60;
        }
        minfo->disp_time = 240;
    }
    return ret;
}

int chk_oldkey(u_int key, int id);

void free_mark(MARKDATA *m);

void chk_renewal(void);

MARKDATA *get_mark(void);



int mark_make_key(u_int key)
{
    MARKDATA *m;

    key &= 0xF800;
    if (key == 0 || chk_oldkey(key, 1)) {
        return 0;
    }
    chk_renewal();
    while ((m = get_mark()) == NULL) {
        free_mark(markbuf[1]);
    }
    m->key = key;
    m->id = 1;
    if (key & 0x1000) {
        if (key & 0x8000) {
            m->buf[0] = '7';
        } else if (key & 0x2000) {
            m->buf[0] = '9';
        } else {
            m->buf[0] = '8';
        }
    } else if (key & 0x4000) {
        if (key & 0x8000) {
            m->buf[0] = '1';
        } else if (key & 0x2000) {
            m->buf[0] = '3';
        } else {
            m->buf[0] = '2';
        }
    } else if (key & 0x8000) {
        m->buf[0] = '4';
    } else if (key & 0x2000) {
        m->buf[0] = '6';
    }
    return 1;
}



int mark_make_button(u_int button)
{
    MARKDATA *m;
    int data = 0;
    int i;

    button &= 0x8FF;
    minfo->trg_time = 0;
    minfo->trg_douji = 0;
    if (button == 0 || chk_oldkey(button, 2)) {
        return 0;
    }
    chk_renewal();
    while ((m = get_mark()) == NULL) {
        free_mark(markbuf[1]);
    }
    m->id = 2;
    m->key = button;
    if (button & 0x20) {
        data |= 1;
    }
    if (button & 0x40) {
        data |= 2;
    }
    if (button & 0x80) {
        data |= 4;
    }
    if (button & 0x10) {
        data |= 8;
    }
    if (data) {
        data += 0x60;
        m->buf[0] = data;
        i = 1;
    } else {
        i = 0;
    }
    if (button & 4) {
        data = 'A';
        m->buf[i] = data;
        i++;
    }
    if (button & 8) {
        data = 'B';
        m->buf[i] = data;
        i++;
    }
    if (button & 2) {
        data = 'D';
        m->buf[i] = data;
        i++;
    }
    if (button & 1) {
        data = 'H';
        m->buf[i] = data;
    }
    return 1;
}


int chk_oldkey(u_int key, int id)
{
    MARKDATA *m = markbuf[1];

    if (m == NULL) {
        return 0;
    }
    while (m->next != NULL) {
        m = m->next;
    }
    while (m != NULL && m->id != id) {
        m = m->prev;
    }
    return m != NULL && m->key == key;
}

void mark_disp(void)
{
    MARKDATA *m;
    int i;

    for (i = 0, m = markbuf[1]; m != NULL; m = m->next, i++) {
        MarkDraw(m->buf, i * 32 + 16, 400, 4, 1);
    }
}

void mark_init_main(void)
{
    if (minfo == NULL) {
        minfo = mmalloc(0x18, "minfo");
        mdata = mmalloc(0x17C, "mdata");
    }
    mark_init_info();
    mark_init_clear();
    mark_init_sort();
    markbuf[0] = mdata;
    markbuf[1] = NULL;
}

void mark_init_info(void)
{
    minfo->disp_time = 0;
    minfo->renewal_time = 0;
    minfo->key = 0;
    minfo->trg_time = 0;
    minfo->trg_douji = 0;
}

void mark_init_sort(void)
{
    int i;

    mdata[0].prev = NULL;
    mdata[0].next = &mdata[1];
    for (i = 1; i < 18; i++) {
        mdata[i].prev = &mdata[i - 1];
        mdata[i].next = &mdata[i + 1];
    }
    mdata[18].next = NULL;
    mdata[18].prev = &mdata[17];
}





MARKDATA *get_mark(void)
{
    MARKDATA *p = markbuf[0];

    if (p == NULL) {
        return NULL;
    }
    if (p->next != NULL) {
        markbuf[0] = p->next;
        markbuf[0]->prev = NULL;
    } else {
        markbuf[0] = NULL;
    }
    mark_dataclear(p);
    link_mark_busy(p);
    return p;
}


void link_mark_busy(MARKDATA *m)
{
    MARKDATA *p = markbuf[1];

    if (p != NULL) {
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = m;
        m->prev = p;
    } else {
        markbuf[1] = m;
        m->prev = NULL;
    }
    m->next = NULL;
}

void free_mark(MARKDATA *m)
{
    if (m->prev == NULL) {
        if (m->next != NULL) {
            m->next->prev = NULL;
            markbuf[1] = m->next;
        } else {
            markbuf[1] = NULL;
        }
    } else {
        m->prev->next = m->next;
        if (m->next != NULL) {
            m->next->prev = m->prev;
        }
    }
    mark_dataclear(m);
    link_mark_blank(m);
}

void link_mark_blank(MARKDATA *m)
{
    MARKDATA *p = markbuf[0];

    if (p != NULL) {
        m->next = p;
        p->prev = m;
    } else {
        markbuf[0] = m;
        m->next = NULL;
    }
    m->prev = NULL;
}

void mark_init_clear(void)
{
    int i;

    for (i = 0; i < 19; i++) {
        mark_dataclear(&mdata[i]);
    }
}

void mark_dataclear(MARKDATA *m)
{
    m->next = NULL;
    m->prev = NULL;
    m->buf[0] = 0;
    m->buf[1] = 0;
    m->buf[2] = 0;
    m->buf[3] = 0;
    m->id = 0;
    m->key = 0;
}

void mark_oldkey_clr(u_int key)
{
    MARKDATA *m;

    if (key == 0) {
        for (m = markbuf[1]; m != NULL; m = m->next) {
            m->key = 0;
        }
    }
}

void chk_renewal(void)
{
    if (minfo->renewal_time == 0) {
        mark_init_clear();
        mark_init_sort();
        markbuf[0] = mdata;
        minfo->renewal_time = 60;
        markbuf[1] = NULL;
    }
}

void trg_wait_frame(u_int trg)
{
    trg &= 0xFFFF07FF;
    if (trg) {
        minfo->trg_douji |= trg;
        minfo->trg_time++;
    } else if (minfo->trg_time) {
        minfo->trg_time++;
    }
}