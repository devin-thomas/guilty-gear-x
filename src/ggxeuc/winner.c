/* Guilty Gear X US (SLUS-202.87): 5 functions ported from the X Plus
   decomp (ggxeuc/winner.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/winner.h"

int IST_Load(int *texadr, int sprid, int palid);
void LinkAddrTb(u_int *addr);
extern GAME_INFOMATION g_info;
int IsEvent(u_int flag);
int WinnerNexstep(void);
extern CREDIT_RAM cr_ram[2];
extern int final;
extern int BootControl;
int VsCpuCtrl(void);
int SetLimitStatus(void);
extern SYS_COND sc;
extern USER_SETTING userinfo;
char *FileReadSelect(int fid, int chan);
void JobNext(void);
void heap_clear(void);
int FileReadCheck(void);
extern int SubJob;
extern int SubJobCount;
void BGM_Free(void);
void JobChange(int job);
void gFadeValSet(float val);
void nlObjPutSetFadeColor(float r, float g, float b);
void sound_alloff(void);
extern int rannyuuflag;
extern int JobMode;
void EnterFight_Main(void);
void dfree(void *mem);
int iodat_chk(int stat);
int sound_request(int no);
extern PAD pads[2];
void FontDrawDC(char *str, float x, float y, float z, int alpha, int id);
void polydraw_FBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
void BattleTimeInit(void);
void EliminateRoundEnd(void);
void MayBack(int a);
void MaySHPOrg(void);
void PlayerParaInit(void);
int SetActNo(CHARACTER_WORK *offset, unsigned int no);
void SetAnimeNo(CHARACTER_WORK *offset, int no);
void SetScreenFlag(int flag);
void font_entry(int side, char *str);
void nlSetBackColor(int c1, int c2, int c3);
extern CHARACTER_WORK *PlayerWork;
extern int StageNo;
void g_main_End(void);
void FadeInit(void);
void PXF_SpriteClear(void);
void objkill_winmrk(void);
void ResultInitial(void);
void SetFadeEndCommand(int val);
void SetFadeSpeed(int frame);
void Rq_Fadein(void);
int IsFadeEnd(void);
void ggx_bgm_start(void);
void BeforeFight_Init(void);
void DispWinnerCredit(void);
void gSpriteDraw_NaomiLibVersion(NL_SPR_ARG *sprite);
void polydraw_LBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
void polydraw_FLine(float x1, float y1, float x2, float y2, float z, unsigned int color1);
void FightControl_Execute(FIGHT_CONTROL *tablep, NL_SPR_ARG *sp, int time);
void polydraw_GBox(float x1, float y1, float x2, float y2, float z, unsigned int color1, unsigned int color2, unsigned int color3, unsigned int color4);
extern int D_5C4118[]; /* RODATA_EXTERN 0x5c4118: sndtb {0, 0x20, 0x10, 0x30} */
void *dmalloc(int size, char *name);
void SprArgSet(int count, NL_SPR_ARG *arg, NL_SPR_ARG *send);
void memcopy(void *src, void *dst, int size);
void FTO_Exe(int no, int arg);
void BGM_Request(int no, int loop);
void CockpitLoadInit(void);
void CreditFontLoad(int id);
void InitCancelFlag(void);
extern NL_SPR_ARG sprarg[];
extern POLY polyarg[];
void WinnerMainGameOver(void);
int DispGameOver(int sw);
int SnDrvBgmCheck(void);
int Cr_IsNameEntry(int side);
int IOPLock(void);
void IOPUnLock(void);
int Credit_Init(void);
int MemAccess_AutoSave(int mode);
extern int current_bgm;

static NL_SPR_ARG *winsp;
static int localcancel;
static POLY *poly;

void IstBinLoad(void)
{
    int i;
    u_int *p;

    p = (u_int *)WinnerChrFile;
    LinkAddrTb(p);
    IST_Load((int *)p, 0x30, -1);
    p = (u_int *)WinnerFile;
    for (i = 0; i < g_info.Winnerid; i++) {
        p++;
    }
    p = (u_int *)*p;
    LinkAddrTb(p);
    for (i = 0; p[i]; i++) {
        IST_Load((int *)&p[i], i + 0x31, -1);
    }
    p = (u_int *)WinnerFile;
    p += 17;
    for (i = 0; i < g_info.Winnerid - 1; i++) {
        p++;
    }
    p = (u_int *)*p;
    LinkAddrTb(p);
    for (i = 0; p[i]; i++) {
        IST_Load((int *)&p[i], i + 0x34, -1);
    }
}

void func_00161F78(void)
{
    GAME_INFOMATION *gp;

    gp = &g_info;
    winner_mode = 0;
    if (final == 2) {
        winner_mode = 12;
        gp->BattleControlFlag |= 4;
        return;
    }
    if (!WinnerNexstep()) {
        winner_mode |= 6;
        gp->BattleControlFlag |= 4;
    } else {
        winner_mode |= 1;
        gp->BattleControlFlag &= ~4;
    }
    if ((IsEvent(1) & 1) && gp->BattleCnt[gp->WinnerSide] >= 8) {
        BootControl |= 4;
        winner_mode = (winner_mode | 4) & ~3;
        g_info.BattleControlFlag |= 4;
    } else if (!(IsEvent(1) & 1) && gp->BattleCnt[gp->WinnerSide] >= 10) {
        winner_mode = (winner_mode | 0x20) & ~3;
        g_info.BattleControlFlag |= 4;
    }
    if (IsEvent(0x10) && gp->WinCount >= 10) {
        g_info.BattleControlFlag |= 4;
        winner_mode = (winner_mode | 4) & ~3;
    }
    if (IsEvent(0x20) == 1) {
        winner_mode &= ~2;
    }
}





