#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f1xx_hal.h"

//struct  a keydata;
//struct b keysendbuf;
//struct c keymediasendbuf;

//struct
//{
//  uint8_t keydown[6][16];
//  uint8_t keydown2[6][16];
//  uint8_t keyflag[6][16];
//  uint8_t keyflag_last[6][16];
//  uint8_t commonkeychangedflag;
//  uint8_t specialkeychangedflag;
//}keydata = {0};           //按键状态类

//struct
//  {
//  uint8_t reportid;
//  uint8_t specialkey;
//  uint8_t undefinedkey;
//  uint8_t commonkey[6];
//}keysendbuf = {0};        //按键发送类

//struct
//{
//  uint8_t reportid;
//  uint8_t keymedia[2];
//}keymediasendbuf = {0};

void keyscan(void);
void keyscan2(void);
void keydownjarje(void);
void keychangedjarje(void);
void keysendjarje(void);
void keymediasend(void);
void keysend(void);
void keyfn(void);

#endif
