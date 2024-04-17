	.arch armv8-a
	.file	"Out_of_order.c"
// GNU C17 (Debian 12.2.0-14) version 12.2.0 (aarch64-linux-gnu)
//	compiled by GNU C version 12.2.0, GMP version 6.2.1, MPFR version 4.1.1-p1, MPC version 1.3.1, isl version isl-0.25-GMP

// warning: MPFR header version 4.1.1-p1 differs from library version 4.2.0.
// GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
// options passed: -mlittle-endian -mabi=lp64 -O0 -fasynchronous-unwind-tables
	.text
	.global	location
	.bss
	.align	2
	.type	location, %object
	.size	location, 4
location:
	.zero	4
	.global	TOP_N
	.data
	.align	2
	.type	TOP_N, %object
	.size	TOP_N, 4
TOP_N:
	.word	10
	.global	CACHE_HIT_THRESHOLD
	.align	2
	.type	CACHE_HIT_THRESHOLD, %object
	.size	CACHE_HIT_THRESHOLD, 4
CACHE_HIT_THRESHOLD:
	.word	94
	.global	array1_size
	.align	2
	.type	array1_size, %object
	.size	array1_size, 4
array1_size:
	.word	16
	.global	array1
	.bss
	.align	3
	.type	array1, %object
	.size	array1, 160
array1:
	.zero	160
	.global	array2
	.align	3
	.type	array2, %object
	.size	array2, 65600
array2:
	.zero	65600
	.global	array3
	.align	3
	.type	array3, %object
	.size	array3, 160
array3:
	.zero	160
	.global	secret
	.section	.rodata
	.align	3
.LC0:
	.string	"LBWNB"
	.section	.data.rel.local,"aw"
	.align	3
	.type	secret, %object
	.size	secret, 8
secret:
	.xword	.LC0
	.global	temp
	.bss
	.type	temp, %object
	.size	temp, 1
temp:
	.zero	1
	.text
	.align	2
	.type	read_pmccntr, %function
read_pmccntr:
.LFB0:
	.cfi_startproc
	sub	sp, sp, #16	//,,
	.cfi_def_cfa_offset 16
// Out_of_order.c:25: 	asm volatile("mrs %0, pmccntr_el0" : "=r"(val));
#APP
// 25 "Out_of_order.c" 1
	mrs x0, pmccntr_el0	// val
// 0 "" 2
#NO_APP
	str	x0, [sp, 8]	// val, val
// Out_of_order.c:26: 	return val;
	ldr	x0, [sp, 8]	// _2, val
// Out_of_order.c:27: }
	add	sp, sp, 16	//,,
	.cfi_def_cfa_offset 0
	ret	
	.cfi_endproc
.LFE0:
	.size	read_pmccntr, .-read_pmccntr
	.section	.rodata
	.align	3
.LC1:
	.string	"hit time is %ld \n"
	.align	3
.LC2:
	.string	"miss time is %ld \n"
	.align	3
.LC3:
	.string	"Time 6 - Time 5 is %ld \n"
	.align	3
.LC4:
	.string	"Time 8 - Time 7 is %ld \n"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB1:
	.cfi_startproc
	stp	x29, x30, [sp, -80]!	//,,,
	.cfi_def_cfa_offset 80
	.cfi_offset 29, -80
	.cfi_offset 30, -72
	mov	x29, sp	//,
	stp	x19, x20, [sp, 16]	//,,
	stp	x21, x22, [sp, 32]	//,,
	.cfi_offset 19, -64
	.cfi_offset 20, -56
	.cfi_offset 21, -48
	.cfi_offset 22, -40
// Out_of_order.c:35:     for(i = 0;i < array1_size * 10; i++){
	str	wzr, [sp, 76]	//, i
// Out_of_order.c:35:     for(i = 0;i < array1_size * 10; i++){
	b	.L4		//
