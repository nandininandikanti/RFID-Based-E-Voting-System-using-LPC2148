#include <LPC21xx.H>

#include "types.h"
#include "keypad_defines.h"
#include "lcd.h"
#include "delay.h"


//keypad lookup table
char keypad_lut[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

//variables declaration
unsigned char row_val,col_val;
unsigned int c0,c1,c2,c3;

void Init_KPM(void)
{
	//configuring the rows port pins as output port pins
	IODIR1 |= (1<<ROW0 | 1<<ROW1 | 1<<ROW2 | 1<<ROW3);
	//initially making all rows to zero 
	//IOCLR1 = (1<<ROW0 | 1<<ROW1 | 1<<ROW2 | 1<<ROW3);
	//initially making all columns to one
	//IOSET1 = (1<<COL0 | 1<<COL1 | 1<<COL2 | 1<<COL3);
}	

//read the all columns status and any one of the column 
//is 0 then switch is pressed. if switch is pressed return 0 
//otherwise return 1																
u32 ColScan(void)
{
	c0 = (IOPIN1>>COL0)&1;
	c1 = (IOPIN1>>COL1)&1;
	c2 = (IOPIN1>>COL2)&1;
	c3 = (IOPIN1>>COL3)&1;
	if((c0&c1&c2&c3) == 0)
		return 0;
	else
		return 1;	
}

//row identification process

u32 RowCheck(void)
{
	u32 rNo;
	
	while(1)
	{
		//row0 identification
		
		//row0 making low
		//remining rows making high
		IOCLR1 = 1<<ROW0;
		IOSET1 = (1<<ROW1 | 1<<ROW2 | 1<<ROW3);
		//if any one of the column value is 0 then row value 
		//is 0 then no need to check other rows
		if(ColScan() == 0)
		{
			rNo = 0;
			return rNo;;
		}
		//row1 identification
		//row1 making low
		//remining rows making high
		IOCLR1 = 1<<ROW1;
		IOSET1 = (1<<ROW0 | 1<<ROW2 | 1<<ROW3);
		//if any column value is 0 then row value is 1 then no 
		//need to check other rows
		if(ColScan() == 0)
		{
			rNo = 1;
			return rNo;
		}
		
		//row2 identification
		//row2 making low
		//remining rows making high
		IOCLR1 = 1<<ROW2;
		IOSET1 = (1<<ROW0 | 1<<ROW1 | 1<<ROW3);
		//if any column value is 0 then row value is 2 then no need to check other rows
		if(ColScan() == 0)
		{
			rNo = 2;
			return rNo;
		}
		//row3 identification
		//row3 making low
		//remining rows making high
		IOCLR1 = 1<<ROW3;
		IOSET1 = (1<<ROW0 | 1<<ROW1 | 1<<ROW2);
		//if any column value is 0 then row value is 3
		if(ColScan() == 0)
		{
			rNo = 3;
			return rNo;
		}
	}
}

//columns identification process
//if col0 status is low then column value is 0
//else if col1 status is low then column value is 1
//else if col2 status is low then column value is 2
//else if col3 status is low then column value is 3
u32 ColCheck(void)
{
	u32 cNo;
	
	if(((IOPIN1>>COL0)&1) == 0)
		cNo = 0;
	else if(((IOPIN1>>COL1)&1) == 0)
		cNo = 1;
	else if(((IOPIN1>>COL2)&1) == 0)
		cNo = 2;
	else if(((IOPIN1>>COL3)&1) == 0)
		cNo = 3;
	return cNo;	
}	
u8 KeyDetect(void)
{
//int t=0;
//while(t<time);

{
	//making all rows to zero 
	IOCLR1 = (1<<ROW0 | 1<<ROW1 | 1<<ROW2 | 1<<ROW3);
	//waiting for switch press
	while(ColScan()==0);
	{	
  //row identification process
	row_val = RowCheck();

	//columns identification process
	col_val = ColCheck();
	}	
	//waiting for switch release
	while(!ColScan());
 }
	//collect the pressed switch equivalent value from defined keypad lookup table and display it on any o/p device		
	return keypad_lut[row_val][col_val];	
}

void password(u8 *a)
{

	unsigned char ch;
	int i=0;
	while(1)
	{
	ch=KeyDetect();
	delay_ms(100);
	if(ch>='0'&&ch<='9')
	{
	if(i==19)
	{
	continue;
	}
	a[i]=ch;
	CmdLCD(0xC0+i);
	CharLCD(a[i]);
	delay_ms(100);
	CmdLCD(0xC0+i);
	CharLCD('*');
	i++;
	}
	else if(ch=='C')
	{
		if(i==0)
			continue;
	   	 i--;
		 CmdLCD(0xC0+i);
		 CharLCD(' ');
		 CmdLCD(0xC0+i);
		 //i--;
	}
	else if(ch=='D')
	{
	break;
	}
	}
}

 u8 KeyDetectTimout(unsigned int time)
{
int t=0;
while(t<time)

{
	//making all rows to zero 
	IOCLR1 = (1<<ROW0 | 1<<ROW1 | 1<<ROW2 | 1<<ROW3);
	//waiting for switch press
	if(ColScan()==0)
	{	
  //row identification process
	row_val = RowCheck();

	//columns identification process
	col_val = ColCheck();
		
	//waiting for switch release
	while(!ColScan());
	delay_ms(50);
	//collect the pressed switch equivalent value from defined keypad lookup table and display it on any o/p device		
	return keypad_lut[row_val][col_val];	
	}
	delay_ms(50);
	  t=t+50;
	}
	return '0';
}  


