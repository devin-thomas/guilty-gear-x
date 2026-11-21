/* Guilty Gear X US (SLUS-202.87): 2 functions ported from the X Plus
   decomp (ggxeuc/memacc.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/memacc.h"
static const short idtb[] = { 1, 2, 3 };
void FontDraw(char *str, int x, int y, float z, int alfa);
int strlen(const char *s);
int sound_request(int no);
extern PAD pads[2];
int AutoSaveMode = 0;
int AutoSaveCursor = 0;
int AutoSaveFormat = 0;
int AutoLoadMode = 0;
int AutoLoadCursor = 0;
static int type_buf;
void slot_display(int slot);
void slot_display_ex(int slot, char *str, int size);
void vm_error(int no, int flg, int slot);
int save_err_mes(char *str);
int save_format_mes(char *str);
int save_err_mes_ex(char *str);
int load_err_mes_ex(char *str);
void AutoLoadInit(void);
int load_err_mes(char *str);

void FontDraw(char *str, int x, int y, float z, int alfa);

void slot_display(int slot)
{
    if (slot == 0) {
        FontDraw("MEMORY CARD SLOT 1", 212, 216, 2.0f, 255);
    } else {
        FontDraw("MEMORY CARD SLOT 2", 212, 216, 2.0f, 255);
    }
}

void FontDraw(char *str, int x, int y, float z, int alfa);
int strlen(const char *s);

void FontDraw(char *str, int x, int y, float z, int alfa);
int strlen(const char *s);

int MemoryCheck(void);
void MemoryClose(void);
void MemoryFormatAndSave(int slot);
void MemoryOpen(void);
void MemorySave(int slot);
void MemorySetSystemDir(char *data, int size);
void MemorySlotClear(int slot);
void MemoryType(int slot, int *type, int *free, int *exist);
extern SYS_COND sc;
extern USER_SETTING userinfo;

void JobChange(int job);
void JobNext(void);
void MemoryOpen(void);
void MemorySetSystemDir(char *data, int size);
void MemorySlotClear(int slot);
void g_main_End(void);
void load_font(void);
extern SYS_COND sc;
extern USER_SETTING userinfo;
extern int SubJob;
extern int SubJobCount;

void JobChange(int job);
int MemoryCheck(void);
void MemoryClose(void);
void MemoryFormatAndSave(int slot);
void MemorySave(int slot);
void MemorySetSystemDir(char *data, int size);
void MemoryType(int slot, int *type, int *free, int *exist);
extern SYS_COND sc;
extern USER_SETTING userinfo;
extern int SubJob;
extern int SubJobCount;

void MemoryOpen(void);
void MemorySetSystemDir(char *data, int size);
void MemorySlotClear(int slot);
extern USER_SETTING userinfo;

int MemoryCheck(void);
void MemoryClose(void);
void MemoryLoad(int slot);
void MemoryType(int slot, int *type, int *free, int *exist);
void debugf(char *format);

void MemoryType(int slot, int *type, int *free, int *exist);



void vm_error(int no, int flg, int slot)
{
    char *error_mes[9] = {
        "FATAL ERROR!!",
        "MEMORY CARD NOT INSERTED!!",
        "ILLEGAL MEMORY CARD!!",
        "MEMORY CARD IS NOT FORMATTED!!",
        "SHORT OF MEMORY!!",
        "FILE NOT FOUND",
        "FORMAT ERROR!!",
        "LOAD ERROR!!",
        "SAVE ERROR!!",
    };

    slot_display(slot);
    FontDraw(error_mes[-no - 1], 320 - strlen(error_mes[-no - 1]) * 6, 236, 2.0f, 255);
}


void func_00195D48(int slot);



int func_00113238(int mode)
{
    static int sw;
    static int time;

    if (mode == 0) {
        MemorySetSystemDir((char *)&userinfo, 0xF28);
        func_00195D48(0);
        time = 0;
        sw = 1;
        return 0;
    }
    if (sw > 0) {
        sw = MemoryCheck();
        FontDraw("NOW SAVING...", 242, 224, 1.0f, 255);
        FontDraw("PLEASE DO NOT REMOVE THE MEMORY CARD", 104, 260, 1.0f, 255);
        return 0;
    }
    if (++time < 180) {
        if (sw == 0) {
            FontDraw("SAVE COMPLETE", 242, 224, 1.0f, 255);
        } else if (sw == -8) {
            vm_error(-9, 2, 0);
        } else {
            vm_error(sw, 2, 0);
            if (sw == -5) {
                FontDraw("70KBYTE IS NEEDED", 218, 276, 2.0f, 255);
            }
        }
        return 0;
    }
    sc.grpvar[1] = 1;
    return 1;
}



void func_00113390(void)
{
    if (sc.grpvar[1] == -1) {
        g_main_End();
        load_font();
        sc.grpvar[1] = 0;
        MemorySetSystemDir((char *)&userinfo, 0xF28);
        func_00195D48(0);
        SubJobCount = 0;
        SubJob = 1;
        JobNext();
    } else if (sc.grpvar[1]) {
        JobChange(3);
        sc.grpvar[1] = 0;
    }
}

void func_00113420(void)
{
    if (SubJob > 0) {
        SubJob = MemoryCheck();
        FontDraw("NOW SAVING...", 242, 224, 1.0f, 255);
        FontDraw("PLEASE DO NOT REMOVE THE MEMORY CARD", 104, 244, 1.0f, 255);
    } else if (++SubJobCount < 180) {
        if (SubJob == 0) {
            FontDraw("SAVE COMPLETE", 242, 224, 1.0f, 255);
        } else if (SubJob == -8) {
            vm_error(-9, 2, 0);
        } else {
            vm_error(SubJob, 2, 0);
        }
    } else {
        sc.grpvar[1] = 1;
        JobChange(3);
    }
}




void func_00113528(void)
{
    MemorySetSystemDir((char *)&userinfo, 0xF28) /* sizeof(USER_SETTING) in GGX; header says 0x1328 */;
    MemorySlotClear(0);
    MemorySlotClear(1);
    MemoryOpen();
    AutoLoadMode = 0;
    AutoLoadCursor = 0;
}

