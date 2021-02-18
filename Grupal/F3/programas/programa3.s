.data
 
  A: .word32 2
  B: .word32 3
  C: .word32 0
  D: .word32 4
  E: .word32 5
  F: .word32 0
 
.code
 
  lw r1, A(r0)
  lw r2, B(r0)
  dadd r3, r1, r2
  sw r3, C(r0)

  lw r4, D(r0)
  lw r5, E(r0)
  dadd r6, r4, r5
  sw r6, F(r0)
 
halt
