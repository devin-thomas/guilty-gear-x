/* Guilty Gear X US (SLUS-202.87): declarations from the X Plus decomp
   (ggxeuc/skillref.c); each function byte-checked by the build gate. */
#include "generated/skillref.h"

static int inst_ypos[18] = { 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70 };
void polydraw_FBox(float x1, float y1, float x2, float y2, float z, unsigned int color1);
extern short int UseCharacter1p2p[];
static SKILLREF sr;
extern USER_SETTING userinfo;
void FontDraw(char *str, int x, int y, float z, int alfa);
void FontDrawDC(char *str, float x, float y, float z, int alpha, int id);
char *G_sprintf(char *buff, char *str, int *valtb);
void SkillDraw(int no, int side, int x, int y, int z, int alpha);
void CommandDraw(char *str, int x, int y, int z, int alpha);
extern DCFONT_INFO fi;
void gSpriteDraw_NaomiLibVersion(NL_SPR_ARG *sprite);

extern int D_548FC8[];

extern GAME_INFOMATION g_info;
extern TRAINING_SETTING *tr_info;
extern SYS_COND sc;
extern PAD pads[2];
int sound_request(int no);
void func_0016D150(int id);


void func_0016CF90(int sw)
{
    static int id;

    if (sw == 0) {
        if (g_info.mode & 0x100) id = UseCharacter1p2p[tr_info->ply];
        else id = UseCharacter1p2p[sc.grpvar[0] - 1];
        return;
    }
    if ((pads[0].trg | pads[1].trg) & 0x8000) {
        if (--id == 0) id = 16;
        if (id == 11 && (sc.kakusi | userinfo.phase) <= 0) id = 10;
        if (id == 16 && (sc.kakusi | userinfo.phase) < 2) id = 15;
    }
    if ((pads[0].trg | pads[1].trg) & 0x2000) {
        if (++id > 16) id = 1;
        if (id == 11 && (sc.kakusi | userinfo.phase) <= 0) id = 12;
        if (id == 16 && (sc.kakusi | userinfo.phase) < 2) id = 1;
    }
    if ((pads[0].trg | pads[1].trg) & 0x10) {
        sound_request(0x3b);
        sc.grpvar[0] = 0;
        return;
    }
    func_0016D150(id);
}

extern char D_569370[];
extern char D_5694D0[];
extern char D_5695F0[];
extern char D_569730[];
extern char D_569870[];
extern char D_569970[];
extern char D_569A90[];
extern char D_569C30[];
extern char D_569D70[];
extern char D_569E70[];
extern char D_569FF0[];
extern char D_56A0B0[];
extern char D_56A250[];
extern char D_56A330[];
extern char D_56A410[];
extern char D_56A530[];
extern const char D_56A610[];


void func_0016D150(int id)
{
    int i;
    int j;
    int max;
    char command_buf[32];
    char *p;
    char *p2;
    void *adrs[16] = {
        D_569370, D_5694D0, D_5695F0, D_569730, D_569870, D_569970, D_569A90, D_569C30,
        D_569D70, D_569E70, D_569FF0, D_56A0B0, D_56A250, D_56A330, D_56A410, D_56A530,
    };
    char *name[16] = {
        "SOL BADGUY", "KY KISKE", "MAY", "MILLIA RAGE", "AXL LOW", "POTEMKIN",
        "CHIPP ZANUFF", "ZATO ONE", "BAIKEN", "FAUST", "TESTAMENT", "JAM KURADOBERI",
        "ANJI MITO", "JOHNNY", "VENOM", "DIZZY",
    };
    int val[2];

    FontDrawDC(name[id - 1], 96.0f, D_548FC8[id - 1] - 24, 1.0f, 1, 1);
    max = D_56A610[id - 1];
    p = adrs[id - 1];
    for (i = 0; i < max; i++) {
        p2 = p + i * 32;
        for (j = 1; p2[j] != -1; j++) {
            command_buf[j - 1] = p2[j] + '0';
        }
        command_buf[j - 1] = 0;
        val[0] = i + 1;
        FontDraw(G_sprintf(0, "%2d", val), 0x40, D_548FC8[id - 1] + i * 32 + 12, 1.0f, 0xff);
        CommandDraw(command_buf, 0x60, D_548FC8[id - 1] + i * 32, 1, 1);
        func_0016D420(id, p2[0], i);
    }
}




