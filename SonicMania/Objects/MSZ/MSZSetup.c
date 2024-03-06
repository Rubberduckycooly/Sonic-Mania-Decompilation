// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMSZSetup *MSZSetup;

void MSZSetup_Update(void)
{
    RSDK_THIS(MSZSetup);
    StateMachine_Run(self->state);
}

void MSZSetup_LateUpdate(void) {}

void MSZSetup_StaticUpdate(void)
{
    if (MSZSetup->usingRegularPalette) {
        if (!(Zone->timer & 3))
            ++MSZSetup->background1->deformationOffset;

        MSZSetup->fadeTimer += 6;
        if (MSZSetup->fadeTimer > 255) {
            MSZSetup->fadeTimer -= 256;
            RSDK.RotatePalette(1, 204, 207, false);
            RSDK.RotatePalette(2, 204, 207, false);
        }

        RSDK.SetLimitedFade(0, 1, 2, MSZSetup->fadeTimer, 204, 207);
    }

    if (!(Zone->timer & 3))
        RSDK.RotatePalette(0, 240, 243, false);

    --MSZSetup->bladeCactusFlowerAniTimer;
    if (MSZSetup->bladeCactusFlowerAniTimer < 1) {
        ++MSZSetup->bladeCactusFlowerAniFrame;
        MSZSetup->bladeCactusFlowerAniFrame %= 13;
        MSZSetup->bladeCactusFlowerAniTimer = MSZSetup->bladeCactusFlowerAniDuration[MSZSetup->bladeCactusFlowerAniFrame];

        RSDK.DrawAniTiles(MSZSetup->aniTiles, 720, 16 * ((MSZSetup->bladeCactusFlowerAniSprPos[MSZSetup->bladeCactusFlowerAniFrame] & 0xFFFC) + 8),
                          32 * (MSZSetup->bladeCactusFlowerAniSprPos[MSZSetup->bladeCactusFlowerAniFrame] & 3), 64, 32);
    }

    --MSZSetup->bobbingCactusFlowerAniTimer;
    if (MSZSetup->bobbingCactusFlowerAniTimer < 1) {
        ++MSZSetup->bobbingCactusFlowerAniFrame;
        MSZSetup->bobbingCactusFlowerAniFrame &= 7;
        MSZSetup->bobbingCactusFlowerAniTimer = MSZSetup->bobbingCactusFlowerAniDuration[MSZSetup->bobbingCactusFlowerAniFrame];
        RSDK.DrawAniTiles(MSZSetup->aniTiles, 74, 0, 16 * (MSZSetup->bobbingCactusFlowerAniFrame + 8), 80, 16);
    }

    if (!(Zone->timer & 3)) {
        MSZSetup->spinningCactusFlowerAniFrame += 16;
        MSZSetup->spinningCactusFlowerAniFrame &= 0x7F;
        RSDK.DrawAniTiles(MSZSetup->aniTiles, 89, MSZSetup->spinningCactusFlowerAniFrame, 96, 16, 32);
    }

    MSZSetup->parallaxPos += MSZSetup->parallaxMult;
    globals->tempFlags = MSZSetup->usingRegularPalette;

    if (MSZSetup->starPostID != StarPost->postIDs[0]) {
        MSZSetup->starPostID  = StarPost->postIDs[0];
        globals->restartFlags = MSZSetup->usingRegularPalette;
    }
}

void MSZSetup_Draw(void) {}

void MSZSetup_Create(void *data)
{
    RSDK_THIS(MSZSetup);

    self->active = ACTIVE_NORMAL;
    if (data)
        self->state = (Type_StateMachine)data;

    if (SceneInfo->minutes || SceneInfo->seconds || SceneInfo->milliseconds) {
        if (SceneInfo->minutes == globals->tempMinutes && SceneInfo->seconds == globals->tempSeconds
            && SceneInfo->milliseconds == globals->tempMilliseconds)
            MSZSetup->usingRegularPalette = globals->tempFlags;
        else
            MSZSetup->usingRegularPalette = globals->restartFlags;
    }
    else {
        globals->restartFlags         = 0;
        globals->tempFlags            = 0;
        MSZSetup->usingRegularPalette = false;
    }

    if (MSZSetup->usingRegularPalette) {
#if MANIA_USE_PLUS
        if (SceneInfo->filter & FILTER_ENCORE) {
            RSDK.LoadPalette(0, "EncoreMSZ2.act", 0b0000000011111111);
            self->state = MSZSetup_State_CheckTrainStart;
        }
        else {
#endif
            for (int32 i = 0; i < 0x400; ++i) MSZSetup->background1->deformationData[i] = MSZSetup->deformData[i & 0x1F];
            RSDK.CopyPalette(4, 128, 0, 128, 128);
            destroyEntity(self);
#if MANIA_USE_PLUS
        }

        RSDK.CopyPalette(0, 128, 1, 128, 128);
        RSDK.CopyPalette(0, 128, 2, 128, 128);
        RSDK.RotatePalette(2, 204, 207, false);
#endif
    }
#if MANIA_USE_PLUS
    else if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreMSZ1.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 3, 128, 80);
        RSDK.LoadPalette(4, "EncoreMSZ2.act", 0b0000000011111111);
    }
