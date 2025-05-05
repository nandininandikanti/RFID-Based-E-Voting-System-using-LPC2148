#include "keypad.h"
#include "delay.h"
#include "lcd.h"
#include "types.h"
#include "string.h"
#include "i2c_eeprom.h"

int p1_cnt,p2_cnt,p3_cnt,p4_cnt,status;
u8 cgramLUT[]={0x17,0x14,0x14,0x1F,0x05,0x05,0x1D,0x00,0x0E,0x0E,0x04,0x15,0x04,0x0A,0x0A,0x00,0x15,0x15,0x15,0x1F,0x04,0x04,0x04,0x00,0x1B,0x1F,0x1F,0x1F,0x0E,0x00,0x00};
void voting(unsigned int);
void edit_password(void);
void voting_result(void);
extern u8 r_password[5];
extern u8 officer_password[5];
u8 n_password[5];
u8 c_password[5];
u8 cnf_password[5];
void symblos(void)
{
CmdLCD(0x01);
CmdLCD(0x80);
StrLCD("PARTY_1--->");
BuildCGRAM(cgramLUT,32);
CmdLCD(0x8C);
CharLCD(0);
//CmdLCD(0x8C);

CmdLCD(0xC0);
StrLCD("PARTY_2--->");
//BuildCGRAM(cgramLUT,16,0x48);
CmdLCD(0xCC);
CharLCD(1);
//CmdLCD(0xCC);

CmdLCD(0x94);
StrLCD("PARTY_3--->");
//BuildCGRAM(cgramLUT,24,0x50);
CmdLCD(0xA0);
CharLCD(2);
//CmdLCD(0x9F);

CmdLCD(0xD4);
StrLCD("PARTY_4--->");
//BuildCGRAM(cgramLUT,32,0x58);
CmdLCD(0xE0);
CharLCD(3);
//CmdLCD(0xDF);
}
void voting(unsigned int n)
{
char ch;
status=i2c_eeprom_read(0x50,0x0060+n);
	if(status==1)
	{
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("vote already done");
	delay_s(1);
	return;
	}
//u8 result;	
    CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("enter the password");
	CmdLCD(0xC0);
	password(c_password);
	if(strcmp(r_password,c_password)==0)
	{
	   symblos();
	   delay_s(2);
	   ch=KeyDetect();
	   switch(ch)
	   {
	   case '1': p1_cnt++; 
	   i2c_eeprom_write(0x50,0x0020,p1_cnt);
	   //result=i2c_eeprom_read(0x50,0x0020);
	   CmdLCD(0x01);
	   CmdLCD(0x80);
	   StrLCD("voted for party_1");
	   //CmdLCD(0xC0);
	   //CharLCD(result);
	   delay_s(1);
	   break;
	   case '2': p2_cnt++;
	   i2c_eeprom_write(0x50,0x0030,p2_cnt++);
	   CmdLCD(0x01);
	   CmdLCD(0x80);
	   StrLCD("voted for party_2");
	   delay_s(1);
	    break;
	   case '3': p3_cnt++; 
	   i2c_eeprom_write(0x50,0x0040,p3_cnt++);
	   CmdLCD(0x01);
	   CmdLCD(0x80);
	   StrLCD("voted for party_3");
	   delay_s(1);
	   break;
	   case '4': p4_cnt++;
	   i2c_eeprom_write(0x50,0x0050,p4_cnt++);
	   CmdLCD(0x01);
	   CmdLCD(0x80);
	   StrLCD("voted for party_4");
	   delay_s(1);
	   break;
	   }
	   i2c_eeprom_write(0x50,0x60+n,1);
	 
	}
	else
	{
		CmdLCD(0x01);
	   CmdLCD(0x80);
	   StrLCD("Invalid password");
	   delay_s(1);
	}
}
void edit_password(void)
{
CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("enter old password");
	password(c_password);
	if(strcmp(r_password,c_password)==0)
	{
	l1:CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("enter the new pwd");
	password(n_password);
	if(strcmp(r_password,n_password)==0)
	{
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("you entered same pwd");
	delay_s(1);
	   goto l1;
	}
	else
	{
	l2: CmdLCD(0x01);
	CmdLCD(0x80);
	  StrLCD("confrim password");
		 password(cnf_password);
		 if(strcmp(n_password,cnf_password)==0)
		 {
		 i2c_eeprom_page_write(0x50,0x0000,cnf_password,4);
		 i2c_eeprom_seq_read(0x50,0x0000,r_password,4);
		 CmdLCD(0x01);
	     CmdLCD(0x80);
		 StrLCD("password updated");
		 delay_s(2);
		 }
		 else
		 {
		 CmdLCD(0x01);
		 CmdLCD(0x80);
		 StrLCD("password not match");
		 delay_s(2);
		 goto l2;
		 }

	}

	}
	else
	{
	CmdLCD(0x01);
	   CmdLCD(0x80);
	   StrLCD("invalid password");
	   delay_s(1);
	}

}

