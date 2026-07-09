#include "cpu.h"
#include <stdio.h>
#include <string.h>

void (*cpu_phase_hook)(char phase) = 0;

static const char *opname[16] = {
    "HALT", "LOAD", "ADD", "SUB", "CMP", "JMP", "JZ", "JNZ",
    "LOADM", "STORE", "MOV", "OUT", "???", "???", "???", "???"
};

void cpu_reset(CPU *c)
{
    memset(c, 0, sizeof *c);
    c->last_write = -1;
    c->next_phase = PH_FETCH;
    snprintf(c->msg, sizeof c->msg,
             "Machine online. Program in memory, PC at 0x00. Step to fetch.");
}

/* Read a word through the "instruction lens". Any word decodes to something —
 * that is the honest consequence of instructions and data sharing memory. */
void disasm(uint16_t w, char *buf, int n)
{
    uint8_t op  = w >> 12;
    uint8_t rd  = (w >> 8) & 0x3;   /* field is 4 bits; only 4 registers wired */
    uint8_t imm = w & 0xFF;
    uint8_t rs  = w & 0x3;

    switch (op) {
    case OP_HALT:  snprintf(buf, n, "HALT");                              break;
    case OP_LOAD:  snprintf(buf, n, "LOAD  R%u, #%u", rd, imm);           break;
    case OP_ADD:
    case OP_SUB:
    case OP_CMP:
    case OP_MOV:   snprintf(buf, n, "%-5s R%u, R%u", opname[op], rd, rs); break;
    case OP_JMP:
    case OP_JZ:
    case OP_JNZ:   snprintf(buf, n, "%-5s 0x%02X", opname[op], imm);      break;
    case OP_LOADM:
    case OP_STORE: snprintf(buf, n, "%-5s R%u, [0x%02X]", opname[op], rd, imm); break;
    case OP_OUT:   snprintf(buf, n, "OUT   R%u", rd);                     break;
    default:       snprintf(buf, n, "???");                               break;
    }
}

static void fetch(CPU *c)
{
    uint8_t at = c->pc;
    c->ir = c->mem[at];
    c->pc++;   /* uint8_t wraps at 256 by itself — the address bus is 8 bits wide */
    snprintf(c->msg, sizeof c->msg,
             "FETCH    IR <- mem[0x%02X] = 0x%04X            PC advances to 0x%02X",
             at, c->ir, c->pc);
    if (cpu_phase_hook) cpu_phase_hook('F');
}

static void decode(CPU *c)
{
    char d[32];
    c->opcode  = c->ir >> 12;
    c->rd      = (c->ir >> 8) & 0x3;
    c->operand = c->ir & 0xFF;
    c->rs      = c->operand & 0x3;
    disasm(c->ir, d, sizeof d);
    snprintf(c->msg, sizeof c->msg,
             "DECODE   0x%04X splits into opcode=%X (%s) rd=%X operand=0x%02X  ->  %s",
             c->ir, c->opcode, opname[c->opcode], (c->ir >> 8) & 0xF, c->operand, d);
    if (cpu_phase_hook) cpu_phase_hook('D');
}

static void execute(CPU *c)
{
    uint16_t *R = c->reg;
    uint16_t t;
    char m[160];

    switch (c->opcode) {
    case OP_HALT:
        c->halted = true;
        snprintf(m, sizeof m, "EXECUTE  HALT -- the machine stops.");
        break;
    case OP_LOAD:
        R[c->rd] = c->operand;
        snprintf(m, sizeof m, "EXECUTE  LOAD: R%u <- %u", c->rd, c->operand);
        break;
    case OP_ADD:
        R[c->rd] = (uint16_t)(R[c->rd] + R[c->rs]);
        c->zf = (R[c->rd] == 0);
        snprintf(m, sizeof m, "EXECUTE  ADD: R%u <- R%u + R%u = %u    Z=%d",
                 c->rd, c->rd, c->rs, R[c->rd], c->zf);
        break;
    case OP_SUB:
        R[c->rd] = (uint16_t)(R[c->rd] - R[c->rs]);
        c->zf = (R[c->rd] == 0);
        snprintf(m, sizeof m, "EXECUTE  SUB: R%u <- R%u - R%u = %u    Z=%d",
                 c->rd, c->rd, c->rs, R[c->rd], c->zf);
        break;
    case OP_CMP:
        t = (uint16_t)(R[c->rd] - R[c->rs]);
        c->zf = (t == 0);
        snprintf(m, sizeof m,
                 "EXECUTE  CMP: R%u - R%u computed, result discarded    Z=%d",
                 c->rd, c->rs, c->zf);
        break;
    case OP_JMP:
        c->pc = c->operand;
        snprintf(m, sizeof m, "EXECUTE  JMP: PC <- 0x%02X", c->operand);
        break;
    case OP_JZ:
        if (c->zf) { c->pc = c->operand;
            snprintf(m, sizeof m, "EXECUTE  JZ: Z=1 -> taken, PC <- 0x%02X", c->operand); }
        else
            snprintf(m, sizeof m, "EXECUTE  JZ: Z=0 -> not taken, PC stays 0x%02X", c->pc);
        break;
    case OP_JNZ:
        if (!c->zf) { c->pc = c->operand;
            snprintf(m, sizeof m, "EXECUTE  JNZ: Z=0 -> taken, PC <- 0x%02X", c->operand); }
        else
            snprintf(m, sizeof m, "EXECUTE  JNZ: Z=1 -> not taken, PC stays 0x%02X", c->pc);
        break;
    case OP_LOADM:
        R[c->rd] = c->mem[c->operand];
        snprintf(m, sizeof m, "EXECUTE  LOADM: R%u <- mem[0x%02X] = 0x%04X",
                 c->rd, c->operand, R[c->rd]);
        break;
    case OP_STORE:
        c->mem[c->operand] = R[c->rd];
        c->last_write = c->operand;
        snprintf(m, sizeof m,
                 "EXECUTE  STORE: mem[0x%02X] <- R%u = 0x%04X   ** MEMORY REWRITTEN **",
                 c->operand, c->rd, R[c->rd]);
        break;
    case OP_MOV:
        R[c->rd] = R[c->rs];
        snprintf(m, sizeof m, "EXECUTE  MOV: R%u <- R%u = %u", c->rd, c->rs, R[c->rd]);
        break;
    case OP_OUT:
        if (c->nout < MAX_OUT) c->out[c->nout++] = R[c->rd];
        snprintf(m, sizeof m, "EXECUTE  OUT: prints R%u = %u", c->rd, R[c->rd]);
        break;
    default:
        /* An unknown opcode is a fault: the machine refuses rather than guesses. */
        c->halted = true;
        snprintf(m, sizeof m,
                 "EXECUTE  illegal opcode %X -- fault, machine halts.", c->opcode);
        break;
    }
    memcpy(c->msg, m, sizeof c->msg);
    if (cpu_phase_hook) cpu_phase_hook('E');
}

void cpu_step_phase(CPU *c)
{
    if (c->halted) return;
    switch (c->next_phase) {
    case PH_FETCH:   fetch(c);   c->next_phase = PH_DECODE;  break;
    case PH_DECODE:  decode(c);  c->next_phase = PH_EXECUTE; break;
    case PH_EXECUTE: execute(c); c->next_phase = PH_FETCH;   c->instr_done++; break;
    }
    c->phases_done++;
}

void cpu_step_instruction(CPU *c)
{
    if (c->halted) return;
    do cpu_step_phase(c); while (!c->halted && c->next_phase != PH_FETCH);
}