#endif
    else {
#if !MANIA_USE_PLUS
        RSDK.CopyPalette(0, 204, 4, 204, 4);
#endif
        RSDK.CopyPalette(3, 128, 0, 128, 128);
    }
}

void MSZSetup_StageLoad(void)
{
    MSZSetup->aniTiles    = RSDK.LoadSpriteSheet("MSZ/AniTiles.gif", SCOPE_STAGE);
    MSZSetup->background1 = RSDK.GetTileLayer(0);

    if (RSDK.CheckSceneFolder("MSZCutscene")) {
        RSDK.CopyPalette(0, 204, 4, 204, 4);
        RSDK.CopyPalette(3, 128, 0, 128, 128);

        MSZSetup_StoreBGParallax();
        MSZSetup_ReloadBGParallax_Multiply(0x400);
    }
    else {
#if MANIA_USE_PLUS
        GiantPistol->inCutscene = false;
        MSZSetup->chuggaVolume  = 0;
#endif

        if (Zone->actID) {
            MSZSetup->usingRegularPalette = true;
            RSDK.CopyPalette(4, 128, 0, 128, 128);
#if MANIA_USE_PLUS
            if (SceneInfo->filter & FILTER_ENCORE) {
                RSDK.CopyPalette(0, 128, 5, 128, 128);
                RSDK.LoadPalette(0, "EncoreMSZ2.act", 0b0000000011111111);
                RSDK.CopyPalette(0, 128, 1, 128, 128);
                RSDK.CopyPalette(0, 128, 2, 128, 128);
                RSDK.RotatePalette(2, 204, 207, false);
            }
            else {
                for (int32 i = 0; i < 0x400; ++i) MSZSetup->background1->deformationData[i] = MSZSetup->deformData[i & 0x1F];
            }
#else
            for (int32 i = 0; i < 0x400; ++i) MSZSetup->background1->deformationData[i] = MSZSetup->deformData[i & 0x1F];
#endif

#if MANIA_USE_PLUS
            if (!(SceneInfo->filter & FILTER_ENCORE) && GET_CHARACTER_ID(1) == ID_KNUCKLES) {
#else
            if (GET_CHARACTER_ID(1) == ID_KNUCKLES) {
#endif
                Zone->cameraBoundsL[0] = 0;
                Zone->cameraBoundsL[1] = 0;
                Zone->cameraBoundsL[2] = 0;
                Zone->cameraBoundsL[3] = 0;

                if (!CutsceneRules_CheckStageReload() && CutsceneRules_CheckPlayerPos(TO_FIXED(76), TO_FIXED(324), TO_FIXED(500), TO_FIXED(564))) {
                    Zone->cameraBoundsB[0] = 564;
                    Zone->cameraBoundsB[1] = 564;
                    Zone->cameraBoundsB[2] = 564;
                    Zone->cameraBoundsB[3] = 564;
                }

                if (isMainGameMode() && globals->atlEnabled && !CutsceneRules_CheckStageReload()) {
                    Zone_ReloadStoredEntities(TO_FIXED(288), TO_FIXED(560), true);
                }
            }
            else if (isMainGameMode() && globals->atlEnabled) {
                if (!CutsceneRules_CheckStageReload()) {
                    MSZSetup_HandleRestart();
                }
                else {
                    globals->parallaxOffset[0] = 0;
                    globals->parallaxOffset[1] = 0;
                }
            }
            else {
                globals->parallaxOffset[0] = 0;
                globals->parallaxOffset[1] = 0;
            }

            if (isMainGameMode() && CutsceneRules_IsAct2()) {
                foreach_all(MSZ2Cutscene, cutscene)
                {
                    MSZSetup->msz2Cutscene = cutscene;
                    foreach_break;
                }

                Zone->stageFinishCallback = MSZSetup_StageFinish_EndAct2;
            }

            GenericTrigger->callbacks[MSZ_GENERICTRIGGER_ACHIEVEMENT] = MSZSetup_Trigger_AwardAchievement;
        }
        else {
#if MANIA_USE_PLUS
            if ((SceneInfo->filter & FILTER_ENCORE)) {
                RSDK.ResetEntitySlot(32, MSZSetup->classID, MSZSetup_State_ManageFade_E);

                if (isMainGameMode()) {
                    if (CutsceneRules_IsAct1())
                        Zone->stageFinishCallback = MSZSetup_StageFinish_EndAct1E;
                }
            }
            else {
                if (RSDK.GetEntityCount(Tornado->classID, false) <= 0) {
                    RSDK.ResetEntitySlot(32, MSZSetup->classID, MSZSetup_State_ManageFade_K);

                    if (!CutsceneRules_IsIntroEnabled())
                        FXFade_StopAll();

                    if (CutsceneRules_IsAct1Regular()) {
                        Zone->shouldRecoverPlayers = true;
                        Zone->stageFinishCallback  = MSZSetup_StageFinish_EndAct1K;
                    }
                }
                else {
                    MSZSetup_StoreBGParallax();
                    MSZSetup_ReloadBGParallax_Multiply(0x400);
                    RSDK.ResetEntitySlot(32, MSZSetup->classID, MSZSetup_State_ManageFade_ST);

                    if (CutsceneRules_IsAct1Regular())
                        Zone->stageFinishCallback = MSZSetup_StageFinish_EndAct1ST;
                    GiantPistol->inCutscene = true;
                }
            }
#else
            if (CHECK_CHARACTER_ID(ID_KNUCKLES, 1)) {
                RSDK.ResetEntitySlot(17, MSZSetup->classID, MSZSetup_State_ManageFade_K);
                if (!CutsceneRules_IsIntroEnabled())
                    FXFade_StopAll();

                if (CutsceneRules_IsAct1Regular()) {
                    Zone->shouldRecoverPlayers = true;
                    Zone->stageFinishCallback  = MSZSetup_StageFinish_EndAct1K;
                }
            }
            else {
                MSZSetup_StoreBGParallax();
                MSZSetup_ReloadBGParallax_Multiply(0x400);
                RSDK.ResetEntitySlot(17, MSZSetup->classID, MSZSetup_State_ManageFade_ST);

                if (CutsceneRules_IsAct1Regular())
                    Zone->stageFinishCallback = MSZSetup_StageFinish_EndAct1ST;
            }
#endif
        }
    }

#if MANIA_USE_PLUS
    MSZSetup->sfxLocoChugga = RSDK.GetSfx("MSZ/LocoChugga.wav");
#endif

    Animals->animalTypes[0] = ANIMAL_LOCKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;
}

void MSZSetup_SetBGScrollOrigin(int32 x, int32 y)
{
    int32 posX = x >> 8;

    TileLayer *background1 = RSDK.GetTileLayer(0);
    background1->scrollPos += (y >> 8) * background1->parallaxFactor;
    for (int32 i = 0; i < background1->scrollInfoCount; ++i) {
        background1->scrollInfo[i].scrollPos = posX * background1->scrollInfo[i].parallaxFactor;
    }

    TileLayer *background2 = RSDK.GetTileLayer(1);
    background2->scrollPos += (y >> 8) * background2->parallaxFactor;
    for (int32 i = 0; i < background2->scrollInfoCount; ++i) {
        background2->scrollInfo[i].scrollPos = posX * background2->scrollInfo[i].parallaxFactor;
    }

    foreach_all(ParallaxSprite, sprite)
    {
        if (sprite->aniID < MSZ_PARALLAXSPRITE_OOZPEEK)
            sprite->scrollPos.x += posX * (sprite->parallaxFactor.x >> 16);
    }
}

void MSZSetup_StoreBGParallax(void)
{
    int32 id = 0;

    TileLayer *background1 = RSDK.GetTileLayer(0);
    for (int32 i = 0; i < background1->scrollInfoCount; ++i) {
        MSZSetup->storedParallax[id++] = background1->scrollInfo[i].parallaxFactor;
#if MANIA_USE_PLUS
        if (SceneInfo->filter == (FILTER_BOTH | FILTER_MANIA))
#endif
            background1->scrollInfo[i].parallaxFactor = 0;
    }

    TileLayer *background2 = RSDK.GetTileLayer(1);
    for (int32 i = 0; i < background2->scrollInfoCount; ++i) {
        MSZSetup->storedParallax[id++] = background2->scrollInfo[i].parallaxFactor;
#if MANIA_USE_PLUS
        if (SceneInfo->filter == (FILTER_BOTH | FILTER_MANIA))
#endif
            background2->scrollInfo[i].parallaxFactor = 0;
    }

    foreach_all(ParallaxSprite, parallaxSprite)
    {
        MSZSetup->storedParallax[id++] = (parallaxSprite->parallaxFactor.x >> 8);
#if MANIA_USE_PLUS
        if (SceneInfo->filter == (FILTER_BOTH | FILTER_MANIA))
#endif
            parallaxSprite->parallaxFactor.x = 0;
    }
}

void MSZSetup_ReloadBGParallax(void)
{
    int32 id               = 0;
    TileLayer *background1 = RSDK.GetTileLayer(0);
    for (int32 i = 0; i < background1->scrollInfoCount; ++i) background1->scrollInfo[i].scrollSpeed = MSZSetup->storedParallax[id++];

    TileLayer *background2 = RSDK.GetTileLayer(1);
    for (int32 i = 0; i < background2->scrollInfoCount; ++i) background2->scrollInfo[i].scrollSpeed = MSZSetup->storedParallax[id++];

    foreach_all(ParallaxSprite, parallaxSprite) { parallaxSprite->scrollSpeed.x = MSZSetup->storedParallax[id++] << 8; }
}

void MSZSetup_ReloadBGParallax_Multiply(int32 parallaxMultiplier)
{
    int32 id               = 0;
    TileLayer *background1 = RSDK.GetTileLayer(0);
    for (int32 i = 0; i < background1->scrollInfoCount; ++i)
        background1->scrollInfo[i].scrollSpeed = parallaxMultiplier * MSZSetup->storedParallax[id++];

    TileLayer *background2 = RSDK.GetTileLayer(1);
    for (int32 i = 0; i < background2->scrollInfoCount; ++i)
        background2->scrollInfo[i].scrollSpeed = parallaxMultiplier * MSZSetup->storedParallax[id++];

    foreach_all(ParallaxSprite, parallaxSprite)
    {
        parallaxSprite->scrollSpeed.x = parallaxMultiplier * MSZSetup->storedParallax[id++];
#if MANIA_USE_PLUS
        if (!(SceneInfo->filter & FILTER_ENCORE))
#endif
            parallaxSprite->scrollSpeed.x >>= 8;
    }

    MSZSetup->parallaxMult = parallaxMultiplier << 8;
}

void MSZSetup_State_ManageFade_ST(void)
{
    RSDK_THIS(MSZSetup);

#if GAME_VERSION == VER_100
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > TO_FIXED(6528)) {
#else
    if (ScreenInfo->position.x + ScreenInfo->center.x > 6528) {
#endif
        self->state = MSZSetup_State_SwitchPalettes;
    }
}

void MSZSetup_State_ManageFade_K(void)
{
    RSDK_THIS(MSZSetup);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x < TO_FIXED(8448) && player1->position.y < TO_FIXED(1600))
        self->state = MSZSetup_State_SwitchPalettes;
}

