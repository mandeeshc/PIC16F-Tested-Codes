#include <stdio.h>
#include <string.h>
//===============================================================
#define LCD PORTD
#define RS RC0	     //LCD Register Select
#define EN RC1	     //LCD Enable

void lcd_delay(unsigned int);
void lcd_data(unsigned char);
void lcd_cmd(char);
void disp_val(char, int, int);
void disp_val_float(char,float,int);
void lcd_clr();
char numStr[10];


void lcd_init()
{
    RS=0;
    lcd_cmd(0x38);
    lcd_cmd(0x01);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x80);
    EN=1;
    lcd_delay(20);
    EN=0;
}

void lcd_data(unsigned char dta)
{
    RS=1;
    LCD = dta;
    EN=1;
    lcd_delay(20);
    EN=0;
}

void lcd_cmd(char cmmd)
{
    RS=0;
    LCD = cmmd;
    EN=1;
    lcd_delay(20);
    EN=0;
}


void lcd_delay(unsigned int DD)
{
    unsigned int i,j;
    for(i=1;i<=DD;i++)
    {
    for(j=1;j<=50;j++);
    }
}

void lcd_clr()
{
    lcd_cmd(0x01);
    lcd_cmd(0x80);
}

void lcd_clr2()
{
int var;
lcd_cmd(0xC0);
for(var = 0; var < 4; var++)
	{
	 lcd_data(' ');
	}
lcd_cmd(0xC0);
}


void lcd_string(const char *dat)
{
    while(*dat)
    lcd_data(*dat++);
}

//****************************************
void disp_val(char add, int value, int space)
{
	char arr_t[10];
	int c1=0,i;
	int v1=value;
	lcd_cmd(add);
	for(i=0;i<space;i++)
	{
		lcd_data(' ');
	}
	if(v1!=0)
	{
		while(v1!=0)
		{
			arr_t[c1] = (v1%10)+48;
			v1 = v1/10;
			c1++;
		}
		lcd_cmd(add);
		for(i=c1-1;i>=0;i--)
		{
			lcd_data(arr_t[i]);
		}
	}
	else
	{
		lcd_cmd(add);
		lcd_data('0');
	}
}

void disp_val_float(char add, float total, int space)
{
  int i=0;
  int wholeNum = total;
  int decimalNum = (int) ((total - wholeNum) * 100);
  char wholeNumStr[6];
  char decNumStr[4];
  char decPtStr = '.';
  sprintf(wholeNumStr, "%d", wholeNum);
  sprintf(decNumStr, "%d", decimalNum);
  strcpy(numStr, wholeNumStr);
  strcat(numStr, ".");
  decNumStr[1]='\0';
  strcat(numStr, decNumStr);
  strcat(numStr, "\0");
  lcd_cmd(add);
  for(i=0;i<space;i++)
  {
  	lcd_data(' ');
  }
  i=0;
  lcd_cmd(add);
  while(numStr[i]!='\0')
  {
  	lcd_data(numStr[i]);
	i++;
  }
}