void voting_result(void)
{
int p1_res,p2_res,p3_res,p4_res;
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("enter the password");
	password(c_password);
	if(strcmp(officer_password,c_password)==0)
	{
		  p1_res=i2c_eeprom_read(0x50,0x0020);
		  p2_res=i2c_eeprom_read(0x50,0x0030);
		  p3_res=i2c_eeprom_read(0x50,0x0040);
		  p4_res=i2c_eeprom_read(0x50,0x0050);

		  CmdLCD(0x01);
		  CmdLCD(0x80);
		  StrLCD("party_1 count:");
		  CmdLCD(0x8F);
		  U32LCD(p1_res);

		  CmdLCD(0xC0);
		  StrLCD("party_2 count:");
		  CmdLCD(0xCF);
		  U32LCD(p2_res);

		  CmdLCD(0x94);
		  StrLCD("party_3 count:");
		  CmdLCD(0xA3);
		  U32LCD(p3_res);

		  CmdLCD(0xD4);
		  StrLCD("party_4 count:");
		  CmdLCD(0xE3);
		  U32LCD(p4_res);
			delay_s(5);
		  if((p1_res>p2_res)&&(p1_res>p3_res)&&(p1_res>p3_res))
		  {
		  CmdLCD(0x01);
		  CmdLCD(0x80);
		  StrLCD("winner: party_1");
		  delay_s(5);
		  }
		  else if((p2_res>p1_res)&&(p2_res>p3_res)&&(p2_res>p4_res))
		  {
		  	 CmdLCD(0x01);
		  	 CmdLCD(0x80);
			 StrLCD("winner: party_2");
			 delay_s(5);
		  }
		  else if((p3_res>p1_res)&&(p3_res>p2_res)&&(p3_res>p4_res))
		  {
		  	CmdLCD(0x01);
		  	CmdLCD(0x80);
			 StrLCD("winner: party_3");
			 delay_s(5);
		  }
		  else if((p4_res>p1_res)&&(p4_res>p2_res)&&(p4_res>p3_res))
		  {
		  		CmdLCD(0x01);
		  	    CmdLCD(0x80);
			  StrLCD("winner: party_4");
			  delay_s(5);
		  }
		  else if((p1_res==0)&&(p2_res==0)&&(p3_res==0)&&(p4_res==0))
		  {
				 CmdLCD(0x01);
		  	    CmdLCD(0x80);
			  StrLCD("voting not done");
			  delay_s(5);
		  }
	}
	else
	{
	CmdLCD(0x01);
	   CmdLCD(0x80);
	   StrLCD("Invalid password");
	   delay_s(1);
	}

}
void voting_reset(void)
{
int n;
   CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("enter the password");
	password(c_password);
	if(strcmp(officer_password,c_password)==0)
	{
	for(n=0; n<=2; n++)
	{
	i2c_eeprom_write(0x50,0x60+n,0);
	}
	   i2c_eeprom_write(0x50,0x0020,0);
	   i2c_eeprom_write(0x50,0x0030,0);
	   i2c_eeprom_write(0x50,0x0040,0);
	   i2c_eeprom_write(0x50,0x0050,0);

	   if((p1_cnt==0)&&(p2_cnt==0)&&(p3_cnt==0)&&(p4_cnt==0))
	   {
	   CmdLCD(0x01);
	   CmdLCD(0x80);
	   StrLCD("result is reset");
	   delay_s(2);
	   }

	}
}

