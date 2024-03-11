#include "stm32f1xx_hal.h"                  // Device header
#include "usb_device.h"
#include "key.h"

extern uint8_t USBD_CUSTOM_HID_SendReport(USBD_HandleTypeDef  *pdev,
                            uint8_t *report,
                            uint16_t len);

extern USBD_HandleTypeDef hUsbDeviceFS;												//声明USB相关函数

//uint8_t keyname[6][16] = {
//  "del","prtsc","pause","f12","f11","f10","f9","f8","f7","f6","f5","f4","f3","f2","f1","esc",
//  "null","insert","backspace","=","-","0","9","8","7","6","5","4","3","2","1","~",
//  "valueup","pageup","|","]","[","p","o","i","u","y","t","r","e","w","q","tab",
//  "null","valuedown","pagedown","enter,""",";","l","k","j","h","g","f","d","s","a","capslock",
//  "null","null","mute","up","rshift","/",".",",","m","n","b","v","c","x","z","lshift",
//  "null","null","null","null","null","null","right","down","left","rctrl","fn","ralt","space","lalt","win","lalt",
//};

uint8_t keyvalue[6][16] = {											//输入键值
	0X4C,0X46,0X48,0X45,0X44,0X43,0X42,0X41,0X40,0X3F,0X3E,0X3D,0X3C,0X3B,0X3A,0X29,
  0X92,0X49,0X2A,0X2E,0X2D,0X27,0X26,0X25,0X24,0X23,0X22,0X21,0X20,0X1F,0X1E,0X35,
  0XE9,0X4B,0X31,0X30,0X2F,0X13,0X12,0X0C,0X18,0X1C,0X17,0X15,0X08,0X1A,0X14,0X2B,
  0X00,0XEA,0X4E,0X28,0X34,0X33,0X0F,0X0E,0X0D,0X0B,0X0A,0X09,0X07,0X16,0X04,0X39,
  0X00,0X00,0XE2,0X52,0X20,0X38,0X37,0X36,0X10,0X11,0X05,0X19,0X06,0X1B,0X1D,0X02,
  0X00,0X00,0X00,0X00,0X00,0X00,0X4F,0X51,0X50,0X10,0XFF,0X40,0X2C,0X04,0X08,0X01,
};

uint8_t keysendready = 0;
uint8_t keymediasendready = 0;

struct
{
  uint8_t keydown[6][16];
  uint8_t keydown2[6][16];
  uint8_t keyflag[6][16];
  uint8_t keyflag_last[6][16];
  uint8_t fndown[6][16];
  uint8_t commonkeychangedflag;
  uint8_t specialkeychangedflag;
}keydata = {0};           //按键状态类

struct
  {
  uint8_t reportid;
  uint8_t specialkey;
  uint8_t undefinedkey;
  uint8_t commonkey[6];
}keysendbuf = {1,0,0,0,0,0,0,0};        //按键发送类

struct
{
  uint8_t reportid;
  uint8_t keymedia[2];
}keymediasendbuf = {2,0,0};

void keyscan(void)
{ 
  uint8_t j;
  uint16_t keybuf = 0x8000;  
  for(j = 0; j < 16; j ++)
  {
    GPIOB -> ODR = (~keybuf);                       //给某一列写入低电平    
    HAL_Delay(1);                             //此处没有延迟会出现不可描述的问题  
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0)
    {
      keydata.keydown[0][j] = 1;
    }
    else
      keydata.keydown[0][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 0)
    {
      keydata.keydown[1][j] = 1;
    }
    else
      keydata.keydown[1][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
    {
      keydata.keydown[2][j] = 1;
    }
    else
      keydata.keydown[2][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 0)
    {
      keydata.keydown[3][j] = 1;
    }
    else
      keydata.keydown[3][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 0)
    {
      keydata.keydown[4][j] = 1;
    }
    else
      keydata.keydown[4][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 0)
    {
      keydata.keydown[5][j] = 1;
    }
    else
      keydata.keydown[5][j] = 0;                        //读取行，被拉低则按下
    
    keybuf = keybuf >> 1;
  }
}