#if MANIA_USE_PLUS
void MSZSetup_State_ManageFade_E(void)
{
    RSDK_THIS(MSZSetup);

    foreach_all(Decoration, decoration)
    {
        if (decoration->animator.animationID == 2 || decoration->animator.animationID == 3)
            decoration->animator.speed = 0;
    }

    self->state = MSZSetup_State_CheckFadeTrigger_E;
}
#endif

void MSZSetup_StageFinish_EndAct1ST(void) { MSZCutsceneST->finishedAct = true; }

void MSZSetup_StageFinish_EndAct1K(void)
{
    Zone_StoreEntities((Zone->cameraBoundsL[0] + ScreenInfo->center.x) << 16, Zone->cameraBoundsB[0] << 16);
    RSDK.LoadScene();
}

#if MANIA_USE_PLUS
void MSZSetup_StageFinish_EndAct1E(void) {}
#endif

void MSZSetup_StageFinish_EndAct2(void) { MSZSetup->msz2Cutscene->active = ACTIVE_NORMAL; }

void MSZSetup_Trigger_AwardAchievement(void)
{
    if (!MSZSetup->hasAchievement) {
        API_UnlockAchievement(&achievementList[ACH_MSZ]);
        MSZSetup->hasAchievement = true;
    }
}

void MSZSetup_State_SwitchPalettes(void)
{
    RSDK_THIS(MSZSetup);

    self->timer += 4;
    RSDK.SetLimitedFade(0, 3, 4, self->timer, 128, 255);

    if (self->timer >= 256) {
#if MANIA_USE_PLUS
        if (!(SceneInfo->filter & FILTER_ENCORE)) {
#endif
            for (int32 i = 0; i < 0x400; ++i) MSZSetup->background1->deformationData[i] = MSZSetup->deformData[i & 0x1F];
#if MANIA_USE_PLUS
        }
#endif
        MSZSetup->usingRegularPalette = true;

        RSDK.CopyPalette(4, 128, 0, 128, 128);
#if MANIA_USE_PLUS
        RSDK.CopyPalette(4, 128, 1, 128, 128);
        RSDK.CopyPalette(4, 128, 2, 128, 128);
        RSDK.RotatePalette(2, 204, 207, false);

        if ((SceneInfo->filter & FILTER_ENCORE))
            self->state = MSZSetup_State_CheckTrainStart;
        else
#endif
            destroyEntity(self);
    }
}

