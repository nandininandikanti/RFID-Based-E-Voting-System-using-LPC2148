 #include "uart.h"
 #include "lcd.h"
 #include "string.h"
 #include "delay.h"
 #include "keypad.h"
 #include "i2c.h"
 #include "i2c_eeprom.h"
 #include <stdlib.h>
 void voting(unsigned int);
void edit_password(void);
void voting_result(void);
void voting_reset(void);
extern char buff[200];  
extern unsigned char r_flag;
u8 r_password[5];
u8 officer_password[5];
unsigned char a_password[5]="1234";
 unsigned char result_password[5]="4321";

const char* card[]={"12626214","12534588","12534288"};
char officer_card[]="12608176";
int main()
{
char card_flag;
u8 choice;
int i;
	InitLCD();
   InitUART0();
   init_i2c();
   Init_KPM();
  first: CmdLCD(0x01); 
  		 StrLCD("RFID BASED E-VOTING");
   		 CmdLCD(0xC0);
   		 StrLCD("   SYSTEM DESIGN  ");
		 delay_s(2);
//	i2c_eeprom_page_write(0x50,0x0000,a_password,4);
	i2c_eeprom_seq_read(0x50,0x0000,r_password,4);
	i2c_eeprom_page_write(0x50,0x0010,result_password,4);
	i2c_eeprom_seq_read(0x50,0x0010,officer_password,4);
  /* for(i=0;i<3; i++)
   {
   i2c_eeprom_write(0x50,0x0060+i,0);
   }*/
		//StrLCD(r_password);
		//delay_s(1);
   while(1)
   {
   card_flag=0; 
  l3: CmdLCD(0x01);
   CmdLCD(0x80);
   StrLCD("waiting for card...");
   CmdLCD(0xC0);
   StrLCD("..............");
   memset(buff,'\0',200);
   while(r_flag<2);
   //StrLCD("ok");
   r_flag=0;
   for(i=0; i<3; i++) 
	{
   if((strcmp(buff,card[i]))==0)
   {
   card_flag=1;
   CmdLCD(0x01);
	CmdLCD(0x80);
   StrLCD("1.VOTING");
   CmdLCD(0XC0);
   StrLCD("2.EDIT PASSWORD");
   CmdLCD(0X94);
   StrLCD("3.EXIT");
  // delay_s(5);
   //goto l3;
   choice=KeyDetectTimout(5000);
   //choice=KeyDetect();
   delay_ms(100);
   if(choice=='0')
   {
   goto l3;
   }	   
   //CharLCD(choice);
   //delay_ms(100);
   switch(choice){
   case '1': voting(i); break;
   case '2': edit_password(); break;
   case '3': goto first;
   }
	//delay_ms(1000);
   //break;
   }
 	  else if((strcmp(buff,officer_card))==0)
	  {
		   card_flag=1;
   CmdLCD(0x01);
	CmdLCD(0x80);
   StrLCD("1.VOTING RESULT");
   CmdLCD(0XC0);
   StrLCD("2.VOTING RESET");
   CmdLCD(0X94);
   StrLCD("3.EXIT");
  // choice=KeyDetect();
   delay_ms(100);
  	  choice=KeyDetectTimout(5000);
	  if(choice=='0')
   {
   goto l3;
   }
   //CharLCD(choice);
   delay_ms(100);
   switch(choice){
   case '1': voting_result(); break;
   case '2': voting_reset(); break;
   case '3': goto first;
   }
	  }
	}
	if((card_flag==0))
	{
	CmdLCD(0x01);
	CmdLCD(0x80);
	  StrLCD("invalid card");
	  delay_ms(1000);
	}
	card_flag=0; 
}				
				  }		




