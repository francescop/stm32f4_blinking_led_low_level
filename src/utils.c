/*
 * utils.c
 *
 *  Created on: 11 jul 2012
 *      Author: benjamin
 */
#include "utils.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>



void Print(const char *msg) {
 // Manual semi-hosting, because the GCC ARM Embedded's semihosting wasn't working.
 for (; *msg; ++msg) {
  // Moves a pointer to msg into r1, sets r0 to 0x03,
  // and then performs a special breakpoint that OpenOCD sees as
  // the semihosting call. r0 tells OpenOCD which semihosting
  // function we're calling. In this case WRITEC, which writes
  // a single char pointed to by r1 to the console.
  __asm__ ("mov r1,%0; mov r0,$3; BKPT 0xAB" :
                                             : "r" (msg)
                                             : "r0", "r1"
  );
 }
}
