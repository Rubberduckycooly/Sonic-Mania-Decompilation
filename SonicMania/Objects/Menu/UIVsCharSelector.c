// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIVsCharSelector Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIVsCharSelector *UIVsCharSelector;

void UIVsCharSelector_Update(void)
{
    RSDK_THIS(UIVsCharSelector);

    self->prevSelected = self->isSelected;

    if (self->textFrames != UIWidgets->textFrames || self->prevFrameID != self->frameID) {
        UIVsCharSelector_SetupText();
        self->textFrames  = UIWidgets->textFrames;
        self->prevFrameID = self->frameID;
    }

    StateMachine_Run(self->state);

    if (self->isSelected) {
        self->triBounceVelocity -= 0x600;
        self->triBounceOffset += self->triBounceVelocity;

        if (self->triBounceOffset <= 0x10000 && self->triBounceVelocity < 0) {
            self->triBounceOffset   = 0x10000;
            self->triBounceVelocity = 0;
        }

        self->playerBounceVelocity -= 0x1800;
        self->playerBounceOffset += self->playerBounceVelocity;

        if (self->playerBounceOffset <= 0x8000 && self->playerBounceVelocity < 0) {
            self->playerBounceOffset   = 0x8000;
            self->playerBounceVelocity = 0;
        }
    }
    else if (self->state != UIVsCharSelector_State_Selected) {
        if (self->triBounceOffset > 0) {
            self->triBounceOffset -= 0x2000;

            if (self->triBounceOffset < 0)
                self->triBounceOffset = 0;
        }

        if (self->playerBounceOffset > 0) {
            self->playerBounceOffset -= 0x2000;

            if (self->playerBounceOffset < 0)
                self->playerBounceOffset = 0;
        }
    }

    self->triBounceOffset    = MIN(self->triBounceOffset, 0x13600);
    self->playerBounceOffset = MIN(self->playerBounceOffset, 0x11800);

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if ((
#if MANIA_USE_PLUS
            self->state == UIVsCharSelector_State_HandlePlayerJoin ||
#endif
            self->state == UIVsCharSelector_State_Selected)
        && (parent->state != UIControl_ProcessInputs || parent->active != ACTIVE_ALWAYS || parent->selectionDisabled)) {
        self->isSelected = false;
        self->ready      = false;
        self->state      = UIVsCharSelector_State_CharSelect;
    }
}

void UIVsCharSelector_LateUpdate(void) {}

void UIVsCharSelector_StaticUpdate(void) {}

void UIVsCharSelector_Draw(void)
{
    RSDK_THIS(UIVsCharSelector);

    RSDK.DrawRect(self->position.x - 0x2D0000, self->position.y - 0x2D0000, 0x600000, 0x600000, 0xFFFFFF, 0x7F, INK_BLEND, false);

    UIVsCharSelector_DrawBG();
    UIVsCharSelector_DrawOutline();
    UIVsCharSelector_DrawPlayer();
}

void UIVsCharSelector_Create(void *data)
{
    RSDK_THIS(UIVsCharSelector);

    self->frameID       = self->playerID;
    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
    self->state           = UIVsCharSelector_State_CharSelect;

    UIVsCharSelector_SetupText();

    self->textFrames = UIWidgets->textFrames;
}

void UIVsCharSelector_StageLoad(void) { UIVsCharSelector->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsCharSelector_SetupText(void)
{
    RSDK_THIS(UIVsCharSelector);

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->playerIDAnimator, true, self->playerID + 8);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->waitingAnimator, true, 7);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 8, &self->playerNameAnimator, true, self->frameID);
}

void UIVsCharSelector_DrawOutline(void)
{
    RSDK_THIS(UIVsCharSelector);

    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(self->position.x + 0x30000, self->position.y + 0x30000, 96, 96);

    if (self->isSelected)
        UIWidgets_DrawRectOutline_Flash(self->position.x, self->position.y, 96, 96);
    else
        UIWidgets_DrawRectOutline_Black(self->position.x, self->position.y, 96, 96);
}

