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
aux1 equ 4h
aux2 equ 5h
aux3 equ 6h
Inicio ;Instrucciones
    bsf TRISB,0
    bsf TRISB,1
    ;CONFIGURACION TIMER 0 PARA SER USADO COMO CONTADOR
    ;movlw b'10000011';configuracion del TIMER0 prescaler 16
    ;movwf T0CON
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
    nop
    btfsc PORTB,0
    call Reinicio
    btfsc PORTB,1
    call startdelay ;error de 1.6e-4%
    call Retardo200
    btfsc PORTD,4
    goto decremental    
incremental
    movf trunk,w
    cpfslt cnt;Compare f with WREG, Skip <
    clrf cnt
    incf cnt
    goto Menu
decremental
    decfsz cnt ,f
    goto Menu
    movff trunk ,cnt
    goto Menu
Retardo200
	movlw .64
	movwf aux1
	movlw .145
	movwf aux2
	movlw .7
	movwf aux3 ; 10+((3*64+5)*145+5)*7)
AuxRetardo200
	decfsz aux1,f145
	goto AuxRetardo200
	movlw .64
	movwf aux1
	decfsz aux2,f
	goto AuxRetardo200
	movlw .145
	movwf aux2
	decfsz aux3,f
	goto AuxRetardo200
	return
Reinicio
  clrf cnt
  incf cnt
  movff cnt,LATD
  return
startdelay ;[2]
  movlw .25 ;[1]
  movwf cnt25 ;[1]
delay
  call Retardo200
  decfsz cnt25 ,f ;[3][1]
  goto delay ;[2]
  return ;[2] 2+1+1+2 =6+3*25
end


