/* Guilty Gear X US (SLUS-202.87): declarations from the X Plus decomp
   (ggxeuc/opt_vm.c); each function byte-checked by the build gate. */
#include "generated/opt_vm.h"

extern USER_SETTING userinfo;
static VM_INFO vinfo;
void FontDraw(char *str, int x, int y, float z, int alfa);
extern PAD pads[2];
extern SYS_COND sc;
int strlen(char *s);
int sound_request(int no);
int iodat_chk(int stat);
int MemoryCheck(void);
unsigned int gStripGetFadeColor(unsigned int color);
void slot_display(int slot);
void vm_error(int no, int flg, int slot);
void polydraw_FBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
int vm_get_freeblock(int port);
void MemoryDir(int slot, char *fname, int *p);
void MemoryClose(void);
void MemorySave(int slot);
void MemoryFormatAndSave(int slot);
void debugf(char *format, ...);
void MemoryOpen(void);
int SetLimitStatus(void);
extern char D_5C2BB8[];
/* RODATA_EXTERN 0x5c2bb8: "MEMORY CARD" */
extern char D_5C2BC8[];
/* RODATA_EXTERN 0x5c2bc8: "MEMORY CARD SLOT 1 ONLY" */
extern char D_5CBB60[];
/* RODATA_EXTERN 0x5cbb60: "(PS2)" (.sdata) */
void FontDrawDC(char *str, float x, float y, float z, int alpha, int id);
extern OPTINFO opt;
typedef struct { PARTS p[3]; } PT3;
extern PT3 D_5C2B88;
/* RODATA_EXTERN 0x5c2b88: PARTS pt[3] = { {"OFF",288,10,40}, {"ON",352,10,32}, {0,-16,64,72} } */
void MemorySetSystemDir(char *data, int size);

int MemAccess_Save(int sw, int slot);

int MemAccess_Load(int sw, int slot);

void func_0012E4E0(void);

void Opt_MemAccess(int sw)
{
    static int ret;
    static int subthd;
    int pad;
    static HEADWORK hw[8] = {
        { "DATA SAVE", 64.0f, 112.0f, 0, 0, 0 },
        { "DATA LOAD", 352.0f, 112.0f, 0, 0, 0 },
        { "AUTO SAVE", 64.0f, 352.0f, 0, 2, 0 },
        { "EXIT", 64.0f, 400.0f, 0, 0, 0 },
        { "MEMORY CARD SLOT1", 64.0f, 160.0f, 0, 1, 0 },
        { "MEMORY CARD SLOT2", 64.0f, 256.0f, 0, 1, 0 },
    };
    PARTS ptw[3] = { { "OFF", 288.0f, 10.0f, 40 }, { "ON", 352.0f, 10.0f, 32 }, { 0, -16.0f, 64.0f, 72 } };

    if (sw == 0) {
        MemorySetSystemDir((char *)&userinfo, 0xF28);
        MemoryOpen();
        MemAccess_Init(hw, ptw);
        ret = 0;
        subthd = 0;
        sc.cnt[0] = 0;
        sc.cnt[1] = 0;
        return;
    }
    if (vinfo.pos != -2 && !((pads[0].trg | pads[1].trg) & 0x800)) {
        vm_disp(hw);
        disp_freeblock(hw, ptw);
        if (vinfo.slot < 0) {
        if ((pad = iodat_chk(0x1000))) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                vinfo.pos = vinfo.pos ? vinfo.pos - 1 : 3;
            }
            sc.cnt[pad - 1]++;
        } else if ((pad = iodat_chk(0x4000))) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                vinfo.pos = (vinfo.pos == 3) ? 0 : vinfo.pos + 1;
            }
            sc.cnt[pad - 1]++;
        } else if ((pad = iodat_chk(0x8000))) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                if (hw[vinfo.pos].p) {
                    if (hw[vinfo.pos].pos) hw[vinfo.pos].pos--;
                    else hw[vinfo.pos].pos = hw[vinfo.pos].maxhead - 1;
                } else {
                    vinfo.pos = vinfo.pos ? vinfo.pos - 1 : 3;
                }
            }
            sc.cnt[pad - 1]++;
        } else if ((pad = iodat_chk(0x2000))) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                if (hw[vinfo.pos].p) {
                    if (hw[vinfo.pos].pos >= hw[vinfo.pos].maxhead - 1) hw[vinfo.pos].pos = 0;
                    else hw[vinfo.pos].pos++;
                } else {
                    vinfo.pos = (vinfo.pos == 3) ? 0 : vinfo.pos + 1;
                }
            }
            sc.cnt[pad - 1]++;
        } else {
            sc.cnt[0] = 0;
            sc.cnt[1] = 0;
        }
        switch (vinfo.pos) {
        case 0:
            if ((pads[0].trg | pads[1].trg) & 0x40) {
                sound_request(0x39);
                vinfo.mode = 0;
                vinfo.slot = 0;
                slot_execute(0);
            }
            break;
        case 1:
            if ((pads[0].trg | pads[1].trg) & 0x40) {
                sound_request(0x39);
                vinfo.mode = 1;
                vinfo.slot = 0;
                slot_execute(0);
            }
            break;
        case 2:
            userinfo.autosave = hw[2].pos;
            break;
        case 3:
            if ((pads[0].trg | pads[1].trg) & 0x40) {
                sound_request(0x39);
                vinfo.pos = -2;
            }
            break;
        }
        if ((pads[0].trg | pads[1].trg) & 0x10) {
            sound_request(0x3b);
            vinfo.pos = -2;
        }
        } else {
            slot_execute(1);
        }
    } else if (MemoryCheck() > 0) {
        vm_disp(hw);
        disp_freeblock(hw, ptw);
        slot_execute(1);
    } else {
        if ((pads[0].trg | pads[1].trg) & 0x810) sound_request(0x3b);
        opt.thdflg &= ~4;
        MemoryClose();
    }
}



