#ifndef OBJ_COLORHELPERS_H
#define OBJ_COLORHELPERS_H

#include "SonicMania.h"

// Object Class
struct ObjectColorHelpers {
	RSDK_OBJECT
};

// Entity Class
struct EntityColorHelpers {
	RSDK_ENTITY
};

// Object Entity
extern ObjectColorHelpers *ColorHelpers;

// Standard Entity Events
void ColorHelpers_Update(void);
void ColorHelpers_LateUpdate(void);
void ColorHelpers_StaticUpdate(void);
void ColorHelpers_Draw(void);
void ColorHelpers_Create(void* data);
void ColorHelpers_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ColorHelpers_EditorDraw(void);
void ColorHelpers_EditorLoad(void);
#endif
void ColorHelpers_Serialize(void);

// Extra Entity Functions
uint16 ColorHelpers_PackRGB(uint8 r, uint8 g, uint8 b);

void ColorHelpers_Unknown1(int32 r, int32 g, int32 b, uint32 *rPtr, uint32 *gPtr, uint32 *bPtr);
void ColorHelpers_Unknown2(int32 a1, int32 a2, int32 brightness, uint32 *r, uint32 *g, uint32 *b);

#endif //!OBJ_COLORHELPERS_H
