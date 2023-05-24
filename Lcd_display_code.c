#include<pic.h>
#include<xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define _XTAL_FREQ   8000000


#define rs RC0
#define rw RC1
#define en RC2
#define sw RC4;
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
#define delay for(j=0;j<5000;j++)

#define SWITCH_PRESSED    1
#define SWITCH_BOUNCE     0
#define LED_ON           1
#define LED_OFF          0
#define LED    RB4
#define SWITCH  RA0
#define DEBOUNCE_VALUE  240

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
//#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

unsigned char Pattern1[]= {0x0e,0x0e,0x04,0x04,0x1f,0x04,0x0a,0x0a};
unsigned char Pattern2[]= {0x0e,0x0e,0x15,0x0e,0x04,0x04,0x0a,0x11};
unsigned char Pattern3[]= {0x00,0x00,0x0a,0x15,0x11,0x0a,0x04,0x00};
unsigned char Pattern4[]= {0x00,0x00,0x0a,0x1f,0x1f,0x0e,0x04,0x00};

int j;
void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char s);
void CreateCustomCharacter (unsigned char *Pattern, const char Location);

void main()
{
    unsigned int i;
    TRISD=TRISC0=TRISC1=TRISC2=0;
    TRISBbits.TRISB1 = 1;
    //int r2 = rand() % 2;
    //int r1 = rand() % 16;
    lcd_init();
    CreateCustomCharacter(Pattern1, 1);
    CreateCustomCharacter(Pattern2, 2);
    CreateCustomCharacter(Pattern3, 3);
    CreateCustomCharacter(Pattern4, 4);
    //unsigned char pos = 0x46;
    //cmd(pos);
    //dat(1);
   
    while(1) {
        cmd(0x88);
        dat(1);

        cmd(0x46);
        dat(1);
        cmd(0x48);
        dat(3);
        //delay;
        //delay;
        cmd(0x46);
        dat(2);
        cmd(0x48);
        dat(4);
        delay;
        //delay;
        /*if(PORTBbits.RB1)      
        {
            cmd(pos);
            pos += 0x01;
        }*/

       
    }


}

int isSwitchPressed(void)
{
    int switchStatus =  SWITCH_BOUNCE;
    if(SWITCH == SWITCH_PRESSED)
    {
        __delay_us(DEBOUNCE_VALUE); //Wait time more then bouncing period
        if(SWITCH == SWITCH_PRESSED)
        {
            switchStatus = SWITCH_PRESSED;
        }
    }
    return switchStatus ;
}

void lcd_init()
{
    cmd(0x38);
    cmd(0x0f);
    cmd(0x40);
}
void CreateCustomCharacter (unsigned char *Pattern, const char Location)
{
    int i=0;
    cmd (0x80+(Location*8));     //Send the Address of CGRAM
    for (i=0; i<8; i++) {
        dat(Pattern[i]);
    }//Pass the bytes of pattern on LCD
}
void cmd(unsigned char a)
{
    PORTD=a;
    rs=0;
    rw=0;
    en=1;
    delay;
    en=0;
}
void dat(unsigned char b)
{
    PORTD=b;
    rs=1;
    rw=0;
    en=1;
    delay;
    en=0;
}
void show(unsigned char s)
{
    while(s) {
        dat(s++);
    }
}