void keyscan2(void)
{ 
  uint8_t j;
  uint16_t keybuf = 0x8000;  
  for(j = 0; j < 16; j ++)
  {
    GPIOB -> ODR = (~keybuf);                       //给某一列写入低电平    
    HAL_Delay(1);                             //此处没有延迟会出现不可描述的问题  
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0)
    {
      keydata.keydown2[0][j] = 1;
    }
    else
      keydata.keydown2[0][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 0)
    {
      keydata.keydown2[1][j] = 1;
    }
    else
      keydata.keydown2[1][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
    {
      keydata.keydown2[2][j] = 1;
    }
    else
      keydata.keydown2[2][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 0)
    {
      keydata.keydown2[3][j] = 1;
    }
    else
      keydata.keydown2[3][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 0)
    {
      keydata.keydown2[4][j] = 1;
    }
    else
      keydata.keydown2[4][j] = 0;                        //读取行，被拉低则按下
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 0)
    {
      keydata.keydown2[5][j] = 1;
    }
    else
      keydata.keydown2[5][j] = 0;                        //读取行，被拉低则按下
    
    keybuf = keybuf >> 1;
  }
}

void keydownjarje(void)
{
  uint8_t i, j;
  for(i = 0; i < 6; i ++)
  {
    for(j = 0; j < 16; j ++)
    {
      if(keydata.keydown[i][j] && keydata.keydown2[i][j])
      {
        keydata.keyflag[i][j] = 1;
      }
      else
      {
        keydata.keyflag[i][j] = 0;
      }
    }
  }
}

void keychangedjarje(void)
{
  uint8_t i, j;
  for(i = 0; i < 6; i ++)
  {
    for(j = 0; j < 16; j ++)
    {    
      if(keydata.keyflag[i][j] != keydata.keyflag_last[i][j])
      {
        if((i == 4 && j == 4) || (i == 4 && j == 15) || (i == 5 && j == 9) || (i == 5 && j == 11) || (i == 5 && j == 13) || (i == 5 && j == 14) || (i == 5 && j == 15))
        {
          keydata.specialkeychangedflag = 1;
          break;
        }
        else
        {
          keydata.commonkeychangedflag = 1;
          break;
        }
      }
      if(keydata.commonkeychangedflag || keydata.specialkeychangedflag)
      {
        break;
      }      
    }
  }
  for(i = 0; i < 6; i ++)
  {
    for(j = 0; j < 16; j ++)
    {
      keydata.keyflag_last[i][j] = keydata.keyflag[i][j];
    }
  }
}

void keyfn(void)
{
  if(keydata.keyflag[5][10])
  {
    if(keydata.keyflag[2][0])
    {
      keydata.keyflag[5][10] = 0;
      keydata.keyflag[2][0] = 0;
      keydata.fndown[2][0] = 1;
      keymediasendbuf.keymedia[0] = 0x6f;
      keymediasendready = 1;
      keymediasend();
    }
    else if(keydata.keyflag[3][1])
    {
      keydata.keyflag[5][10] = 0;
      keydata.keyflag[3][1] = 0;
      keydata.fndown[3][1] = 1;
      keymediasendbuf.keymedia[0] = 0x70;
      keymediasendready = 1;
      keymediasend();
    }
    else if(keydata.keyflag[4][2])
    {
      keydata.keyflag[5][10] = 0;
      keydata.keyflag[4][2] = 0;
      keydata.fndown[4][2] = 1;
      keymediasendbuf.keymedia[0] = 0x32;
      keymediasendready = 1;
      keymediasend();
    }
  }
}

