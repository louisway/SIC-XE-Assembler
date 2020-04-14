. just test the comments of assembler
TEST	START	1000

. first	CLEAR	A	
first   LDA	=10
. increase  ADD	1
.	TD	stdout
.	WD	stdout
.	J	increase	

. constants
. stdout	BYTE	X'01'
	END	first
