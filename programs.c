#include "cpu.h"
#include <string.h>

/*
 * Demo A — sum 1..N. The loop that makes the machine feel alive:
 * the PC visibly runs forward, then JNZ throws it backwards.
 *
 * Hand-assembly of every word (format [opcode|rd|operand]):
 *   LOAD R0,#0  -> op 1, rd 0, imm 00 -> 0x1000
 *   ADD  R0,R1  -> op 2, rd 0, rs  01 -> 0x2001   (as a number: 8193)
 */
static const uint16_t demo_a[] = {
    0x1000,  /* 0x00  LOAD  R0, #0     sum = 0                       */
    0x110A,  /* 0x01  LOAD  R1, #10    counter = N (patched by -n)   */
    0x1201,  /* 0x02  LOAD  R2, #1     the constant 1                */
    0x2001,  /* 0x03  ADD   R0, R1     sum += counter           <--+ */
    0x3102,  /* 0x04  SUB   R1, R2     counter--, sets Z            | */
    0x7003,  /* 0x05  JNZ   0x03       loop while counter != 0  ---+ */
    0xB000,  /* 0x06  OUT   R0         print the sum                 */
    0x0000,  /* 0x07  HALT                                           */
};

/*
 * Demo B — self-modifying code: "the program writes its own ending".
 *
 * As written, this program can never stop: slot 0x07 jumps back to the start.
 * But its second instruction stores the number 0 over slot 0x07. The number 0,
 * read through the instruction lens, is HALT. When the countdown finishes and
 * the PC finally arrives at 0x07, it executes the program's own edit.
 * Only possible because instructions and data share one memory. (Von Neumann)
 */
static const uint16_t demo_b[] = {
    0x1200,  /* 0x00  LOAD  R2, #0      R2 = 0 -- "just a number"          */
    0x9207,  /* 0x01  STORE R2, [0x07]  rewrite slot 7: JMP becomes HALT   */
    0x1003,  /* 0x02  LOAD  R0, #3      countdown from 3                   */
    0x1101,  /* 0x03  LOAD  R1, #1      the constant 1                     */
    0xB000,  /* 0x04  OUT   R0          print counter               <--+   */
    0x3001,  /* 0x05  SUB   R0, R1      counter--, sets Z              |   */
    0x7004,  /* 0x06  JNZ   0x04        loop while counter != 0    ---+   */
    0x5000,  /* 0x07  JMP   0x00        run forever... unless rewritten    */
};

/* Load a demo into a freshly reset CPU. n patches Demo A's counter (1..255). */
const char *load_demo(CPU *c, char which, int n)
{
    cpu_reset(c);
    switch (which) {
    case 'a': case 'A':
        memcpy(c->mem, demo_a, sizeof demo_a);
        if (n >= 1 && n <= 255) c->mem[1] = (uint16_t)(0x1100 | n);
        return "A: sum 1..N (a loop with a branch)";
    case 'b': case 'B':
        memcpy(c->mem, demo_b, sizeof demo_b);
        return "B: self-modifying code";
    default:
        return 0;
    }
}