void keysendbufset(void)
{
  uint8_t i, j, k, m, keyexist = 0, keymediaexist = 0;
  for(i = 0; i < 6; i ++)
  {
    for(j = 0; j < 16; j ++)
    {
      if(keydata.fndown[i][j])
      {
        keydata.fndown[i][j] = 0;
        keydata.keyflag[i][j] = 0;
      }
      if(keydata.keyflag[i][j])
      {
        keysendbuf.reportid = 0x01;
        keymediasendbuf.reportid = 0x02;         
        if((i == 4 && j == 4) || (i == 4 && j == 15) || (i == 5 && j == 9) || (i == 5 && j == 11) || (i == 5 && j == 13) || (i == 5 && j == 14) || (i == 5 && j == 15))
        {
          keysendbuf.specialkey = (keysendbuf.specialkey | keyvalue[i][j]);
          keysendready = 1;
        }
        else
        {
          if((i == 1 && j == 0) || (i == 2 && j == 0) || (i == 3 && j == 1) || (i == 4 && j == 2))
          {
//            if(i == 1 && j == 0)
//            {
//              keymediasendbuf.keymedia[0] = 0x92;
//              keymediasendbuf.keymedia[1] = 0x01;
//              keymediasendready = 1;
//              break;      //计算器，但功能未实现
//            }
//            else
//            {
            for(m = 0; m < 2; m ++)
            {
              if(keymediasendbuf.keymedia[m] == keyvalue[i][j])
              {
                keymediaexist = 1;
                break;
              }
              else
              {
                keymediaexist = 0;
              }
            }
            if(keymediaexist)
            {
              keymediaexist = 0;
            }
            else
            {
              for(m = 0; m < 2; m ++)
              {
                if(keymediasendbuf.keymedia[m] == 0x00)
                {
                  keymediasendbuf.keymedia[m] = keyvalue[i][j];
                  break;
                }
              }
            }
            keymediasendready = 1;
          }
//        }
          else
          {
            if(keyvalue[i][j] == 0xff)
            {
              keyfn();
            }         
            for(k = 0; k < 6; k ++)
            {
              if(keysendbuf.commonkey[k] == keyvalue[i][j])
              {
                keyexist = 1;
                break;
              }
              else
              {
                keyexist = 0;
              }
            }
            if(keyexist)
            {
              keyexist = 0;
            }
            else
            {
              for(k = 0; k < 6; k ++)
              {
                if(keysendbuf.commonkey[k] == 0x00)
                {
                  keysendbuf.commonkey[k] = keyvalue[i][j];
                  break;
                }
              }
            }
            keysendready = 1;
          }
        }
      }
      else
      {
        if((i == 4 && j == 4) || (i == 4 && j == 15) || (i == 5 && j == 9) || (i == 5 && j == 11) || (i == 5 && j == 13) || (i == 5 && j == 14) || (i == 5 && j == 15))
        {
          keysendbuf.specialkey = (keysendbuf.specialkey & (~keyvalue[i][j]));
          keysendready = 1;
        }
        else
        {
          if((i == 1 && j == 0) || (i == 2 && j == 0) || (i == 3 && j == 1) || (i == 4 && j == 2))
          {
            for(m = 0; m < 2; m ++)
            {
              if(keymediasendbuf.keymedia[m] == keyvalue[i][j] || keymediasendbuf.keymedia[m] == 0x6f || keymediasendbuf.keymedia[m] == 0x70 || keymediasendbuf.keymedia[m] == 0x32)
//              if(keymediasendbuf.keymedia[m] != 0x00)
              {
//                if(keyvalue[i][j] == 0x92)
//                {
//                  keymediasendbuf.keymedia[1] = 0x00;
//                }
                keymediasendbuf.keymedia[m] = 0x00;
              }
            }
            keymediasendready = 1;
          }
          else
          {
            for(k = 0; k < 6; k ++)
            {
              if(keysendbuf.commonkey[k] == keyvalue[i][j])
              {
                keysendbuf.commonkey[k] = 0x00;
              }
            }
            keysendready = 1;
          }
        }
      }
    }  
  }
}

void keysendjarje(void)
{ 
  if(keydata.commonkeychangedflag || keydata.specialkeychangedflag)
  {
    keydata.commonkeychangedflag = 0;
    keydata.specialkeychangedflag = 0;
    keysendbufset();
    
  }
}

void keymediasend(void)
{
  if(keymediasendready)
  {
    keymediasendready = 0;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keymediasendbuf, sizeof(keymediasendbuf));								//HID发送键值
  }
}

void keysend(void)
{
  if(keysendready)
  {
    keysendready = 0;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keysendbuf, sizeof(keysendbuf));									//HID发送键值
  }
}