.L5:
// Out_of_order.c:36:         array1[i] = i;
	ldr	w0, [sp, 76]	// tmp131, i
	and	w2, w0, 255	// _1, tmp131
	adrp	x0, array1	// tmp133,
	add	x1, x0, :lo12:array1	// tmp132, tmp133,
	ldrsw	x0, [sp, 76]	// tmp134, i
	strb	w2, [x1, x0]	// tmp135, array1[i_29]
// Out_of_order.c:37:         array3[i] = i;
	ldr	w0, [sp, 76]	// tmp136, i
	and	w2, w0, 255	// _2, tmp136
	adrp	x0, array3	// tmp138,
	add	x1, x0, :lo12:array3	// tmp137, tmp138,
	ldrsw	x0, [sp, 76]	// tmp139, i
	strb	w2, [x1, x0]	// tmp140, array3[i_29]
// Out_of_order.c:38:         asm volatile ("dsb ish"); // Data synchronization barrier ensure write has completed
#APP
// 38 "Out_of_order.c" 1
	dsb ish
// 0 "" 2
// Out_of_order.c:39:         asm volatile ("ISB");
// 39 "Out_of_order.c" 1
	ISB
// 0 "" 2
// Out_of_order.c:35:     for(i = 0;i < array1_size * 10; i++){
#NO_APP
	ldr	w0, [sp, 76]	// tmp142, i
	add	w0, w0, 1	// tmp141, tmp142,
	str	w0, [sp, 76]	// tmp141, i
.L4:
// Out_of_order.c:35:     for(i = 0;i < array1_size * 10; i++){
	adrp	x0, array1_size	// tmp144,
	add	x0, x0, :lo12:array1_size	// tmp143, tmp144,
	ldr	w1, [x0]	// array1_size.0_3, array1_size
	mov	w0, w1	// tmp145, array1_size.0_3
	lsl	w0, w0, 2	// tmp146, tmp145,
	add	w0, w0, w1	// tmp145, tmp145, array1_size.0_3
	lsl	w0, w0, 1	// tmp147, tmp145,
	mov	w1, w0	// _4, tmp145
// Out_of_order.c:35:     for(i = 0;i < array1_size * 10; i++){
	ldr	w0, [sp, 76]	// i.1_5, i
	cmp	w1, w0	// _4, i.1_5
	bhi	.L5		//,
// Out_of_order.c:43:     asm volatile("isb"); // Serialize before reading the counter
#APP
// 43 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:44:     time1 = read_pmccntr();
#NO_APP
	bl	read_pmccntr		//
	mov	x19, x0	// time1,
// Out_of_order.c:45:     asm volatile("isb"); // Serialize after reading the counter
#APP
// 45 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:46:     temp = array3[160];
#NO_APP
	adrp	x0, array3	// tmp149,
	add	x0, x0, :lo12:array3	// tmp148, tmp149,
	ldrb	w0, [x0, 160]	// _6, array3[160]
// Out_of_order.c:46:     temp = array3[160];
	str	w0, [sp, 72]	// _6, temp
// Out_of_order.c:47:     asm volatile("isb"); // Serialize before reading the counter
#APP
// 47 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:48:     time2 = read_pmccntr();
#NO_APP
	bl	read_pmccntr		//
	mov	x20, x0	// time2,
// Out_of_order.c:49:     asm volatile("isb"); // Serialize after reading the counter
#APP
// 49 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:50:     time_diff = time2 - time1;
#NO_APP
	sub	x19, x20, x19	// time_diff, time2, time1
// Out_of_order.c:51:     printf("hit time is %ld \n",time_diff);
	mov	x1, x19	//, time_diff
	adrp	x0, .LC1	// tmp150,
	add	x0, x0, :lo12:.LC1	//, tmp150,
	bl	printf		//
// Out_of_order.c:54:     for(i = 0;i < array1_size * 10; i++){
	str	wzr, [sp, 76]	//, i
// Out_of_order.c:54:     for(i = 0;i < array1_size * 10; i++){
	b	.L6		//