void MSZSetup_HandleRestart(void)
{
    Zone_ReloadStoredEntities(WIDE_SCR_XCENTER << 16, 1440 << 16, true);

    int32 id               = 0;
    TileLayer *background1 = RSDK.GetTileLayer(0);
    for (int32 i = 0; i < background1->scrollInfoCount; ++i) background1->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];

    TileLayer *background2 = RSDK.GetTileLayer(1);
    for (int32 i = 0; i < background2->scrollInfoCount; ++i) background2->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];

    foreach_all(ParallaxSprite, parallaxSprite) { parallaxSprite->scrollPos.x = globals->parallaxOffset[id++]; }
}

#if MANIA_USE_PLUS
void MSZSetup_State_CheckFadeTrigger_E(void)
{
    RSDK_THIS(MSZSetup);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > 0x29800000) {
        if (player1->position.y > 0x4200000 && player1->position.y < 0x6800000)
            self->state = MSZSetup_State_SwitchPalettes;
    }
}

void MSZSetup_State_CheckTrainStart(void)
{
    RSDK_THIS(MSZSetup);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > 0x2B600000) {
        foreach_all(Decoration, decoration)
        {
            if (decoration->animator.animationID == 2 || decoration->animator.animationID == 3)
                decoration->animator.speed = 1;
        }

        Zone->cameraBoundsL[0] = 10624;
        Zone->playerBoundsL[0] = 10624 << 16;

        MSZSetup->chuggaChannel = RSDK.PlaySfx(MSZSetup->sfxLocoChugga, 1, 255);
        RSDK.SetChannelAttributes(MSZSetup->chuggaChannel, 0.0, 0.0, 1.0);

        MSZSetup_StoreBGParallax();
        MSZSetup_ReloadBGParallax_Multiply(0x000);
        Camera_ShakeScreen(0, 4, 4);

        RSDK.CopyTileLayer(4, 640, 94, 5, 0, 0, 62, 16);
        RSDK.CopyTileLayer(4, 655, 0, 5, 0, 0, 17, 70);
        RSDK.CopyTileLayer(3, 655, 0, 5, 0, 0, 17, 70);
        RSDK.CopyTileLayer(2, 655, 0, 5, 0, 0, 17, 70);

        self->state = MSZSetup_State_TrainStarting;
    }
}

