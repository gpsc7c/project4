	LOAD 5
	PUSH
	STACKW 0
	LOAD -7
	PUSH
	STACKW 0
	LOAD 8
	STACKW 0
	LOAD 7
	STORE T0
	LOAD 5
	MULT T0
	STORE T1
	STACKR 1
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
	STORE T2
	LOAD T1
	ADD T2
	STACKW 0
	STACKR 0
	STORE T4
	WRITE T4
	POP
	POP
STOP
V0 0
V1 0
V2 0
V3 0
V4 0
