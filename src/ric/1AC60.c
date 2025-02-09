/*
 * File: 1AC60.c
 * Overlay: RIC
 * Description: Overlay for the character Richter.
 */

#include "ric.h"
#include "player.h"

void func_80156C60(Entity* entity) {
    s32 i;
    s32 length;
    u32* ptr;

    if (entity->flags & FLAG_HAS_PRIMS) {
        g_api.FreePrimitives(entity->primIndex);
    }

    ptr = (u32*)entity;
    length = sizeof(Entity) / sizeof(u32);
    for (i = 0; i < length; i++)
        *ptr++ = 0;
}

TeleportCheck GetTeleportToOtherCastle(void) {
    s32 xCheckTop;
    s32 yCheckTop;
    s32 xCheckRTop;
    s32 yCheckRTop;

    // Is player in the pose when pressing UP?
    if (PLAYER.step != 0 || PLAYER.step_s != 1) {
        return TELEPORT_CHECK_NONE;
    }

    // Check for X/Y boundaries in TOP
    if (g_StageId == STAGE_TOP) {
        xCheckTop = (g_CurrentRoom.left << 8) + playerX - 8000;
        if (ABS(xCheckTop) < 4) {
            yCheckTop = (g_CurrentRoom.top << 8) + playerY - 2127;
            if (ABS(yCheckTop) < 4) {
                return TELEPORT_CHECK_TO_RTOP;
            }
        }
    }

    // Check for X/Y boundaries in RTOP
    if (g_StageId == STAGE_RTOP) {
        xCheckRTop = (g_CurrentRoom.left << 8) + playerX - 8384;
        if (ABS(xCheckRTop) < 4) {
            yCheckRTop = (g_CurrentRoom.top << 8) + playerY;
            if (ABS(yCheckRTop) - 14407 < 4) {
                return TELEPORT_CHECK_TO_TOP;
            }
        }
    }

    return TELEPORT_CHECK_NONE;
}

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_80156DE4);

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_80156F40);

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_801572A8);

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_80157894);

// DECOMP_ME_WIP func_80157A50 https://decomp.me/scratch/hk5zb
// Almost matched, just a jump issue
INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_80157A50);

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", UpdateEntityRichter);

void func_801587C0() { D_80175956 = 0; }

void func_801587D0(void) {
    D_80175956 = 1;
    D_80175950 = PLAYER.animCurFrame;
    D_80175952 = PLAYER.drawFlags;
    D_80175954 = PLAYER.palette;
}

void func_80158814(void) {
    D_80175956 = 0;
    PLAYER.unk44 = 0;
    PLAYER.animCurFrame = D_80175950;
    PLAYER.drawFlags = *(u8*)&D_80175952;
    PLAYER.palette = D_80175954;
}

bool func_8015885C(void) {
    if (D_80175956 == 0) {
        if (g_Player.padTapped & PAD_L2) {
            if (g_Player.D_80072EFC == 0) {
                func_801587D0();
                return true;
            }
        }
        return false;
    }

    if ((g_Player.D_80072EFC != 0) || (g_Player.padTapped & PAD_L2)) {
        func_80158814();
        return false;
    }

    if (g_Player.padPressed & PAD_CROSS) {
        if (g_Player.padPressed & PAD_RIGHT) {
            g_Entities->posX.val += FIX(16.0);
        }
        if (g_Player.padPressed & PAD_LEFT) {
            g_Entities->posX.val -= FIX(16.0);
        }
        if (g_Player.padPressed & PAD_UP) {
            PLAYER.posY.val -= FIX(16.0);
        }
        if (g_Player.padPressed & PAD_DOWN) {
            PLAYER.posY.val += FIX(16.0);
        }

    } else {
        if (g_Player.padTapped & PAD_RIGHT) {
            g_Entities->posX.val += FIX(16.0);
        }
        if (g_Player.padTapped & PAD_LEFT) {
            g_Entities->posX.val -= FIX(16.0);
        }
        if (g_Player.padTapped & PAD_UP) {
            PLAYER.posY.val -= FIX(16.0);
        }
        if (g_Player.padTapped & PAD_DOWN) {
            PLAYER.posY.val += FIX(16.0);
        }
    }

    if (g_Player.padTapped & PAD_CIRCLE) {
        PLAYER.animCurFrame--;
    }
    if (g_Player.padTapped & PAD_SQUARE) {
        PLAYER.animCurFrame++;
    }

    if (PLAYER.animCurFrame <= 0) {
        PLAYER.animCurFrame = 1;
    }
    if (PLAYER.animCurFrame < 212 == 0) {
        PLAYER.animCurFrame = 211;
    }
    FntPrint("charal:%02x\n", PLAYER.animCurFrame);
    return true;
}

