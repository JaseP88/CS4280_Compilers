PUSH
a 0
PUSH
b 0
READ a
LOAD a
STACKW 1
STACKR 1
STORE t0
LOAD 0
STORE t1
LOAD t0
SUB t1
BRZNEG out0
LOAD 0
STORE t0
LOAD t0
STACKW 0
loop0: NOOP
STACKR 0
STORE t0
STACKR 1
STORE t1
LOAD t0
SUB t1
BRPOS out1
STACKR 0
STORE t0
WRITE t0
STACKR 0
STORE t0
LOAD 1
STORE t1
LOAD t0
ADD t1
STORE t0
LOAD t0
STACKW 0
BR loop0
out1: NOOP
out0: NOOP
STACKR 1
STORE t0
LOAD 0
STORE t1
LOAD t0
SUB t1
BRZERO cond0
BR out2
cond0: NOOP
LOAD 0
STORE t0
WRITE t0
out2: NOOP
POP
POP
STOP
t0 0
t1 0
t2 0