int func_00113570(char *str)
{
    FontDraw(str, 320 - strlen(str) * 6, 240, 2.0f, 255);
    FontDraw("CONTINUE?", 266, 260, 2.0f, 255);
    if (AutoLoadCursor == 0) {
        FontDraw("NO     ", 278, 288, 2.0f, 255);
        FontDraw("     YES", 272, 288, 2.0f, 80);
    } else {
        FontDraw("NO     ", 278, 288, 2.0f, 80);
        FontDraw("     YES", 272, 288, 2.0f, 255);
    }
    if ((pads[0].trg | pads[1].trg) & 0x40) {
        sound_request(0x39);
        if (AutoLoadCursor == 1) {
            return 1;
        }
    }
    if ((pads[0].trg | pads[1].trg) & 0xA000) {
        sound_request(0x37);
        AutoLoadCursor ^= 1;
    }
    return 0;
}

void func_00195D08(int slot);
int func_00113570(char *str);


int func_001136D0(void)
{
    int ret;
    int type[2];
    int free[2];
    int exist[2];
    static int port;

    if (AutoLoadMode == 0) {
        MemoryType(0, &type[0], &free[0], &exist[0]);
        MemoryType(1, &type[1], &free[1], &exist[1]);
        if (type[0] == 0 || type[1] == 0) {
            return 0;
        }
        if (type[0] == 4 && exist[0] == -1) {
            return 0;
        }
        if (type[1] == 4 && exist[1] == -1) {
            return 0;
        }
        if (type[0] == 4) {
            if (exist[0] == 0) {
                if (free[0] >= 70) {
                    MemoryClose();
                    return 1;
                }
                slot_display(0);
                FontDraw("70KBYTE IS NEEDED", 218, 316, 2.0f, 255);
                if (func_00113570("SHORT OF MEMORY")) {
                    MemoryClose();
                    return 1;
                }
            } else {
                AutoLoadMode = 1;
            }
            port = 0;
            return 0;
        }
        if (type[1] == 4) {
            if (exist[1] == 0) {
                if (free[1] >= 70) {
                    MemoryClose();
                    return 1;
                }
                slot_display(1);
                FontDraw("70KBYTE IS NEEDED", 218, 316, 2.0f, 255);
                if (func_00113570("SHORT OF MEMORY")) {
                    MemoryClose();
                    return 1;
                }
            } else {
                AutoLoadMode = 1;
            }
            port = 1;
            return 0;
        }
        if (type[0] == 1 && type[1] == 1) {
            if (func_00113570("NO MEMORY CARD")) {
                MemoryClose();
                return 1;
            }
            return 0;
        }
        if (type[0] == 3 || type[1] == 3) {
            if (type[0] == 3) {
                slot_display(0);
            } else {
                slot_display(1);
            }
            if (func_00113570("UNFORMATTED MEMORY CARD")) {
                MemoryClose();
                return 1;
            }
            return 0;
        }
        func_00113570("ILLEGAL MEMORY CARD");
        MemoryClose();
        return 1;
    } else if (AutoLoadMode == 1) {
        debugf("Memory Loading\n");
        MemoryClose();
        func_00195D08(port);
        AutoLoadMode = 2;
        return 0;
    }
    ret = MemoryCheck();
    if (ret > 0) {
        slot_display(port);
        FontDraw("LOADING.....", 248, 240, 2.0f, 255);
        return 0;
    }
    if (ret == 0) {
        slot_display(port);
        FontDraw("LOAD COMPLETE", 242, 240, 2.0f, 255);
    } else if (ret != -6) {
        vm_error(ret, 2, port);
    }
    return ++AutoLoadMode > 180;
}






int vm_get_freeblock(int port)
{
    int type;
    int free;

    MemoryType(port, &type, &free, 0);
    if (type == 3) {
        return -1;
    }
    if (type == 2) {
        return -2;
    }
    if (type == 1 || (type == 4 && free < 0)) {
        return -3;
    }
    if (type == 4) {
        return free;
    }
    return -3;
}