int GetWinnerNextMode(void)
{
    if (winner_mode & 4) {
        return 3;
    }
    if (winner_mode & 0x10) {
        return 12;
    }
    if (winner_mode & 1) {
        return VsCpuCtrl();
    }
    if (winner_mode & 0x20) {
        return 0x28;
    }
    return 0;
}



int Cr_IsPlaying(int side);
int Cr_SetGameOver(int side);
void EasySpriteDrawEX(int id, int x, int y, float z, int flg, float trnsl);
void gStripDraw_BG(NL_SPR_ARG *sprite);
int Random(void);




int DispGameOver(int sw)
{
    static int fms;
    static int count;
    static float over_x;
    static float over_t;

    if (sw == 0) {
        winsp[11].trnsl = 0;
        winsp[11].attr = 0x205;
        over_x = 40.0f;
        fms = 0;
        over_t = 0;
        count = 0;
        if (Cr_IsPlaying(0)) {
            Cr_SetGameOver(0);
        }
        if (Cr_IsPlaying(1)) {
            Cr_SetGameOver(1);
        }
        return 0;
    }
    if (fms >= 0x1CC) {
        return 1;
    }
    gFadeValSet(1.0f);
    DispWinnerCredit();
    FightControl_Execute(game_overtb, winsp, fms);
    if (winsp[11].trnsl < 1.0 && fms < 100) {
        winsp[11].trnsl += 0.1;
    }
    if (fms >= 0x29 && fms < 0x60) {
        over_x -= 0.71428573f;
        over_t += 0.017857144f;
    } else if (fms >= 0x61 && fms < 0x9E) {
        over_x = 0;
        over_t = 1.0f;
    } else if (fms >= 0x9F && fms < 0xB8) {
        over_x += 1.4285715f;
        over_t -= 0.033333335f;
    }
    if (fms >= 0x29 && fms < 0xB8) {
        EasySpriteDrawEX(0, 244.0f - over_x, 0xFC, 7.0f, 0xD, over_t);
        EasySpriteDrawEX(0, over_x + 244.0f, 0xFC, 7.0f, 0xD, over_t);
    }
    if (fms == 0xF8) {
        over_t = 0;
    } else if (fms >= 0xF9 && fms < 0x134) {
        over_t += 0.015625f;
    } else if (fms >= 0x135 && fms < 0x1AA) {
        over_t = 1.0f;
    }
    if (fms >= 0xF9 && fms < 0x1AA) {
        EasySpriteDrawEX(1, 0xD8, 0x158, 7.0f, 0xD, over_t);
    }
    if (fms < 0x1AA) {
        gFadeValSet(0.5f);
        gSpriteDraw_NaomiLibVersion(&winsp[11]);
        if (fms & 1) {
            winsp[11].attr = D_5C4118[count & 3] | 0x205;
            winsp[11].zm_x = ((float)(Random() & 9) + 640.0) / 640.0;
            winsp[11].zm_y = ((float)(Random() & 9) + 480.0) / 480.0;
            count++;
        }
    }
    if (fms == 0x1AE) {
        over_t = 0.7f;
    }
    if (fms >= 0x1AF && fms < 0x1BA) {
        polydraw_GBox(0, 270.0f, 640.0f, 240.0f, 3.0f, 0x01000000, 0x01000000, ((int)(over_t * 255.0) << 24) | 0xFFFFFF, ((int)(over_t * 255.0) << 24) | 0xFFFFFF);
        polydraw_GBox(0, 240.0f, 640.0f, 210.0f, 3.0f, ((int)(over_t * 255.0) << 24) | 0xFFFFFF, ((int)(over_t * 255.0) << 24) | 0xFFFFFF, 0x01000000, 0x01000000);
        over_t -= 0.058333334f;
    }
    if (fms >= 0x1AA) {
        gStripDraw_BG(&winsp[11]);
        gStripDraw_BG(&winsp[12]);
    }
    fms++;
    return 0;
}



int Cr_Request_Start(int side);
int Cr_GetCrditCnt(int side);
void EasySpriteDrawNum(int id, int x, int y, int w, float z, int n);
void ClrVSFlag(void);
int ko(int count);
extern short int UseCharacter1p2p[];



