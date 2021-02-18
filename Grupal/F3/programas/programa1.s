.data

num: .word 7
num2: .word 8

.code

  ld r2, num(r0)
  dadd r3, r8, r9
  dsub r10, r5, r6
  dsll r1, r4, 1
  sd r4, num2(r0)

halt