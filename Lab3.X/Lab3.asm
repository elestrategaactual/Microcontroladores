include P18F4550.inc
;CONFIG FOSC = INTOSC_EC ; RELOJ INTERNO
;CONFIG FOSC = ECIO_EC ; RELOJ EXTERNO 
;CONFIG FOSC = XT_XT ; CRITSTAL SI F ES <4MHz
CONFIG FOSC = HS ; CRISTAL SI F ES >=4MHz
CONFIG WDT= OFF
CONFIG LVP= OFF
CONFIG PBADEN=OFF
;CONFIG MCLRE = OFF
cnt equ 0h
cnt25 equ 1h
trunk equ 2h
contexto equ 3h
aux1 equ 4h
aux2 equ 5h
aux3 equ 6h
ORG 0x0 ;vector de reset
    goto Inicio
ORG 0x8 ; vector de interrupcion prio alta
    goto ISR
ORG 0x18 ; vector de interrupcion prio baja
    goto ISR2
Inicio ;Instrucciones
    ;CONFIGURACION REGISTROS INTERUPCIONES
    bsf INTCON2,6 ;se asigna por flanco de subida para INT0
    bsf INTCON2,5 ; lo mismo que el de arriba pero para INT1
    bcf INTCON,1 ;se inicializa la bandera INT0
    bcf INTCON3,0 ;se inicializa la bandera de INT1
    bsf INTCON,4 ;se habilita localmente la interrupcion INT0
    bsf INTCON3,3;se habilita localmente la interrupcion INT1
    bsf INTCON,7 ;se habilita globalmente las interrupciones de prioridad alta
    bsf INTCON,6 ;se habilita globalmente las interrupciones de prioridad baja
    bsf RCON,7 ; se habilitan los niveles de prioridad
    bcf INTCON3, 6 ;Se asigna como baja prioridad a INT1
    bsf TRISB,0
    bsf TRISB,1
    ;CONFIGURACION TIMER 0 PARA SER USADO COMO CONTADOR
    movlw b'10000011';configuracion del TIMER0
    movwf T0CON
    ;CONFIGURACION DE SALIDAS Y CONSTANTES
    clrf LATD; se asigna el valor inicial de las salidas
    movlw b'11110000'; asignacion de pines D0 D1 D2 D3 como salidas
    movwf TRISD; asignacion de pines D0 D1 D2 D3 como salidas
    movlw .6 ;se almacena el valor del limite superior del contador
    movwf trunk
    movlw .1 ; se inicializa el contor con ek limite inferior
    movwf cnt
Menu
    movff cnt,LATD
    call inicioret200ms
    btfsc PORTD,4
    goto decremental    
incremental
    movf trunk,w
    cpfslt cnt
    clrf cnt
    incf cnt
    goto Menu
decremental
    decfsz cnt ,f
    goto Menu
    movff trunk ,cnt
    goto Menu
inicioret200ms
   movlw 0xCF
   movwf TMR0H
   movlw 0x2D ;2c
   movwf TMR0L
ret200ms
    btfss INTCON, TMR0IF
    goto ret200ms
    bcf INTCON, TMR0IF
    nop
    nop
    return
Retardo2
	movlw .64
	movwf aux1
	movlw .145
	movwf aux2
	movlw .7
	movwf aux3
AuxRetardo2
	decfsz aux1,f
	goto AuxRetardo
	movlw .64
	movwf aux1
	decfsz aux2,f
	goto AuxRetardo
	movlw .145
	movwf aux2
	decfsz aux3,f
	goto AuxRetardo
	return
ISR
  btfsc INTCON,1
  goto Reinicio ;me faltta salvar el contexto
Reinicio
  movwf contexto
  bcf INTCON,1
  clrf cnt
  incf cnt
  movff cnt,LATD
  movf contexto , w
  retfie
ISR2
  btfsc INTCON3,INT1IF
  goto startdelay ;me falta salvar el contexto
startdelay
  movwf contexto
  movlw .25
  movwf cnt25
delay
   ;movlw 0xcf
   ;movwf TMR0H
   ;movlw 0x2c
   ;movwf TMR0L
    ;espera
  ;btfss INTCON, TMR0IF
   ;goto espera
  ;bcf INTCON, TMR0IF
  call Retardo2
  decfsz cnt25 ,f
   goto delay
  bcf INTCON3,INT1IF
  movf contexto , w
  retfie
end


