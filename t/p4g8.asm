	LOAD 5
	PUSH
	STACKW 0
	LOAD -7
	PUSH
	STACKW 0
	LOAD 0
	PUSH
	STACKW 0
	LOAD 8
	STACKW 1
	LOAD 49
	STORE T1
	LOAD 21
	STORE T2
	LOAD T1
	ADD T2
	STORE T3
	LOAD 7
	STORE T4
	LOAD T3
	DIV T4
	STORE T5
	STACKR 2
	BRZERO L0
	SUB 1
	BRZERO L1
	ADD 1
	MULT -1
	BR L2
L0:	ADD 1
	BR L2
L1:	SUB 1
	BR L2
L2:	NOOP
	STORE T6
	LOAD T5
	ADD T6
	STACKW 1
	STACKR 2
	STORE T7
	WRITE T7
	LOAD 5
STORE T8
	STACKR 1
SUB T8
BRNEG L3
BRPOS L3
	LOAD 5
	BRZERO L4
	SUB 1
	BRZERO L5
	ADD 1
	MULT -1
	BR L6
L4:	ADD 1
	BR L6
L5:	SUB 1
	BR L6
L6:	NOOP
	STACKW 1
L3:	NOOP
	STACKR 1
	STORE T9
	WRITE T9
	POP
	POP
	POP
STOP
T0 0
T1 0
T2 0
T3 0
T4 0
T5 0
T6 0
T7 0
T8 0
T9 0