void UIVsCharSelector_DrawBG(void)
{
    RSDK_THIS(UIVsCharSelector);

    UIWidgets_DrawRightTriangle(self->position.x - 0x2D0000, self->position.y - 0x2D0000, (self->triBounceOffset >> 11), 232, 40, 88);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-64 * self->triBounceOffset) >> 16, 96, 160, 176);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-44 * self->triBounceOffset) >> 16, 88, 112, 224);
}

void UIVsCharSelector_DrawPlayer(void)
{
    RSDK_THIS(UIVsCharSelector);

    Vector2 drawPos;
    drawPos.x = self->position.x - 0x2D0000;
    drawPos.y = self->position.y + 0x180000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);
#if MANIA_USE_PLUS
    RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &self->edgeAnimator, true, 1);

    drawPos.x = self->position.x + 0x2D0000;
#else
    RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &self->edgeAnimator, true, self->playerID);

    drawPos.x = self->position.x + (self->playerID ? 0x2D0000 : -0x2D0000);
#endif
    drawPos.y = self->position.y - 0x2D0000;
    RSDK.DrawSprite(&self->edgeAnimator, &drawPos, false);

#if MANIA_USE_PLUS
    drawPos.x -= 0xA0000;
#else
    drawPos.x += self->playerID ? -0xA0000 : 0xA0000;
#endif
    drawPos.y += 0x80000;
    RSDK.DrawSprite(&self->playerIDAnimator, &drawPos, false);

    if (self->state == UIVsCharSelector_State_WaitingForPlayer) {
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &self->playerAnimator, true, 7);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        RSDK.DrawSprite(&self->playerAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->waitingAnimator, &drawPos, false);
    }
    else if (self->state != UIVsCharSelector_State_Selected || !(self->timer & 2)) {
        int32 frameID = self->frameID;
#if MANIA_USE_PLUS
        if (self->frameID >= UICHARBUTTON_MIGHTY)
            frameID++;
#endif

        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 1, &self->playerAnimator, true, frameID);
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 2, &self->shadowAnimator, true, frameID);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        drawPos.x += 4 * self->playerBounceOffset;
        drawPos.y += 4 * self->playerBounceOffset;
        RSDK.DrawSprite(&self->shadowAnimator, &drawPos, false);

        drawPos.x -= 8 * self->playerBounceOffset;
        drawPos.y -= 8 * self->playerBounceOffset;
        RSDK.DrawSprite(&self->playerAnimator, &drawPos, false);

        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x200000;
        RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, false);
    }

    if (!self->ready && self->state != UIVsCharSelector_State_WaitingForPlayer) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x200000;

        if (RSDK.Sin256(2 * UIControl->timer) < 0)
            UIWidgets_DrawLeftRightArrows(drawPos.x, drawPos.y, (-RSDK.Sin256(2 * UIControl->timer) + 0x880) << 11);
        else
            UIWidgets_DrawLeftRightArrows(drawPos.x, drawPos.y, (RSDK.Sin256(2 * UIControl->timer) + 0x880) << 11);
    }
}