static void slot_execute(int sw)
{
    static int mode;
    int pad;

    if (sw == 0) {
        mode = 0;
        return;
    }
    if (mode == 0) {
        if ((pad = iodat_chk(0x1000))) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                if (--vinfo.slot < 0) vinfo.slot = 1;
            }
            sc.cnt[pad - 1]++;
        } else if ((pad = iodat_chk(0x4000))) {
            if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
                sound_request(0x37);
                if (++vinfo.slot > 1) vinfo.slot = 0;
            }
            sc.cnt[pad - 1]++;
        } else {
            sc.cnt[0] = 0;
            sc.cnt[1] = 0;
        }
        if ((pads[0].trg | pads[1].trg) & 0x10) {
            sound_request(0x3b);
            vinfo.slot = -1;
        } else if ((pads[0].trg | pads[1].trg) & 0x40) {
            sound_request(0x39);
            if (vinfo.mode == 0) {
                MemoryClose();
                MemAccess_Save(0, vinfo.slot);
                mode = 1;
            } else if (vinfo.mode == 1) {
                MemoryClose();
                MemAccess_Load(0, vinfo.slot);
                mode = 2;
            }
        }
    } else if (mode == 1) {
        if (MemAccess_Save(1, vinfo.slot)) {
            MemoryOpen();
            mode = 0;
        }
    } else if (mode == 2) {
        if (MemAccess_Load(1, vinfo.slot)) {
            func_0012E4E0();
            MemoryOpen();
            mode = 0;
        }
    }
}

static void vm_disp(HEADWORK *hw)
{
    int i;
    int n;
    int alpha;

    FontDrawDC("MEMORY CARD", 32.0f, 40.0f, 4.0f, 1, 0);
    for (n = 0; n < 4; n++) {
        if (n == vinfo.pos) alpha = 1; else alpha = 0xa0;
        FontDrawDC(hw[n].head, hw[n].xp, hw[n].yp, 4.0f, alpha, 1);
        if (hw[n].p != 0) {
            for (i = 0; i < hw[n].maxhead; i++) {
                if (hw[n].p[i].head) {
                    if (hw[n].pos == i) alpha = 0xff; else alpha = 0x50;
                    FontDraw(hw[n].p[i].head, hw[n].xp + hw[n].p[i].xp, hw[n].yp + hw[n].p[i].yp, 4.0f, alpha);
                }
            }
        }
    }
    for (n = 0; n < 2; n++) {
        FontDrawDC(hw[n + 4].head, hw[n + 4].xp + 32.0f, hw[n + 4].yp, 4.0f, (vinfo.slot == n) ? 1 : 0xa0, 1);
    }
}





