		.global __reset
	
		.global symAT1
		.global symAT2
		.global symAT3
		.global symAT4
		.global symAT5
	
		.text
__reset:	nop

		nopr
		nopr
			
		
		nopr
		nopr
		nopr
		nopr
		nopr
		nopr
		

		.equiv symAT1, 0x0000
		.equiv symAT2, 0x0001
		.equiv symAT3, 0x7FFF
		.equiv symAT4, 0x8000
		.equiv symAT5, 0xFFFF
				

tAD1:		.word symAD1
tAD2:		.word symAD2
tAD3:		.word symAD3
tAD4:		.word symAD4
tAD5:		.word symAD5

tBD1:		.word symBD1
tBD2:		.word symBD2
tBD3:		.word symBD3
tBD4:		.word symBD4
tBD5:		.word symBD5

tAT1:		.word symAT1
tAT2:		.word symAT2
tAT3:		.word symAT3
tAT4:		.word symAT4
tAT5:		.word symAT5
 
tBT1:		.word symBT1
tBT2:		.word symBT2
tBT3:		.word symBT3
tBT4:		.word symBT4
tBT5:		.word symBT5

	
		.data

		.equiv symAD1, 0x0000
		.equiv symAD2, 0x0001
		.equiv symAD3, 0x7FFF
		.equiv symAD4, 0x8000
		.equiv symAD5, 0xFFFF

dAD1:		.word symAD1
dAD2:		.word symAD2
dAD3:		.word symAD3
dAD4:		.word symAD4
dAD5:		.word symAD5

dBD1:		.word symBD1
dBD2:		.word symBD2
dBD3:		.word symBD3
dBD4:		.word symBD4
dBD5:		.word symBD5

dAT1:		.word symAT1
dAT2:		.word symAT2
dAT3:		.word symAT3
dAT4:		.word symAT4
dAT5:		.word symAT5

dBT1:		.word symBT1
dBT2:		.word symBT2
dBT3:		.word symBT3
dBT4:		.word symBT4
dBT5:		.word symBT5

		.global symAD1
		.global symAD2
		.global symAD3
		.global symAD4
		.global symAD5
		
		.end

