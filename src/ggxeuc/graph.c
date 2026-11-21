/* Guilty Gear X US (SLUS-202.87): 8 functions ported from the X Plus
   decomp (ggxeuc/graph.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/graph.h"

void MakeFrameBuffer(void) {
}


void BackColor(u_int rgb)
{
    back_color = rgb;
}

void PacketBegin(void);
void PacketEnd(int wait);

void GraphBegin(void)
{
    PacketBegin();
    MakeFullBuffer();
    PacketEnd(1);
    PacketBegin();
}

int sceGsSyncPath(int mode, u_short timeout);
int sceGsSyncV(int mode);

void GraphEnd(void)
{
    PacketEnd(1);
    sceGsSyncPath(0, 0);
    sceGsSyncV(0);
    PacketBegin();
    GraphConv();
    PacketEnd(1);
    sceGsSyncPath(0, 0);
    frame++;
}

int abs(int x);








void GS_runtime_preset(void)
{
    *(volatile u_long *)0x12000000 = 0x7F27;
    *(volatile u_long *)0x12000070 = 0x1400;
    *(volatile u_long *)0x12000090 = 0x1400;
    *(volatile u_long *)0x12000080 = G_DISPLAY(disp.display.DX + 636, disp.display.DY + 50, 3, 0, dispoffset.sizex, dispoffset.sizey);
    *(volatile u_long *)0x120000A0 = G_DISPLAY(disp.display.DX + 636, disp.display.DY + 51, 3, 0, dispoffset.sizex, dispoffset.sizey);
    *(volatile u_long *)0x120000D0 = 0;
}


void GraphSetOffset(int x, int y)
{
    dispoffset.sizey = 0x1BF - abs(y);
    dispoffset.x = x;
    dispoffset.sizex = (640 - abs(x)) * 4 - 1;
    dispoffset.y = y;
    if (x > 0) {
        disp.display.DX = x * 4;
    } else {
        disp.display.DX = 0;
    }
    if (y > 0) {
        disp.display.DY = y;
    } else {
        disp.display.DY = 0;
    }
}

sceDmaChan *sceDmaGetChan(int id);

u_long *PacketDmaTagCnt(int id);
u_long *PacketPackRegWithTag(int id, u_long reg, u_long data);
u_long *PacketGifTag(int id, u_long tag, u_long taglist);
void PacketPointerSet(int id, u_long *ptr);

void MakeFullBuffer(void)
{
    u_long *ptr;

    PacketDmaTagCnt(0);
    PacketPackRegWithTag(0, 0x4C, 0xA008C);
    PacketPackRegWithTag(0, 0x40, 0x01BF0000027F0000UL);
    PacketPackRegWithTag(0, 0x42, 0x80000000A8UL);
    ptr = PacketGifTag(0, 0x4400000000000001UL, 0x5510);
    *ptr++ = 0x146;
    *ptr++ = back_color;
    *ptr++ = 0x79006C00;
    *ptr++ = 0x95009400;
    PacketPointerSet(0, ptr);
}

sceDmaEnv *sceDmaGetEnv(sceDmaEnv *env);
int sceDmaPutEnv(sceDmaEnv *env);

u_long *PacketPackRegsWithTag(int id, u_long *reglist, int num);

void GraphConv(void)
{
    u_long reg[8] = {
        0x50, 0xA0000000A1180UL,
        0x51, 0,
        0x52, 0x1C000000280UL,
        0x53, 2,
    };
    int sx;
    int sy;
    int sizex;
    int sizey;

    if (dispoffset.x >= 0) {
        sx = 0;
        sizex = 640;
    } else {
        sx = -dispoffset.x;
        sizex = dispoffset.x + 640;
    }
    sy = dispoffset.y;
    if (sy >= 0) {
        sy = 0;
        sizey = 448;
    } else {
        sizey = sy + 448;
        sy = -sy;
    }
    reg[5] = sizex | ((u_long)sizey << 32);
    reg[3] = sx | ((u_long)sy << 16);
    PacketDmaTagCnt(3);
    PacketPackRegsWithTag(3, reg, 4);
    PacketPackRegWithTag(3, 0x42, 0x8000000064UL);
}

#define G_DISPLAY(dx, dy, magh, magv, dw, dh) ((u_long)(dx) | ((u_long)(dy) << 12) | ((u_long)(magh) << 23) | ((u_long)(magv) << 27) | ((u_long)(dw) << 32) | ((u_long)(dh) << 44))


int sprintf(char *buf, const char *fmt, ...);
int sceOpen(const char *name, int flags, ...);
int sceWrite(int fd, const void *buf, int nbyte);
int sceClose(int fd);

void sceDevVif0Reset(void);
void sceDevVu0Reset(void);
void sceGsResetPath(void);
void sceDmaReset(int mode);
void sceDevGifPutImtMode(int mode);
void PacketInit(void);
void sceGsResetGraph(short mode, short inter, short omode, short ffmode);
void sceGsSetDefDispEnv(sceGsDispEnv *disp, short psm, short w, short h, short dx, short dy);
void sceGsSetDefDrawEnv(sceGsDrawEnv1 *draw, short psm, short w, short h, short ztest, short zpsm);
void PacketSendNow(int id);
void sceGsPutDispEnv(sceGsDispEnv *disp);

void GraphInit(void)
{
    sceDmaEnv env;

    sceDevVif0Reset();
    sceDevVu0Reset();
    sceGsResetPath();
    sceDmaReset(1);
    sceDevGifPutImtMode(0);
    PacketInit();
    sceDmaGetEnv(&env);
    env.notify = 2;
    sceDmaPutEnv(&env);
    sceGsResetGraph(0, 1, 2, 0);
    sceGsSetDefDispEnv(&disp, 0, 640, 448, 0, 0);
    sceGsSetDefDrawEnv(&draw, 0, 640, 224, 0, 0);
    BackColor(0);
    PacketBegin();
    PacketDmaTagCnt(0);
    PacketPackRegWithTag(0, 0x4C, *(u_long *)&draw.frame1);
    PacketPackRegWithTag(0, 0x4E, *(u_long *)&draw.zbuf1);
    PacketPackRegWithTag(0, 0x18, *(u_long *)&draw.xyoffset1);
    PacketPackRegWithTag(0, 0x40, *(u_long *)&draw.scissor1);
    PacketPackRegWithTag(0, 0x1A, *(u_long *)&draw.prmodecont);
    PacketPackRegWithTag(0, 0x08, 0);
    PacketPackRegWithTag(0, 0x46, *(u_long *)&draw.colclamp);
    PacketPackRegWithTag(0, 0x45, *(u_long *)&draw.dthe);
    PacketPackRegWithTag(0, 0x47, *(u_long *)&draw.test1);
    PacketPackRegWithTag(0, 0x42, 0x44);
    PacketPackRegWithTag(0, 0x49, 0);
    PacketPackRegWithTag(0, 0x3B, 0x8000000000UL);
    PacketPackRegWithTag(0, 0x4A, 0);
    PacketSendNow(0);
    GraphBegin();
    GraphEnd();
    FlushCache(0);
    sceGsPutDispEnv(&disp);
    frame = 0;
}