void UIVsCharSelector_ProcessButtonCB(void)
{
    RSDK_THIS(UIVsCharSelector);

    EntityUIControl *parent = (EntityUIControl *)self->parent;

#if MANIA_USE_PLUS
    if (parent->active == ACTIVE_ALWAYS) {
#endif
        int32 storedFrame = self->frameID;
        int32 inc         = 1;

        if (UIControl->leftPress[self->playerID] || UIControl->rightPress[self->playerID]) {
            if (UIControl->leftPress[self->playerID]) {
                inc = -1;
                --self->frameID;
            }
            else if (UIControl->rightPress[self->playerID]) {
                inc = 1;
                ++self->frameID;
            }

            self->playerBounceOffset   = 0;
            self->playerBounceVelocity = 0x8000;

            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }

#if MANIA_USE_PLUS
        int32 max = UICHARBUTTON_KNUX + (API.CheckDLC(DLC_PLUS) ? 2 : 0);
#else
    int32 max = UICHARBUTTON_KNUX;
#endif
        max++;

        while (self->frameID < 0) self->frameID += max;
        while (self->frameID >= max) self->frameID -= max;

#if MANIA_USE_PLUS
        int32 activePlayers = 0;

        for (int32 i = 0; i < parent->buttonCount; ++i) {
            EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)parent->buttons[i];
            if (button->state == UIVsCharSelector_State_Selected || button->processButtonCB == UIVsCharSelector_ProcessButtonCB_CharSelected)
                activePlayers |= 1 << button->frameID;
        }

        int32 frame = self->frameID;
        while ((1 << frame) & activePlayers) {
            frame += inc;

            if (frame < 0)
                frame += max;

            if (frame >= max)
                frame -= max;

            self->frameID = frame;
        }

        if (self->frameID != frame) {
            self->playerBounceOffset   = 0;
            self->playerBounceVelocity = 0x8000;
        }
#endif

        if (storedFrame != self->frameID)
            UIVsCharSelector_SetupText();

        bool32 pressed = false;
#if MANIA_USE_PLUS
        if (API_GetConfirmButtonFlip())
            pressed = ControllerInfo[CONT_P1 + self->playerID].keyB.press;
        else
            pressed = ControllerInfo[CONT_P1 + self->playerID].keyA.press;
#else
    pressed = UIControl->confirmPress[self->playerID];
#endif

        if (pressed) {
            self->timer           = 0;
            self->state           = UIVsCharSelector_State_Selected;
            self->processButtonCB = StateMachine_None;
            self->ready           = true;

            RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        }
        else if (!self->isSelected && !self->ready) {
            if (parent->active == ACTIVE_ALWAYS)
                UIVsCharSelector_State_ResetState();
        }
#if MANIA_USE_PLUS
    }
#endif
}

void UIVsCharSelector_ProcessButtonCB_CharSelected(void)
{
    RSDK_THIS(UIVsCharSelector);

    EntityUIControl *parent = (EntityUIControl *)self->parent;

#if MANIA_USE_PLUS
    if (parent->active == ACTIVE_ALWAYS) {
#endif
        parent->targetPos.x = self->position.x;

        bool32 pressed = false;
#if MANIA_USE_PLUS
        if (API_GetConfirmButtonFlip())
            pressed = ControllerInfo[CONT_P1 + self->playerID].keyB.press;
        else
            pressed = ControllerInfo[CONT_P1 + self->playerID].keyA.press;
#else
    pressed = UIControl->backPress[self->playerID];
#endif

        if (pressed) {
            self->timer = 0;
            UIVsCharSelector_State_ResetState();

            self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
            self->ready           = false;
        }
#if MANIA_USE_PLUS
    }
#endif
}

void UIVsCharSelector_State_ResetState(void)
{
    RSDK_THIS(UIVsCharSelector);

#if MANIA_USE_PLUS
    self->state = UIVsCharSelector_State_HandlePlayerJoin;
#else
    self->state = UIVsCharSelector_State_CharSelect;
#endif

    if (!self->isSelected) {
        self->triBounceVelocity    = 0x4000;
        self->playerBounceOffset   = 0;
        self->playerBounceVelocity = 0x8000;
        self->isSelected           = true;
    }
}

void UIVsCharSelector_State_CharSelect(void)
{
    RSDK_THIS(UIVsCharSelector);

    uint32 id = API_GetInputDeviceID(CONT_P1 + self->playerID);
    if (id == INPUT_NONE || id == (uint32)INPUT_AUTOASSIGN) {
        self->state = UIVsCharSelector_State_WaitingForPlayer;
#if MANIA_USE_PLUS
        UIVsCharSelector_State_WaitingForPlayer();
#endif
    }
}