.L7:
// Out_of_order.c:55:         asm volatile("dc civac, %0" : : "r"(&array1[i]) : "memory");
	ldrsw	x1, [sp, 76]	// tmp151, i
	adrp	x0, array1	// tmp153,
	add	x0, x0, :lo12:array1	// tmp152, tmp153,
	add	x0, x1, x0	// _7, tmp151, tmp152
// Out_of_order.c:55:         asm volatile("dc civac, %0" : : "r"(&array1[i]) : "memory");
#APP
// 55 "Out_of_order.c" 1
	dc civac, x0	// _7
// 0 "" 2
// Out_of_order.c:56:         asm volatile("dc civac, %0" : : "r"(&array3[i]) : "memory");
#NO_APP
	ldrsw	x1, [sp, 76]	// tmp154, i
	adrp	x0, array3	// tmp156,
	add	x0, x0, :lo12:array3	// tmp155, tmp156,
	add	x0, x1, x0	// _8, tmp154, tmp155
// Out_of_order.c:56:         asm volatile("dc civac, %0" : : "r"(&array3[i]) : "memory");
#APP
// 56 "Out_of_order.c" 1
	dc civac, x0	// _8
// 0 "" 2
// Out_of_order.c:57:         asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
// 57 "Out_of_order.c" 1
	dsb ish
// 0 "" 2
// Out_of_order.c:58:         asm volatile("isb"); // Insert isb for serialization after cache flush
// 58 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:54:     for(i = 0;i < array1_size * 10; i++){
#NO_APP
	ldr	w0, [sp, 76]	// tmp158, i
	add	w0, w0, 1	// tmp157, tmp158,
	str	w0, [sp, 76]	// tmp157, i
.L6:
// Out_of_order.c:54:     for(i = 0;i < array1_size * 10; i++){
	adrp	x0, array1_size	// tmp160,
	add	x0, x0, :lo12:array1_size	// tmp159, tmp160,
	ldr	w1, [x0]	// array1_size.2_9, array1_size
	mov	w0, w1	// tmp161, array1_size.2_9
	lsl	w0, w0, 2	// tmp162, tmp161,
	add	w0, w0, w1	// tmp161, tmp161, array1_size.2_9
	lsl	w0, w0, 1	// tmp163, tmp161,
	mov	w1, w0	// _10, tmp161
// Out_of_order.c:54:     for(i = 0;i < array1_size * 10; i++){
	ldr	w0, [sp, 76]	// i.3_11, i
	cmp	w1, w0	// _10, i.3_11
	bhi	.L7		//,
// Out_of_order.c:61:     for(i = 0;i < 256 * stride + DELTA; i++){
	str	wzr, [sp, 76]	//, i
// Out_of_order.c:61:     for(i = 0;i < 256 * stride + DELTA; i++){
	b	.L8		//
.L9:
// Out_of_order.c:62:         array2[i] = i;
	ldr	w0, [sp, 76]	// tmp164, i
	and	w2, w0, 255	// _12, tmp164
	adrp	x0, array2	// tmp166,
	add	x1, x0, :lo12:array2	// tmp165, tmp166,
	ldrsw	x0, [sp, 76]	// tmp167, i
	strb	w2, [x1, x0]	// tmp168, array2[i_31]
// Out_of_order.c:63:         asm volatile ("dsb ish"); // Data synchronization barrier ensure write has completed
#APP
// 63 "Out_of_order.c" 1
	dsb ish
// 0 "" 2
// Out_of_order.c:64:         asm volatile ("ISB");
// 64 "Out_of_order.c" 1
	ISB
// 0 "" 2
// Out_of_order.c:61:     for(i = 0;i < 256 * stride + DELTA; i++){
#NO_APP
	ldr	w0, [sp, 76]	// tmp170, i
	add	w0, w0, 1	// tmp169, tmp170,
	str	w0, [sp, 76]	// tmp169, i
.L8:
// Out_of_order.c:61:     for(i = 0;i < 256 * stride + DELTA; i++){
	ldr	w1, [sp, 76]	// tmp171, i
	mov	w0, 63	// tmp172,
	movk	w0, 0x1, lsl 16	// tmp172,,
	cmp	w1, w0	// tmp171, tmp172
	ble	.L9		//,