int ko(int count)
{
    POSTB postb[24] = {
        { 561, 83 }, { 553, 71 }, { 541, 63 }, { 527, 63 }, { 516, 71 }, { 512, 83 }, { 512, 97 }, { 516, 108 },
        { 527, 112 }, { 541, 112 }, { 553, 108 }, { 561, 97 }, { 570, 82 }, { 558, 66 }, { 542, 54 }, { 516, 50 },
        { 496, 63 }, { 489, 82 }, { 489, 108 }, { 496, 128 }, { 516, 135 }, { 542, 135 }, { 561, 128 }, { 570, 108 },
    };
    static NL_SPR_ARG sprite[24];
    static int credit;
    static int credit_old;
    static int fcount;
    static int lcount;
    static int tcount;
    static int flag;
    static float trnsl;
    int i;

    if (count == 0) {
        for (i = 0; i < 24; i++) {
            if (i < 12) {
                sprite[i].sprno = i + 6;
            } else if (i >= 12 && i < 15) {
                sprite[i].sprno = i + 15;
            } else if (i >= 15) {
                sprite[i].sprno = i + 3;
            }
            sprite[i].x = postb[i].x;
            sprite[i].y = postb[i].y;
            sprite[i].z = 5.0f;
            sprite[i].zm_x = 1.0f;
            sprite[i].zm_y = 1.0f;
            sprite[i].ang = 0;
            sprite[i].u0 = 0;
            sprite[i].u1 = 1.0f;
            sprite[i].v0 = 0;
            sprite[i].v1 = 1.0f;
            sprite[i].listType = 1;
            sprite[i].trnsl = 1.0f;
            sprite[i].attr = 0xD;
        }
        tcount = 9;
        trnsl = 1.0f;
        lcount = 0;
        fcount = 0;
        flag = 0;
        credit_old = credit = Cr_GetCrditCnt(g_info.WinnerSide == 0);
        return 0;
    }
    if (tcount) {
        if (pads[g_info.WinnerSide ? 0 : 1].trg & 0x800) {
            Cr_Request_Start(g_info.WinnerSide == 0);
            ClrVSFlag();
            winner_mode = (winner_mode | 0x11) & ~6;
            g_info.BattleTime[0] = g_info.BstartTime[0];
            g_info.BattleTime[1] = g_info.BstartTime[1];
            sc.conti[g_info.WinnerSide == 0] = UseCharacter1p2p[g_info.WinnerSide ? 0 : 1];
            return 0;
        }
        if (pads[g_info.WinnerSide ? 0 : 1].trg & 0x8FC) {
            lcount = 12;
            fcount = 9;
        }
        credit = Cr_GetCrditCnt(g_info.WinnerSide == 0);
        if (credit_old < credit) {
            credit_old = credit;
            tcount = 9;
            lcount = 0;
            fcount = 0;
        }
        EasySpriteDrawEX(5, 0x1DA, 0x97, 7.0f, 0x20D, 1.0f);
        EasySpriteDrawEX(3, 0x1DA, 0xAF, 7.0f, 0xD, trnsl);
        for (i = 0; i < lcount; i++) {
            gSpriteDraw_NaomiLibVersion(&sprite[i + 12]);
            gSpriteDraw_NaomiLibVersion(&sprite[i]);
        }
        if (lcount != -1) {
            EasySpriteDrawNum(4, 0x210, 0x5D, 0xC, 5.0f, tcount);
        }
        if (lcount >= 9) {
            trnsl -= 0.045454545454545456;
        }
        if (++fcount == 10) {
            fcount = 0;
            if (++lcount >= 13) {
                lcount = -1;
                trnsl = 1.0f;
                tcount--;
            }
        }
        if (flag != tcount) {
            if (tcount == 0) {
                sound_request(0x4A);
            } else {
                sound_request(tcount + 0x40);
            }
            flag = tcount;
        }
        return 0;
    }
    winner_mode &= ~2;
    Cr_SetGameOver(g_info.WinnerSide == 0);
    return 1;
}

extern int RTimeLimitTb[];


void func_00162AF8(int winner)
{
    int id;

    userinfo.clear |= 1 << winner;
    id = g_info.WinnerSide;
    if (userinfo.phase < 3) {
        if (userinfo.phase >= 2 && RTimeLimitTb[userinfo.roundtime] == 60 && sc.loose == 0) {
            userinfo.phase = 3;
        }
        if (userinfo.phase < 2 && g_info.Difficulty >= 2 && g_info.CPURoundMax >= 2 && sc.conti[id] == 0) {
            userinfo.phase = 2;
        }
        if (userinfo.phase <= 0 && sc.change[id] == 0) {
            userinfo.phase = 1;
        }
        SetLimitStatus();
    }
}










void Comment_Main(int cc)
{
    static int count = 0;
    static float x = 64.0f;
    static float y = 296.0f;
    static NL_SPR_ARG sprite;
    NL_SPR_ARG sprarg[1] = {
        { 1, 320.0f, 360.0f, 3.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0.0f, 1, 0x20A },
    };

    if (cc == 0) {
        SprArgSet(1, sprarg, &sprite);
        sprite.sprno = g_info.Loserid + 0x33;
        if (localcancel) {
            count = 20;
            x = 576.0f;
            y = 424.0f;
        } else {
            x = 64.0f;
            y = 296.0f;
            count = 0;
        }
    }
    if (count <= 240) {
        if (count < 9) x += 64.0;
        if (count >= 10) x = 576.0f;
        if (count >= 9 && count < 15) y += 21.333333333333332;
        if (count >= 16) y = 424.0f;
        if (count == 20) sprite.trnsl = 1.0f;
        if (count >= 231) sprite.trnsl -= 0.1;
        polydraw_FBox(64.0f, 296.0f, (int)x, (int)y, 4.0f, 0xA0000000);
        gSpriteDraw_NaomiLibVersion(&sprite);
        count++;
    }
}






void WinnerInit(void)
{
    int id = g_info.Winnerid;

    heap_clear();
    WinnerFile = FileReadSelect(0x3f, 2);
    WinnerChrFile = FileReadSelect(id + 0x2e, 4);
    JobNext();
}



void WinnerLoad(void)
{
    if (FileReadCheck()) {
        SubJob = 0;
        SubJobCount = 0;
        JobNext();
    }
}



