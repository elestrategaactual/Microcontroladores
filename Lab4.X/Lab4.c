#include<xc.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"
#include <math.h>
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF
#pragma config LVP=OFF
int estado=0;
int var1;
int var2;
double res;
int op;
char Teclal='N';
int Tecla=10;
int contador=0;
int led;
unsigned char pacman[8] = {0b11100, 0b11010, 0b01111, 0b00111, 0b00011, 0b00111, 0b01110, 0b11100};
unsigned char fantasma[8] = {0b01110, 0b11111, 0b10101, 0b11111, 0b11111, 0b11111, 0b10101, 0b10101};
void operar(void);
double factorial(int);
void __interrupt() ISR(void);
void main(void){
    ADCON1=15;
    TRISB=0b11110000;
    TRISD=0;
    LATB=0b00000000;
    LATD=0;
    TRISC=y el puerto C p0;
    RBPU=0;
    led=0;
    __delay_ms(1000);
    ConfiguraLCD(4);
    InicializaLCD();
    CaracterEspecial(0 , pacman);
    CaracterEspecial(1, fantasma);
    BorraLCD();
    MensajeLCD_Var("Lab 4");
    EscribeLCD_c(0);
    for (int i = 1; i<=5;i++){
        __delay_ms(1000);
        DesplazaCursorI();
        EscribeLCD_c(' ');
        DesplazaCursorI();
        DesplazaCursorI();
        EscribeLCD_c(0);
    }
    DesplazaCursorI();
    EscribeLCD_c(1);
    __delay_ms(1000);
    BorraLCD();
    if(POR==0){
    MensajeLCD_Var("R energia");
    POR=1;
    }
    else {
    MensajeLCD_Var("R MCLR");
    }
    __delay_ms(1000);
    BorraLCD();
      //se inicializa el timer0 y se configura
    //TMR0L=0xDA;
    //TMR0H=0xD9;
    T0CON=0b10000001;
    TMR0=3036;
    TMR0IF=0;
    TMR0IE=1;
    //TMR0ON=1;
    //interrupccion de telcado
    RBIF=0;
    RBIE=1;
    GIE=1;
    while(1){ 
    }
}

void operar(void){
   if (op=='+'){
       res=var1+var2;
       EscribeLCD_n8(res, 2);
    }
   else if (op=='-'){
       res=var1-var2;
       if(res<0){EscribeLCD_c('-');
       res=res*(-1);
       }
       EscribeLCD_n8(res, 2);
   }
   else if (op=='*'){
       res=var1*var2;
       EscribeLCD_n8(res, 2);
   }
   else if(op=='/'){
       if(var2==0){
           if(var1==0)MensajeLCD_Var("INDEF");
           else MensajeLCD_Var("INF");
           return;
       }
       res=(1.0*(var1))/(1.0*(var2));
       EscribeLCD_n8(res, 1);
       EscribeLCD_c('.');
       res=((((var1%var2)*100.0)/var2));
       EscribeLCD_n8(res, 2);
       
   }
   else if(op=='^'){
       res=pow(var1,var2);
       EscribeLCD_n64(res, 9);//revizar esto
   }
   else if(op=='!'){
       res=factorial(var1);
       EscribeLCD_n64(res, 6);//revizar esto
   }
}
double factorial(int n){
    double r=1;
    if (n>1){
        for (int i = 1; i<=n;i++){
            r=r*i;
        }
    }
    return r;
}
void __interrupt() ISR(void){
    if(RBIF==1){
        contador=0;
        if(PORTB!=0b11110000){
            led++;
            LATC=led;
            led=led%8;
            Tecla=10;
            Teclal='N';
            LATB=0b11111110;
            if(RB4==0) Tecla=1;
            else if(RB5==0) Tecla=2;
            else if(RB6==0) Tecla=3;
            else if(RB7==0) {
                if(RE0==0)Teclal='+';
                else Teclal='^';
            }
            else{
                LATB=0b11111101;
                if(RB4==0) Tecla=4;
                else if(RB5==0) Tecla=5;
                else if(RB6==0) Tecla=6;
                else if(RB7==0) {
                if(RE0==0)Teclal='-';
                else Teclal='!';
            }
                else{
                    LATB=0b11111011;
                    if(RB4==0) Tecla=7;
                    else if(RB5==0) Tecla=8;
                    else if(RB6==0) Tecla=9;
                    else if(RB7==0) Teclal='*';
                    else{
                        LATB=0b11110111;
                        if(RB4==0) Teclal='B';
                        else if(RB5==0) Tecla=0;
                        else if(RB6==0) Teclal='=';
                        else if(RB7==0) Teclal='/';
                    }
                }
                
            }
            LATB=0b11110000;
        }
        if(((estado==0) ||(estado==4))&&(Tecla !=10)){
            if(estado==4){
                BorraLCD();
                estado=0;
            }
            var1=Tecla;
            EscribeLCD_n8(var1, 1);
            Tecla=10;
            estado++;
        }
        else if((Teclal=='B')&&(estado>0)){
            if(estado==4){
            BorraLCD();
            estado=0;
            }
            else{
            if(op=='!'){estado--;
            op='N';
            }    
            estado--;
            Tecla=10;
            Teclal='N';
            DesplazaCursorI();
            EscribeLCD_c(' ');
            DesplazaCursorI();}
                    //borrar pantalla
        }
        else if((estado==1) &&((Teclal !='N') && (Teclal !='='))){
            op=Teclal;
            EscribeLCD_c(op);
            Teclal='N';
            estado++;
            if(op=='!')estado++;
        }
        else if((estado==2) && (Tecla !=10)){
            var2=Tecla;
            EscribeLCD_n8(var2, 1);
            Tecla=10;
            estado++;
        }
        else if((estado==3) && (Teclal=='=')){
            EscribeLCD_c('=');
            operar();
            Teclal='N';
            estado++;
        }
        
        __delay_ms(100);
        RBIF=0;
        
    }
    if(TMR0IF==1){
        contador++;
        TMR0IF=0;
        LATD2=!LATD2;
        TMR0=3036;
        if(contador>10){
            LATD2=0;
            SLEEP();
        }
    }
}