static void MemAccess_Init(HEADWORK *hw, PARTS *pt)
{
    hw[2].p = pt;
    hw[4].p = &pt[2];
    hw[2].pos = userinfo.autosave;
vinfo.mode = -1; vinfo.slot = -1; vinfo.errcode = 0; vinfo.pos = 0; vinfo.wait = 0;

}


char *G_sprintf(char *buff, char *str, int *valtb);



static void disp_freeblock(HEADWORK *hw, PARTS *pt)
{
    int x;
    int y;
    int i;
    int xoff;
    int size;
    int valtb[2];

    for (i = 0; i < 2; i++) {
        size = vm_get_freeblock(i);
        xoff = 42;
        x = hw[4].p->xp + hw[4 + i].xp;
        y = hw[4].p->yp + hw[4 + i].yp + 56.0f;
        if (size >= 0) {
            valtb[0] = size;
            if (size >= 100) {
                xoff = 46;
            }
            FontDraw(G_sprintf(0, "%3dKBYTE", valtb), x + xoff, y - 72, 4.0f, 0xff);
        } else if (size == -1) {
            FontDraw("UNFORMAT", x + 46, y - 72, 4.0f, 0xff);
        } else if (size == -2) {
            FontDraw("ILLEGAL", x + 46, y - 72, 4.0f, 0xff);
        } else {
            FontDraw("NO CARD", x + 46, y - 72, 4.0f, 0xff);
        }
    }
}




static int MessageSet(char *s1, char *s2, int *cursor)
{
    int pad;

    FontDraw(s1, 320 - strlen(s1) * 6, 240, 2.0f, 0xff);
    FontDraw(s2, 320 - strlen(s2) * 6, 260, 2.0f, 0xff);
    if (*cursor == 0) {
        FontDraw("NO     ", 278, 288, 2.0f, 0xff);
        FontDraw("     YES", 272, 288, 2.0f, 0x50);
    } else {
        FontDraw("NO     ", 278, 288, 2.0f, 0x50);
        FontDraw("     YES", 272, 288, 2.0f, 0xff);
    }
    if ((pads[0].trg | pads[1].trg) & 0x40) {
        sound_request(0x39);
        return 1;
    }
    if ((pad = iodat_chk(0xa000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 3))) {
            *cursor ^= 1;
        }
        sc.cnt[pad - 1]++;
    } else {
        sc.cnt[0] = 0;
        sc.cnt[1] = 0;
    }
    return 0;
}

static OPTSND os;
extern GDCOND gcond;
extern PROFILE_BGM pbgm[34]; /* GGX: TU static at 0x546AD0, 34 entries (X Plus 36) */
int SnDrvBgmCheck(void);
void BGM_Request(int no, int repeat);


static int ret;
void func_00161D80(void);
void func_00161C30(int flg);
void func_001609D8(HEADWORK *hw);
void func_001611D0(HEADWORK *hw);
void BGM_Free(void);
void VoiceFree(void);

