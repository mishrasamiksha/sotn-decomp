#include "sel.h"

void func_801B642C(void) {
    D_801BC360 = 2;
    D_801BC35E = 2;
    D_801BC362 = 0;
    D_801BC366 = 0;
    D_801BC36A = 0;
    D_801BC36B = 8;
    D_801BC35A = D_801BC35C + 0x14;
}

bool func_801B6480(s32 textDialogue) {
    Primitive* prim;
    s16 firstPrimIndex;

    firstPrimIndex = g_api.AllocPrimitives(PRIM_SPRT, 7);
    g_Dialogue.primIndex[2] = firstPrimIndex;
    if (firstPrimIndex == -1) {
        g_Dialogue.primIndex[2] = 0;
        return false;
    }
    g_Dialogue.nextCharDialogue = textDialogue;
    g_Dialogue.unk3C = 0;
    g_Dialogue.primIndex[1] = -1;
    g_Dialogue.primIndex[0] = -1;
    func_801B642C();

    //! FAKE:
    if (prim && prim) {
    }

    prim = g_Dialogue.prim[0] = &g_PrimBuf[g_Dialogue.primIndex[2]];

    prim->blendMode = BLEND_VISIBLE;
    prim = g_Dialogue.prim[1] = prim->next;

    prim->blendMode = BLEND_VISIBLE;
    prim = g_Dialogue.prim[2] = prim->next;

    prim->blendMode = BLEND_VISIBLE;
    prim = g_Dialogue.prim[3] = prim->next;

    prim->blendMode = BLEND_VISIBLE;
    prim = g_Dialogue.prim[4] = prim->next;

    prim->blendMode = BLEND_VISIBLE;
    prim = g_Dialogue.prim[5] = prim->next;

    prim->type = PRIM_GT4;
    prim->blendMode = BLEND_VISIBLE;

    prim = prim->next;
    prim->type = PRIM_G4;
    prim->b0 = prim->b1 = 0x7F;
    prim->b2 = prim->b3 = 0;
    prim->g0 = prim->g1 = prim->g2 = prim->g3 = 0;
    prim->r0 = prim->r1 = prim->r2 = prim->r3 = 0;
    prim->x0 = prim->x2 = 4;
    prim->x1 = prim->x3 = 0xF8;
    prim->priority = 0x1FD;
    prim->blendMode = BLEND_VISIBLE;

    prim = prim->next;
    prim->type = PRIM_TILE;
    prim->r0 = prim->g0 = prim->b0 = 16;
    prim->x0 = 3;
    prim->y0 = 0x2F;
    prim->v0 = 0x4A;
    prim->priority = 0x1FC;
    prim->blendMode = BLEND_VISIBLE;
    return true;
}

void func_801B6648(s16 yOffset) {
    RECT rect;

    rect.y = (yOffset * 12) + 384;
    rect.w = 64;
    rect.x = 0;
    rect.h = 12;
    ClearImage(&rect, 0, 0, 0);
}

void func_801B66A4(void) {
    Primitive* prim;

    func_801B6648(g_Dialogue.nextCharY);
    prim = g_Dialogue.prim[g_Dialogue.nextCharY];
    prim->tpage = 0x10;
    prim->clut = g_Dialogue.clutIndex;
    prim->y0 = g_Dialogue.nextLineY;
    prim->u0 = 0;
    prim->x0 = g_Dialogue.startX;
    prim->x0 = prim->x0 + 4;
    prim->v0 = g_Dialogue.nextCharY * 0xC - 0x80;
    prim->u1 = 0xC0;
    prim->v1 = 0xC;
    prim->priority = 0x1FF;
    prim->blendMode = 0;
}

// Creates primitives for the actor name at the head of the dialogue
void func_801B675C(u16 actorIndex, Entity* self) {
    Primitive* prim;
    s16 primIndex;
    s32 x;
    u16 chCount;
    const char* actorName;
    char ch;

    actorName = D_801808D0[actorIndex];
    chCount = 0;
    while (true) {
        ch = *actorName++;
        if (ch == DIAG_EOL) {
            ch = *actorName++;
            if (ch == DIAG_EOS) {
                break;
            }
        }
        if (ch == MENUCHAR(' ')) {
            continue;
        }
        chCount++;
    }

    // Create chCount amount of sprites based on the actor name's letter count
    primIndex = g_api.AllocPrimitives(PRIM_SPRT, chCount);
    if (primIndex == -1) {
        DestroyEntity(self);
        return;
    }

    // Fill prims to render the actor name on screen
    prim = &g_PrimBuf[primIndex];
    g_Dialogue.primIndex[1] = primIndex;
    actorName = D_801808D0[actorIndex];
    x = 0x38;
    while (prim != NULL) {
        ch = *actorName++;
        if (ch == MENUCHAR(' ')) {
            x += FONT_SPACE;
        } else {
            prim->type = PRIM_SPRT;
            prim->tpage = 0x1E;
            prim->clut = 0x196;
            prim->u0 = (ch & 0x0F) * FONT_W;
            prim->v0 = (ch & 0xF0) / (FONT_H / 4);
            prim->v1 = FONT_H;
            prim->u1 = FONT_W;
            prim->priority = 0x1FF;
            prim->blendMode = BLEND_VISIBLE;
            prim->x0 = x;
            prim->y0 = g_Dialogue.startY + 6;
            prim = prim->next;
            x += FONT_GAP;
        }
    }
}

void func_801B68E0(s32 arg0) {
    D_801BC394 = arg0 + 0x100000;
    D_801BC392 = 0;
    D_801BC390 = 1;
}

void func_801B690C(u8 ySteps, Entity* self) {
    s32 primIndex = g_Dialogue.nextCharY + 1;
    Primitive* prim;
    s32 i;

    while (primIndex >= 5) {
        primIndex -= 5;
    }

    if (self->step_s == 0) {
        prim = g_Dialogue.prim[primIndex];
        prim->v1 -= ySteps;
        prim->v0 = ySteps + prim->v0;
        if (prim->v1 == 0) {
            self->step_s++;
            prim->blendMode = 8;
        }
    }

    for (i = 0; i < 5; i++) {
        if (i != primIndex) {
            prim = g_Dialogue.prim[i];
            prim->y0 -= ySteps;
        }
    }
    g_Dialogue.portraitAnimTimer++;
}

INCLUDE_ASM("asm/us/st/sel/nonmatchings/3642C", func_801B69F8);

INCLUDE_ASM("asm/us/st/sel/nonmatchings/3642C", func_801B76F0);

void func_801B786C(s16 arg0) {
    RECT rect;
    rect.x = 0x200;
    rect.y = arg0 * 16;
    rect.w = 0x80;
    rect.h = 0x10;

    ClearImage(&rect, 0, 0, 0);
}

INCLUDE_ASM("asm/us/st/sel/nonmatchings/3642C", func_801B78BC);

INCLUDE_ASM("asm/us/st/sel/nonmatchings/3642C", func_801B79D4);
