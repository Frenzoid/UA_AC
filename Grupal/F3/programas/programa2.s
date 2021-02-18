.data ; Inicio de segmento de datos.
 
  A: .word 8 ; Reserva en memoria una variable (64 bits) y le asigna el valor 8.
  B: .word 6 ; Reserva en memoria una variable (64 bits) y le asigna el valor 6.
  C: .word 3 ; Reserva en memoria una variable (64 bits) y le asigna el valor 3.
  D: .word 0, 0, 0 ; Reserva en memoria 4 variables (64 bits), y les asigna el valor 0 a cada una.
 
.code ; Inicio del segmento de codigo.
 
  ld r1, A(r0) ; Carga desde la direccion en memoria A + R0 = 8 + 0 (R0 = 0, no está seteado), y lo guarda en R1.
  ld r2, B(r0) ; Carga desde la direccion en memoria B + R0 = 6 + 0 (R0 = 0, no está seteado), y lo guarda en R2.
  ld r3, C(r0) ; Carga desde la direccion en memoria C + R0 = 8 + 0 (R0 = 0, no está seteado), y lo guarda en R3.

  xor r5, r5, r5 ; Hace xor de R5 con R5 y lo guarda en R5, R5 no estar seteado (por defecto valor 0), el xor devolvera 0.

  dadd r6, r2, r3 ; Suma R3 y R2 y lo guarda en R6.
  dadd r7, r6, r3 ; Suma R3 y R6 y lo guarda en R7.
  dadd r8, r7, r2 ; Suma R2 y R7 y lo guarda en R8.

  sd r6, D(r5) ; Guarda en la dir de memoria D + R5 el valor de R6.
  dadd r5, r5, r1 ; Suma R1 y R5 y lo guarda en  R5.
  sd r7, D(r5) ; Guarda en la dir de memoria D + R5 el valor de R7.
  dadd r5, r5, r1 ; Suma R1 y R5 y lo guarda en  R5.
  sd r8, D(r5) ; Guarda en la dir de memoria D + R5 el valor de R8.
  daddi r9, r5, 8 ; Suma 8 y R5 y lo guarda en R9.

  ld r10, D(r5) ; Carga el valor de la dir de memoria D + R5 y lo guarda en R10.
  sd r10, D(r9) ; Guarda el valor de la dir de memoria D + R9 y lo guarda en R10.
 
halt ; Detiene el programa.