void func_0016D420(int id, int type, int cnt)
{
    int col;

    if (type == 0) {
        col = 0x800000c0;
    } else if (type == 1) {
        col = 0x8000c000;
    } else if (type == 2) {
        col = 0x80c00000;
    } else {
        return;
    }
    polydraw_FBox(96.0f, D_548FC8[id - 1] + cnt * 32, 560.0f, D_548FC8[id - 1] + cnt * 32 + 32, 2.0f, col);
}









void CommandDraw(char *str, int x, int y, int z, int alpha)
{
    int u;
    int v;
    float off;
    float len;
    NL_SPR_ARG sprite;

    sprite.x = x;
    sprite.y = y;
    sprite.z = z;
    sprite.trnsl = alpha;
    sprite.listType = 1;
    sprite.attr = 0x205;
    sprite.zm_x = 0.125f;
    sprite.zm_y = 0.125f;
    sprite.ang = 0;
    while (*str) {
        len = 32.0f;
        sprite.zm_x = len * 0.00390625;
        off = len;
        if (*str == '0') {
            u = 4;
            v = 0;
        } else if (*str == '1') {
            u = 4;
            v = 1;
        } else if (*str == '2') {
            u = 4;
            v = 2;
        } else if (*str == '3') {
            u = 4;
            v = 3;
        } else if (*str == '4') {
            u = 3;
            v = 1;
        } else if (*str == '5') {
            u = 2;
            v = 1;
        } else if (*str == '6') {
            u = 3;
            v = 0;
        } else if (*str == '7') {
            u = 2;
            v = 0;
        } else if (*str == '8') {
            u = 3;
            v = 2;
        } else if (*str == '9') {
            u = 2;
            v = 2;
        } else if (*str == ':') {
            u = 1;
            v = 1;
        } else if (*str == ';') {
            u = 0;
            v = 0;
        } else if (*str == '<') {
            u = 1;
            v = 0;
        } else if (*str == '=') {
            u = 1;
            v = 2;
        } else if (*str == '>') {
            u = 0;
            v = 1;
        } else if (*str == '?') {
            u = 1;
            v = 3;
        } else if (*str == '@') {
            u = 0;
            v = 2;
        } else if (*str == 'A') {
            u = 0;
            v = 3;
        } else if (*str == 'B') {
            u = 0;
            off = 24.0f;
            v = 4;
        } else if (*str == 'C') {
            u = 1;
            v = 4;
        } else if (*str == 'D') {
            len = 128.0f;
            u = 1;
            off = 96.0f;
            v = 4;
            sprite.zm_x = len * 0.00390625;
        } else if (*str == 'E') {
            len = 64.0f;
            u = 1;
            v = 4;
            sprite.zm_x = len * 0.00390625;
            off = len;
        } else if (*str == 'F') {
            len = 64.0f;
            u = 3;
            off = 42.0f;
            v = 5;
            sprite.zm_x = len * 0.00390625;
        } else if (*str == 'G') {
            len = 128.0f;
            u = 0;
            off = 112.0f;
            v = 5;
            sprite.zm_x = len * 0.00390625;
        } else if (*str == 'H') {
            len = 80.0f;
            u = 0;
            off = 78.0f;
            v = 6;
            sprite.zm_x = len * 0.00390625;
        } else if (*str == 'I') {
            len = 96.0f;
            u = 1;
            v = 6;
            sprite.zm_x = len * 0.00390625;
            off = len;
        } else if (*str == 'J') {
            u = 6;
            off = 16.0f;
            v = 6;
        } else if (*str == 'K') {
            len = 32.0f;
            u = 7;
            v = 6;
            sprite.zm_x = len * 0.00390625;
            off = len;
        } else if (*str == 'L') {
            len = 128.0f;
            u = 0;
            v = 7;
            sprite.zm_x = len * 0.00390625;
            off = len;
        } else if (*str == 'M') {
            len = 128.0f;
            u = 1;
            off = 120.0f;
            v = 7;
            sprite.zm_x = len * 0.00390625;
        } else {
            goto next;
        }
        u = u * len;
        sprite.sprno = fi.font[3].spno;
        v = v * 32.0;
        sprite.u0 = u * 0.00390625;
        sprite.v0 = v * 0.00390625;
        sprite.u1 = (u + len) * 0.00390625;
        sprite.v1 = (v + 32.0) * 0.00390625;
        gSpriteDraw_NaomiLibVersion(&sprite);
next:
        sprite.x += off;
        str++;
    }
}