static int MemAccess_Save(int sw, int slot)
{
    static int time;
    static int cursor;
    static int accmode;
    static int ret;
    static int err;

    polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 3.0f, gStripGetFadeColor(0xe0000000));
    if (sw == 0) {
        accmode = 0;
        time = 0;
        cursor = 0;
        return 0;
    }
    switch (accmode) {
    case 0:
        MemoryDir(slot, "BASLUS-20287GGX", &ret);
        debugf("MemoryDir Start %s\n", "BASLUS-20287GGX");
        accmode++;
        break;
    case 1:
        err = MemoryCheck();
        if (err > 0) break;
        if (err == 0) {
            if (ret) accmode = 3; else accmode = 2;
        } else if (err == -4) {
            accmode = 4;
        } else {
            accmode = 6;
        }
        break;
    case 2:
        slot_display(slot);
        if (!MessageSet("", "SAVE NOW?", &cursor)) return 0;
        if (!cursor) return 1;
        MemorySave(slot);
        accmode = 5;
        break;
    case 3:
        slot_display(slot);
        if (!MessageSet("FILE EXIST", "OVERWRITE?", &cursor)) return 0;
        if (!cursor) return 1;
        MemorySave(slot);
        accmode = 5;
        break;
    case 4:
        slot_display(slot);
        if (!MessageSet("MEMORY CARD IS NOT FORMATTED", "FORMAT NOW?", &cursor)) return 0;
        if (!cursor) return 1;
        MemoryFormatAndSave(slot);
        accmode = 5;
        break;
    case 5:
        err = MemoryCheck();
        if (err > 0) {
            slot_display(slot);
            FontDraw("NOW SAVING...", 242, 236, 2.0f, 0xff);
            FontDraw("PLEASE DO NOT REMOVE THE MEMORY CARD", 104, 272, 2.0f, 0xff);
            return 0;
        }
        accmode = 6;
    case 6:
        if (err == 0) {
            slot_display(slot);
            FontDraw("SAVE COMPLETED", 236, 236, 2.0f, 0xff);
        } else if (err == -8) {
            vm_error(-9, 2, slot);
        } else {
            vm_error(err, 2, slot);
            if (err == -5) FontDraw("70KBYTE IS NEEDED", 218, 276, 2.0f, 0xff);
        }
        if (++time > 180) return 1;
        if ((pads[0].trg | pads[1].trg) & 0x40) return 1;
        break;
    }
    return 0;
}

void MemoryLoad(int slot);


static int func_001604B8(int sw, int slot)
{
    static int time;
    int err;

    if (sw == 0) {
        MemoryLoad(slot);
        time = -100;
    } else {
        if (time == -100) {
            if (MemoryCheck() > 0) return 0;
            time = 180;
        }
        polydraw_FBox(0.0f, 0.0f, 640.0f, 480.0f, 3.0f, gStripGetFadeColor(0xe0000000));
        err = MemoryCheck();
        if (err > 0) {
            slot_display(slot);
            FontDraw("NOW LOADING", 254, 236, 2.0f, 0xff);
            return 0;
        }
        if (err == 0) {
            slot_display(slot);
            FontDraw("LOAD COMPLETED", 236, 236, 2.0f, 0xff);
        } else {
            vm_error(err, 2, slot);
        }
        if (time-- <= 0) return 1;
        if ((pads[0].trg | pads[1].trg) & 0x40) return 1;
    }
    return 0;
}





void func_00160618(int sw)
{
    int i;
    int alpha;
    int pad;
    void (*stst_proc[2])() = { func_001609D8, func_001611D0 };
    static HEADWORK mh = { 0, 64.0f, 112.0f, 0, 2, 0 };
    PARTS mp[2] = {
        { "BGM", 0.0f, 0.0f, 0 },
        { "SE", 128.0f, 0.0f, 0 },
    };
    HEADWORK hw[3] = {
        { 0, 64.0f, 160.0f, 0, 0, 0 },
        { 0, 64.0f, 160.0f, 0, 0, 0 },
        { 0, 64.0f, 160.0f, 0, 0, 0 },
    };
    if (sw == 0) {
        func_00161D80();
        ret = 0;
        mh.p = mp;
        return;
    }
    if (ret == 0) {
        FontDrawDC("SOUND TEST", 32.0f, 40.0f, 4.0f, 1, 0);
        for (i = 0; i < mh.maxhead; i++) {
            FontDrawDC(mh.p[i].head, mh.xp + mh.p[i].xp, mh.yp + mh.p[i].yp,
                       4.0f, (mh.pos == i) ? 1 : 0xa0, 1);
        }
        func_00161C30(mh.pos);
        if ((pads[0].trg | pads[1].trg) & 4) {
            BGM_Free();
            opt.plyno = -1;
            os.repeat = -1;
            gcond.reserve = -1;
            gcond.cond = 0;
            sound_request(0x37);
            if (mh.pos > 0) mh.pos--; else mh.pos = 1;
            os.wakupos = 0;
            os.bgmpos = 0;
        }
        if ((pads[0].trg | pads[1].trg) & 8) {
            BGM_Free();
            opt.plyno = -1;
            os.repeat = -1;
            gcond.reserve = -1;
            gcond.cond = 0;
            sound_request(0x37);
            if (mh.pos <= 0) mh.pos++; else mh.pos = 0;
            os.wakupos = 0;
            os.bgmpos = 0;
            os.pos[2] = 0;
            os.pos[1] = 0;
            os.pos[0] = 0;
        }
        if ((pads[0].trg | pads[1].trg) & 0x10) {
            if (opt.plyno == -1) {
                sound_request(0x3b);
                ret = 1;
            }
        }
        stst_proc[mh.pos](&hw[mh.pos]);
    } else {
        VoiceFree();
        opt.thdflg &= ~2;
    }
}


