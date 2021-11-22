#include "SonicMania.h"

ObjectLRZConvSwitch *LRZConvSwitch;

void LRZConvSwitch_Update(void)
{
    RSDK_THIS(LRZConvSwitch);

    if ((LRZ2Setup->conveyorDir != self->conveyorDir) != self->calibration)
        LRZConvSwitch_Calibrate();

    int ty1 = self->position.x;
    int ty2 = self->position.y - 0x180000;

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);

        if (self->playerPositions[playerID].x || self->playerPositions[playerID].y) {
            int dir = 0;
            if (player->position.x <= self->playerPositions[playerID].x) {
                if (player->position.x < self->playerPositions[playerID].x)
                    dir = -1;
            }
            else {
                dir = 1;
            }

            int val = MathHelpers_Unknown12(player->position.x, player->position.y, self->playerPositions[playerID].x,
                                            self->playerPositions[playerID].y, self->position.x, self->position.y, ty1, ty2);
            if (!self->dir && !player->sidekick) {
                if (dir < 0) {
                    if (val && !self->conveyorDir) {
                        RSDK.PlaySfx(LRZConvSwitch->sfxClack, false, 255);
                        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 1, &self->animator, true, 0);
                        self->conveyorDir    = 1;
                        LRZ2Setup->conveyorDir = self->calibration ^ 1;
                        self->dir            = 1;
                    }
                }
                if (dir > 0 && val && self->conveyorDir == 1) {
                    RSDK.PlaySfx(LRZConvSwitch->sfxClack, false, 255);
                    RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 3, &self->animator, true, 0);
                    self->conveyorDir    = 0;
                    LRZ2Setup->conveyorDir = self->calibration;
                    self->dir            = 1;
                }
            }
        }

        self->playerPositions[playerID].x = player->position.x;
        self->playerPositions[playerID].y = player->position.y;
    }

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.animationID == 1 && self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 2, &self->animator, true, 0);
        self->dir = 0;
    }
    if (self->animator.animationID == 3 && self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 0, &self->animator, true, 0);
        self->dir = 0;
    }
}

void LRZConvSwitch_LateUpdate(void) {}

void LRZConvSwitch_StaticUpdate(void) {}

void LRZConvSwitch_Draw(void)
{
    RSDK_THIS(LRZConvSwitch);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void LRZConvSwitch_Create(void *data)
{
    RSDK_THIS(LRZConvSwitch);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    LRZConvSwitch_Calibrate();
}

void LRZConvSwitch_StageLoad(void)
{
    LRZConvSwitch->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvSwitch.bin", SCOPE_STAGE);
    LRZConvSwitch->sfxClack  = RSDK.GetSfx("Stage/Clack.wav");
}

void LRZConvSwitch_Calibrate(void)
{
    RSDK_THIS(LRZConvSwitch);

    self->conveyorDir = self->calibration ^ LRZ2Setup->conveyorDir;
    if (!self->conveyorDir)
        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 0, &self->animator, true, 0);
    else if (self->conveyorDir == 1)
        RSDK.SetSpriteAnimation(LRZConvSwitch->aniFrames, 2, &self->animator, true, 0);
}

#if RETRO_INCLUDE_EDITOR
void LRZConvSwitch_EditorDraw(void)
{
    LRZConvSwitch_Calibrate();
    LRZConvSwitch_Draw();
}

void LRZConvSwitch_EditorLoad(void)
{
    LRZConvSwitch->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvSwitch.bin", SCOPE_STAGE);
    RSDK_ACTIVE_VAR(LRZConvSwitch, calibration);
    RSDK_ENUM_VAR("Right", LRZCONVSWITCH_RIGHT);
    RSDK_ENUM_VAR("Left", LRZCONVSWITCH_LEFT);
}
#endif

void LRZConvSwitch_Serialize(void) { RSDK_EDITABLE_VAR(LRZConvSwitch, VAR_UINT8, calibration); }
