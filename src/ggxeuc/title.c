/* Guilty Gear X US (SLUS-202.87): 7 functions ported from the X Plus
   decomp (ggxeuc/title.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/title.h"
#define TITLE_SPRITES() \
    EasySpriteDrawEX(0x30, 0, 0x1e0, 19.0f, 0x20d, 1.0f); \
    EasySpriteDrawEX(0x31, 0, 0x1e0, 20.0f, 0x20d, 1.0f); \
    EasySpriteDrawEX(0x32, 0x140, 0xf0, 18.0f, 0x20a, 1.0f); \
    EasySpriteDrawEX(0x33, 0x140, 0xf0, 17.0f, 0x20a, 1.0f); \
    EasySpriteDrawEX(0x34, 0x140, 0x1c8, 10.0f, 0x20e, 1.0f)

void EasySpriteDrawEX(int id, int x, int y, float z, int flg, float trnsl);
extern char *OpeningFile;
void dfree(void *mem);
extern SYS_COND sc;
void Init_Game_Info(void);
void heap_clear(void);
char *FileReadSelect(int fid, int chan);
void kakusi_packload(void);
void JobNext(void);
void FontDrawDC(char *str, float x, float y, float z, int alpha, int id);
extern GAME_INFOMATION g_info;
extern int SubJob;
extern int SubJobCount;
static int Loop = 0;
static int Flag = 0;
int FileReadCheck(void);
void pack_allfree(void);
void PXF_SpriteClear(void);
static int Ranking_ScrollOffset;
int IST_Load(int *texadr, int sprid, int palid);
void Rq_Fadein(void);
void JobChange(int job);
int GetLimitStatus(void);
static int emess = 0;
static int IDTransTb[19] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 11, 16, 17, 18 };
static int diagramnum;
extern char D_55C62A[];
char *Diagram_GetAccessPointer(char *p, int a, int b);
int Diagram_GetWinnerCount(char *src);
int Diagram_GetScore(int cnt);
extern char *ChName[];
void FontDraw(char *str, int x, int y, float z, int alfa);
char *G_sprintf_arg1(char *str, int arg1);
extern SCOREDATA_BYCHARACTER D_55CFC0[];
unsigned int HighScore_GetScore(SCOREDATA_BYCHARACTER *p, int idno, int rank);
char *HighScore_GetName(SCOREDATA_BYCHARACTER *p, int idno, int rank);
int strlen(char *s);
extern PAD pads[2];
int IsFadeEnd(void);
void Rq_Fadeout(void);
void PXF_Free(int id, int pal);
extern USER_SETTING userinfo;
int sound_request_main(int no, int ch);
int SetLimitStatus(void);
extern int BootControl;
extern u_int TotalCounter;
extern CHARANGE_CTRL *gl_info;
void sound_alloff(void);
void nlFreeCommonTextureAll(void);
void nlFreePaletteAll(void);
void RandomInit(int seed);
void VoiceFree(void);
extern TRAINING_SETTING *tr_info;
int iodat_chk(int stat);
int sound_request(int no);
void Tranning_init(void);
void Tranning_init_prac(void);
void clr_preaction(void);
void gallery_init(void);
void init_storymode(int side);
void init_survival(int ply);
void polydraw_FBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
static const short idtb[] = { 1, 2, 3 };
extern unsigned int DebugFlag;
int Cr_IsStartWait(int side);
int Cr_Request_Start(int side);
int Credit_Init(void);
void CockpitLoadInit(void);
void CreditFontLoad(int id);
void FadeInit(void);
void InitDemoBattle(void);
void LinkAddrTb(u_int *addr);
void SetFadeSpeed(int frame);
void VsCpuInit(void);
void dcfont_init(void);
void DispCreditMsg(void);







void func_00108EA8(void) {
    int a;
    int b;
    int sctb[17];
    int total;
    int maxid;
    int max;
    for (a = 1; a < 17; a++) {
        total = 0;
        sctb[a] = 0;
        if (GetLimitStatus() == 0) {
            if (a == 11) continue;
            if (a == 16) continue;
        } else if (GetLimitStatus() == 1) {
            if (a == 16) continue;
        }
        for (b = 1; b < 17; b++) {
            if (GetLimitStatus() == 0) {
                if (a == 11) continue;
                if (a == 16) continue;
            } else if (GetLimitStatus() == 1) {
                if (a == 16) continue;
            }
            max = Diagram_GetScore(Diagram_GetWinnerCount(Diagram_GetAccessPointer(D_55C62A, a, b)));
            if (a >= b) max = 10 - max;
            if (a != b) total += max;
        }
        sctb[a] = total;
    }
    diagramnum = 0;
    for (a = 1; a < 17; a++) {
        maxid = 0;
        max = 0;
        for (b = 1; b < 17; b++) {
            if (max < sctb[b]) {
                max = sctb[b];
                maxid = b;
            }
        }
        if (max) {
            IDTransTb[a] = maxid;
            sctb[maxid] = 0;
            diagramnum++;
        } else {
            IDTransTb[a] = 0;
        }
    }
}


void DispDiagram(unsigned char alfa, int x, int y) {
    int a;
    int b;
    int cnt;
    int ida;
    int idb;
    int total;
    for (a = 1; a <= diagramnum; a++) {
        total = 0;
        ida = IDTransTb[a];
        if (ida == 0) continue;
        FontDraw(ChName[ida - 1], x, y + a * 16, 4.0f, alfa);
        for (b = 1; b <= diagramnum; b++) {
            idb = IDTransTb[b];
            cnt = Diagram_GetScore(Diagram_GetWinnerCount(Diagram_GetAccessPointer(D_55C62A, ida, idb)));
            if (ida >= idb) cnt = 10 - cnt;
            if (ida == idb) {
                FontDraw("-", b * 24 + x + 96, y + a * 16, 4.0f, alfa);
            } else {
                FontDraw(G_sprintf_arg1("%d", cnt), b * 24 + x + 96, y + a * 16, 4.0f, alfa);
                total += cnt;
            }
        }
        FontDraw(G_sprintf_arg1("%3d", total), b * 24 + x + 96, y + a * 16, 4.0f, alfa);
    }
}

void RankBGDrawSub(void) {
    EasySpriteDrawEX(0x30, 0, 480, 20.0f, 0x20d, 1.0f);
}









void RankDrawSub(int idno, int x, int y) {
    int rank;
    int offy;
    static char *ranknametb[5] = { "1ST", "2ND", "3RD", "4TH", "5TH" };
    int IDTransTb[17] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 11, 16 };
    idno = IDTransTb[idno];
    if (idno == 0) {
        FontDraw("TOTAL RANKING", x + 0x42, y, 4.0f, 255);
    } else {
        FontDraw(ChName[idno - 1], x + 144 - strlen(ChName[idno - 1]) * 6, y, 4.0f, 255);
    }
    for (offy = 32, rank = 0; rank < 5; rank++, offy += 24) {
        FontDraw(ranknametb[rank], x + 16, y + offy, 4.0f, 255);
        FontDraw(G_sprintf_arg1("%9d ", HighScore_GetScore(D_55CFC0, idno, rank)), x + 96, y + offy, 4.0f, 255);
        FontDraw(HighScore_GetName(D_55CFC0, idno, rank), x + 216, y + offy, 4.0f, 255);
    }
}




void DispDiagram(unsigned char alfa, int x, int y);

void RankDrawSub(int idno, int x, int y);







void func_00109568(int type, int basey) {
    int y;
    int i;
    int num;
    if (type == 0) {
        y = 120 - basey;
        if (y < -320) return;
        if (y > 480) return;
        RankDrawSub(0, 160, y);
    } else if (type == 1) {
        if (GetLimitStatus() == 0) {
            num = 14;
        } else if (GetLimitStatus() == 1) {
            num = 15;
        } else {
            num = 16;
        }
        for (i = 1; i <= num; i++) {
            y = 512 - basey + (i - 1) / 2 * 240;
            if (y >= -240) {
                if (y <= 480) RankDrawSub(i, (i & 1) ? 20 : 340, y);
            }
        }
    } else if (type == 2) {
        y = 0x9a0 - basey;
        if (y < -320) return;
        if (y > 480) return;
        DispDiagram(255, 32, y);
    }
}





void RankingInit(void) {
    Ranking_ScrollOffset = 0;
    IST_Load((int *)(OpeningFile + 0x22c), 0x30, -1);
    Rq_Fadein();
    DiagramMakeSortTable();
    RankBGDrawSub();
    RankDraw(0, Ranking_ScrollOffset);
    RankDraw(1, Ranking_ScrollOffset);
    RankDraw(2, Ranking_ScrollOffset);
    SubJob = 0;
    SubJobCount = 0;
    JobChange(10);
}

int func_00109720(void) {
    if (SubJob < 5) {
        if (SubJobCount >= 180) {
            if (SubJobCount < 240) {
                Ranking_ScrollOffset += 8;
            } else {
                SubJob++;
                SubJobCount = 0;
            }
        }
    } else if (SubJobCount >= 180) {
        return 1;
    }
    return 0;
}



void RankingMain(void) {
    static int endflag = 0;
    if (SubJob == 0 && SubJobCount == 0) endflag = 0;
    if (endflag == 0) {
        if (RankingScroll()) endflag = 1;
        if ((pads[0].trg | pads[1].trg) & 0x800) endflag = 1;
        if (endflag) Rq_Fadeout();
    } else if (IsFadeEnd()) {
        PXF_Free(0x30, -1);
        JobChange(6);
        return;
    }
    SubJobCount++;
    RankBGDrawSub();
    RankDraw(0, Ranking_ScrollOffset);
    RankDraw(1, Ranking_ScrollOffset);
    RankDraw(2, Ranking_ScrollOffset);
}

static int vschk(int pos) {
    if (pos == 1 && (!sc.connect[0] || !sc.connect[1])) return 1;
    return 0;
}



static void select_disp(int pos, WORKTYPE *wt) {
    int i;
    for (i = 0; i < 6; i++, wt++) {
        FontDrawDC(wt->head, wt->xp, wt->yp, 4.0f, i == pos ? 1 : 0xa0, 2);
    }
}


int func_001946B0(int no, int ch);
int func_0012E4E0(void);






static void func_00109920(int flg) {
    int command[2][16] = {
        { 0x1000, 0x1000, 0x4000, 0x8000, 0x800 },
        { 0x4000, 0x2000, 0x2000, 0x1000, 0x800 },
    };
    static int pos[2] = { 0, 0 };
    int mode[2] = { 1, 2 };
    int i;
    int key;
    if (flg) {
        pos[1] = 0;
        pos[0] = 0;
        return;
    }
    key = pads[0].trg | pads[1].trg;
    if (key == 0) return;
    for (i = 0; i < 2; i++) {
        if (command[i][pos[i]] & key) {
            pos[i]++;
            if (key == 0x800) {
                if ((sc.kakusi | userinfo.phase) >= 2) {
                    if (sc.faust) {
                        func_001946B0(0x1f, 2);
                    } else if (i) {
                        func_001946B0(0x13, 4);
                    } else {
                        func_001946B0(1, 2);
                    }
                    return;
                }
                if (i > (sc.kakusi | userinfo.phase) - 1) {
                    pos[1] = 0;
                    pos[0] = 0;
                    sc.kakusi = mode[i];
                    func_0012E4E0();
                    if (i) {
                        func_001946B0(0xc, 4);
                    } else {
                        func_001946B0(0xc, 2);
                    }
                    return;
                }
            }
        } else {
            pos[i] = 0;
        }
    }
}





void TitleInit(void) {
    sc.boot = 1;
    Init_Game_Info();
    heap_clear();
    OpeningFile = FileReadSelect(0x2a, 0);
    kakusi_packload();
    JobNext();
}


void TitleLoad(void) {
    GAME_INFOMATION *g = &g_info;
    if (FileReadCheck()) {
        pack_allfree();
        g->Demo = 0;
        Flag = 0;
        Loop = 0;
        PXF_SpriteClear();
        SubJob = 0;
        SubJobCount = 0;
        JobNext();
    }
}

void TitleClose(void) {
    dfree(OpeningFile);
    OpeningFile = 0;
}

void TitleClose(void);

void chk_kakusicmd(int flg);


int func_00109BE8(int type) {
    static int pos;
    static int alpha;
    static int start;
    int n;
    int pad;
    WORKTYPE wt[6] = {
        { "ARCADE", 0x108, 0xf4 },
        { "V S", 0x12c, 0x110 },
        { "TRAINING", 0x100, 0x12c },
        { "SURVIVAL", 0xfe, 0x148 },
        { "RECORDING", 0xf6, 0x164 },
        { "OPTIONS", 0x108, 0x180 },
    };
    if (type == 0) {
        chk_kakusicmd(1);
        type = 1;
        start = 0;
        pos = 0;
        alpha = 0;
        sc.cnt[0] = 0;
        sc.cnt[1] = 0;
        userinfo.gguse[0] = 0;
        userinfo.gguse[1] = 0;
    }
    if (type == 1) {
        if (start < 2 && Loop <= 1200) {
            if (start) {
                Loop = 0;
                sc.boot = 0;
                select_disp(pos, wt);
                if (alpha < 0xc0) alpha += 0x18;
                polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 8.0f, alpha << 24);
                if ((pad = iodat_chk(0x1000))) {
                    if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                        sound_request(0x37);
                        Loop = 0;
                        pos = pos ? pos - 1 : 5;
                        if (vschk(pos)) pos--;
                    }
                    sc.cnt[pad - 1]++;
                } else if ((pad = iodat_chk(0x4000))) {
                    if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                        sound_request(0x37);
                        Loop = 0;
                        pos = (pos == 5) ? 0 : pos + 1;
                        if (vschk(pos)) pos++;
                    }
                    sc.cnt[pad - 1]++;
                } else {
                    if (vschk(pos)) pos--;
                    sc.cnt[0] = 0;
                    sc.cnt[1] = 0;
                }
                sc.reset_possib = 0;
                for (n = 0; n < 2; n++) {
                    if (pads[n].trg & 0x840) {
                        sound_request(0x39);
                        Loop = 0;
                        g_info.mode = n + 1;
                        g_info.MedalMode[1] = 0;
                        g_info.MedalMode[0] = 0;
                        switch (pos) {
                        case 1:
                            g_info.mode = 0x803;
                            g_info.PlayerSelected[0] = 0;
                            g_info.PlayerSelected[1] = 0;
                            g_info.wincnt[0] = 0;
                            g_info.wincnt[1] = 0;
                            break;
                        case 2:
                            g_info.mode |= 0x100;
                            Tranning_init();
                            if (userinfo.medal) g_info.MedalMode[n] = 1;
                            tr_info->ply = n;
                            break;
                        case 3:
                            g_info.mode |= 0x200;
                            init_survival(n);
                            break;
                        case 4:
                            sc.replay_first = 0;
                            g_info.mode |= 0x4000;
                            clr_preaction();
                            break;
                        case 5:
                            g_info.mode = 0x1000;
                            break;
                        case 0:
                            if (userinfo.medal) g_info.MedalMode[n] = 1;
                            sc.loose = 0;
                            break;
                        }
                        start = 2;
                    }
                }
                if ((pads[0].trg | pads[1].trg) & 0x10) {
                    if (start != 2) {
                        sound_request(0x3b);
                        start = 0;
                        chk_kakusicmd(1);
                    }
                }
            } else {
                sc.boot = type;
                FontDrawDC("PRESS START BUTTON", 152.0f, 376.0f, 4.0f, 1, 2);
                chk_kakusicmd(0);
                if ((pads[0].trg | pads[1].trg) & 0x800) {
                    sound_request(0x39);
                    start = type;
                    Loop = 0;
                }
            }
        } else {
            if (start || (g_info.mode & 0x400)) {
                sc.boot = 0;
            } else {
                sc.boot = 1;
            }
            type = 2;
        }
    }
    if (type == 2) {
        if (start) {
            select_disp(pos, wt);
            if (alpha < 0xc0) alpha += 0x18;
            polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 8.0f, alpha << 24);
        } else {
            FontDrawDC("PRESS START BUTTON", 152.0f, 376.0f, 4.0f, 1, 2);
        }
    }
    return type;
}



void func_0010A1B0(void) {
    GAME_INFOMATION *g = &g_info;
    sound_alloff();
    nlFreeCommonTextureAll();
    nlFreePaletteAll();
    if (Flag == 1) {
        TitleClose();
        JobChange(12);
        g->ModeFlag = 1;
    } else if (Flag == 2) {
        if (BootControl & 8) {
            BootControl &= ~8;
            JobChange(4);
        } else {
            TitleClose();
            BootControl |= 8;
            JobChange(0x15);
            g->ModeFlag = 1;
            g->Demo = 1;
        }
    } else if (Flag == 3) {
        TitleClose();
        JobChange(12);
        g->ModeFlag = 1;
    } else if (Flag == 4) {
        TitleClose();
        JobChange(12);
        g->ModeFlag = 1;
    } else if (Flag == 5) {
        TitleClose();
        JobChange(0x18);
        g->ModeFlag = 1;
    } else if (Flag == 6) {
        JobChange(0x2c);
        g->ModeFlag = 0;
    } else if (Flag == 7) {
        TitleClose();
        JobChange(0x19);
        g->ModeFlag = 0;
    } else {
        JobChange(6);
        g->ModeFlag = 0;
    }
    VoiceFree();
}


void KariEndInit(void) {
}

void RankingInit(void);

int ModeSelect(int type);

void SkipTitle(void);


void TitleMain(void) {
    int *p;
    static int side;
    static int cnt;
    static int selectmode;
    p = (int *)OpeningFile;
    if (SubJob == 0) {
        selectmode = 0;
        LinkAddrTb((u_int *)p);
        IST_Load(p, 0x30, -1);
        IST_Load(p + 135, 0x31, -1);
        IST_Load(p + 136, 0x32, -1);
        IST_Load(p + 138, 0x33, -1);
        IST_Load(p + 137, 0x34, -1);
        dcfont_init();
        emess = 0;
        sound_request(0);
        Credit_Init();
        CockpitLoadInit();
        CreditFontLoad(20);
        VsCpuInit();
        FadeInit();
        SetFadeSpeed(15);
        if (BootControl & 4) {
            BootControl &= ~4;
            return;
        }
        if (BootControl & 2) {
            BootControl &= ~2;
            RankingInit();
            return;
        }
        if (!(BootControl & 1)) {
            BootControl |= 9;
            Flag = 2;
            SkipTitle();
            return;
        }
        Rq_Fadein();
        Loop = 0;
        SubJob++;
    }
    if (SubJob == 1) {
        selectmode = ModeSelect(selectmode);
        TITLE_SPRITES();
        for (side = 0; side < 2; side++) {
            if (!IsFadeEnd()) continue;
            if (Loop++ > 1200) Flag = 2;
            if ((((g_info.mode >> side) & 1) && Cr_IsStartWait(side)) || (g_info.mode & 0x1000)) {
                if (!(g_info.mode & 0x4000)) Cr_Request_Start(side);
                if (g_info.mode < 4 || (g_info.mode & 0x800)) {
                    Flag = 1;
                } else if (g_info.mode & 0x100) {
                    Flag = 3;
                } else if (g_info.mode & 0x200) {
                    Flag = 4;
                } else if (g_info.mode & 0x400) {
                    Flag = 5;
                } else if (g_info.mode & 0x1000) {
                    Flag = 6;
                } else if (g_info.mode & 0x4000) {
                    Flag = 7;
                }
            }
        }
        if (Flag) {
            SubJobCount = 0;
            SubJob++;
            if (Flag == 2) {
                int i;
                for (i = 0; i < 2; i++) {
                    if (pads[i].data & 4) {
                        if (pads[i].data & 8) {
                            g_info.mode = 0x400;
                            Flag = 5;
                            break;
                        }
                    }
                }
            }
            if (Flag == 2 && !(BootControl & 8)) InitDemoBattle();
            cnt = 20;
            return;
        }
        DispCreditMsg();
    } else if (SubJob == 2) {
        selectmode = ModeSelect(selectmode);
        cnt--;
        TITLE_SPRITES();
        if (cnt == 0) {
            Rq_Fadeout();
            SubJob++;
        }
    } else if (SubJob == 3) {
        if (!IsFadeEnd()) {
            selectmode = ModeSelect(selectmode);
            TITLE_SPRITES();
        } else {
            SubJob++;
        }
    } else if (SubJob == 4) {
        DebugFlag = 0;
        SkipTitle();
    }
}