void UIVsCharSelector_State_WaitingForPlayer(void)
{
    RSDK_THIS(UIVsCharSelector);

    self->processButtonCB = StateMachine_None;
    self->isSelected      = false;

    uint32 id = API_GetInputDeviceID(CONT_P1 + self->playerID);
#if MANIA_USE_PLUS
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    self->ready    = false;
    int32 assigned = API_IsInputDeviceAssigned(id);

    if (parent->active == ACTIVE_ALWAYS) {
        if (!id || (!assigned && id != (uint32)INPUT_AUTOASSIGN)) {
            API_AssignInputSlotToDevice(CONT_P1 + self->playerID, INPUT_AUTOASSIGN);
        }
        else if (id != (uint32)INPUT_AUTOASSIGN) {
            self->playerBounceOffset   = 0;
            self->playerBounceVelocity = 0x8000;
            self->timer                = 4;
            self->state                = UIVsCharSelector_State_HandlePlayerJoin;
        }
    }
#else
    if (id != INPUT_NONE && id != INPUT_AUTOASSIGN) {
        self->timer = 4;
        self->state = UIVsCharSelector_State_HandlePlayerJoin;
    }
#endif
}

void UIVsCharSelector_State_HandlePlayerJoin(void)
{
    RSDK_THIS(UIVsCharSelector);

#if MANIA_USE_PLUS
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    self->isSelected = true;
    int32 id         = API_GetInputDeviceID(CONT_P1 + self->playerID);
    int32 assigned   = API_IsInputDeviceAssigned(id);

    if (parent->active == ACTIVE_ALWAYS) {
        if (id != INPUT_NONE && id != INPUT_AUTOASSIGN && assigned) {
            if (self->timer > 0) {
                if (!--self->timer)
                    self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
            }
        }
        else {
            self->state = UIVsCharSelector_State_WaitingForPlayer;
        }
    }
#else
    int32 id = API_GetInputDeviceID(CONT_P1 + self->playerID);

    if (id != INPUT_NONE && id != INPUT_AUTOASSIGN) {
        if (self->timer > 0) {
            if (!--self->timer)
                self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
        }
    }
    else {
        self->state = UIVsCharSelector_State_WaitingForPlayer;
    }
#endif
}

void UIVsCharSelector_State_Selected(void)
{
    RSDK_THIS(UIVsCharSelector);

    if (self->timer >= 30) {
        if (self->parent->active == ACTIVE_ALWAYS) {
            self->isSelected = false;
            self->timer      = 0;

            self->state           = UIVsCharSelector_State_CharSelect;
            self->processButtonCB = UIVsCharSelector_ProcessButtonCB_CharSelected;

            StateMachine_Run(self->actionCB);
        }
    }
    else {
        if (self->timer == 2) {
            switch (self->frameID) {
                case UICHARBUTTON_SONIC: RSDK.PlaySfx(Announcer->sfxSonic, false, 255); break;
                case UICHARBUTTON_TAILS: RSDK.PlaySfx(Announcer->sfxTails, false, 255); break;
                case UICHARBUTTON_KNUX: RSDK.PlaySfx(Announcer->sfxKnuckles, false, 255); break;
#if MANIA_USE_PLUS
                case UICHARBUTTON_MIGHTY: RSDK.PlaySfx(Announcer->sfxMighty, false, 255); break;
                case UICHARBUTTON_RAY: RSDK.PlaySfx(Announcer->sfxRay, false, 255); break;
#endif
                default: break;
            }
        }

        ++self->timer;
    }
}

#if GAME_INCLUDE_EDITOR
void UIVsCharSelector_EditorDraw(void)
{
    RSDK_THIS(UIVsCharSelector);

    self->inkEffect = self->disabled ? INK_BLEND : INK_NONE;

    UIVsCharSelector_Draw();
}

void UIVsCharSelector_EditorLoad(void)
{
    UIVsCharSelector->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIVsCharSelector, playerID);
    RSDK_ENUM_VAR("Player 1", UIVSCHARSELECTOR_1P);
    RSDK_ENUM_VAR("Player 2", UIVSCHARSELECTOR_2P);
#if MANIA_USE_PLUS
    RSDK_ENUM_VAR("Player 3", UIVSCHARSELECTOR_3P);
    RSDK_ENUM_VAR("Player 4", UIVSCHARSELECTOR_4P);
#endif
}
#endif

void UIVsCharSelector_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsCharSelector, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsCharSelector, VAR_UINT8, playerID);
}
