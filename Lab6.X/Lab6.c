/*
 * File:   Lab6.c
 * Author: ivanm
 *
 * Created on 17 de junio de 2022, 07:53 AM
 */
#include<xc.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF
#pragma config LVP=OFF
unsigned int tiempo=4;
unsigned int PWM=0;
float var1,calc;
unsigned char lectura;
unsigned int estado,temporal;
void aceleracion(int , int );
void desaceleracion(int , int );
void cambios(int);
void Transmitir(unsigned char);
void __interrupt() ISR(void);
void main(void){
    unsigned char i;
    TRISC1=0;
    TRISC2=0;
    TRISD=0;
    LATD=0;
    LATE=0;
    TRISE=0;
    ADCON1=0b00001111;
    TMR2=0;
    PR2=249;    //Fpwm=1KHz
    T2CON=0b00000000;   
    CCPR1L=0;
    CCP1CON=0b00001100;
//Inicializacion de la direccion
    LATE0=1;
    LATE1=0;
    TMR2ON=1;
    //Configuracion comunicaciones BT
    TXSTA=0b00100100;
    RCSTA=0b10010000;
    BAUDCON=0b00001000;
    //SPBRG=12;
    SPBRG=25;
    //configuracion interrupciones
    INTEDG0=1;
    INT0IF=0;
    INT0IE=1;
    //recepcion de datos
    RCIP=1;
    RCIF=0;
    RCIE=1;
    //Timer 1s 
    T0CON=0b10000001;
    TMR0=3036;
    TMR0IF=0;
    TMR0IE=1;
    GIE=1;
//Inicio LCD
    __delay_ms(1000);
    ConfiguraLCD(4);
    InicializaLCD();
    Transmitir('o');
    Transmitir('k');
    while(1){
        if(estado==1){
            temporal=PWM;
            desaceleracion(0,tiempo);
            aceleracion(temporal,tiempo);
            estado=0;
            BorraLCD();
            EscribeLCD_n8(CCPR1L,3);
        }
        if(estado==2){
            if(LATE0==1)cambios(0);
            estado=0;
            BorraLCD();
            EscribeLCD_n8(CCPR1L,3);
        }
        if(estado==3){
            if(LATE1==1)cambios(1);
            estado=0;
            BorraLCD();
            EscribeLCD_n8(CCPR1L,3);
        }
        if(estado==4){
            if(PWM>CCPR1L){
            aceleracion(PWM,tiempo);
            }
            else if(PWM<CCPR1L){
            desaceleracion(PWM,tiempo);
            }
            Transmitir('o');
            Transmitir('k');
            estado=0;
            BorraLCD();
            EscribeLCD_n8(CCPR1L,3);
        } 
    }
    
}
void cambios(int s){
    Transmitir('C');
    temporal=PWM;
    desaceleracion(0,tiempo);
    LATE0=0;
    LATE1=0;
    //sentido horario
    if(s==1){
    LATE0=1;
    }
    //sentido antihorario
    else{
    LATE1=1;
    }
    aceleracion(temporal,tiempo);

}
void aceleracion(int objetivo, int t){
    Transmitir('a');
    if(CCP1CON==0b00000000){
       CCP1CON=0b00001100;
    }
    if(objetivo>249)objetivo=249;
    calc=(objetivo-CCPR1L)*50.0/(t*1000.0);
    var1=CCPR1L;
    while(objetivo>var1){
        var1=var1+calc;
        CCPR1L=var1;
        __delay_ms(50);
    }
    CCPR1L=objetivo;
    if(objetivo>=249){
       CCP1CON=0b00000000;
       LATC2=1;
    }
    PWM=CCPR1L;
}
void desaceleracion(int objetivo, int t){
    Transmitir('d');
    if(CCP1CON==0b00000000){
      CCP1CON=0b00001100;
    }
    if(objetivo<11)objetivo=10;
    calc=(CCPR1L-objetivo)*50.0/(t*1000.0);
    var1=CCPR1L;
    while(objetivo<var1){
        var1=var1-calc;
        CCPR1L=var1;
        __delay_ms(50);
    }
    CCPR1L=objetivo;
    if(objetivo<11){
       CCP1CON=0b00000000;
       LATC2=0;
    }
     PWM=CCPR1L;
}
void Transmitir(unsigned char dato){
    //se configura momentaneamente para envio
    while(TRMT==0);
    TXREG=dato;
}
void __interrupt() ISR(void){
    if(RCIF==1){
      lectura=RCREG;
      if(LATD3==0){
      if(lectura=='A' || lectura=='a')PWM=0;
      if(lectura=='B' || lectura=='b')PWM=25;
      if(lectura=='C' || lectura=='c')PWM=50;
      if(lectura=='D' || lectura=='d')PWM=75;
      if(lectura=='E' || lectura=='e')PWM=100;
      if(lectura=='F' || lectura=='f')PWM=125;
      if(lectura=='G' || lectura=='g')PWM=150;
      if(lectura=='H' || lectura=='h')PWM=175;
      if(lectura=='I' || lectura=='i')PWM=200;
      if(lectura=='J' || lectura=='j')PWM=225;
      if(lectura=='K' || lectura=='k')PWM=250;
      if(lectura=='L' || lectura=='l')estado=1;
      if(lectura=='M' || lectura=='m')PWM=PWM/2;
      if(lectura=='N' || lectura=='n')PWM=PWM*2;
      if(lectura=='W' || lectura=='w')estado=2;
      if(lectura=='X' || lectura=='x')estado=3;
      if(lectura=='Y' || lectura=='y')tiempo=4;
      if(lectura=='Z' || lectura=='z')tiempo=10;
      if(estado==0) estado=4;
      //BorraLCD();
     // EscribeLCD_c(lectura);
      //EscribeLCD_n8(PWM,3);
      Transmitir(lectura);
      Transmitir(estado+48);
      }
      RCIF=0;
    }
    if(INT0IF==1){
       //RCIE=!RCIE;
       LATD3=!LATD3;
       Transmitir('P');
       Transmitir(RCIE+48);
       desaceleracion(0,tiempo);
       estado=0;
       INT0IF=0;
    }
if(TMR0IF==1){
        LATD2=!LATD2;//led de conmutacion de 1s
        TMR0=3036;
        TMR0IF=0;
    }    
}

