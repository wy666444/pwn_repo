/* Copyright (C) 1997-2016 Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdeps/generic/sysdep.h>

#ifdef	__ASSEMBLER__

/* Syntactic details of assembler.  */

#define ASM_SIZE_DIRECTIVE(name) .size name,.-name

/* Define an entry point visible from C.  */
#define ENTRY(name)						\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .align 4;							\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

/* Define an entry point visible from C.  */
#define ENTRY_ALIGN(name, align)				\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .p2align align;						\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

/* Define an entry point visible from C with a specified alignment and
   pre-padding with NOPs.  This can be used to ensure that a critical
   loop within a function is cache line aligned.  Note this version
   does not adjust the padding if CALL_MCOUNT is defined. */

#define ENTRY_ALIGN_AND_PAD(name, align, padding)		\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .p2align align;						\
  .rep padding;							\
  nop;								\
  .endr;							\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

#undef	END
#define END(name)						\
  cfi_endproc;							\
  ASM_SIZE_DIRECTIVE(name)

/* If compiled for profiling, call `mcount' at the start of each function.  */
#ifdef	PROF
# define CALL_MCOUNT						\
	str	x30, [sp, #-80]!;				\
	cfi_adjust_cfa_offset (80);				\
	cfi_rel_offset (x30, 0);				\
	stp	x0, x1, [sp, #16];				\
	cfi_rel_offset (x0, 16);				\
	cfi_rel_offset (x1, 24);				\
	stp	x2, x3, [sp, #32];				\
	cfi_rel_offset (x2, 32);				\
	cfi_rel_offset (x3, 40);				\
	stp	x4, x5, [sp, #48];				\
	cfi_rel_offset (x4, 48);				\
	cfi_rel_offset (x5, 56);				\
	stp	x6, x7, [sp, #64];				\
	cfi_rel_offset (x6, 64);				\
	cfi_rel_offset (x7, 72);				\
	mov	x0, x30;					\
	bl	mcount;						\
	ldp	x0, x1, [sp, #16];				\
	cfi_restore (x0);					\
	cfi_restore (x1);					\
	ldp	x2, x3, [sp, #32];				\
	cfi_restore (x2);					\
	cfi_restore (x3);					\
	ldp	x4, x5, [sp, #48];				\
	cfi_restore (x4);					\
	cfi_restore (x5);					\
	ldp	x6, x7, [sp, #64];				\
	cfi_restore (x6);					\
	cfi_restore (x7);					\
	ldr	x30, [sp], #80;					\
	cfi_adjust_cfa_offset (-80);				\
	cfi_restore (x30);
#else
# define CALL_MCOUNT		/* Do nothing.  */
#endif

/* Local label name for asm code.  */
#ifndef L
# define L(name)         .L##name
#endif

/* Load or store to/from a pc-relative EXPR into/from R, using T.  */
#define LDST_PCREL(OP, R, T, EXPR)  \
	adrp	T, EXPR;	    \
	OP	R, [T, #:lo12:EXPR];\

/* Load or store to/from a got-relative EXPR into/from R, using T.  */
#define LDST_GLOBAL(OP, R, T, EXPR)     \
	adrp	T, :got:EXPR;		\
	ldr	T, [T, #:got_lo12:EXPR];\
	OP	R, [T];

/* Since C identifiers are not normally prefixed with an underscore
   on this system, the asm identifier `syscall_error' intrudes on the
   C name space.  Make sure we use an innocuous name.  */
#define syscall_error	__syscall_error
#define mcount		_mcount

#endif	/* __ASSEMBLER__ */