// Out_of_order.c:68:     for(i = 0;i < 256 * stride + DELTA; i++){
	str	wzr, [sp, 76]	//, i
// Out_of_order.c:68:     for(i = 0;i < 256 * stride + DELTA; i++){
	b	.L10		//
.L11:
// Out_of_order.c:69:         asm volatile("dc civac, %0" : : "r"(&array2[i]) : "memory");
	ldrsw	x1, [sp, 76]	// tmp173, i
	adrp	x0, array2	// tmp175,
	add	x0, x0, :lo12:array2	// tmp174, tmp175,
	add	x0, x1, x0	// _13, tmp173, tmp174
// Out_of_order.c:69:         asm volatile("dc civac, %0" : : "r"(&array2[i]) : "memory");
#APP
// 69 "Out_of_order.c" 1
	dc civac, x0	// _13
// 0 "" 2
// Out_of_order.c:70:         asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
// 70 "Out_of_order.c" 1
	dsb ish
// 0 "" 2
// Out_of_order.c:71:         asm volatile("isb"); // Insert isb for serialization after cache flush
// 71 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:68:     for(i = 0;i < 256 * stride + DELTA; i++){
#NO_APP
	ldr	w0, [sp, 76]	// tmp177, i
	add	w0, w0, 1	// tmp176, tmp177,
	str	w0, [sp, 76]	// tmp176, i
.L10:
// Out_of_order.c:68:     for(i = 0;i < 256 * stride + DELTA; i++){
	ldr	w1, [sp, 76]	// tmp178, i
	mov	w0, 63	// tmp179,
	movk	w0, 0x1, lsl 16	// tmp179,,
	cmp	w1, w0	// tmp178, tmp179
	ble	.L11		//,
// Out_of_order.c:74:     asm volatile("isb"); // Serialize before reading the counter
#APP
// 74 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:75:     time3 = read_pmccntr();
#NO_APP
	bl	read_pmccntr		//
	mov	x19, x0	// time3,
// Out_of_order.c:76:     asm volatile("isb"); // Serialize after reading the counter
#APP
// 76 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:77:     temp = array2[1];
#NO_APP
	adrp	x0, array2	// tmp181,
	add	x0, x0, :lo12:array2	// tmp180, tmp181,
	ldrb	w0, [x0, 1]	// _14, array2[1]
// Out_of_order.c:77:     temp = array2[1];
	str	w0, [sp, 72]	// _14, temp
// Out_of_order.c:78:     asm volatile("isb"); // Serialize before reading the counter
#APP
// 78 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:79:     time4 = read_pmccntr();
#NO_APP
	bl	read_pmccntr		//
	mov	x20, x0	// time4,
// Out_of_order.c:80:     asm volatile("isb"); // Serialize after reading the counter
#APP
// 80 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:81:     time_diff = time4 - time3;
#NO_APP
	sub	x19, x20, x19	// time_diff, time4, time3
// Out_of_order.c:82:     printf("miss time is %ld \n",time_diff);
	mov	x1, x19	//, time_diff
	adrp	x0, .LC2	// tmp182,
	add	x0, x0, :lo12:.LC2	//, tmp182,
	bl	printf		//
// Out_of_order.c:85:     for(i = 0;i < 256 * stride + DELTA; i++){
	str	wzr, [sp, 76]	//, i
// Out_of_order.c:85:     for(i = 0;i < 256 * stride + DELTA; i++){
	b	.L12		//
.L13:
// Out_of_order.c:86:         asm volatile("dc civac, %0" : : "r"(&array2[i]) : "memory");
	ldrsw	x1, [sp, 76]	// tmp183, i
	adrp	x0, array2	// tmp185,
	add	x0, x0, :lo12:array2	// tmp184, tmp185,
	add	x0, x1, x0	// _15, tmp183, tmp184
// Out_of_order.c:86:         asm volatile("dc civac, %0" : : "r"(&array2[i]) : "memory");
#APP
// 86 "Out_of_order.c" 1
	dc civac, x0	// _15
