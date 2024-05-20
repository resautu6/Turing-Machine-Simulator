; The turing machine that accept strings whose format is ucu and u is only composed of a and b.

; the finite set of states
#Q = {q0,halt_accept,halt_reject,cp,cmp,reset1,reset0,accept1,accept2,accept3,clear_tape0,reject1,reject2,reject3,reject4}

; the finite set of input symbols
#S = {a,b,c}

; the finite set of tape symbols
#G = {a,b,c,_,t,r,u,e,f,l,s}

;the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2
;tape0: i/o and work tape
;tape1: store u_

; the transition function
q0 *_ *_ ** cp
q0 __ ff r* reject1

;cp: copy u to tape1
cp a_ _a rr cp
cp b_ _b rr cp
cp c_ c_ *l reset1
cp __ _f ** reset0

;reset1: reset tape1
reset1 ** ** *l reset1
reset1 c_ __ rr cmp

;cmp: compare u and u_
cmp __ tt r* accept1
cmp aa __ rr cmp
cmp bb __ rr cmp
cmp _a ff l* reset0
cmp _b ff l* reset0
cmp a_ ff l* reset0
cmp ab ff l* reset0
cmp b_ ff l* reset0
cmp ba ff l* reset0
cmp c_ ff l* reset0
cmp c* ff l* reset0

; accept
accept1 _* r* r* accept2
accept2 _* u* r* accept3
accept3 _* e* r* halt_accept

;reset0: reset tape0
reset0 ** ** l* reset0
reset0 _* _* r* clear_tape0

;clear_tape0: clear tape0
clear_tape0 ** _* r* clear_tape0
clear_tape0 _* f* r* reject1

;reject
reject1 _* a* r* reject2
reject2 _* l* r* reject3
reject3 _* s* r* reject4
reject4 _* e* r* halt_reject