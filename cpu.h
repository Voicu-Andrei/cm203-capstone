#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

/*
 * A tiny Von Neumann machine.
 *
 * Memory: 256 words of 16 bits. Instructions and data share it — that IS the point.
 * Word:   uint16_t, so two's-complement wraparound is the real thing, not simulated.
 *
 * Instruction format (fixed width, one word = one instruction):
 *
 *      [ opcode : 4 ][ rd : 4 ][ operand : 8 ]
 *
 * Each field is a whole hex digit, so 0x2001 reads off as: opcode 2 (ADD), rd R0,
 * operand 01 (R1). The 8-bit operand field names exactly the 256 memory slots —
 * the address space is precisely what an instruction can point at, by design.
 */

#define MEM_SIZE 256
#define NREGS      4
#define MAX_OUT   32

typedef enum {
    OP_HALT  = 0x0,   /* stop the machine                       */
    OP_LOAD  = 0x1,   /* LOAD  Rd, #imm8   Rd <- immediate      */
    OP_ADD   = 0x2,   /* ADD   Rd, Rs      Rd <- Rd + Rs, sets Z */
    OP_SUB   = 0x3,   /* SUB   Rd, Rs      Rd <- Rd - Rs, sets Z */
    OP_CMP   = 0x4,   /* CMP   Rd, Rs      sets Z from Rd - Rs, result discarded */
    OP_JMP   = 0x5,   /* JMP   addr        PC <- addr            */
    OP_JZ    = 0x6,   /* JZ    addr        PC <- addr if Z       */
    OP_JNZ   = 0x7,   /* JNZ   addr        PC <- addr if !Z      */
    OP_LOADM = 0x8,   /* LOADM Rd, addr    Rd <- mem[addr]       */
    OP_STORE = 0x9,   /* STORE Rd, addr    mem[addr] <- Rd       */
    OP_MOV   = 0xA,   /* MOV   Rd, Rs      Rd <- Rs              */
    OP_OUT   = 0xB,   /* OUT   Rd          print Rd              */
} Opcode;

typedef enum { PH_FETCH, PH_DECODE, PH_EXECUTE } Phase;

typedef struct {
    uint16_t mem[MEM_SIZE];
    uint16_t reg[NREGS];
    uint8_t  pc;              /* 8 bits on purpose: one operand's worth of address */
    uint16_t ir;
    bool     zf;
    bool     halted;

    Phase    next_phase;      /* which third of the cycle runs on the next step */
    uint8_t  opcode, rd, rs;  /* filled in by decode */
    uint8_t  operand;

    uint32_t instr_done;      /* completed instructions */
    uint32_t phases_done;     /* completed phases (3 per instruction) */
    int      last_write;      /* mem slot most recently changed by STORE, -1 = none */
    uint16_t out[MAX_OUT];    /* everything OUT has printed */
    int      nout;
    char     msg[160];        /* plain-English account of the last phase, for the UI */
} CPU;

/* Optional observer, called once per phase with 'F'/'D'/'E'.
 * This is the Arduino serial hook: point it at a function that writes the
 * byte to the serial port and three LEDs become the machine's heartbeat. */
extern void (*cpu_phase_hook)(char phase);

void cpu_reset(CPU *c);
void cpu_step_phase(CPU *c);        /* advance one phase: fetch, decode or execute */
void cpu_step_instruction(CPU *c);  /* run whatever remains of the current cycle */
void disasm(uint16_t word, char *buf, int buflen);

#endif
