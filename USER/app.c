#include "app.h"
#include "math.h"



#define Gate0 40	//单个按键阈值Single Key Gate Value
#define Gate1 50

void GetTouch(void){
	u16 up,dn,lt,rt;
	u8 temp;
	float temp0,temp1;
	TouchValue.CenterKey = ((TouchValue.CenterRef + Gate0) < TouchValue.Center);
	TouchValue.RightKey = ((TouchValue.RightRef + Gate0) < TouchValue.Right);
	TouchValue.DownKey = ((TouchValue.DownRef + Gate0) < TouchValue.Down);
	TouchValue.LeftKey = ((TouchValue.LeftRef + Gate0) < TouchValue.Left);
	TouchValue.UpKey = ((TouchValue.UpRef + Gate0) < TouchValue.Up);
	
	TouchValue.CycKey = ((TouchValue.UpRef + TouchValue.LeftRef + TouchValue.RightRef + TouchValue.DownRef + Gate1)
											<(TouchValue.Right + TouchValue.Down + TouchValue.Left + TouchValue.Up));
	
	TouchValue.AllKey = ((TouchValue.CenterRef + TouchValue.UpRef + TouchValue.LeftRef + TouchValue.RightRef + TouchValue.DownRef + Gate1)
											<(TouchValue.Center + TouchValue.Right + TouchValue.Down + TouchValue.Left + TouchValue.Up));
	
	up=dn=lt=rt=0;
	if(TouchValue.RightRef < TouchValue.Right)	rt = TouchValue.Right - TouchValue.RightRef;
	if(TouchValue.DownRef < TouchValue.Down)		dn = TouchValue.Down - TouchValue.DownRef;
	if(TouchValue.LeftRef < TouchValue.Left)		lt = TouchValue.Left - TouchValue.LeftRef;
	if(TouchValue.UpRef < TouchValue.Up)				up = TouchValue.Up - TouchValue.UpRef;
	
	if(up>=dn)		//Get Quadrant
		if(rt>=lt) temp = 1;
		else temp = 4;
	else 
		if(rt>=lt) temp = 2;
		else temp = 3;
	
	temp0 = (int)up-(int)dn;
	temp1 = (int)rt-(int)lt;
	
	if(temp0==0)temp0 = 0.001;
	temp0 = temp1/temp0;
	if(temp0<0)temp0 = -temp0;
	temp0 = (float)atan(temp0)*180/3.1415927;
	switch(temp){
		case 2:temp0 = 180-temp0;break;
		case 3:temp0 = 180+temp0;break;
		case 4:temp0 = 360-temp0;break;
	}
	TouchValue.Deg = temp0;
	
}



void TouchToDisp(void){
	static u8 PressCyc = 0;
	u8 temp;
	if(PressCyc == 0){
		DispData.All = TouchValue.CenterKey;
		if(TouchValue.CycKey)
			PressCyc = 1;
	}
	else{
		DispData.All = 0;
		if(!TouchValue.AllKey)
			PressCyc = 0;
	}
	DispData.EN = TouchValue.AllKey;
	
	temp = TouchValue.Deg*32/360;
	if(DispData.Pos != temp){
		if(temp > DispData.Pos){
			if(temp - DispData.Pos <= 16)
				DispData.Pos++;
			else{
				if(DispData.Pos)DispData.Pos--;
				else DispData.Pos = 31;
			}
		}
		else if(temp < DispData.Pos){
			if(DispData.Pos - temp <= 16)
				DispData.Pos--;
			else{
				if(DispData.Pos<31)DispData.Pos++;
				else DispData.Pos = 0;
			}
		}
	}
}