void WinnerMainFirst(void)
{
    int *p;
    int i;

    PXF_SpriteClear();
    FadeInit();
    p = (int *)WinnerFile;
    LinkAddrTb((u_int *)p);
    p = (int *)*p;
    LinkAddrTb((u_int *)p);
    for (i = 0; p[i]; i++) {
        if (i == 0x22 || i == 0x1E || i == 0x1F || i == 0x20 || i == 0x21 || (u_int)i < 0x23) {
            IST_Load(p + i, i, -1);
        }
    }
    winsp = dmalloc(0x410, "winsp");
    poly = dmalloc(0xC8, "winpoly");
    SprArgSet(0xD, sprarg, winsp);
    memcopy(polyarg, poly, 0xC8);
    FTO_Exe(0, 0);
    FTO_Exe(5, 0);
    FTO_Exe(9, 0);
    BGM_Request(0x1C, 0);
    FTO_Exe(3, 0);
    CockpitLoadInit();
    CreditFontLoad(0x14);
    InitCancelFlag();
    SetWinnerFlag();
    rannyuuflag = 0;
    localcancel = 0;
    if (winner_mode & 8) {
        if (g_info.mode & 0x800) {
            SubJob = 5;
        } else {
            SubJob = 4;
        }
    } else {
        IstBinLoad();
        SubJob++;
    }
}



int ChkCancel(void);
int IsCancelFlag(void);
int IsRannyuu(void);
int SetCancelFlag(int flag);
unsigned int gStripGetFadeColor(unsigned int color);
void Comment_Main(int cc);
void DispCreditMsg(void);
void Rq_Fadeout(void);
extern FIGHT_CONTROL winnertb[];

 /* RODATA_EXTERN 0x5c4128: TU static winnertb */

