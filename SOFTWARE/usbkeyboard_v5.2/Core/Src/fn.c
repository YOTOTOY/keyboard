#include "stm32f1xx_hal.h"
#include "fn.h"
#include "key.h"

struct keydata;
struct keymediasendbuf;

uint8_t fn = 0;

void keyfn(void)
{
  if(keydata.keyflag[0][1])
  {
    keymediasendbuf.keymedia[0] = 0x32;
    keydata.keyflag[0][1] = 0;
  }
}
