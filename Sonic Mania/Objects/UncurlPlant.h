#ifndef OBJ_UNCURLPLANT_H
#define OBJ_UNCURLPLANT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    colour value1;
    int value2[8]; //= { 0, 16, 64, 96, 112, 144, 160, 192 };
    int value3[8]; //= { 0, -8, 32, 64, 64, 64, 64, 64 };
    int value4[8]; //= { 0, -24, 8, 8, 8, 8, 8, 8 };
    ushort value5;
} ObjectUncurlPlant;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUncurlPlant;

// Object Struct
extern ObjectUncurlPlant *UncurlPlant;

// Standard Entity Events
void UncurlPlant_Update(void);
void UncurlPlant_LateUpdate(void);
void UncurlPlant_StaticUpdate(void);
void UncurlPlant_Draw(void);
void UncurlPlant_Create(void* data);
void UncurlPlant_StageLoad(void);
void UncurlPlant_EditorDraw(void);
void UncurlPlant_EditorLoad(void);
void UncurlPlant_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UNCURLPLANT_H