void func_00160F28(void);
void func_00161020(void);
void *memcpy(void *d, const void *s, unsigned int n);



void func_001609D8(HEADWORK *hw)
{
    int i;
    int pad;
    int alpha;
    char no[16];
    char buf[48];

    polydraw_FBox(56.0f, 144.0f, 560.0f, 300.0f, 12.0f, gStripGetFadeColor(0x80100000));
    polydraw_FBox(56.0f, 324.0f, 560.0f, 400.0f, 12.0f, gStripGetFadeColor(0x80100000));
    func_00160F28();
    func_00161020();
    if (opt.plyno != -1) {
        FontDraw("PLAYNO", 0x40, 0x14c, 4.0f, 255);
        memcpy(buf, pbgm[opt.plyno].bgmname, 48);
        for (i = 0; i < 3; i++) {
            no[i] = buf[i];
            buf[i] = ' ';
        }
        no[2] = 0;
        FontDraw(no, 0x90, 0x14c, 4.0f, 255);
        FontDraw(pbgm[opt.plyno].profile, 0xe0, 0x14c, 4.0f, 255);
        FontDraw(buf, 0x40, 0x174, 4.0f, 255);
    }
    if (hw->head) {
        FontDrawDC(hw->head, hw->xp, hw->yp, 4.0f, 1, 1);
    }
    for (i = 0; i < 7; i++) {
        alpha = os.wakupos == i ? 255 : 0x50;
        FontDraw(pbgm[i + os.bgmpos].bgmname, 0x40, 0x98 + i * 20,
                 4.0f, alpha);
    }
    if ((pad = iodat_chk(0x1000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 1))) {
            if (os.wakupos) {
                sound_request(6);
                os.wakupos--;
            } else if (os.bgmpos) {
                sound_request(6);
                os.bgmpos--;
            }
        }
        sc.cnt[pad - 1]++;
    } else if ((pad = iodat_chk(0x4000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 1))) {
            if (os.wakupos < 6) {
                sound_request(6);
                os.wakupos++;
            } else if (os.bgmpos < 27) {
                sound_request(6);
                os.bgmpos++;
            }
        }
        sc.cnt[pad - 1]++;
    } else {
        sc.cnt[0] = 0;
        sc.cnt[1] = 0;
    }
    if ((pads[0].trg | pads[1].trg) & 0x840) {
        BGM_Free();
        opt.plyno = os.bgmpos + os.wakupos;
        gcond.cond = 1;
        gcond.reserve = opt.plyno;
        if ((pads[0].trg | pads[1].trg) & 0x800) {
            os.repeat = 0;
        } else {
            os.repeat = 1;
        }
        BGM_Request(pbgm[os.bgmpos + os.wakupos].no - 3, os.repeat);
    }
    if ((pads[0].trg | pads[1].trg) & 0x10) {
        if (opt.plyno != -1) {
            sound_request(0x3b);
            BGM_Free();
            opt.plyno = -1;
            os.repeat = -1;
            gcond.reserve = -1;
            gcond.cond = 0;
            BGM_Free();
        }
    }
    func_00161150(pbgm[opt.plyno].no);
}

void get_soundmode(void);
void set_soundmode(int mode);


void func_00160F28(void)
{
    char *mode[2] = { "-STEREO", "-MONO" };

    FontDraw("MODE", 0x1a0, 0x58, 4.0f, 0xff);
    FontDraw(mode[userinfo.sound_mode], 0x1e0, 0x58, 4.0f, 0xff);
    FontDraw("(y)", 0x250, 0x58, 4.0f, 0xff);
    if ((pads[0].trg | pads[1].trg) & 0x80) {
        get_soundmode();
        if (userinfo.sound_mode == 0) {
            set_soundmode(1);
        } else {
            set_soundmode(0);
        }
    }
}

