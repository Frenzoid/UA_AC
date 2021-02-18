.data
 
  cant: .word 8
  datos: .word 1, 2, 3, 4, 5, 6, 7, 8
  res: .word 0
 
.code
 
  ld r2, cant(r0)
  dadd r1, r0, r0
  loop: 
    beqz r2, fin
    ld r3, datos(r1)
    daddi r2, r2, -1
    dsll r3, r3, 1
    sd r3, res(r1)
  j loop
  daddi r1, r1, 8

  fin:
 
halt