void MSZSetup_State_TrainStarting(void)
{
    RSDK_THIS(MSZSetup);

    RSDK.SetChannelAttributes(MSZSetup->chuggaChannel, ++MSZSetup->chuggaVolume * (1 / 256.0f), 0.0, 1.0);
    MSZSetup_ReloadBGParallax_Multiply(8 * MSZSetup->chuggaVolume);

    if (MSZSetup->chuggaVolume >= 256)
        self->state = MSZSetup_State_TrainSequence_MSZ1E;

    RSDK.GetTileLayer(3)->scrollInfo[1].scrollSpeed = 0x600 * MSZSetup->chuggaVolume;
}

void MSZSetup_State_TrainSequence_MSZ1E(void)
{
    RSDK_THIS(MSZSetup);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    TileLayer *fgSupaLow  = RSDK.GetTileLayer(3);
    fgSupaLow->scrollInfo[1].scrollPos &= 0x00FFFFFF;
    fgSupaLow->scrollInfo[1].scrollSpeed = 0x600 * MSZSetup->chuggaVolume;

    if (player1->position.x > 0x360C0000) {
        GiantPistol->inCutscene = true;
        self->state             = MSZSetup_State_Boss_MSZ1E;
    }
}

void MSZSetup_State_Boss_MSZ1E(void)
{
    RSDK_THIS(MSZSetup);

    TileLayer *fgSupaLow = RSDK.GetTileLayer(3);
    fgSupaLow->scrollInfo[1].scrollPos &= 0x00FFFFFF;
    fgSupaLow->scrollInfo[1].scrollSpeed = 0x600 * MSZSetup->chuggaVolume;

    if (!MSZSetup->parallaxMult) {
        MSZSetup->chuggaVolume -= 4;
        RSDK.SetChannelAttributes(MSZSetup->chuggaChannel, MSZSetup->chuggaVolume * (1 / 256.0f), 0.0, 1.0);

        if (MSZSetup->chuggaVolume <= 0) {
            fgSupaLow->scrollInfo[1].scrollSpeed = 0;
            fgSupaLow->scrollInfo[1].scrollPos   = 0;
            MSZSetup->chuggaVolume               = 0;
            self->state                          = MSZSetup_State_AwaitActClearStart;
        }
    }
}

