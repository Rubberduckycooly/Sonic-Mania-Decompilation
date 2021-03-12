#ifndef OBJ_ITEMBOX_H
#define OBJ_ITEMBOX_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectItemBox : Object {
    Hitbox hitbox;
    Hitbox hiddenHitbox;
    int brokenFrame;
    ushort spriteIndex;
    ushort sfx_Destroy;
    ushort sfx_Teleport;
    ushort sfx_HyperRing;
    ushort sfx_PowerDown;
    ushort sfx_Revovery;
};

// Entity Class
struct EntityItemBox : Entity {
    void (*state)();
    int type;
    Vector2 contentsPos;
    Vector2 unknownPos;
    int contentsSpeed;
    Entity *storedEntity;
    int timer;
    bool32 isFalling;
    bool32 isContents;
    bool32 hidden;
    int planeFilter;
    bool32 lrzConvPhys;
    EntityAnimationData brokenData;
    EntityAnimationData contentsData;
    EntityAnimationData overlayData;
    EntityAnimationData debrisData;
    Entity *parent;
};

// Object Struct
extern ObjectItemBox *ItemBox;

// Standard Entity Events
void ItemBox_Update();
void ItemBox_LateUpdate();
void ItemBox_StaticUpdate();
void ItemBox_Draw();
void ItemBox_Create(void* data);
void ItemBox_StageLoad();
void ItemBox_EditorDraw();
void ItemBox_EditorLoad();
void ItemBox_Serialize();

// Extra Entity Functions
void ItemBox_DebugDraw();
void ItemBox_DebugSpawn();
void ItemBox_CheckHit();
void ItemBox_GivePowerup();
void ItemBox_Break(EntityItemBox *itemBox, void *p);
bool32 ItemBox_HandleFallingCollision();
bool32 ItemBox_HandlePlatformCollision(void *p);
void ItemBox_HandleObjectCollisions();

//Entity States
void ItemBox_State_Broken();
void ItemBox_State_Contents();
void ItemBox_State_Unknown();
void ItemBox_State_Normal();
void ItemBox_State_Falling();
void ItemBox_State_Conveyor();

#endif //!OBJ_ITEMBOX_H