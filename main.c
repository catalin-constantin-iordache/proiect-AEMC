/*Template Proiect AEMC - ETTI Iasi*/
#include <xc.h>
#include <pic.h>
#include <pic16f887.h>
#include <math.h>
__PROG_CONFIG(1,0x20D4); 	// config. uC 
__PROG_CONFIG(2,0x0000); 	// config. uC 

#define LED1 RB0 
#define LED2 RB1
#define _XTAL_FREQ 8000000
char *Temperature = (char *) "temperature";
 unsigned int ADC_Read(char channel);
void init_ADC(void);
void init_uC(void); 
void interrupt etti(void); 	// functie de intreruperi globala ptr. TOATE intreruperile de pe un 
void init_LCD(void);
void lcd_clear(void);
void lcd_goto(unsigned char pos);
void lcd_puts(char * s);
void lcd_putch(char c);
void lcd_write(unsigned char c);
void delay_LCD(unsigned long t);
void initializare_diacritice(void);
void scrie_diacritice_in_CGRAM(char matrice[8], unsigned char pozitie_DDRAM,unsigned char *diacritic);
void afisare_diacritice(unsigned char diacritic_afisat, unsigned char linia);

void main(void)
 {
	
    init_uC();
	init_LCD();
	init_ADC(); 
	 
	while (1)
	{
		//lcd_clear();
	   float temp = ADC_Read(1)*5000.0/1024.0;;
	   temp=temp/10.0;
	  
	   int s1=(int)temp;
	   int s2 = (int)((temp - (float)s1)*10);
	   
	     
		
	   lcd_goto(0x80);	
		delay_LCD(100);
		char *temperature = (char *) "Temperature:";
       lcd_puts(temperature);
		char *pozitiv = (char *) "+";
        char *negativ = (char *) "-";
		lcd_goto(0xC0);
	    delay_LCD(100); 
		if(s1>=0){
		lcd_puts(pozitiv);
			}
		if(s1<0){
				lcd_puts(negativ);
			}
		char str1 = (s1/100) + '0';
		lcd_goto(0xC1);
	    delay_LCD(100); 
		lcd_putch(str1);
		if(s1>=100){
		 s1=s1 % 100;
			}
		str1 = (s1/10) + '0';
	    lcd_goto(0xC2);
	    delay_LCD(100); 
		lcd_putch(str1);
		s1=s1 % 10;
		str1 = (s1) + '0';
	   lcd_goto(0xC3);
	    delay_LCD(100); 
		lcd_putch(str1);
        str1 = s2 + '0';		
	   lcd_goto(0xC4);
	    delay_LCD(100); 
       lcd_putch('.');
       lcd_goto(0xC5);	
	    delay_LCD(100);
       lcd_putch(str1);
       lcd_goto(0xC6);
        delay_LCD(100);	
		afisare_diacritice(0xDF,0xC6);
       lcd_goto(0xC7);
	   lcd_putch('C');
	   
	 
	   
	}
 }
 
 void init_ADC(){
   ADCON1 = 0x80;
    TRISA = 0x02;
    //ADCON0 = 0x81;
   }
    
    
  unsigned int ADC_Read(char channel) {
	 __delay_ms(10);
   ADCON0 &=0xc3;
   ADCON0 |=(channel<<2);
   ADCON0 |=0xc5;
	 __delay_ms(2);
   GO_nDONE=1;
  while(GO_nDONE==1);   
  
  return ((ADRESH<<8)+ADRESL); 
   }  
 void init_uC (void) 
{ 
	OSCCON 	= 0x71;			// setez Osc. intern uC de 8MHz // pag. 64
	TRISA   = 0b11111111;
    TRISB 	= 0b00000000;	// tot Portul B este de iesire
	TRISD 	= 0b00000000;	// tot Portul B este de iesire
	ANSEL  = 0x02;
	ANSELH  = 0x00;
	PORTB 	= 0b00000000;	// initializez PORTB cu valori de 0 logic
	OPTION_REG = 0b00000111;// Frecv. intrare T0 = Frecv. Osc./4 (=8MHz/4) = 2MHz
							// prescaler=256 => Frecv. T0 = 2MHz/256 = KHz (sau T=128us)
	TMR0IF 	= 0;			// 
	TMR0 	= 178;	  		// porneste numararea de la valoarea 178; pana la maxim (255) 
	TMR0IE 	= 1;    		// activez intreruperea ptr. T0
	GIE 	= 1;       		// activez Global intreruperile
} 

void interrupt etti(void)	// ajung aici la fiecare 78*128us=~10ms
{       
	if(TMR0IF)				// daca flagul TMR0IF=1
	{
		 TMR0IF = 0;   		// obligatoriu - sterg flagul Timerului care a generat intreruperea
		 TMR0 = 178;  		// 
		 LED2 = !LED2; 		// 
	}
}