void MSZSetup_State_AwaitActClearStart(void)
{
    RSDK_THIS(MSZSetup);

    if (RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear)->classID == ActClear->classID)
        self->state = MSZSetup_State_AwaitActClearFinish;
}

void MSZSetup_State_AwaitActClearFinish(void)
{
    RSDK_THIS(MSZSetup);

    if (RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear)->classID != ActClear->classID) {
        self->timer                 = 0;
        Zone->cameraBoundsR[0]      = 17064;
        Zone->playerBoundActiveR[0] = false;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player1->stateInput   = StateMachine_None;

        foreach_active(Player, player)
        {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, true, 0);
            player->up        = false;
            player->down      = false;
            player->left      = false;
            player->right     = false;
            player->jumpPress = false;
            player->jumpHold  = false;
            player->state     = Player_State_Ground;
        }

        self->state = MSZSetup_State_MoveToMSZ2Start;
    }
}

void MSZSetup_State_MoveToMSZ2Start(void)
{
    RSDK_THIS(MSZSetup);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->right        = true;

    if (player1->position.x < (Zone->cameraBoundsR[0] - ScreenInfo->center.x) << 16) {
        foreach_active(Player, player)
        {
            if (player->groundVel > 0x30000)
                player->groundVel = 0x30000;
        }
    }
    else {
        player1->right = false;
        player1->left  = true;
        self->state    = MSZSetup_State_AwaitPlayerStopped;
    }
}

void MSZSetup_State_AwaitPlayerStopped(void)
{
    RSDK_THIS(MSZSetup);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->groundVel <= 0) {
        player1->groundVel = 0;
        player1->left      = false;
        player1->direction = FLIP_NONE;
        self->timer        = 0;
        self->state        = MSZSetup_State_StoreMSZ1ScrollPos_E;
    }
}

