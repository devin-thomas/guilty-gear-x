/* Guilty Gear X US (SLUS-202.87): 5 functions ported from the X Plus
   decomp (ggxeuc/cmn_se.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/cmn_se.h"

int sound_request(int no);



void ReqCmnSE_Jump(CHARACTER_WORK *offset) {
    static unsigned char JumpSETb_ByIdno[19] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0 };

    if (offset->ply->JumpDir & 0x20) {
        sound_request(3);
    } else if (offset->ply->JumpDir & 4) {
        sound_request(3);
    } else {
        sound_request(JumpSETb_ByIdno[offset->idno]);
    }
}



void ReqCmnSE_JumpLand(CHARACTER_WORK *offset) {
    static unsigned char JumpLandSETb_ByIdno[19] = { 6, 7, 7, 10, 6, 7, 8, 7, 7, 6, 7, 9, 6, 7, 7, 7, 9, 0, 0 };

    sound_request(JumpLandSETb_ByIdno[offset->idno]);
}




void ReqCmnSE_Dash(CHARACTER_WORK *offset) {
    static unsigned char DashSETb_ByIdno[19] = { 11, 12, 12, 11, 11, 12, 12, 12, 14, 11, 12, 14, 11, 12, 14, 14, 14, 0, 0 };

    sound_request(DashSETb_ByIdno[offset->idno]);
}





void ReqCmnSE_Reduce(CHARACTER_WORK *offset) {
    static unsigned char ReduceSETb_ByIdno[19] = { 16, 16, 16, 16, 16, 16, 16, 16, 0, 16, 16, 0, 16, 16, 0, 0, 0, 0, 0 };

    if (ReduceSETb_ByIdno[offset->idno]) {
        sound_request(ReduceSETb_ByIdno[offset->idno]);
    }
}


void ReqCmnSE_SkyDash(CHARACTER_WORK *offset) {
    sound_request(0x12);
}



void ReqCmnSE_DownLand(CHARACTER_WORK *offset) {
    static unsigned char DownLandSETb_ByIdno[19] = { 34, 35, 35, 34, 34, 35, 36, 35, 35, 34, 35, 35, 34, 35, 35, 35, 34, 0, 0 };

    sound_request(DownLandSETb_ByIdno[offset->idno]);
}


void ReqCmnSE_Ukemi(CHARACTER_WORK *offset) {
    sound_request(0x25);
}

void ReqCmnSE_Sousai(CHARACTER_WORK *offset) {
    sound_request(0x16);
    sound_request(0x21);
}

void ReqCmnSE_PerfectGuard(CHARACTER_WORK *offset) {
    sound_request(0x18);
}

int IsBattleDecision(void);
int Random(void);
void voice_off(CHARACTER_WORK *offset);
int voice_request(CHARACTER_WORK *offset, int no);

CHARACTER_WORK *GetEnemyPlayer(CHARACTER_WORK *offset);
void voice_off(CHARACTER_WORK *offset);
int voice_request(CHARACTER_WORK *offset, int no);

int Random(void);
void voice_off(CHARACTER_WORK *offset);
int voice_request(CHARACTER_WORK *offset, int no);

CHARACTER_WORK *GetEnemyPlayer(CHARACTER_WORK *offset);
void voice_off(CHARACTER_WORK *offset);
int voice_request(CHARACTER_WORK *offset, int no);
extern GAME_INFOMATION g_info;

void ReqCmnVoice_Damage(CHARACTER_WORK *offset) {
    static unsigned char LV0TB[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static unsigned char LV1TB[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    static unsigned char LV2TB[16] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
    static unsigned char DEADTB[80] = {
        1, 8, 0, 0, 0,
        2, 9, 10, 11, 1,
        2, 9, 10, 11, 14,
        3, 8, 9, 10, 0,
        3, 9, 10, 11, 0,
        2, 9, 10, 0, 0,
        2, 9, 10, 0, 0,
        2, 9, 10, 11, 4,
        3, 9, 10, 11, 0,
        3, 9, 10, 11, 0,
        2, 9, 10, 0, 0,
        2, 9, 10, 0, 0,
        2, 9, 10, 0, 0,
        2, 9, 10, 0, 0,
        2, 7, 8, 0, 0,
        2, 8, 9, 0, 0,
    };
    int reqno;
    unsigned char lv;
    unsigned char prob;
    unsigned char rnd;
    CHARACTER_WORK *enemy;
    int index;

    lv = offset->DamParam->dno;
    prob = offset->DamParam->dprob;
    rnd = Random();
    if (!offset->ply) {
        return;
    }
    if (offset->HitPoint <= 0) {
        enemy = offset->ply->Enemy;
        if (IsBattleDecision()) {
            return;
        }
        index = (offset->idno - 1) * 5;
        if (DEADTB[index + 4] == enemy->idno) {
            reqno = DEADTB[index + 3];
        } else {
            reqno = DEADTB[index + rnd % DEADTB[index] + 1];
        }
    } else {
        if (!prob) {
            return;
        }
        if (prob < rnd) {
            return;
        }
        if (lv == 0) {
            reqno = LV0TB[offset->idno - 1];
        } else if (lv == 1) {
            reqno = LV1TB[offset->idno - 1];
        } else {
            reqno = LV2TB[offset->idno - 1];
        }
    }
    if (!~reqno) {
        return;
    }
    voice_off(offset);
    voice_request(offset, reqno);
}

void ReqCmnVoice_Guard(CHARACTER_WORK *offset) {
    static unsigned char LV0TB[16] = { 255, 6, 6, 5, 6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 5, 6 };
    static unsigned char LV1TB[16] = { 6, 7, 7, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 6 };
    static unsigned char LV2TB[16] = { 7, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 7 };
    int reqno;
    unsigned char lv;
    unsigned char prob;
    unsigned char rnd;

    lv = offset->DamParam->gno;
    prob = offset->DamParam->gprob;
    rnd = Random();
    if (!prob) {
        return;
    }
    if (prob < rnd) {
        return;
    }
    if (lv == 0) {
        reqno = LV0TB[offset->idno - 1];
    } else if (lv == 1) {
        reqno = LV1TB[offset->idno - 1];
    } else {
        reqno = LV2TB[offset->idno - 1];
    }
    if (!~reqno) {
        return;
    }
    voice_off(offset);
    voice_request(offset, reqno);
}






void ReqBKVoice_EntryC(CHARACTER_WORK *offset) {
    static unsigned char VoiceTb[19] = { 0, 27, 24, 25, 25, 26, 27, 24, 27, 28, 27, 27, 25, 26, 26, 24, 25, 0, 0 };
    int idno;

    idno = GetEnemyPlayer(offset)->idno;
    voice_off(offset);
    voice_request(offset, VoiceTb[idno]);
}


void ReqKYVoice_EntryC(CHARACTER_WORK *offset) {
    static unsigned char VoiceTb[] = { 22, 23, 24 };
    CHARACTER_WORK *eoffset;

    eoffset = GetEnemyPlayer(offset);
    voice_off(offset);
    voice_request(offset, VoiceTb[g_info.PlayerVoice[eoffset->padid]]);
}