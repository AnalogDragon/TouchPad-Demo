#ifndef _VarDefine_
#define _VarDefine_
#include "sys.h"



struct SysTime_REG
{
	volatile u16 SysTimeCNT1ms;
	volatile u16 SysTimeCNT10ms;
	volatile u16 SysTimeCNT100ms;
	volatile u16 SysTimeCNT1s;
	volatile u16 SysTimeCNT1min;
	volatile u16 SysTimeCNT1h;
		
	volatile u8 SysTimeFLG1ms;
	volatile u8 SysTimeFLG10ms;
	volatile u8 SysTimeFLG100ms;
	volatile u8 SysTimeFLG1s;
	volatile u8 SysTimeFLG1min;
	volatile u8 SysTimeFLG1h;
};  

struct DispData_REG{
	u8 EN;
	u8 Pos;		//位置 0-31
	u8 All;		//中间按键状态
};

struct TouchValue_REG{
	u16 CenterRef;
	u16 RightRef;
	u16 DownRef;
	u16 LeftRef;
	u16 UpRef;
	
	u16 Center;
	u16 Right;
	u16 Down;
	u16 Left;
	u16 Up;
	
	u8 CenterKey;
	u8 RightKey;
	u8 DownKey;
	u8 LeftKey;
	u8 UpKey;
	u8 CycKey;
	u8 AllKey;
	
	u8 KeySet;
	
	float Deg;
};

extern struct SysTime_REG SysTime;
extern struct DispData_REG DispData;
extern struct TouchValue_REG TouchValue;

#endif
