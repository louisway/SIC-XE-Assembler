. just test the comments of assembler
TEST	START	1000

. first	CLEAR	A	
first   LDA	=10
	ADD	=100
.	LDF	=50.20
	JSUB	load
.	TD	stdout
.	WD	stdout
.	J	increase	

load 	LDA	=20
	RSUB
. constants
. stdout	BYTE	X'01'
	END	first
