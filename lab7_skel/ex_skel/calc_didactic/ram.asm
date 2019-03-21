inc     RA                              ; 0001 000 0 11 000 000     0308
; RA should be 1
; IND should be _____ (0000)

inc     RB                              ; 0001 000 0 11 000 001     8308
; RB should be 1
; IND should be _____ (0000)

inc     RC                              ; 0001 000 0 11 000 010     6308
; RC should be 1
; IND should be _____ (0000)

inc     IS                              ; 0001 000 0 11 000 011     c308
; RC should be 1
; IND should be _____ (0000)

inc     XA                              ; 0001 000 0 11 000 100     2308
; XA should be 1
; IND should be _____ (0000)

inc     XB                              ; 0001 000 0 11 000 101     a308
; XB should be 1
; IND should be _____ (0000)

inc     BA                              ; 0001 000 0 11 000 110     6308
; BA should be 1
; IND should be _____ (0000)

inc     BB                              ; 0001 000 0 11 000 111     e308
; BB should be 1
; IND should be _____ (0000)

dec     RB                              ; 0001 001 0 11 000 001     8348
dec     RB                              ; 0001 001 0 11 000 001     8348
; RB should be -1 (ffff)
; IND should be PS__C (0019)

dec     RC                              ; 0001 001 0 11 000 010     4348
dec     RC                              ; 0001 001 0 11 000 010     4348
neg     RC                              ; 0001 010 0 11 000 010     4328
; RC should be 1
; IND should be ____C (0001)

dec     IS                              ; 0001 001 0 11 000 011     c348
not     IS                              ; 0001 011 0 11 000 011     c368
; IS should be ffff
; IND should be PS___ (0018)

dec     XA                              ; 0001 001 0 11 000 100     2348
inc     XA                              ; 0001 000 0 11 000 100     2308
shl     XA                              ; 0001 100 0 11 000 100     2318
; XA should be 2
; IND should be _____ (0000)

dec     XB                              ; 0001 001 0 11 000 101     a348
dec     XB                              ; 0001 001 0 11 000 101     a348
shr     XB                              ; 0001 101 0 11 000 101     a358
; XB should be 32767 (7fff)
; IND should be ___OC (0003)

dec     BA                              ; 0001 001 0 11 000 110     6348
dec     BA                              ; 0001 001 0 11 000 110     6348
sar     BA                              ; 0001 110 0 11 000 110     6338
; BA should be -1 (ffff)
; IND should be PS__C (0019)