void WinnerMainWin(void)
{
    static int winner_win_fms;
    static float winner_win_over_x;
    static float winner_win_over_t;
    static float winner_win_h;
    static float winner_win_b;
    static float winner_win_fade;
    int i;
    int color;

    if (SubJobCount == 0) {
        SubJobCount = 1;
        winner_win_fade = 1.0f;
        winner_win_fms = 0;
        winner_win_over_x = 0;
        winner_win_over_t = 0;
        winner_win_h = 0;
        winner_win_b = 0;
    }
    if (winner_win_fms < 0x1D7) {
        if (IsEvent(8)) {
            g_info.BattleControlFlag |= 4;
        } else {
            rannyuuflag |= IsRannyuu();
        }
        if (IsCancelFlag() && IsFadeEnd()) {
            if (!(winner_mode & 4)) {
                SubJob = 5;
            } else {
                SubJob = 2;
            }
            return;
        }
        if (ChkCancel()) {
            if ((localcancel && winner_win_fms >= 0xED) || (winner_win_fms >= 0xE9 && !localcancel)) {
                if (!IsCancelFlag()) {
                    SetFadeSpeed(10);
                    Rq_Fadeout();
                    SetCancelFlag(1);
                }
            } else if (winner_win_fms < 0xE8) {
                localcancel = 1;
                winsp[9].y = 480.0f;
                winner_win_fms = 0xE8;
winsp[1].trnsl = 0;
                winsp[2].trnsl = 0;
                winsp[3].trnsl = 0;
                winsp[4].trnsl = 0;
                winsp[5].trnsl = 0;
                winsp[6].trnsl = 0;
                winsp[10].trnsl = 0;
                winsp[8].u1 = 0.52734375f;
                winsp[8].zm_x = 0.52734375f;
                winsp[7].trnsl = 0;
            }
        }
        if (!rannyuuflag) {
            FightControl_Execute(winnertb, winsp, winner_win_fms);
        }
        DispCreditMsg();
        if (!rannyuuflag) {
            if (winner_win_fms >= 0x79 && winner_win_fms < 0x89) {
                winsp[7].x = winner_win_over_x;
                winsp[7].u0 = (winner_win_over_x - 32.0) * 0.001953125;
                winsp[7].u1 = (winsp[7].x + 32.0) * 0.001953125;
                winsp[7].zm_x = 0.125f;
            }
            if (winner_win_fms >= 0x6C && winner_win_fms < 0x78) {
                winner_win_over_t += 0.08333333333333333;
            }
            if (winner_win_fms >= 0x79 && winner_win_fms < 0x89) {
                winsp[8].u1 = winner_win_over_x / 540.0;
                winsp[8].zm_x = winner_win_over_x / 540.0;
                winner_win_over_x += 33.75;
            }
            if (winner_win_fms >= 0x88) {
                winsp[8].u1 = 1.0f;
                winsp[8].zm_x = 1.0f;
                winner_win_over_x = 540.0f;
            }
            if (winner_win_fms >= 0x97 && winner_win_fms < 0xB5) {
                winsp[9].y -= 16.0;
            } else if (winner_win_fms >= 0xB5) {
                winsp[9].y = 480.0f;
            }
            if (winner_win_fms >= 0x47 && winner_win_fms < 0x5A) {
                poly[0].t += 12.75;
            } else if (winner_win_fms >= 0x5B) {
                poly[0].t = 255.0f;
            }
            if (winner_win_fms >= 0x47 && winner_win_fms < 0x5A) {
                poly[1].t += 12.75;
            } else if (winner_win_fms >= 0x5B) {
                poly[1].t = 255.0f;
            }
            if (winner_win_fms >= 0x29 && winner_win_fms < 0x46) {
                poly[2].t += 8.5;
            } else if (winner_win_fms >= 0x47) {
                poly[2].t = 255.0f;
            }
            if (winner_win_fms >= 0x1F && winner_win_fms < 0x34) {
                poly[3].t += 11.590909090909092;
            } else if (winner_win_fms >= 0x35) {
                poly[3].t = 255.0f;
            }
            if (winner_win_fms >= 0x3D && winner_win_fms < 0x50) {
                poly[4].t += 12.75;
            } else if (winner_win_fms >= 0x51) {
                poly[4].t = 255.0f;
            }
            if (winner_win_fms >= 0x1F && winner_win_fms < 0x34) {
                poly[5].t += 11.590909090909092;
            } else if (winner_win_fms >= 0x35) {
                poly[5].t = 255.0f;
            }
            if (winner_win_fms >= 0x3D && winner_win_fms < 0x45) {
                poly[6].t += 25.5;
                poly[6].x -= 37.25;
            } else if (winner_win_fms >= 0x46) {
                poly[6].t = 255.0f;
                poly[6].x = 566.0f;
            }
            if (winner_win_fms >= 0x3D && winner_win_fms < 0x45) {
                poly[7].t += 25.5;
                poly[7].x += 66.0;
            } else if (winner_win_fms >= 0x46) {
                poly[7].t = 255.0f;
                poly[7].x = 308.0f;
            }
            if (winner_win_fms >= 0x47 && winner_win_fms < 0x5B) {
                poly[8].t += 12.75;
                poly[8].y -= 14.4;
            } else if (winner_win_fms >= 0x5B) {
                poly[8].t = 255.0f;
                poly[8].y = 328.0f;
            }
            if (winner_win_fms >= 0x47 && winner_win_fms < 0x5B) {
                poly[9].t += 12.75;
                poly[9].y += 24.45;
            } else if (winner_win_fms >= 0x5B) {
                poly[9].t = 255.0f;
                poly[9].y = 249.0f;
            }
            if (winner_win_fms < 0x1E) {
                winner_win_h += 16.0;
                winner_win_b += 8.5;
            } else {
                winner_win_h = 480.0f;
                winner_win_b = 255.0f;
            }
        }
        if (!IsCancelFlag()) {
            if (winner_win_fms == 0xE8) {
                if (!rannyuuflag) {
                    Comment_Main(0);
                }
            } else if (winner_win_fms >= 0xE9) {
                Comment_Main(1);
            }
            if (!(winner_mode & 2) && winner_win_fms >= 0x1CF && !rannyuuflag) {
                winner_win_fade -= 0.1;
            }
        }
        if (!rannyuuflag) {
            winner_win_fms++;
        }
        if (!IsCancelFlag()) {
            gFadeValSet(winner_win_fade);
        }
        color = gStripGetFadeColor(((int)winner_win_b << 24) | ((int)(winner_win_fade * 143.0) << 16) | ((int)(winner_win_fade * 19.0) << 8) | (int)(winner_win_fade * 20.0));
        polydraw_FBox(0, 480.0 - winner_win_h, 50.0f, 480.0f, 15.0f, color);
        polydraw_FBox(590.0f, 0, 640.0f, winner_win_h, 15.0f, color);
        if (winner_win_fms >= 0x6D && winner_win_fms < 0x78) {
            polydraw_GBox(0, 328.0f, 640.0f, 360.0f, 13.0f, gStripGetFadeColor(0x01000000), gStripGetFadeColor(0x01000000),
                          gStripGetFadeColor(((int)(winner_win_over_t * 255.0) << 24) | 0xFFFFFF), gStripGetFadeColor(((int)(winner_win_over_t * 255.0) << 24) | 0xFFFFFF));
            polydraw_GBox(0, 360.0f, 640.0f, 392.0f, 13.0f, gStripGetFadeColor(((int)(winner_win_over_t * 255.0) << 24) | 0xFFFFFF),
                          gStripGetFadeColor(((int)(winner_win_over_t * 255.0) << 24) | 0xFFFFFF), gStripGetFadeColor(0x01000000), gStripGetFadeColor(0x01000000));
        }
        if (winner_win_fms >= 0x79 && winner_win_fms < 0x88) {
            polydraw_GBox(winner_win_over_x - 32.0f, 328.0f, winner_win_over_x, 392.0f, 13.0f, gStripGetFadeColor(0x01000000), gStripGetFadeColor(0xB2FFFFFF),
                          gStripGetFadeColor(0x01000000), gStripGetFadeColor(0xB2FFFFFF));
            polydraw_GBox(winner_win_over_x, 328.0f, winner_win_over_x + 32.0f, 392.0f, 13.0f, gStripGetFadeColor(0xB2FFFFFF), gStripGetFadeColor(0x01000000),
                          gStripGetFadeColor(0xB2FFFFFF), gStripGetFadeColor(0x01000000));
        }
        for (i = 0; i < 11; i++) {
            if (winsp[i].trnsl >= 0.1) {
                gSpriteDraw_NaomiLibVersion(&winsp[i]);
            }
        }
        for (i = 0; i < 6; i++) {
            color = (int)(winner_win_fade * 255.0);
            color = gStripGetFadeColor(((int)poly[i].t << 24) | (color << 16) | (color << 8) | color);
            polydraw_LBox(poly[i].x - poly[i].w, poly[i].y - poly[i].h, poly[i].x + poly[i].w, poly[i].y + poly[i].h, 23.0f, color);
        }
        for (i = 6; i < 10; i++) {
            color = (int)(winner_win_fade * 255.0);
            color = gStripGetFadeColor(((int)poly[i].t << 24) | (color << 16) | (color << 8) | color);
            polydraw_FLine(poly[i].x - poly[i].w, poly[i].y - poly[i].h, poly[i].x + poly[i].w, poly[i].y + poly[i].h, 23.0f, color);
        }
    } else {
        SubJob++;
    }
    if (rannyuuflag) {
        EnterFight_Main();
    }
}


int Cr_SetContinue(int side);
int Cr_SetContinueTime(int side, int time);