// 0 "" 2
// Out_of_order.c:87:         asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
// 87 "Out_of_order.c" 1
	dsb ish
// 0 "" 2
// Out_of_order.c:88:         asm volatile("isb"); // Insert isb for serialization after cache flush
// 88 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:85:     for(i = 0;i < 256 * stride + DELTA; i++){
#NO_APP
	ldr	w0, [sp, 76]	// tmp187, i
	add	w0, w0, 1	// tmp186, tmp187,
	str	w0, [sp, 76]	// tmp186, i
.L12:
// Out_of_order.c:85:     for(i = 0;i < 256 * stride + DELTA; i++){
	ldr	w1, [sp, 76]	// tmp188, i
	mov	w0, 63	// tmp189,
	movk	w0, 0x1, lsl 16	// tmp189,,
	cmp	w1, w0	// tmp188, tmp189
	ble	.L13		//,
// Out_of_order.c:93:     int a = 1;
	mov	w0, 1	// tmp190,
	str	w0, [sp, 68]	// tmp190, a
// Out_of_order.c:94:     int b = 2;
	mov	w0, 2	// tmp191,
	str	w0, [sp, 64]	// tmp191, b
// Out_of_order.c:95:     int c = 3;
	mov	w0, 3	// tmp192,
	str	w0, [sp, 60]	// tmp192, c
// Out_of_order.c:96:     asm volatile("dc civac, %0" : : "r"(&a) : "memory");
	add	x0, sp, 68	// tmp193,,
#APP
// 96 "Out_of_order.c" 1
	dc civac, x0	// tmp193
// 0 "" 2
// Out_of_order.c:97:     asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
// 97 "Out_of_order.c" 1
	dsb ish
// 0 "" 2
// Out_of_order.c:98:     asm volatile("isb"); // Insert isb for serialization after cache flush
// 98 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:100:     asm volatile("dc civac, %0" : : "r"(&b) : "memory");
#NO_APP
	add	x0, sp, 64	// tmp194,,
#APP
// 100 "Out_of_order.c" 1
	dc civac, x0	// tmp194
// 0 "" 2
// Out_of_order.c:101:     asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
// 101 "Out_of_order.c" 1
	dsb ish
// 0 "" 2
// Out_of_order.c:102:     asm volatile("isb"); // Insert isb for serialization after cache flush
// 102 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:104:     asm volatile("dc civac, %0" : : "r"(&c) : "memory");
#NO_APP
	add	x0, sp, 60	// tmp195,,
#APP
// 104 "Out_of_order.c" 1
	dc civac, x0	// tmp195
// 0 "" 2
// Out_of_order.c:105:     asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
// 105 "Out_of_order.c" 1
	dsb ish
// 0 "" 2
// Out_of_order.c:106:     asm volatile("isb"); // Insert isb for serialization after cache flush
// 106 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:108:     if(array3[a+b+c] >= 1){
#NO_APP
	ldr	w1, [sp, 68]	// a.4_16, a
	ldr	w0, [sp, 64]	// b.5_17, b
	add	w1, w1, w0	// _18, a.4_16, b.5_17
// Out_of_order.c:108:     if(array3[a+b+c] >= 1){
	ldr	w0, [sp, 60]	// c.6_19, c
	add	w2, w1, w0	// _20, _18, c.6_19
// Out_of_order.c:108:     if(array3[a+b+c] >= 1){
	adrp	x0, array3	// tmp197,
	add	x1, x0, :lo12:array3	// tmp196, tmp197,
	sxtw	x0, w2	// tmp198, _20
	ldrb	w0, [x1, x0]	// _21, array3[_20]
// Out_of_order.c:108:     if(array3[a+b+c] >= 1){
	cmp	w0, 0	// _21,
	beq	.L14		//,
// Out_of_order.c:109:         asm volatile("isb"); // Serialize before reading the counter
#APP
// 109 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:110:         time5 = read_pmccntr();
#NO_APP
	bl	read_pmccntr		//
	mov	x21, x0	// time5,
