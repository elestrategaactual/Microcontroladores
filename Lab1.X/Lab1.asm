include P18F4550.inc ;librerias
;directivas de configuracion
;declaracion de variables
aux1 equ 0h
aux2 equ 1h
aux3 equ 2h
aux4 equ 3h
  
Inicio ;Instrucciones
    movlw .7
    movwf aux1 
Menu
 ;Ejercicio 1
    movf aux1,w ;Llevar el valor de la variable var1 al sistema
    addlw .3 ;Sumar la constante 3 al valor de la variable aux1
    movwf aux1 ;Llevar el resultado de la suma del sistema hasta la variable aux
 ;Ejercicio 2
    movlw .8 ; Se le asigna a W el valor de una constante
    movwf aux1 ; Se carga la variable aux1 con el valor de W 
    movlw .10 ; Se le asigna a W el valor de una constante
    movwf aux2 ; Se carga la variable aux2 con el valor de W
    movf aux1 ,w ;Se le asigna  a W el valor almacenado en aux 1
    addwf aux2 ,f ; se reliza la operacion de suma entre W y la variable aux2 y se almacena el resultado en aux2
 ;Ejercicio 3
    movlw .5
    movwf aux1
    movf aux1 ,W
    sublw .9
    movwf aux1
 ;Ejercicio 4
    movlw .6
    movwf aux1
    movlw .4
    movwf aux2
    movf aux1 ,w
    subwf aux2 ,f
 ;Ejercicio 5
    movlw .12
    movwf aux1
    comf aux1
;Ejercicio 6
    movlw .12
    movwf aux1
    negf aux1
;Ejercicio 7
    movlw .35
    movwf aux1
    movf aux1 ,w
    iorlw .7
    movwf aux1
;Ejercicio 8
    movlw .20
    movwf aux1
    movlw .56
    movwf aux2
    movf aux1 ,w
    iorwf aux2 ,f
;ejercicio 9
    movlw .62
    movwf aux1
    movf aux1 ,w
    andlw .15
    movwf aux1
;Ejercicio 10
    movlw .100
    movwf aux1
    movlw .45
    movwf aux2
    movf aux1 ,w
    andwf aux2 ,f
;Ejercico 11
    movlw .120
    movwf aux1
    movf aux1 ,w
    xorlw .1
    movwf aux1
;Ejercicio 12
    movlw .17
    movwf aux1
    movlw .90
    movwf aux2
    movf aux1 ,w
    xorwf aux2 ,f
;Ejercicio 13
    movlw .25
    movwf aux1
    movlw .40
    movwf aux2
    movlw .103
    movwf aux3
    movf aux1 ,w
    iorwf aux2,f
    movf aux3 ,w
    xorlw 0xD0
    andwf aux2, w
    movwf aux4
    goto Menu
end