void WinnerMainSkipWinner(void)
{
    static float y1;
    static float y2;
    static float fade;
    static int target;
    static int fms;
    static int off_x;
    int i;
    int color;

    if (SubJobCount == 0) {
        BGM_Free();
        if (g_info.mode & 0x200) {
            SubJob = 4;
            return;
        }
        sound_alloff();
        if (winner_mode & 2) {
            fade = 0.7f;
            BGM_Request(0x19, 1);
            Cr_SetContinue(g_info.WinnerSide == 0);
            Cr_SetContinueTime(g_info.WinnerSide == 0, 0x28);
            y1 = -2.0f;
            y2 = -128.0f;
            fade = 0.7f;
            target = 0;
            fms = 0;
            off_x = 0;
            ko(0);
        }
        SubJobCount++;
    }
    if (winner_mode & 2) {
        if (target == 0 && (Random() & 0x1F) == 0x1F) {
            target = Random() & 3;
        }
        if ((Random() & 0x1F) == 0x1F && off_x == 0) {
            int r = Random();

            off_x = r & 0x1F;
            if (r & 1) {
                off_x = -off_x;
            }
        }
        DispWinnerCredit();
        color = ((int)(fade * 143.0) << 16) | (int)(fade * 20.0) | 0xFF000000 | ((int)(fade * 19.0) << 8);
        polydraw_FBox(off_x, 0, off_x + 50, 480.0f, 15.0f, color);
        polydraw_FBox(off_x + 590, 0, off_x + 640, 480.0f, 15.0f, color);
        gFadeValSet(fade);
        winsp[11].attr = D_5C4118[fms & 3] | 5;
        winsp[11].trnsl = 1.0 - (fade + 0.3);
        switch (target) {
        case 0:
            fade = 0.7f;
            break;
        case 1:
            fade -= 0.013333333333333334;
            if (fade < 0.3) {
                target = 2;
            }
            break;
        case 2:
            fade += 0.02;
            if (fade >= 0.7) {
                target = 0;
            }
            break;
        case 3:
            fade -= 0.03;
            if (fade < 0.4) {
                target = 4;
            }
            break;
        case 4:
            fade += 0.016666666666666666;
            if (fade >= 0.5) {
                target = 5;
            }
            break;
        case 5:
            fade -= 0.015;
            if (fade < 0.2) {
                target = 6;
            }
            break;
        case 6:
            fade += 0.05;
            if (fade >= 0.7) {
                target = 0;
            }
            break;
        }
        polydraw_FBox(0, y1, 640.0f, y1 + 2.0f, 9.0f, 0x66000000);
        y1 += 4.137931034482759;
        if (y1 > 480.0f) {
            y1 = 0;
        }
        polydraw_FBox(0, y2, 640.0f, y2 + 128.0f, 9.0f, 0x66000000);
        y2 += 1.8461538461538463;
        if (y2 > 480.0f) {
            y2 = 0;
        }
        color = (int)(fade * 255.0);
        color = 0xFF000000 | (color << 16) | (color << 8) | color;
        for (i = 0; i < 12; i++) {
            winsp[i].x += off_x;
            gSpriteDraw_NaomiLibVersion(&winsp[i]);
            winsp[i].x -= off_x;
        }
        for (i = 0; i < 6; i++) {
            polydraw_LBox(poly[i].x + off_x - poly[i].w, poly[i].y - poly[i].h,
                          poly[i].x + off_x + poly[i].w, poly[i].y + poly[i].h, 23.0f, color);
        }
        for (i = 6; i < 10; i++) {
            polydraw_FLine(poly[i].x + off_x - poly[i].w, poly[i].y - poly[i].h,
                           poly[i].x + off_x + poly[i].w, poly[i].y + poly[i].h, 23.0f,
                           ((int)poly[i].t << 24) | 0xFFFFFF);
        }
        if (fms & 1) {
            if (off_x > 0) {
                off_x -= 15;
                if (off_x < 0) {
                    off_x = 0;
                }
            } else if (off_x < 0) {
                off_x += 15;
                if (off_x > 0) {
                    off_x = 0;
                }
            }
        }
        ko(1);
        fms++;
    } else {
        SubJob++;
    }
}


void WinnerMainContinue(void)
{
    static float fade;
    static float msk;
    static int fms;
    static int off_x = 0;
    int i;
    int color;

    if (SubJobCount == 0) {
        SubJobCount = 1;
        fade = 0.7f;
        fms = 0;
        msk = 0;
    }
    if (fms < 60 && (winner_mode & 4)) {
        DispWinnerCredit();
        if (fms < 6) {
            fade += 0.05f;
        } else if (fms >= 7 && fms < 26) {
            fade = 1.0f;
        } else if (fms >= 27 && fms < 52) {
            fade -= 0.029411765f;
        } else if (fms >= 53) {
            fade = 0;
        }
        if (fms < 12) {
            msk += 0.083333336f;
        } else if (fms >= 13 && fms < 18) {
            msk -= 0.16666667f;
        } else {
            msk = 0;
        }
        gFadeValSet(fade);
        color = ((int)(msk * 255.0) << 24) | 0xFFFFFF;
        if (fms < 20) {
            polydraw_FBox(0, 0, 640.0f, 480.0f, 3.0f, color);
        }
        if (fms >= 27) {
            winsp[11].attr = D_5C4118[fms & 3] | 0x205;
            winsp[11].trnsl = 1.0 - fade;
        }
        color = 0xFF000000 | ((int)(fade * 143.0) << 16) | ((int)(fade * 19.0) << 8) | (int)(fade * 20.0);
        polydraw_FBox(off_x, 0, off_x + 50, 480.0f, 15.0f, color);
        polydraw_FBox(off_x + 590, 0, off_x + 640, 480.0f, 15.0f, color);
        color = (int)(fade * 255.0);
        color = 0xFF000000 | (color << 16) | (color << 8) | color;
        for (i = 0; i < 11; i++) {
            gSpriteDraw_NaomiLibVersion(&winsp[i]);
        }
        for (i = 0; i < 6; i++) {
            polydraw_LBox((int)(poly[i].x + off_x - poly[i].w), (int)(poly[i].y - poly[i].h),
                          (int)(poly[i].x + off_x + poly[i].w), (int)(poly[i].y + poly[i].h), 23.0f, color);
        }
        for (i = 6; i < 10; i++) {
            polydraw_FLine((int)(poly[i].x + off_x - poly[i].w), (int)(poly[i].y - poly[i].h),
                           (int)(poly[i].x + off_x + poly[i].w), (int)(poly[i].y + poly[i].h), 23.0f,
                           ((int)poly[i].t << 24) | 0xFFFFFF);
        }
        gFadeValSet(0.5f);
        gSpriteDraw_NaomiLibVersion(&winsp[11]);
        fms++;
    } else {
        SubJob++;
    }
}

