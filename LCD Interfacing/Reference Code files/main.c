//===============================================================
#define _XTAL_FREQ 20e6
#include <xc.h>
#include "lcd.c"
#include "adc.c"
#include "uart.c"
//#include "interrupts.c"
//***************************************************************
#define ms __delay_ms
#define REL1 RC2	     //Relay 1
#define MODE RB1         //Mode - Auto/Manl
#define PMP RB0          //Pump 
#define HIGH 1
#define LOW 0

int mode=0,pump=0,lig_tx;
float ldr, moist=0;


//device configuration bit
//__CONFIG(0x1E32);
// PIC16F877A Configuration Bit Settings
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = ON          // Flash Program Memory Code Protection bit (All program memory code-protected)

void delay(unsigned int DD)
{
    unsigned int i,j;
    for(i=1;i<=DD;i++)
    {
    for(j=1;j<=5000;j++);
    }
}

void delay_ms(unsigned int DD)
{
    unsigned int i,j;
    for(i=1;i<=DD;i++)
    {
    for(j=1;j<=500;j++);
    }
}

void io_init()
{
    //ADCON1 = 0x0F;   // Default all pins to digital
    TRISA  = 0xFF;   // Analog Inputs
    TRISB  = 0b00001011;   // 0 --> Output, 1 --> Input
    TRISC  = 0b11111000;
    TRISD  = 0x00;
    PORTC =  0xFF;
    OPTION_REG = 0x00;
}


//void check_parameters()
//{
//    if(tmp>tmp_max)
//    {
//        lcd_clr();
//        lcd_string("ALT:TEMP. EXCEED");
//        BUZ = 1;
//        REL1 = 0;
//        delay(250);
//        BUZ = 0;
//        lcd_clr();
//    }
//    
//    else
//    {
//        BUZ = 0;  REL1 = 1;   
//    }
//    
//    
//    
//    if(rh>hum_max)
//    {
//        lcd_clr();
//        lcd_string("ALT:HUMI. EXCEED");
//        BUZ = 1;
//        REL4 = 0;
//        delay(250);
//        BUZ = 0;
//        lcd_clr();        
//    }
//    
//    else
//    {
//        BUZ = 0;  REL4 = 1;   
//    }
//    
//    
//    
//    if(lig_tx>light_max)
//    {
//        lcd_clr();
//        lcd_string("ALT:LIGHT EXCEED");
//        BUZ = 1;
//        REL2 = 0;
//        delay(250);
//        BUZ = 0;
//        lcd_clr();
//    }
//    
//    else
//    {
//        BUZ = 0;  REL2 = 1; 
//    }
//    
//    if(pot>pot_max)
//    {
//        lcd_clr();
//        lcd_string("ALT:VOLT. EXCEED");
//        BUZ = 1;
//        REL3 = 0;
//        delay(250);
//        BUZ = 0;
//        lcd_clr();
//    }
//    
//    else
//    {
//        BUZ = 0;  REL3 = 1; 
//    }  
//}



//________________________________________
void display_moisture()
{
    ldr = moist;
    if(ldr <= 0.5)
        {
            lcd_cmd(0x85);
            lcd_string("100%     ");
            //lig_tx = 100;
        }
        else if(ldr > 0.5 && ldr <= 1.0)
        {
            lcd_cmd(0x85);
            lcd_string("90%    ");
            //lig_tx = 90;
        }
        else if(ldr > 1.0 && ldr <= 1.5)
        {
            lcd_cmd(0x85);
            lcd_string("80%    ");
            //lig_tx = 80;
        }
        else if(ldr > 1.5 && ldr <= 2.0)
        {
            lcd_cmd(0x85);
            lcd_string("70%    ");
            //lig_tx = 70;
        }
        else if(ldr > 2.0 && ldr <= 2.5)
        {
            lcd_cmd(0x85);
            lcd_string("60%   ");
            //lig_tx = 60;
        }
        else if(ldr > 2.5 && ldr <= 3.0)
        {
            lcd_cmd(0x85);
            lcd_string("50%   ");
            //lig_tx = 50;
        }
        else if(ldr > 3.0 && ldr <= 3.5)
        {
            lcd_cmd(0x85);
            lcd_string("40%   ");
            //lig_tx = 40;
        }
        else if(ldr > 3.5 && ldr <= 4.0)
        {
            lcd_cmd(0x85);
            lcd_string("30%   ");
            //lig_tx = 30;
        }
        else if(ldr > 4.0 && ldr <= 4.5)
        {
            lcd_cmd(0x85);
            lcd_string("20%   ");
            //lig_tx = 20;
        }
        else if(ldr > 4.5 && ldr <= 4.8)
        {
            lcd_cmd(0x85);
            lcd_string("10%   ");
            //lig_tx = 10;
        }
        else if(ldr > 4.8)
        {
            lcd_cmd(0x85);
            lcd_string("0%    ");
            //lig_tx = 0;
        }
}
//________________________________________
void lcd_update()
{
    lcd_cmd(0xC0);
    lcd_string("MODE:");
    
    if(mode == 0)
    {
        lcd_string("MANL");
    }
    else
    {
        lcd_string("AUTO");
    }
    
    if(pump == 1)
    {
        lcd_string(" P:OFF");
        REL1 = 0;
    }
    else
    {
        lcd_string(" P:ON ");
        REL1 = 1;
    }
}


void status_check()
{
    if(MODE==0) //if mode button is pressed
    {  
        ms(200);
        while(MODE==0);
        
        if(mode == 0)
        {
            mode = 1;
            pump = 1; //Pump off
        }    
            
        else
        {
            mode = 0;
        }
    }    
    
    if(mode==0)  //Change only in manual modes
    {
        if(PMP==0)
        {
            pump = 0;  //Pump on
        }
        else
        {
            pump = 1; //Pump off
        }   
    }
    
    else // Action in Auto Mode
    {
        if(ldr<=1)
        {
            pump = 1; //Pump off
        }
        else if(ldr>=4.0)
        {
            pump = 0;  //Pump on
        }
    }
}

void main(void)
{
    io_init();        
    // Pump Test
    //REL1 = 1;    ms(100);     
    REL1 = 0;
    lcd_init();
    status_check();
    // Display Project Title
    lcd_clr();
    lcd_string("Smart Irrigation");
    lcd_cmd(0xC0);
    lcd_string("     System     ");
    ms(2000);
    //Default Action Screen
    lcd_clr();
    lcd_string("SOIL:");
    lcd_update();
    
    
    while(1)
    {
        status_check();
        lcd_update();
        moist  = ((adc_conv0()*5.0)/1023);
        display_moisture();
        ms(100);
    }
}