void MSZSetup_State_StoreMSZ1ScrollPos_E(void)
{
    RSDK_THIS(MSZSetup);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->direction    = FLIP_NONE;

    if (++self->timer >= 90) {
        int32 id = 0;

        TileLayer *background1 = RSDK.GetTileLayer(0);
        for (int32 i = 0; i < background1->scrollInfoCount; ++i) {
            globals->parallaxOffset[id++] = background1->scrollInfo[i].tilePos;
        }

        TileLayer *background2 = RSDK.GetTileLayer(1);
        for (int32 i = 0; i < background2->scrollInfoCount; ++i) {
            globals->parallaxOffset[id++] = background2->scrollInfo[i].tilePos;
        }

        foreach_all(ParallaxSprite, sprite)
        {
            globals->parallaxOffset[id++] =
                ((sprite->scrollPos.x + sprite->parallaxFactor.x * ScreenInfo->position.x) & 0x7FFF0000) % sprite->loopPoint.x;
        }

        Zone_StoreEntities((ScreenInfo->center.x + 16640) << 16, 1440 << 16);
        globals->atlEnabled = true;
        RSDK.LoadScene();

        destroyEntity(self);
    }
}
#endif

void MSZSetup_State_StoreMSZ1ScrollPos_ST(void)
{
    int32 id = 0;

    TileLayer *background1 = RSDK.GetTileLayer(0);
    for (int32 i = 0; i < background1->scrollInfoCount; ++i) {
        globals->parallaxOffset[id++] = background1->scrollInfo[i].scrollPos;
    }

    TileLayer *background2 = RSDK.GetTileLayer(1);
    for (int32 i = 0; i < background2->scrollInfoCount; ++i) {
        globals->parallaxOffset[id++] = background2->scrollInfo[i].scrollPos;
    }

    foreach_all(ParallaxSprite, sprite) { globals->parallaxOffset[id++] = sprite->scrollPos.x; }

    Zone_StoreEntities((Zone->cameraBoundsL[0] + ScreenInfo->center.x) << 16, 1440 << 16);
    ++SceneInfo->listPos;
    globals->atlEnabled = true;
    RSDK.LoadScene();
}

void MSZSetup_PlayerState_Pilot(void)
{
    RSDK_THIS(Player);

    self->position.x     = 0;
    self->position.y     = 0;
    self->active         = ACTIVE_NEVER;
    self->visible        = false;
    self->stateInput     = StateMachine_None;
    self->position.x     = ScreenInfo->position.x << 16;
    self->position.y     = ScreenInfo->position.y << 16;
    Player->respawnTimer = 0;
}

void MSZSetup_PlayerState_PostCrashJumpIn(void)
{
    RSDK_THIS(Player);

    self->active         = ACTIVE_NORMAL;
    self->visible        = true;
    self->stateInput     = Player_Input_P2_AI;
    Player->respawnTimer = 240;

    Player_HandleSidekickRespawn();
}

#if GAME_INCLUDE_EDITOR
void MSZSetup_EditorDraw(void) {}

void MSZSetup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Award Achievement", MSZ_GENERICTRIGGER_ACHIEVEMENT);

    RSDK_ACTIVE_VAR(Decoration, type);
    RSDK_ENUM_VAR("Main", MSZ_DECORATION_MAIN);
    RSDK_ENUM_VAR("Tanker Rail", MSZ_DECORATION_TANKERRAIL);
    RSDK_ENUM_VAR("Tanker Wheel", MSZ_DECORATION_TANKERWHEEL);
    RSDK_ENUM_VAR("Carriage Wheel", MSZ_DECORATION_CARRIAGEWHEEL);
    RSDK_ENUM_VAR("Connector 1", MSZ_DECORATION_CONNECTOR1);
    RSDK_ENUM_VAR("Connector 2", MSZ_DECORATION_CONNECTOR2);

    RSDK_ACTIVE_VAR(ParallaxSprite, aniID);
    RSDK_ENUM_VAR("Cactus 1", MSZ_PARALLAXSPRITE_CACTUS1);
    RSDK_ENUM_VAR("Cactus 2", MSZ_PARALLAXSPRITE_CACTUS2);
    RSDK_ENUM_VAR("Cactus 3", MSZ_PARALLAXSPRITE_CACTUS3);
    RSDK_ENUM_VAR("Cactus 4", MSZ_PARALLAXSPRITE_CACTUS4);
    RSDK_ENUM_VAR("Cactus 5", MSZ_PARALLAXSPRITE_CACTUS5);
    RSDK_ENUM_VAR("Cactus 6", MSZ_PARALLAXSPRITE_CACTUS6);
    RSDK_ENUM_VAR("Cactus 7", MSZ_PARALLAXSPRITE_CACTUS7);
    RSDK_ENUM_VAR("OOZ Peek", MSZ_PARALLAXSPRITE_OOZPEEK);
}
#endif

void MSZSetup_Serialize(void) {}