int func_00113238(int mode);


void func_00164AF8(void)
{
    static int fms;
    static float over_x;
    static float over_t;

    if (SubJobCount == 0) {
        winsp[11].trnsl = 0;
        over_x = 40.0f;
        fms = 0;
        over_t = 0;
        if (winner_mode & 4) {
            sound_alloff();
            BGM_Request(0x1A, 0);
            if (winner_mode & 4) {
                DispGameOver(0);
            }
        }
        SubJobCount++;
    }
    if (SubJobCount == 1) {
        if (winner_mode & 4) {
            if (DispGameOver(1)) {
                SubJobCount++;
            }
        } else {
            SubJobCount = 2;
        }
    }
    if (SubJobCount == 2) {
        if (winner_mode & 4) {
            if ((Cr_IsNameEntry(0) || Cr_IsNameEntry(1)) && !(g_info.mode & 0x200)) {
                DispWinnerCredit();
            } else {
                SubJobCount++;
            }
        } else {
            BootControl |= 2;
            SubJob++;
            return;
        }
    }
    if (SubJobCount == 3) {
        if (!IOPLock()) {
            Credit_Init();
            if (userinfo.autosave) {
                func_00113238(0);
                SubJobCount++;
                return;
            }
            BootControl |= 2;
            SubJob++;
        }
    }
    if (SubJobCount == 4) {
        if (func_00113238(1)) {
            BootControl |= 2;
            SubJob++;
        }
    }
}




void WinnerMainSkip(void)
{
    int winner;

    winner = g_info.Winnerid - 1;
    if (SubJobCount == 0) {
        SubJobCount = 1;
    }
    if (!rannyuuflag) {
        nlObjPutSetFadeColor(1.0f, 1.0f, 1.0f);
        gFadeValSet(1.0f);
        sound_alloff();
        BGM_Free();
        if (g_info.mode & 0x4000) {
            JobChange(0x19);
        } else if (g_info.mode & 0x800) {
            JobChange(0xB);
        } else {
            JobChange(GetWinnerNextMode());
        }
        if (JobMode == 0x28) {
            set_clearflg(winner);
        }
        g_info.BattleControlFlag &= ~4;
    }
}




void WinnerMain(void)
{
    void (*winner_func[6])() = {
        WinnerMainFirst, WinnerMainWin, WinnerMainSkipWinner,
        WinnerMainContinue, WinnerMainGameOver, WinnerMainSkip,
    };
    int last_job;

    do {
        last_job = SubJob;
        winner_func[last_job]();
        if (last_job == SubJob) {
            break;
        }
        SubJobCount = 0;
    } while (1);
    if (JobMode != 0x27) {
        dfree(poly);
        poly = 0;
        dfree(winsp);
        winsp = 0;
    }
    if (rannyuuflag) {
        EnterFight_Main();
    }
}