extern char D_56CF08[5];
void set_bgmvol(void);


void func_00161020(void)
{
    int n;
    char *level[8] = { "-SILENT", "-SMALL", "-MEDIUM", "-STANDARD", "-MAX" };

    FontDraw("LEVEL", 0x1a0, 0x74, 4.0f, 0xff);
    FontDraw(level[userinfo.bgm], 0x1e0, 0x74, 4.0f, 0xff);
    FontDraw("(z)", 0x250, 0x74, 4.0f, 0xff);
    if ((pads[0].trg | pads[1].trg) & 0x20) {
        n = userinfo.bgm + 1;
        if (n >= 5) n = 0;
        userinfo.bgm = n;
        userinfo.vol_bgm = userinfo.vol_bgm_mst = D_56CF08[userinfo.bgm];
        set_bgmvol();
    }
}






void func_00161150(int no)
{
    if (!SnDrvBgmCheck() && os.repeat == 0) {
        opt.plyno++;
        if (opt.plyno >= 34) opt.plyno = 0;
        BGM_Request(pbgm[opt.plyno].no - 3, 0);
    }
}

void set_char2num(int num, char *block, int id);
void stest_packload(int no, int no2);
int FileReadCheck(void);
void sound_rec_clear(void);
int sound_request_main(int no, int type);


