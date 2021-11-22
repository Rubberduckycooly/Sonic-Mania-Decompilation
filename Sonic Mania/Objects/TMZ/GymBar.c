#include "SonicMania.h"

ObjectGymBar *GymBar;

void GymBar_Update(void) { GymBar_HandlePlayerInteractions(); }

void GymBar_LateUpdate(void) {}

void GymBar_StaticUpdate(void) {}

void GymBar_Draw(void)
{
    RSDK_THIS(GymBar);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->type) {
        drawPos.y                = self->position.y - (self->size << 18);
        self->animator.frameID = 0;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->animator.frameID = 1;
        for (int i = 0; i < self->size; ++i) {
            RSDK.DrawSprite(&self->animator, &drawPos, false);
            drawPos.y += 0x80000;
        }
    }
    else {
        drawPos.x                = self->position.x - (self->size << 18);
        self->animator.frameID = 0;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->animator.frameID = 1;
        for (int i = 0; i < self->size; ++i) {
            RSDK.DrawSprite(&self->animator, &drawPos, false);
            drawPos.x += 0x80000;
        }
    }
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
}

void GymBar_Create(void *data)
{
    RSDK_THIS(GymBar);

    self->drawFX        = FX_FLIP;
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    if (!self->type) {
        self->updateRange.x = (self->size << 18) + 0x400000;
        self->hitbox.left   = (-4 * self->size);
        self->hitbox.top    = 0;
        self->hitbox.right  = 4 * self->size - 8;
        self->hitbox.bottom = 8;
        self->field_68.x    = self->position.x - (self->size << 18) + 0x50000;
        self->field_68.y    = (self->size << 18) + self->position.x - 0xC0000;
    }
    else {
        self->updateRange.y = (self->size + 16) << 18;
        self->hitbox.left   = 0;
        self->hitbox.top    = (8 - (self->size << 2));
        self->hitbox.right  = 0;
        self->hitbox.bottom = (self->size << 2) - 16;
    }
    RSDK.SetSpriteAnimation(GymBar->aniFrames, self->type, &self->animator, true, 0);
}

void GymBar_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        GymBar->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/GymBar.bin", SCOPE_STAGE);
    GymBar->sfxBumper = RSDK.GetSfx("Stage/Bumper3.wav");

    DEBUGMODE_ADD_OBJ(GymBar);
}

void GymBar_DebugSpawn(void)
{
    RSDK_THIS(GymBar);

    CREATE_ENTITY(GymBar, NULL, self->position.x, self->position.y);
}

void GymBar_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(GymBar->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void GymBar_HandlePlayerInteractions(void)
{
    RSDK_THIS(GymBar);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (player->animator.animationID != ANI_HURT && player->state != Player_State_FlyCarried) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                if (!self->playerTimers[playerID]) {
                    player->onGround = false;
                    if (self->type) {
                        if (abs(player->velocity.x) >= 0x40000) {
                            player->position.x = self->position.x;
                            player->direction  = FLIP_NONE;
                            if (player->velocity.x <= 0)
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGV, &player->animator, false, 2);
                            else
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGV, &player->animator, false, 9);
                            player->animator.animationSpeed = ((abs(player->velocity.x) - 0x40000) >> 12) + 224;
                            player->abilityValues[0]              = 0;
                            player->abilityValues[1]              = player->animator.frameID;
                            player->abilityValues[2]              = player->velocity.x;
                            player->state                         = GymBar_PlayerState_SwingH;
                        }
                    }
                    else {
                        player->rotation   = 0;
                        player->position.x = clampVal(player->position.x, self->field_68.x, self->field_68.y);
                        player->position.y = self->position.y;
                        if (abs(player->velocity.y) < 0x40000 || self->noSwing) {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGH, &player->animator, false, 0);
                            player->animator.animationSpeed = 0;
                            player->abilityPtrs[0]                = self;
                            player->state                         = GymBar_PlayerState_Hang;
                        }
                        else {
                            if (player->velocity.y <= 0)
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGH, &player->animator, false, 2);
                            else
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGH, &player->animator, false, 9);
                            player->animator.animationSpeed = ((abs(player->velocity.y) - 0x40000) >> 12) + 256;
                            player->abilityValues[0]              = 0;
                            player->abilityValues[1]              = player->animator.frameID;
                            player->abilityValues[2]              = player->velocity.y;
                            player->state                         = GymBar_PlayerState_SwingV;
                        }
                    }
                    if (player->state == GymBar_PlayerState_Hang || player->state == GymBar_PlayerState_SwingH
                        || player->state == GymBar_PlayerState_SwingV) {
                        self->playerTimers[playerID] = 16;
                        RSDK.PlaySfx(Player->sfxGrab, false, 255);
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        player->groundVel       = 0;
                        player->jumpAbility     = 0;
                    }
                }
            }
            else if (self->playerTimers[playerID])
                self->playerTimers[playerID]--;
        }
    }
}

