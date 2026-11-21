/* Guilty Gear X US (SLUS-202.87): 9 functions ported from the X Plus
   decomp (ggxeuc/memory.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/memory.h"

char *strcpy(char *dst, const char *src);
#ifdef VERSION_JP
#define MC_SYSDIR "BISLPS-20137GGX"
#else
#define MC_SYSDIR "BKSLPM-64525GGX"
#endif

int sceMcInit(void);

void func_001061A8(int fid, void *adr);


void func_00195860(void)
{
    if (IconFile1 == 0) {
        IconSize1 = GetFileSize(0x24);
        IconFile1 = malloc(IconSize1);
        func_001061A8(0x24, IconFile1);
    }
    if (IconFile2 == 0) {
        IconSize2 = GetFileSize(0x25);
        IconFile2 = malloc(IconSize2);
        func_001061A8(0x25, IconFile2);
    }
}




void MemoryInit(void)
{
    int ret;

    mc.dir[0] = '/';
    mc.dir[1] = 0;
    mc.mode = 0;
    mc.command = 0;
    mc.now_port = 0;
    mc.request = 0;
    MemorySlotClear(0);
    MemorySlotClear(1);
    ret = sceMcInit();
    if (ret < 0) {
        if (ret == -101) {
            debugf("Initialize Failed\n");
        } else if (ret == -120) {
            debugf("mcserv.irx Version is Old\n");
        } else if (ret == -121) {
            debugf("mcman.irx Version is Old\n");
        } else {
            debugf("sceMcInit Error %d\n", ret);
        }
    }
}

void func_001959C8(void)
{
    mc.totalsize = (mc.file[0].size + 1023) / 1024 + (mc.file[1].size + 1023) / 1024
                 + (mc.file[2].size + 1023) / 1024;
}

void func_001959C8(void);



void MemorySetSystemDir(char *data, int size)
{
    strcpy(&mc.dir[1], "BASLUS-20287GGX");
    strcpy(mc.file[0].name, "icon.sys");
    strcpy(mc.file[1].name, syscfg.FnameView);
    strcpy(mc.file[2].name, "BASLUS-20287GGX");
    mc.file[2].size = size;
    mc.file[1].data = IconFile1;
    mc.file[1].size = IconSize1;
    mc.file[0].data = (char *)&syscfg;
    mc.file[2].data = data;
    mc.file[0].size = sizeof(sceMcIconSys);
    MemorySlotClear(0);
    MemorySlotClear(1);
    func_001959C8();
}



void MemorySetReplayDir(int id, char *data, int size)
{
    static char *dirname[10] = {
        "BASLUS-20287GGXRP0", "BASLUS-20287GGXRP1", "BASLUS-20287GGXRP2", "BASLUS-20287GGXRP3", "BASLUS-20287GGXRP4", "BASLUS-20287GGXRP5", "BASLUS-20287GGXRP6", "BASLUS-20287GGXRP7", "BASLUS-20287GGXRP8", "BASLUS-20287GGXRP9",
    };

    strcpy(&mc.dir[1], dirname[id]);
    strcpy(mc.file[0].name, "icon.sys");
    strcpy(mc.file[1].name, rpcfg[id].FnameView);
    strcpy(mc.file[2].name, dirname[id]);
    mc.file[2].size = size;
    mc.file[0].size = sizeof(sceMcIconSys);
    mc.file[1].data = IconFile2;
    mc.file[2].data = data;
    mc.file[1].size = IconSize2;
    mc.file[0].data = (char *)&rpcfg[id];
    MemorySlotClear(0);
    MemorySlotClear(1);
    func_001959C8();
}





void MemoryClose(void) {
    mc.execmode = 0;
}

void MemoryErrClear(void) {
    if (mc.mode < 0) {
        mc.mode = 0;
    }
}

int MemoryCheck(void) {
    if (mc.request) {
        return 0x10;
    }
    return mc.mode;
}

char *strcpy(char *dst, const char *src);

void *memcpy(void *dst, const void *src, unsigned int n);

void debugf(char *format, ...);


void *memcpy(void *dst, const void *src, unsigned int n);

void func_00195D08(int slot)
{
    debugf("MemoryLoad Start\n");
    mc.req_port = slot;
    mc.request = 2;
}

void func_00195D48(int slot)
{
    debugf("MemorySave Start\n");
    mc.req_port = slot;
    mc.request = 3;
}

void func_00195D88(int slot)
{
    debugf("Format & MemorySave Start\n");
    mc.req_port = slot;
    mc.request = 4;
}

void func_00195DC8(int slot)
{
    debugf("MemoryDelete Start\n");
    mc.req_port = slot;
    mc.request = 5;
}






void MemoryDir(int slot, char *fname, int *p) {
    mc.req_port = slot;
    mc.request = 6;
    mc.buf = (char *)p;
    mc.fname[0] = 0x2F;
    strcpy(&mc.fname[1], fname);
}

void MemoryOpen(void) {
    MemorySlotClear(0);
    MemorySlotClear(1);
    mc.execmode = 1;
}

void MemorySlotClear(int slot) {
    mc.slot[slot].type = 0;
    mc.slot[slot].free = 0;
    mc.slot[slot].exist = -1;
}

void MemorySetNoDir(void) {
    mc.dir[1] = 0;
    MemorySlotClear(0);
    MemorySlotClear(1);
    mc.totalsize = 0;
}

void strmarge(char *dstr, char *src1, char *src2) {
    int i;

    for (i = 0; src1[i]; i++) {
        *dstr++ = src1[i];
    }
    for (i = 0; src2[i]; i++) {
        *dstr++ = src2[i];
    }
    *dstr = 0;
}

void MemoryType(int slot, int *type, int *free, int *exist) {
    if (type) {
        *type = mc.slot[slot].type;
    }
    if (free) {
        *free = mc.slot[slot].free - 2;
        if (*free < 0) {
            *free = 0;
        }
    }
    if (exist) {
        *exist = mc.slot[slot].exist;
    }
}

int GetFileSize(int fid);
void *malloc(unsigned int size);
void FileRead(int fid, void *adr);






int sceMcGetInfo(int port, int slot, int *type, int *free, int *format);
int sceMcSync(int mode, int *cmd, int *result);
int sceMcOpen(int port, int slot, const char *name, int mode);
int sceMcClose(int fd);
int sceMcRead(int fd, void *buff, int size);
int sceMcWrite(int fd, const void *buff, int size);
int sceMcMkdir(int port, int slot, const char *name);
int sceMcChdir(int port, int slot, const char *newDir, char *currentDir);
int sceMcGetDir(int port, int slot, const char *name, unsigned int mode, int maxent, sceMcTblGetDir *table);
int sceMcDelete(int port, int slot, const char *name);
int sceMcFormat(int port, int slot);
void strmarge(char *dstr, char *src1, char *src2);

int MemoryCheckSumCheck(char *data, int size);

void InitLoadTmp(void);






void MemoryMain(void)
{
    char str[60];
    int func;
    int ret;
    int r;

    if (mc.mode <= 0 && mc.command != 1) {
        if (mc.request > 0) {
            if (sceMcGetInfo(mc.req_port, 0, &type, &freesize, &format) == 0) {
                mc.command = mc.request;
                mc.now_port = mc.req_port;
                mc.mode = 1;
                mc.request = 0;
                mc.retry = 0;
                mc.out_buf = mc.buf;
                mc.list = 0;
            }
        } else if (mc.execmode) {
            mc.now_port ^= 1;
            if (sceMcGetInfo(mc.now_port, 0, &type, &freesize, &format) == 0) {
                mc.command = 1;
                mc.retry = 0;
            }
        }
        return;
    }
    r = sceMcSync(1, &func, &ret);
    if (r <= 0) {
        if (r < 0) {
            mc.mode = -8;
        }
        return;
    }
    switch (func) {
    case 1:
        if (type == 0 || ret < -10) {
            if (mc.command != 1 && ++mc.retry < 3) {
                if (sceMcGetInfo(mc.req_port, 0, &type, &freesize, &format) == 0) {
                    return;
                }
            }
            mc.slot[mc.now_port].type = 1;
            mc.slot[mc.now_port].free = 0;
            mc.slot[mc.now_port].exist = -1;
            if (mc.command != 1) {
                mc.mode = -2;
            } else {
                mc.command = 0;
            }
            break;
        }
        if (type != 2) {
            if (mc.command != 1 && ++mc.retry < 3) {
                if (sceMcGetInfo(mc.req_port, 0, &type, &freesize, &format) == 0) {
                    return;
                }
            }
            mc.slot[mc.now_port].type = 2;
            mc.slot[mc.now_port].free = 0;
            mc.slot[mc.now_port].exist = -1;
            if (mc.command != 1) {
                mc.mode = -3;
            } else {
                mc.command = 0;
            }
            break;
        }
        if (format == 0) {
            if (mc.command != 1 && ++mc.retry < 3) {
                if (sceMcGetInfo(mc.req_port, 0, &type, &freesize, &format) == 0) {
                    return;
                }
            }
            mc.slot[mc.now_port].type = 3;
            mc.slot[mc.now_port].free = 0;
            mc.slot[mc.now_port].exist = -1;
            if (mc.command == 1) {
                mc.command = 0;
                break;
            }
            if (mc.command != 4) {
                mc.mode = -4;
                break;
            }
        } else {
            mc.slot[mc.now_port].type = 4;
            mc.slot[mc.now_port].free = freesize;
        }
        switch (mc.command) {
        case 1:
            if (mc.dir[1]) {
                strmarge(str, mc.dir, mc.dir);
                if (sceMcGetDir(mc.now_port, 0, str, 0, 1, &tmp) == 0) {
                    return;
                }
            }
            mc.command = 0;
            break;
        case 4:
            if (sceMcFormat(mc.now_port, 0)) {
                mc.mode = -7;
            } else {
                mc.mode = 6;
            }
            break;
        case 6:
            strmarge(str, mc.fname, mc.fname);
            if (sceMcGetDir(mc.now_port, 0, str, 0, 1, &tmp)) {
                mc.mode = -8;
            } else {
                mc.mode = 2;
            }
            break;
        default:
            if (sceMcGetDir(mc.now_port, 0, mc.dir, 0, 1, &tmp)) {
                mc.mode = -8;
            } else {
                mc.mode = 2;
            }
            break;
        }
        break;
    case 13:
        if (mc.command == 1) {
            if (ret < 0 && ret != -4) {
                mc.slot[mc.now_port].type = 2;
                mc.slot[mc.now_port].free = 0;
                mc.slot[mc.now_port].exist = -1;
            } else if (ret > 0) {
                mc.slot[mc.now_port].exist = 1;
            } else {
                mc.slot[mc.now_port].exist = 0;
            }
            mc.command = 0;
        } else if (mc.command == 6) {
            if (ret < 0 && ret != -4) {
                mc.mode = -8;
                *(int *)mc.out_buf = 0;
            } else if (ret > 0) {
                mc.mode = 0;
                *(int *)mc.out_buf = 1;
            } else {
                mc.mode = 0;
                *(int *)mc.out_buf = 0;
            }
        } else if (mc.command == 5) {
            if (ret < 0 && ret != -4) {
                mc.mode = -8;
            } else if (ret > 0) {
            chdir:
                if (sceMcChdir(mc.now_port, 0, mc.dir, 0)) {
                    mc.mode = -8;
                }
            } else {
                mc.mode = 0;
            }
        } else if (mc.command == 2) {
            if (ret < 0 && ret != -4) {
                mc.mode = -8;
            } else if (ret > 0) {
                goto chdir;
            } else {
                mc.mode = -6;
            }
        } else if (mc.command == 3) {
            if (ret < 0 && ret != -4) {
                mc.mode = -8;
            } else if (ret > 0) {
                goto chdir;
            } else if (mc.slot[mc.now_port].free < mc.totalsize) {
                mc.mode = -5;
            } else if (sceMcMkdir(mc.now_port, 0, mc.dir)) {
                mc.mode = -8;
            } else {
                mc.mode = 3;
            }
        } else {
            mc.mode = 0;
        }
        break;
    case 16:
        if (ret < 0) {
            mc.mode = -7;
        } else if (mc.command != 4) {
            mc.mode = 0;
        } else if (sceMcMkdir(mc.now_port, 0, mc.dir)) {
            mc.mode = -8;
        } else {
            mc.mode = 3;
        }
        break;
    case 11:
        if (ret < 0 && ret != -4) {
            mc.mode = -8;
        } else if (sceMcChdir(mc.now_port, 0, mc.dir, 0)) {
            mc.mode = -8;
        } else {
            mc.mode = 2;
        }
        break;
    case 12:
        if (ret < 0) {
            if (sceMcDelete(mc.now_port, 0, mc.dir)) {
                mc.mode = -6;
            } else {
                mc.mode = 2;
            }
            break;
        }
        switch (mc.command) {
        case 2:
            if (sceMcOpen(mc.now_port, 0, mc.file[2].name, 1)) {
                mc.mode = -6;
            } else {
                mc.mode = 4;
            }
            break;
        case 4:
            if (sceMcOpen(mc.now_port, 0, mc.file[mc.list].name, 0x202)) {
                mc.mode = -8;
            } else {
                mc.mode = 5;
            }
            break;
        case 3:
        case 5:
            if (mc.list == 3) {
                if (sceMcDelete(mc.now_port, 0, mc.dir)) {
                    mc.mode = -6;
                } else {
                    mc.mode = 7;
                }
            } else {
                if (sceMcDelete(mc.now_port, 0, mc.file[mc.list].name)) {
                    mc.mode = -6;
                } else {
                    mc.mode = 7;
                }
            }
            break;
        }
        break;
    case 2:
        if (ret < 0) {
            mc.mode = -8;
            break;
        }
        mc.fd = ret;
        switch (mc.command) {
        case 3:
        case 4:
            if (sceMcWrite(mc.fd, mc.file[mc.list].data, mc.file[mc.list].size)) {
                mc.mode = -8;
            } else {
                mc.mode = 5;
            }
            break;
        case 2:
            if (sceMcRead(mc.fd, mc_loadtmp, mc.file[2].size)) {
                mc.mode = -6;
            } else {
                mc.mode = 4;
            }
            break;
        }
        break;
    case 5:
    case 6:
        if (ret < 0) {
            mc.mode = -8;
        } else if (sceMcClose(mc.fd)) {
            mc.mode = -8;
        }
        break;
    case 3:
        if (ret < 0) {
            mc.mode = -8;
            break;
        }
        if (mc.command == 2) {
            memcpy(mc.file[2].data, mc_loadtmp, mc.file[2].size);
            mc.mode = 0;
            break;
        }
        if (++mc.list >= 3) {
            mc.mode = 0;
        } else if (mc.command == 3) {
            if (sceMcDelete(mc.now_port, 0, mc.file[mc.list].name)) {
                mc.mode = -6;
            } else {
                mc.mode = 7;
            }
        } else {
            if (sceMcOpen(mc.now_port, 0, mc.file[mc.list].name, 0x202)) {
                mc.mode = -6;
            } else {
                mc.mode = 5;
            }
        }
        break;
    case 15:
        if (ret < 0 && ret != -4) {
            mc.mode = -8;
            break;
        }
        if (mc.command == 5) {
            if (mc.list == 3) {
                mc.mode = 0;
            } else if (++mc.list < 3) {
                if (sceMcDelete(mc.now_port, 0, mc.file[mc.list].name)) {
                    mc.mode = -6;
                } else {
                    mc.mode = 7;
                }
            } else {
                if (sceMcChdir(mc.now_port, 0, "/", 0)) {
                    mc.mode = -6;
                } else {
                    mc.mode = 7;
                }
            }
        } else if (mc.command == 3) {
            if (mc.mode == 7) {
                if (sceMcOpen(mc.now_port, 0, mc.file[mc.list].name, 0x202)) {
                    mc.mode = -8;
                } else {
                    mc.mode = 5;
                }
            } else {
                if (sceMcMkdir(mc.now_port, 0, mc.dir) == 0) {
                    mc.mode = 3;
                } else {
                    mc.mode = -8;
                }
            }
        }
        break;
    }
}