void func_80158B04(s32 arg0) {
    s32 var_s0;

    if (PLAYER.facingLeft != 0) {
        var_s0 = -3;
    } else {
        var_s0 = 3;
    }

    PLAYER.posY.i.hi -= 16;
    PLAYER.posX.i.hi = var_s0 + PLAYER.posX.i.hi;
    func_801606BC(g_CurrentEntity, 0x10004, 0);
    PLAYER.posY.i.hi += 16;
    PLAYER.posX.i.hi = PLAYER.posX.i.hi - var_s0;

    if (arg0 & 1) {
        g_api.func_80102CD8(3);
        g_api.PlaySfx(NA_SE_SECRET_STAIRS);
    }

    if (arg0 & 2) {
        PLAYER.velocityX = 0;
        PLAYER.velocityY = 0;
    }
}

void func_80158BFC(void) {
    s32 var_s0;

    if (PLAYER.step < 64) {
        if (D_8015459C != 0) {
            D_8015459C--;
        } else if (D_80097448[0] >= 49) {
            if (PLAYER.facingLeft != 0) {
                var_s0 = -4;
            } else {
                var_s0 = 4;
            }
            PLAYER.posX.i.hi = var_s0 + PLAYER.posX.i.hi;
            PLAYER.posY.i.hi -= 16;
            func_801606BC(g_CurrentEntity, 0x80004, 0);
            D_8015459C = 0x60;
            PLAYER.posY.i.hi += 16;
            PLAYER.posX.i.hi = PLAYER.posX.i.hi - var_s0;
        }
    }

    if (func_8015DBB0(0x40000 | 0x305C) == false) {
        func_8015C93C(0x2000);
        switch (PLAYER.step_s) {
        case 0:
            if (func_8015C9CC() == 0) {
                if (g_Player.padPressed & PAD_UP) {
                    func_8015C920(&D_80155394);
                    PLAYER.step_s = 1;
                    break;
                }
            } else {
                func_8015CDE0(0);
            }
            break;

        case 1:
            if (func_8015C9CC() != 0) {
                func_8015CDE0(0);
                break;
            } else if (g_Player.padPressed & PAD_UP) {
                break;
            } else {
                func_8015CD98(0);
                break;
            }

        case 64:
            func_8015CB58(1, 1);
            if (PLAYER.animFrameIdx < 3) {
                func_8015C9CC();
                if (g_Player.padPressed & PAD_DOWN) {
                    PLAYER.step = 2;
                    PLAYER.unk4C = &D_801555A8;
                    break;
                }
            }

            if (PLAYER.animFrameDuration < 0) {
                if (g_Player.padPressed & PAD_SQUARE) {
                    g_Player.unk46 = 2;
                    PLAYER.step_s++;
                    func_8015C920(&D_80155730);
                    func_801606BC(g_CurrentEntity, 0x11, 0);
                    break;
                }
                g_Player.unk46 = 0;
                func_8015CD98(0);
            }
            break;

        case 65:
            func_8015CB58(1, 1);
            if (g_Player.padPressed & PAD_SQUARE) {
                break;
            }
            g_Player.unk46 = 0;
            func_8015CD98(0);
            break;

        case 66:
            func_8015CB58(1, 1);
            if (PLAYER.animFrameIdx < 3) {
                func_8015C9CC();
            }
            if (PLAYER.animFrameDuration < 0) {
                g_Player.unk46 = 0;
                func_8015CD98(0);
            }
        }
    }
}

void func_80158F38(void) {
    if (func_8015DBB0(0x305C) == 0) {
        func_8015C93C(0x2000);
        if (func_8015C9CC() == 0) {
            func_8015CD98(0);
            return;
        }

        if (PLAYER.step_s == 0) {
            func_8015CA84(0x14000);
        }
    }
}

void func_80158FA4(void) {
    if (g_Player.unk7A != 0) {
        func_8015CDE0(0);
        return;
    }
    g_Player.D_80072F10 = 8;
    g_Player.D_80072F02 = 8;

    if (!func_8015DBB0(0x305C)) {
        func_8015C93C(0x2000);
        if (func_8015C9CC() == 0) {
            func_8015CD98(0);
            if (g_Player.D_80072F16 == 0) {
                if (!(g_Player.pl_vram_flag & 0xC)) {
                    func_8015C920(&D_8015539C);
                    func_801606BC(g_CurrentEntity, 0, 0);
                }
            } else {
                PLAYER.velocityX = 0;
            }
        } else if (PLAYER.step_s == 0) {
            func_8015CA84(0x24000);
        }
    }
}

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_801590A0);

