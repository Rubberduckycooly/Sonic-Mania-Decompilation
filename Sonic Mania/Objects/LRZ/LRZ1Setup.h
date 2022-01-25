#ifndef OBJ_LRZ1SETUP_H
#define OBJ_LRZ1SETUP_H

#include "SonicMania.h"

// Object Class
struct ObjectLRZ1Setup {
    RSDK_OBJECT
    int32 palTimer;
    int32 fadeTimer;
    TileLayer* bg1;
    TileLayer* bg2;
    TileLayer* fgLow;
    TileLayer *fgHigh;
    TABLE(int32 deformFG[32], { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    TABLE(int32 deformBG[32],
          { -2, -2, -2, 0, -2, 0, -4, -2, 0, -4, -4, -1, 0, 0, -3, -4, -4, -1, -4, -4, -3, -4, -1, 0, 0, 0, -3, -1, -1, -1, -2, 0 });
};

// Entity Class
struct EntityLRZ1Setup {
	RSDK_ENTITY
};

// Object Struct
extern ObjectLRZ1Setup *LRZ1Setup;

// Standard Entity Events
void LRZ1Setup_Update(void);
void LRZ1Setup_LateUpdate(void);
void LRZ1Setup_StaticUpdate(void);
void LRZ1Setup_Draw(void);
void LRZ1Setup_Create(void* data);
void LRZ1Setup_StageLoad(void);
void LRZ1Setup_EditorDraw(void);
void LRZ1Setup_EditorLoad(void);
void LRZ1Setup_Serialize(void);

// Extra Entity Functions
void LRZ1Setup_StageFinishCB(void);
void LRZ1Setup_DrawLayerCB(void);

#endif //!OBJ_LRZ1SETUP_H