// Out_of_order.c:111:         asm volatile("isb"); // Serialize after reading the counter
#APP
// 111 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:112:         temp = array1[array1_size];
#NO_APP
	adrp	x0, array1_size	// tmp200,
	add	x0, x0, :lo12:array1_size	// tmp199, tmp200,
	ldr	w2, [x0]	// array1_size.7_22, array1_size
	adrp	x0, array1	// tmp202,
	add	x1, x0, :lo12:array1	// tmp201, tmp202,
	uxtw	x0, w2	// tmp203, array1_size.7_22
	ldrb	w0, [x1, x0]	// _23, array1[array1_size.7_22]
// Out_of_order.c:112:         temp = array1[array1_size];
	str	w0, [sp, 72]	// _23, temp
// Out_of_order.c:113:         asm volatile("isb"); // Serialize before reading the counter
#APP
// 113 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:114:         time6 = read_pmccntr();
#NO_APP
	bl	read_pmccntr		//
	mov	x22, x0	// time6,
// Out_of_order.c:115:         asm volatile("isb"); // Serialize after reading the counter
#APP
// 115 "Out_of_order.c" 1
	isb
// 0 "" 2
#NO_APP
.L14:
// Out_of_order.c:124:     temp = array1[array1_size];
	adrp	x0, array1_size	// tmp205,
	add	x0, x0, :lo12:array1_size	// tmp204, tmp205,
	ldr	w2, [x0]	// array1_size.8_24, array1_size
	adrp	x0, array1	// tmp207,
	add	x1, x0, :lo12:array1	// tmp206, tmp207,
	uxtw	x0, w2	// tmp208, array1_size.8_24
	ldrb	w0, [x1, x0]	// _25, array1[array1_size.8_24]
// Out_of_order.c:124:     temp = array1[array1_size];
	str	w0, [sp, 72]	// _25, temp
// Out_of_order.c:126:     asm volatile("isb"); // Serialize before reading the counter
#APP
// 126 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:127:     time7 = read_pmccntr();
#NO_APP
	bl	read_pmccntr		//
	mov	x19, x0	// time7,
// Out_of_order.c:128:     asm volatile("isb"); // Serialize after reading the counter
#APP
// 128 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:129:     temp = array1[100];
#NO_APP
	adrp	x0, array1	// tmp210,
	add	x0, x0, :lo12:array1	// tmp209, tmp210,
	ldrb	w0, [x0, 100]	// _26, array1[100]
// Out_of_order.c:129:     temp = array1[100];
	str	w0, [sp, 72]	// _26, temp
// Out_of_order.c:130:     asm volatile("isb"); // Serialize before reading the counter
#APP
// 130 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:131:     time8 = read_pmccntr();
#NO_APP
	bl	read_pmccntr		//
	mov	x20, x0	// time8,
// Out_of_order.c:132:     asm volatile("isb"); // Serialize after reading the counter
#APP
// 132 "Out_of_order.c" 1
	isb
// 0 "" 2
// Out_of_order.c:135:     printf("Time 6 - Time 5 is %ld \n", time6 - time5);
#NO_APP
	sub	x0, x22, x21	// _27, time6, time5
	mov	x1, x0	//, _27
	adrp	x0, .LC3	// tmp211,
	add	x0, x0, :lo12:.LC3	//, tmp211,
	bl	printf		//
// Out_of_order.c:136:     printf("Time 8 - Time 7 is %ld \n", time8 - time7);
	sub	x0, x20, x19	// _28, time8, time7
	mov	x1, x0	//, _28
	adrp	x0, .LC4	// tmp212,
	add	x0, x0, :lo12:.LC4	//, tmp212,
	bl	printf		//
	mov	w0, 0	// _108,
// Out_of_order.c:139: }
	ldp	x19, x20, [sp, 16]	//,,
	ldp	x21, x22, [sp, 32]	//,,
	ldp	x29, x30, [sp], 80	//,,,
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret	
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
