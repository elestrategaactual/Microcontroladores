include P18F4550.inc
;CONFIG FOSC = INTOSC_EC ; RELOJ INTERNO
CONFIG FOSC = ECIO_EC ; RELOJ EXTERNO 
;CONFIG FOSC = XT_XT ; CRITSTAL SI F ES <4MHz
;CONFIG FOSC = HS ; CRISTAL SI F ES >=4MHz
CONFIG WDT= OFF
CONFIG LVP= OFF
;CONFIG MCLRE = OFF
Inicio ;Instrucciones
    setf LATD; se asigna el valor inicial de las salidas
    movlw b'11111000'; asignacion de pines D0 D1 D2 como salidas
    movwf TRISD; asignacion de pines D0 D1 D2 como salidas
Menu
    ; D0-RED D1-GREEN D2-BLUE
    movlw b'00000111';NGR
    movwf LATD
    movlw b'00000010';MAG
    movwf LATD
    movlw b'00000011';AZUL
    movwf LATD
    movlw b'00000001';CYAN
    movwf LATD
    movlw b'00000101';VERDE
    movwf LATD
    movlw b'00000100';AMARILLO
    movwf LATD
    movlw b'00000110';ROJO
    movwf LATD
    movlw b'00000000';BLANCO
    movwf LATD
    goto Menu
end