void func_001611D0(HEADWORK *hw)
{
    int i;
    int pad;
    int alpha;
    char *p;
    PROFILE_SE pvoice[48] = {
        { 0, "SOL BADGUY TYPE-A" }, { 0, "SOL BADGUY TYPE-B" }, { 0, "SOL BADGUY TYPE-C" },
        { 1, "KY KISKE TYPE-A" }, { 1, "KY KISKE TYPE-B" }, { 1, "KY KISKE TYPE-C" }, { 2, "MAY TYPE-A" },
        { 2, "MAY TYPE-B" }, { 2, "MAY TYPE-C" }, { 3, "MILLIA RAGE TYPE-A" }, { 3, "MILLIA RAGE TYPE-B" },
        { 3, "MILLIA RAGE TYPE-C" }, { 4, "AXL LOW TYPE-A" }, { 4, "AXL LOW TYPE-B" },
        { 4, "AXL LOW TYPE-C" }, { 5, "POTEMKIN TYPE-A" }, { 5, "POTEMKIN TYPE-B" }, { 5, "POTEMKIN TYPE-C" },
        { 6, "CHIPP ZANUFF TYPE-A" }, { 6, "CHIPP ZANUFF TYPE-B" }, { 6, "CHIPP ZANUFF TYPE-C" },
        { 7, "ZATO-ONE TYPE-A" }, { 7, "ZATO-ONE TYPE-B" }, { 7, "ZATO-ONE TYPE-C" }, { 8, "BAIKEN TYPE-A" },
        { 8, "BAIKEN TYPE-B" }, { 8, "BAIKEN TYPE-C" }, { 9, "FAUST TYPE-A" }, { 9, "FAUST TYPE-B" },
        { 9, "FAUST TYPE-C" }, { 10, "TESTAMENT TYPE-A" }, { 10, "TESTAMENT TYPE-B" },
        { 10, "TESTAMENT TYPE-C" }, { 11, "JAM KURADOBERI TYPE-A" }, { 11, "JAM KURADOBERI TYPE-B" },
        { 11, "JAM KURADOBERI TYPE-C" }, { 12, "ANJI MITO TYPE-A" }, { 12, "ANJI MITO TYPE-B" },
        { 12, "ANJI MITO TYPE-C" }, { 13, "JOHNNY TYPE-A" }, { 13, "JOHNNY TYPE-B" }, { 13, "JOHNNY TYPE-C" },
        { 14, "VENOM TYPE-A" }, { 14, "VENOM TYPE-B" }, { 14, "VENOM TYPE-C" }, { 15, "DIZZY TYPE-A" },
        { 15, "DIZZY TYPE-B" }, { 15, "DIZZY TYPE-C" },
    };
    PROFILE_SE pse[17] = {
        { 16, "SOL BADGUY" }, { 17, "KY KISKE" }, { 18, "MAY" }, { 19, "MILLIA RAGE" }, { 20, "AXL LAW" },
        { 21, "POTEMKIN" }, { 22, "CHIPP ZANUFF" }, { 23, "ZATO-ONE" }, { 24, "BAIKEN" }, { 25, "FAUST" },
        { 26, "TESTAMENT" }, { 27, "JAM KURADOBERI" }, { 28, "ANJI MITO" }, { 29, "JOHNNY" }, { 30, "VENOM" },
        { 31, "DIZZY" }, { 32, "COMMON" },
    };
    PROFILE_SE se_profile[33] = {
        { 30, "VOICE -- SOL BADGUY --" }, { 41, "VOICE -- KY KISKE --" }, { 43, "VOICE -- MAY --" },
        { 28, "VOICE -- MILLIA RAGE --" }, { 41, "VOICE -- AXL LOW --" }, { 37, "VOICE -- POTEMKIN --" },
        { 40, "VOICE -- CHIPP ZANUFF --" }, { 38, "VOICE -- ZATO-ONE --" }, { 44, "VOICE -- BAIKEN --" },
        { 43, "VOICE -- FAUST --" }, { 32, "VOICE -- TESTAMENT --" }, { 47, "VOICE -- JAM KURADOBERI --" },
        { 34, "VOICE -- ANJI MITO --" }, { 39, "VOICE -- JOHNNY --" }, { 31, "VOICE -- VENOM --" },
        { 25, "VOICE -- DIZZY --" }, { 7, "SE -- SOL BADGUY --" }, { 6, "SE -- KY KISKE --" },
        { 6, "SE -- MAY --" }, { 4, "SE -- MILLIA RAGE --" }, { 5, "SE -- AXL LOW --" },
        { 8, "SE -- POTEMKIN --" }, { 7, "SE -- CHIPP ZANUFF --" }, { 7, "SE -- ZATO-ONE --" },
        { 6, "SE -- BAIKEN --" }, { 8, "SE -- FAUST --" }, { 8, "SE -- TESTAMENT --" },
        { 9, "SE -- JAM KURADOBERI --" }, { 7, "SE -- ANJI MITO --" }, { 6, "SE -- JOHNNY --" },
        { 7, "SE -- VENOM --" }, { 8, "SE -- DIZZY --" }, { 96, "SE -- COMMON -- " },
    };
    PARTS mp[3] = {
        { "LOAD VOICE -", 80.0f, 176.0f, 0 },
        { "LOAD SE -", 116.0f, 200.0f, 0 },
        { "PLAY NO -", 116.0f, 224.0f, 0 },
    };
    int maxpos[3] = { 48, 17, 0 };
    char buf[16];

    polydraw_FBox(56.0f, 144.0f, 560.0f, 300.0f, 12.0f, gStripGetFadeColor(0x80100000));
    polydraw_FBox(56.0f, 324.0f, 560.0f, 400.0f, 12.0f, gStripGetFadeColor(0x80100000));
    if (hw->head) {
        FontDrawDC(hw->head, hw->xp, hw->yp, 4.0f, 1, 1);
    }
    for (i = 0; i < 3; i++) {
        alpha = os.wakupos == i ? 255 : 0x50;
        FontDraw(mp[i].head, mp[i].xp, mp[i].yp, 4.0f, alpha);
        if (i == 0) {
            p = pvoice[os.pos[0]].sename;
        } else if (i == 1) {
            p = pse[os.pos[1]].sename;
        } else {
            set_char2num(os.pos[i], buf, 0);
            p = buf;
        }
        FontDraw(p, 0xf0, mp[i].yp, 4.0f, alpha);
    }
    if (os.profile) {
        FontDraw(os.profile, 0x40, 0x14c, 4.0f, 255);
    }
    if (os.playno != -1) {
        FontDraw("PLAY NO - ", 0x80, 0x174, 4.0f, 255);
        set_char2num(os.playno, buf, 0);
        FontDraw(buf, 0x100, 0x174, 4.0f, 255);
    }
    if ((pad = iodat_chk(0x1000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 1))) {
            if (os.wakupos) {
                sound_request(6);
                os.wakupos--;
            }
        }
        sc.cnt[pad - 1]++;
    } else if ((pad = iodat_chk(0x4000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 1))) {
            if (os.wakupos < 2) {
                sound_request(6);
                os.wakupos++;
            }
        }
        sc.cnt[pad - 1]++;
    } else if ((pad = iodat_chk(0x8000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 1))) {
            if (os.pos[os.wakupos]) {
                sound_request(6);
                os.pos[os.wakupos]--;
            }
        }
        sc.cnt[pad - 1]++;
    } else if ((pad = iodat_chk(0x2000))) {
        if (sc.cnt[pad - 1] == 0 || (sc.cnt[pad - 1] > 16 && !(sc.cnt[pad - 1] & 1))) {
            if (os.wakupos == 2) {
                if (os.pos[os.wakupos] < os.maxno) {
                    sound_request(6);
                    os.pos[os.wakupos]++;
                }
            } else if (os.pos[os.wakupos] < maxpos[os.wakupos] - 1) {
                sound_request(6);
                os.pos[os.wakupos]++;
            }
        }
        sc.cnt[pad - 1]++;
    } else {
        sc.cnt[0] = 0;
        sc.cnt[1] = 0;
    }
    if ((pads[0].trg | pads[1].trg) & 0x40) {
        if (os.wakupos == 0) {
            sound_request(0x39);
            if (os.loaddat[0] != os.wakupos || os.loaddat[1] != os.pos[os.wakupos]) {
                stest_packload(pvoice[os.pos[0]].no + 1, os.pos[0] % 3);
                os.loaddat[0] = os.wakupos;
                os.loaddat[1] = os.pos[os.wakupos];
                os.playno = -1;
                os.pos[2] = 0;
            }
            os.profile = se_profile[pvoice[os.pos[os.wakupos]].no].sename;
            os.maxno = se_profile[pvoice[os.pos[os.wakupos]].no].no;
            os.id = 2;
            os.wakupos = 2;
        } else if (os.wakupos == 1) {
            sound_request(0x39);
            if (os.loaddat[0] != os.wakupos || os.loaddat[1] != os.pos[os.wakupos]) {
                stest_packload(os.pos[1] + 1, -1);
                os.loaddat[0] = os.wakupos;
                os.loaddat[1] = os.pos[os.wakupos];
                os.playno = -1;
                os.pos[2] = 0;
            }
            os.profile = se_profile[pse[os.pos[os.wakupos]].no].sename;
            os.maxno = se_profile[pse[os.pos[os.wakupos]].no].no;
            if (os.pos[1] != 16) {
                os.id = 3;
            } else {
                os.id = 0;
            }
            os.wakupos = 2;
        } else if (os.wakupos == 2) {
            if (FileReadCheck()) {
                sound_rec_clear();
                os.playno = os.pos[2];
                if (os.maxno) {
                    if (os.id == 0) {
                        sound_request(os.pos[2]);
                    } else {
                        sound_request_main(os.pos[2], os.id);
                    }
                }
            }
        }
    }
}




void func_00161C30(int flg)
{
    HEADWORK text[4] = {
        { "w-PLAY", 64.0f, 432.0f, 0, 0, 0 },
        { "x-STOP", 160.0f, 432.0f, 0, 0, 0 },
        { "LR-CHANGE", 256.0f, 432.0f, 0, 0, 0 },
        { "START-MEDLEY", 384.0f, 432.0f, 0, 0, 0 },
    };
    int max;
    int i;
    max = flg ? 3 : 4;
    for (i = 0; i < max; i++) {
        FontDraw(text[i].head, text[i].xp, text[i].yp, 4.0f, 255);
    }
}




void func_00161D80(void)
{
    opt.plyno = -1;
    os.wakupos = 0;
    gcond.cond = 0;
    gcond.reserve = -1;
    os.bgmpos = 0;
    os.pos[0] = 0;
    os.pos[1] = 0;
    os.pos[2] = 0;
    os.profile = 0;
    os.loaddat[0] = -1;
    os.loaddat[1] = -1;
    os.maxno = 0;
    os.playno = -1;
    os.id = -1;
    os.repeat = -1;
}