void GymBar_HandleSwingJump(void)
{
    RSDK_THIS(Player);

    if (!self->down)
        self->velocity.y = -0x50000;
    RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
    if (self->characterID == ID_TAILS) {
        self->animator.animationSpeed = 120;
    }
    else {
        self->animator.animationSpeed = ((abs(self->groundVel) * 0xF0) / 0x60000) + 0x30;
    }
    if (self->animator.animationSpeed > 0xF0)
        self->animator.animationSpeed = 0xF0;
    self->jumpAbility      = 1;
    self->jumpAbilityTimer = 1;
    self->abilityPtrs[0]   = NULL;
    self->abilityValues[0] = 0;
    self->abilityValues[1] = 0;
    self->abilityValues[3] = 0;
    self->state            = Player_State_Air;
}

void GymBar_PlayerState_SwingV(void)
{
    RSDK_THIS(Player);

    if (self->jumpPress)
        GymBar_HandleSwingJump();

    int frame = 11;
    if (self->abilityValues[2] <= 0)
        frame = 4;
    if (self->animator.frameID == frame && self->abilityValues[1] != frame)
        ++self->abilityValues[0];

    self->abilityValues[1] = self->animator.frameID;
    if (self->abilityValues[0] >= 2) {
        if (self->abilityValues[2] <= 0)
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_SPRINGDIAGONAL, &self->animator, false, 0);
        else
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 0);

        self->velocity.y =
            self->abilityValues[2] + (self->abilityValues[2] >> ((abs(self->abilityValues[2]) >> 18) + (abs(self->abilityValues[2]) >> 20)));
        self->abilityPtrs[0]   = NULL;
        self->abilityValues[0] = 0;
        self->abilityValues[1] = 0;
        self->abilityValues[2] = 0;
        self->state            = Player_State_Air;
    }
}

void GymBar_PlayerState_Hang(void)
{
    RSDK_THIS(Player);

    EntityGymBar *gymbar = self->abilityPtrs[0];
    if (self->left) {
        if (self->position.x > gymbar->field_68.x)
            self->position.x -= 0x10000;
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SHIMMYMOVE, &self->animator, false, 0);
        self->direction = FLIP_X;
    }
    else if (self->right) {
        if (self->position.x < gymbar->field_68.y)
            self->position.x += 0x10000;
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SHIMMYMOVE, &self->animator, false, 0);
        self->direction = FLIP_NONE;
    }
    else {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SHIMMYIDLE, &self->animator, false, 0);
    }
    if (self->jumpPress)
        GymBar_HandleSwingJump();
}

void GymBar_PlayerState_SwingH(void)
{
    RSDK_THIS(Player);

    if (self->jumpPress)
        GymBar_HandleSwingJump();

    int frame = 11;
    if (self->abilityValues[2] <= 0)
        frame = 4;
    if (self->animator.frameID == frame && self->abilityValues[1] != frame)
        ++self->abilityValues[0];

    self->abilityValues[1] = self->animator.frameID;
    if (self->abilityValues[0] >= 2) {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 0);
        self->velocity.x       = self->abilityValues[2] + (self->abilityValues[2] >> (abs(self->abilityValues[2]) >> 18));
        self->groundVel        = self->velocity.x;
        self->direction        = abs(self->velocity.x >> 31);
        self->abilityPtrs[0]   = NULL;
        self->abilityValues[0] = 0;
        self->abilityValues[1] = 0;
        self->abilityValues[2] = 0;
        self->state            = Player_State_Air;
    }
}

#if RETRO_INCLUDE_EDITOR
void GymBar_EditorDraw(void)
{
    RSDK_THIS(GymBar);
    if (!self->type) {
        self->updateRange.x = (self->size << 18) + 0x400000;
        self->field_68.x    = self->position.x - (self->size << 18) + 0x50000;
        self->field_68.y    = (self->size << 18) + self->position.x - 0xC0000;
    }
    else {
        self->updateRange.y = (self->size + 16) << 18;
    }
    RSDK.SetSpriteAnimation(GymBar->aniFrames, self->type, &self->animator, true, 0);

    GymBar_Draw();
}

void GymBar_EditorLoad(void) { GymBar->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/GymBar.bin", SCOPE_STAGE); }
#endif

void GymBar_Serialize(void)
{
    RSDK_EDITABLE_VAR(GymBar, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(GymBar, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(GymBar, VAR_BOOL, noSwing);
}
