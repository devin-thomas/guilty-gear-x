/* Guilty Gear X US (SLUS-202.87): 2 functions ported from the X Plus
   decomp (ggxeuc/main.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/main.h"


void func_001058A8(void);
void func_00195068(void);


int main(int argc, char **argv)
{
    sceSifInitRpc(0);
    sceCdInit(0);
    sceCdMmode(1);
    debugf("%s\n", "IOPRP23.IMG");
    while (!sceSifRebootIop("cdrom0:\\IOPRP23.IMG;1")) {
    }
    while (!sceSifSyncIop()) {
    }
    sceSifInitRpc(0);
    sceCdInit(0);
    sceCdMmode(1);
    sceFsReset();
    debugf("CD Init Over\n");
    FileIRXLoad("SIO2MAN.IRX");
    FileIRXLoad("MCMAN.IRX");
    FileIRXLoad("MCSERV.IRX");
    FileIRXLoad("PADMAN.IRX");
    FileIRXLoad("LIBSD.IRX");
    FileIRXLoad("SNDRV.IRX");
    debugf("IRX Init Over\n");
    GraphInit();
    TextRead();
    PadReadInit();
    FileInit();
    FadeInit();
    MemoryInit("BASLUS-20287GGX");
    MeterInit();
    JobInit();
    TotalCounter = 0;
    heap_alloc();
    debugf("Initialize Over\n");
    while (!sceGsSyncV(0)) {
    }
    while (1) {
        GS_runtime_preset();
        MeterReset();
        GraphBegin();
        func_001058A8();
        func_00195068();
        SpriteInit();
        TextColor(0xFFFFFF);
        TextPutD(0x28, 5, JobMode, 4);
        MeterColorChange(0xFF);
        CreditExecute();
        JobMain();
        SpriteDisplay();
        FadeExecute();
        SongContExe();
        FileReadMain();
        MemoryMain();
        MeterColorChange(0xFFFFFF);
        GraphEnd();
        TotalCounter++;
    }
}

void debugf(char *format, ...) {
}

void fdebugf(FILE *stream, char *format, ...) {
}

int sceSifInitRpc(unsigned int mode);
int sceCdInit(int init_mode);
int sceCdMmode(int media);
int sceSifRebootIop(const char *img);
int sceSifSyncIop(void);
int sceFsReset(void);
int sceGsSyncV(int mode);
void FileIRXLoad(char *fname);
void GraphInit(void);
void TextRead(void);
void PadReadInit(void);
void FileInit(void);
void FadeInit(void);
void MemoryInit();
void MeterInit(void);
void JobInit(void);
void heap_alloc(void);
void GS_runtime_preset(void);
void MeterReset(void);
void GraphBegin(void);
void PadReadMain(void);
void SnDrvMain(void);
void SpriteInit(void);
void TextColor(int rgb);
void TextPutD(u_int x, u_int y, long int data, u_int num);
int MeterColorChange(int col);
int CreditExecute(void);
void JobMain(void);
void SpriteDisplay(void);
void FadeExecute(void);
int SongContExe(void);
void FileReadMain(void);
void MemoryMain(void);
void GraphEnd(void);
void soft_reset_check(void);
extern u_int TotalCounter;
extern int JSTokorotenFlag;
extern int JobMode;