void func_801595D8(void) {
    if (func_8015DBB0(0x9009)) {
        return;
    }
    func_8015C93C(0x1000);
    if (PLAYER.step_s != 0) {
        return;
    }
    if (g_Player.D_80072F0A != 0 && g_Player.padTapped & PAD_CROSS) {
        func_8015D020();
    } else if (func_8015C9CC() != 0) {
        func_8015CA84(0xC000);
    }
}

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_80159670);

void func_80159BC8(void) {
    PLAYER.animFrameDuration = 0;
    PLAYER.animFrameIdx = 0;
    g_Player.unk44 = 0;
    g_Player.unk46 = 0;
    PLAYER.drawFlags &= 0xFB;
}

void func_80159C04(void) {
    Entity* entity = PLAYER.ext.generic.unkB8.entityPtr;
    s16 temp_v0;
    s32 var_a0;
    s32 var_a2;

    if (entity->facingLeft != 0) {
        var_a2 = -entity->hitboxOffX;
    } else {
        var_a2 = entity->hitboxOffX;
    }

    if (PLAYER.facingLeft != 0) {
        var_a0 = -PLAYER.hitboxOffX;
    } else {
        var_a0 = PLAYER.hitboxOffX;
    }

    temp_v0 = var_a0 + PLAYER.posX.i.hi - entity->posX.i.hi - var_a2;

    if (ABS(temp_v0) < 16) {
        if (entity->velocityX != 0) {
            if (entity->velocityX < 0) {
                PLAYER.entityRoomIndex = 0;
                return;
            } else {
                PLAYER.entityRoomIndex = 1;
                return;
            }
        }
    }

    if (temp_v0 < 0) {
        PLAYER.entityRoomIndex = 0;
    } else {
        PLAYER.entityRoomIndex = 1;
    }
}

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_80159CE4);

// DECOMP_ME_WIP func_8015A7D0 https://decomp.me/scratch/1JWA0
INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_8015A7D0);

INCLUDE_ASM("asm/us/ric/nonmatchings/1AC60", func_8015A9B0);

void func_8015AFE0(void) {
    if (PLAYER.step_s == 0) {
        PLAYER.velocityY += 0x3800;
        if (PLAYER.velocityY > 0) {
            PLAYER.velocityY = 0;
            PLAYER.step_s = 1;
        }
    } else if (g_Player.unk4E != 0) {
        g_Player.unk46 = 0;
        SetPlayerStep(4);
        func_8015C920(&D_80155528);
        g_Player.unk44 = 0;
    }
    if (g_Player.unk72 != 0) {
        PLAYER.velocityY = 0;
    }
}

void func_8015B098(void) {
    if ((PLAYER.animCurFrame == 0xB5) && (PLAYER.animFrameDuration == 1)) {
        func_801606BC(g_CurrentEntity, 0x23, 0);
        g_api.PlaySfx(NA_SE_UNK_62F);
    }

    if (PLAYER.animFrameDuration < 0) {
        func_8015CD98(0);
        g_Player.unk46 = 0;
        func_801606BC(g_CurrentEntity, 0x450021, 0);
        g_Player.D_80072F00 = 0x800;
    }

    if (!(g_Player.pl_vram_flag & 1)) {
        func_8015CF08();
        g_Player.unk46 = 0;
        func_801606BC(g_CurrentEntity, 0x450021, 0);
        g_Player.D_80072F00 = 0x800;
    }
}

void func_8015B18C(void) {
    if (PLAYER.animFrameDuration < 0) {
        func_8015CD98(0);
        g_Player.unk46 = 0;
    }

    if ((g_Player.pl_vram_flag & 1) == 0) {
        func_8015CF08();
        g_Player.unk46 = 0;
    }
}

void func_8015B1E8(void) {
    if (g_Player.unk4E) {
        func_8015CD98(0);
        g_Player.unk46 = 0;
    }

    if ((g_Player.pl_vram_flag & 1) == 0) {
        func_8015CF08();
        g_Player.unk46 = 0;
    }
}

void func_8015B244(void) {
    if (PLAYER.step_s == 0) {
        D_80174F74 = 0x200;
        PLAYER.step_s++;
    } else {
        func_8015C9CC();
        D_80174F74--;
        if (D_80174F74 == 0) {
            g_Player.unk46 = 0;
            func_8015CD98(0);
            g_Player.unk4E = 1;
        }
    }
    if (g_Player.padTapped & PAD_CROSS) {
        func_8015D020();
        g_Player.unk46 = 0;
        g_Player.unk4E = 1;
        D_80174F74 = 0;
    }
    if (!(g_Player.pl_vram_flag & 1)) {
        func_8015CF08();
        g_Player.unk46 = 0;
        g_Player.unk4E = 1;
        D_80174F74 = 0;
    }
}
