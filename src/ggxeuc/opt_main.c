/* Guilty Gear X US (SLUS-202.87): 1 functions ported from the X Plus
   decomp (ggxeuc/opt_main.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/opt_main.h"

extern char *Player2File;
void dfree(void *mem);
char *FileReadSelect(int fid, int chan);
void LinkAddrTb(u_int *addr);
void PXF_SysSpriteClear(void);
int PXF_GetSysSprite(void);
int IST_Load(int *texadr, int sprid, int palid);
extern char *OpeningFile;
extern int SubJob;
extern int SubJobCount;
void PXF_SpriteClear(void);
void nlSetBackColor(int c1, int c2, int c3);
int FileReadCheck(void);
void CreditFontLoad(int id);
void dcfont_init(void);
void BGM_Request(int no, int loop);
void JobNext(void);
void FontDrawDC(char *str, float x, float y, float z, int alpha, int id);
void FontDraw(char *str, int x, int y, float z, int alfa);
extern char D_5C23E8[];
extern PAD pads[2];
extern SYS_COND sc;
extern USER_SETTING userinfo;
void EasySpriteDrawEX(int id, int x, int y, float z, int flg, float trnsl);
unsigned int gStripGetFadeColor(unsigned int color);
void polydraw_FBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
int sound_request(int no);
int iodat_chk(int stat);
void BGM_Free(void);
void Opt_GameSetting(int sw);
void Opt_SoundTest(int sw);
void Opt_MemAccess(int sw);
void Opt_UserRanking(int sw);
void Opt_DispAdjust(int sw);
void Opt_KeyConfig(int sw);
void Opt_Password(int sw);
void FadeInit(void);
void SetFadeSpeed(int frame);
void Rq_Fadeout(void);
int IsFadeEnd(void);
int MemAccess_AutoSave(int mode);
void PXF_Free(int id, int pal);
void nlFreeCommonTextureAll(void);
void nlFreePaletteAll(void);
void JobChange(int job);



void emess_init(void)
{
    int i;
    int no;
    u_int *p;

    if (Player2File) dfree(Player2File);
    p = (u_int *)(Player2File = FileReadSelect(0x3f, 1));
    LinkAddrTb(p);
    PXF_SysSpriteClear();
    for (i = 0; p[i] != 0 && i < 3; i++) {
        no = PXF_GetSysSprite();
        opt.emess[i] = no;
        IST_Load((int *)&p[i], no, -1);
    }
}

void func_0015C838(int pos, HEADWORK *hw)
{
    int n;

    FontDrawDC("OPTIONS MODE", 32.0f, 40.0f, 4.0f, 1, 0);
    for (n = 0; n < 7; n++) {
        FontDrawDC(hw[n].head, hw[n].xp, hw[n].yp, 4.0f, (n == pos) ? 1 : 0xa0, 1);
    }
}






void set_char2num(int num, char *block, int id)
{
    int i;
    int pos;
    int off;
    int c;
    int basenum[10] = { 1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };

    if (num < 0) {
        num = -num;
        if (id == 1) *block = '-';
    } else if (id == 1) {
        *block = '+';
    }
    off = (id == 1);
    pos = 0;
    for (i = 0; i < 10; i++) {
        if (num >= basenum[i]) { c = num / basenum[i]; block[pos + off] = c + '0'; num = num % basenum[i]; pos++; } else if (pos || i == 9 || (id >= 2 && i + id > 9)) { block[pos + off] = '0'; pos++; }
    }
    block[pos + off] = 0;
}



void kousi_disp(void) {
}



void OptionMenuInit(void)
{
    u_int *p;

    p = (u_int *)OpeningFile;
    PXF_SpriteClear();
    nlSetBackColor(0, 0, 0);
    OpeningFile = FileReadSelect(0x2a, 0);
    if (FileReadCheck()) {
        PXF_SpriteClear();
        CreditFontLoad(20);
        LinkAddrTb(p);
        IST_Load((int *)(p + 139), 0x30, -1);
        dcfont_init();
        BGM_Request(0x15, 1);
        SubJob = 0;
        SubJobCount = 0;
        JobNext();
    }
}




void OptMain_CRTdisp(int pos, HEADWORK *hw);







void OptionMenuMain(void)
{
    static int pos;
    int pad;
    HEADWORK hw[7] = {
        { "GAME OPTIONS", 64.0f, 112.0f, 0, 0, 0 },
        { "SOUND TEST", 64.0f, 160.0f, 0, 0, 0 },
        { "MEMORY CARD", 64.0f, 208.0f, 0, 0, 0 },
        { "RANKING", 64.0f, 256.0f, 0, 0, 0 },
        { "ADJUST SETTINGS", 64.0f, 304.0f, 0, 0, 0 },
        { "KEY CONFIG", 64.0f, 352.0f, 0, 0, 0 },
        { "EXIT", 64.0f, 400.0f, 0, 0, 0 },
    };

    if (SubJob == 0) {
        opt.thdflg = 0; opt.subthd = 0; pos = 0; sc.cnt[0] = 0; sc.cnt[1] = 0;
        SubJob = 1;
    }
    if (SubJob == 1) {
        EasySpriteDrawEX(0x30, 0, 0x1e0, 20.0f, 0x20d, 1.0f);
        polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 16.0f, gStripGetFadeColor(0xc0000010));
        if (opt.thdflg == 0) {
            OptMain_CRTdisp(pos, hw);
            if (!(opt.thdflg & 0x80)) {
                if ((pad = iodat_chk(0x1000))) {
                    if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                        sound_request(0x37);
                        pos = pos ? pos - 1 : 6;
                    }
                    sc.cnt[pad - 1]++;
                } else if ((pad = iodat_chk(0x4000))) {
                    if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                        sound_request(0x37);
                        pos = (pos == 6) ? 0 : pos + 1;
                    }
                    sc.cnt[pad - 1]++;
                } else {
                    sc.cnt[0] = 0;
                    sc.cnt[1] = 0;
                }
                if ((pads[0].trg | pads[1].trg) & 0x810) {
                    sound_request(0x3b);
                    opt.thdflg = -1;
                    SubJob++;
                    return;
                }
                if ((pads[0].trg | pads[1].trg) & 0x40) {
                    sound_request(0x39);
                    switch (pos) {
                    case 0:
                        if (!(opt.thdflg & 1)) {
                            opt.thdflg |= 1;
                            Opt_GameSetting(0);
                        }
                        break;
                    case 1:
                        if (!(opt.thdflg & 2)) {
                            opt.thdflg |= 2;
                            BGM_Free();
                            Opt_SoundTest(0);
                        }
                        break;
                    case 2:
                        if (!(opt.thdflg & 4)) {
                            opt.thdflg |= 4;
                            Opt_MemAccess(0);
                        }
                        break;
                    case 3:
                        if (!(opt.thdflg & 8)) {
                            opt.thdflg |= 8;
                            Opt_UserRanking(0);
                        }
                        break;
                    case 4:
                        if (!(opt.thdflg & 0x10)) {
                            opt.thdflg |= 0x10;
                            Opt_DispAdjust(0);
                        }
                        break;
                    case 5:
                        if (!(opt.thdflg & 0x20)) {
                            opt.thdflg |= 0x20;
                            Opt_KeyConfig(0);
                        }
                        break;
                    case 6:
                        opt.thdflg = -1;
                        SubJob++;
                        return;
                    }
                }
            }
        } else {
            if (opt.thdflg & 1) Opt_GameSetting(1);
            if (opt.thdflg & 2) Opt_SoundTest(1);
            if (opt.thdflg & 4) Opt_MemAccess(1);
            if (opt.thdflg & 8) Opt_UserRanking(1);
            if (opt.thdflg & 0x10) Opt_DispAdjust(1);
            if (opt.thdflg & 0x20) Opt_KeyConfig(1);
        }
    }
    if (SubJob == 2) {
        EasySpriteDrawEX(0x30, 0, 0x1e0, 20.0f, 0x20d, 1.0f);
        polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 16.0f, gStripGetFadeColor(0xc0000010));
        FadeInit();
        SetFadeSpeed(15);
        Rq_Fadeout();
        SubJob++;
    }
    if (SubJob == 3) {
        if (!IsFadeEnd()) {
            EasySpriteDrawEX(0x30, 0, 0x1e0, 20.0f, 0x20d, 1.0f);
            polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 16.0f, gStripGetFadeColor(0xc0000010));
            OptMain_CRTdisp(pos, hw);
        } else if (userinfo.autosave) {
            MemAccess_AutoSave(0);
            SubJob++;
        } else {
            SubJob += 2;
        }
    }
    if (SubJob == 4) {
        if (MemAccess_AutoSave(1)) SubJob++;
    }
    if (SubJob == 5) {
        PXF_Free(0x10, 0);
        BGM_Free();
        PXF_SpriteClear();
        nlFreeCommonTextureAll();
        nlFreePaletteAll();
        JobChange(3);
    }
}