void func_00164E78(int *timer, int *pos)
{
    int pad;
    int t;


    if ((pad = iodat_chk(0x1000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
            sound_request(0x37);
            *pos = *pos ? *pos - 1 : 1;
        }
        sc.cnt[pad - 1]++;
    } else if ((pad = iodat_chk(0x4000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
            sound_request(0x37);
            *pos = (*pos == 1) ? 0 : *pos + 1;
        }
        sc.cnt[pad - 1]++;
    } else {
        sc.cnt[0] = 0;
        sc.cnt[1] = 0;
    }
    if ((pads[0].trg | pads[1].trg) & 0x10) {
        if (*timer / 120 < 8) {
            if ((t = *timer / 120)) {
                *timer = t * 120;
            } else {
                *timer = 0;
            }
        }
    }
}


void func_00165000(HEADWORK *hw, int pos)
{
    int i;
    int alpha;

    polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 3.0f, 0xD0000000);
    for (i = 0; i < 2; i++) {
        alpha = (pos == i) ? 1 : 160;
        FontDrawDC(hw[i].head, hw[i].xp, hw[i].yp, 2.0f, alpha, 1);
    }
}

int func_001947D8(int no);


void func_001650B0(int timer)
{
    static int bk = 0;
    int num;
    int se;
    char dat[2];

    num = timer / 120;
    if (bk != num) {
        se = num ? num + 0x40 : 0x4A;
        func_001947D8(se);
        bk = num;
    }
    dat[0] = num + '0';
    dat[1] = 0;
    FontDrawDC(dat, 312.0f, 312.0f, 1.0f, 1, 1);
}




void quick_playerclr(void)
{
    EliminateRoundEnd();
    font_entry(-1, 0);
    SetScreenFlag(0);
    PlayerParaInit();
    SetActNo(PlayerWork, 0);
    SetActNo(PlayerWork + 1, 0);
    SetAnimeNo(PlayerWork, 0);
    SetAnimeNo(PlayerWork + 1, 0);
    BattleTimeInit();
    nlSetBackColor(0, 0, 0);
    if (StageNo == 5) {
        MaySHPOrg();
        MayBack(1);
    }
}

int quick_restart(int sw)
{
    static int mode;

    if (sw == 0) {
        mode = 0;
        objkill_winmrk();
        EliminateRoundEnd();
        ResultInitial();
        font_entry(-1, 0);
        quick_playerclr();
        SetFadeEndCommand(1);
        SetFadeSpeed(15);
        Rq_Fadein();
        return 0;
    }
    if (mode == 0) {
        if (IsFadeEnd()) {
            mode = 1;
        }
        return 0;
    }
    if (mode++ > 10) {
        ggx_bgm_start();
        BeforeFight_Init();
        return 1;
    }
    return 0;
}

void ClearScore(int side);
int ReqSongOut(int outtime, int mval);
void init_davol(void);
void func_00161F78(void);


int func_00165290(int frm)
{
    static int alpha;
    static int timer;
    static int pos;
    static int mode;
    HEADWORK hw[2] = { { "CONTINUE", 192.0f, 192.0f, 0, 0, 0 }, { "CHARACTER SELECT", 192.0f, 232.0f, 0, 0, 0 } };

    if (frm == 0) {
        g_info.BattleControlFlag |= 4;
        FadeInit();
        func_00161F78();
        pos = 0;
        timer = 1200;
        alpha = 0;
        return 1;
    }
    if (frm == 1) {
        int a = alpha;

        if (a < 0xD0) {
            a += 0x20;
            alpha = a;
            polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 3.0f, a << 24);
            if (alpha == 0xD0) {
                frm = 2;
                return frm;
            }
            return 1;
        }
        frm = 2;
        return frm;
    }
    if (frm == 2) {
        if (timer && !((pads[0].trg | pads[1].trg) & 0x840)) {
            timer--;
            func_00164E78(&timer, &pos);
            func_00165000(hw, pos);
            func_001650B0(timer);
            return 2;
        }
        if ((pads[0].trg | pads[1].trg) & 0x840) {
            func_001947D8(0x39);
            switch (pos) {
            case 0:
                mode = 0x1B;
                break;
            case 1:
                mode = 0xC;
                winner_mode |= 0x10;
                break;
            }
            g_info.BattleTime[0] = g_info.BstartTime[0];
            g_info.BattleTime[1] = g_info.BstartTime[1];
            sc.conti[g_info.WinnerSide == 0] = UseCharacter1p2p[g_info.WinnerSide ? 0 : 1];
            g_info.PlayerSelected[g_info.WinnerSide == 0] = 0;
            winner_mode |= 1;
            winner_mode &= ~6;
            PlayerWork[0].Localid = 0;
            PlayerWork[1].Localid = 0;
        } else {
            winner_mode &= ~2;
            timer = 0;
            Cr_SetGameOver(g_info.WinnerSide == 0);
            if (g_info.mode & 0x800) {
                mode = 0xB;
            } else {
                mode = 0x1F;
            }
        }
        SetFadeSpeed(0x14);
        SetFadeEndCommand(0);
        Rq_Fadeout();
        return frm + 1;
    }
    if (frm == 3) {
        sound_alloff();
        ReqSongOut(10, 0);
        return 4;
    }
    if (frm == 4) {
        if (!IsFadeEnd()) {
            func_00165000(hw, pos);
            func_001650B0(timer);
            return 4;
        }
        sound_alloff();
        init_davol();
        if (mode != 0x1D) {
            return 6;
        }
        quick_restart(0);
        return 5;
    }
    if (frm == 5) {
        if (!quick_restart(1)) {
            return 5;
        }
        BGM_Free();
        ClearScore(0);
        ClearScore(1);
        JobChange(mode);
        g_info.BattleControlFlag &= ~4;
    } else if (frm == 6) {
        BGM_Free();
        ClearScore(0);
        ClearScore(1);
        JobChange(mode);
        g_info.BattleControlFlag &= ~4;
        return 0;
    }
    return 0;
}




void QuickGameOverInit(void)
{
    g_main_End();
    heap_clear();
    WinnerFile = FileReadSelect(0x3f, 2);
    JobNext();
}


void QuickGameOverLoad(void)
{
    if (FileReadCheck()) {
        PXF_SpriteClear();
        FadeInit();
        SubJob = 0;
        SubJobCount = 0;
        JobNext();
    }
}



void QuickGameOverMain(void)
{
    int *p;
    int i;

    if (SubJob == 0) {
        p = (int *)WinnerFile;
        LinkAddrTb((u_int *)p);
        p = (int *)*p;
        LinkAddrTb((u_int *)p);
        for (i = 0; p[i]; i++) {
            if (i == 0x22 || i == 0x1E || i == 0x1F || i == 0x20 || i == 0x21 || (u_int)i < 0x23) {
                IST_Load(p + i, i, -1);
            }
        }
        winsp = dmalloc(0x410, "winsp");
        SprArgSet(0xD, sprarg, winsp);
        CockpitLoadInit();
        CreditFontLoad(0x14);
        SubJobCount = 0;
        SubJob = 1;
    }
    if (SubJob == 1) {
        WinnerMainGameOver();
    } else if (SubJob == 2) {
        nlObjPutSetFadeColor(1.0f, 1.0f, 1.0f);
        gFadeValSet(0);
        sound_alloff();
        BGM_Free();
        JobChange(3);
        g_info.BattleControlFlag &= ~4;
        dfree(winsp);
        winsp = 0;
    }
    if (rannyuuflag) {
        EnterFight_Main();
    }
}