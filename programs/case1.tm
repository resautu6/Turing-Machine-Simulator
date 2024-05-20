; The turing machine that accepts strings in the format (a^i)(b^j), where i and j is not zero and cal c^(i*j)

; the finite set of states
#Q = {q0,movea,reset1,setb,putc,reset0,checkbf,halt_accept,checka,checkb,valid,reset_tape,clear_tape,output1I,output1l,output2l,outpute,outputg,outputa,output3l,output_,output2I,outputn,outputp,outputu,outputt,halt_reject}

; the finite set of input symbols
#S = {a,b}

; the finite set of tape symbols
#G = {a,b,c,_,I,l,e,g,n,p,u,t}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2
; tape0: i/o and calculate ans(c)
; tape1: store a to count the number of a

; the transition function
q0 a_ a_ ** checka
q0 __ __ ** reset_tape
q0 b_ b_ ** reset_tape

; check if the input is valid
checka a_ a_ r* checka
checka b_ b_ r* checkb
checka __ __ l* reset_tape
checkb b_ b_ r* checkb
checkb __ __ l* valid
checkb a_ a_ l* reset_tape
valid *_ *_ l* valid
valid __ __ r* movea

; write error information to tape0
reset_tape *_ *_ l* reset_tape
reset_tape __ __ r* clear_tape
clear_tape *_ __ r* clear_tape
clear_tape __ __ ** output1I
output1I __ I_ r* output1l
output1l __ l_ r* output2l
output2l __ l_ r* outpute
outpute __ e_ r* outputg
outputg __ g_ r* outputa
outputa __ a_ r* output3l
output3l __ l_ r* output_
output_ __ __ r* output2I
output2I __ I_ r* outputn
outputn __ n_ r* outputp
outputp __ p_ r* outputu
outputu __ u_ r* outputt
outputt __ t_ r* halt_reject

; copy a to tape1 until meet the first b
movea a_ _a rr movea
movea b_ b_ *l reset1

; reset head of tape1 to the left of tape1
reset1 *a *a *l reset1
reset1 b_ __ rr setb

; set head of tape0 to the right of tape0
setb *a *a r* setb
setb _a ca rr putc

; put c to right position
putc _a ca rr putc
putc __ __ ll reset0

; reset head of tape0 to the left of tape0
reset0 ** ** l* reset0
reset0 _a _a r* checkbf

;check if b is finished
checkbf ba ba ** reset1
checkbf ca ca ** halt_accept