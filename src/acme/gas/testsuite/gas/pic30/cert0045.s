
	.title cert0045 .asciz in text section
	.sbttl subtitle1 test


	.data
var1:	.int	12345
var2:	.long	54321
var3:	.short	127



	.text
	.global main


ascid1:	.asciz	"This is an ascii string in the data section"
ascid2:	.asciz	"abcdefghijklmnopqrstuvwxyz"
ascid3:	.asciz	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        .align 2
ascid4:	.asciz	"0123456789"
ascid5:	.asciz	"\a\b\t\n\v\f\r*s "
ascid6:	.asciz	"!\"#$%&'()*+'-./"
ascid7:	.asciz	":l<=>?@"
ascid8:	.asciz	"[\]^_`"
ascid9:	.asciz	"{|}~"

	goto externdefine
	goto main2
	goto 1f
	goto 2f
	goto 3f
	goto 4f
	goto 5f

	goto 6f
	goto 7f
	goto 8f
	goto 9f
	goto 0f
	nopr
	.global foo
foo:	nopr
	.eject
	.ascii "Test of listing continuation lines.  This line should be really really really long so that gas is forced to use continuation lines.\n"

	.nolist
	.ascii "This ascii string should not appear in the list file\n"
	.list
	.if 0
	.ascii "This ascii string is within a false conditional"
	.endif
	.sbttl subtitle2 test
	.ascii "Resume listing in list file."

	.eject
	.sbttl subtitle3 test main routine


main:	MOV #5, w5
	MOV #1, w6
1:	addc w5, w6, w7
	MUL.US	w5, w6, w8
	MOV #1, w6
2:	addc w5, w6, w7
	MUL.US	w5, w6, w8
	MOV #1, w6
	addc w5, w6, w7
3:	MUL.US	w5, w6, w8
	MOV #1, w6
4:	addc w5, w6, w7
	MUL.US	w5, w6, w8
5:	MOV #1, w6
	addc w5, w6, w7
	MUL.US	w5, w6, w8
6:	MOV #1, w6
	addc w5, w6, w7
	MUL.US	w5, w6, w8
7:	MOV #1, w6
	addc w5, w6, w7
	MUL.US	w5, w6, w8
	MOV #1, w6
8:	addc w5, w6, w7
	MUL.US	w5, w6, w8
	MOV #1, w6
9:	addc w5, w6, w7
	MUL.US	w5, w6, w8

0:	goto	main



