#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ui_init(int plain);
int  ui_getkey(void);
int  ui_read_hex(const char *prompt, unsigned *out);
void ui_msleep(int ms);
void ui_render(const CPU *c, const char *progname);
const char *load_demo(CPU *c, char which, int n);

#define MAX_INSTR 100000   /* runaway guard: no demo needs more */
#define RUN_DELAY 120      /* ms per instruction in run mode: fast, but watchable */

static int usage(void)
{
    fprintf(stderr,
        "usage: machine <A|B> [options]\n"
        "  A            demo: sum 1..N with a loop        (-n N, default 10)\n"
        "  B            demo: self-modifying code\n"
        "  -n N         set Demo A's counter (1..255); use -n 3 for live stepping\n"
        "  --run        headless: run to HALT, print output and final state\n"
        "  --plain      no colors / no screen clearing (pipes, odd projectors)\n"
        "keys: [space] one phase   [r] run to halt   [q] quit\n");
    return 2;
}

int main(int argc, char **argv)
{
    static CPU cpu;
    const char *name = 0;
    char prog = 0;
    int n = 0, headless = 0, plain = 0, i;

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--run")) headless = 1;
        else if (!strcmp(argv[i], "--plain")) plain = 1;
        else if (!strcmp(argv[i], "-n") && i + 1 < argc) n = atoi(argv[++i]);
        else if (!prog && strlen(argv[i]) == 1) prog = argv[i][0];
        else return usage();
    }
    if (!prog) return usage();
    name = load_demo(&cpu, prog, n);
    if (!name) return usage();

    if (headless) {
        int printed = 0;
        while (!cpu.halted && cpu.instr_done < MAX_INSTR)
            cpu_step_instruction(&cpu);
        printf("program %s\n", name);
        printf("output:");
        for (; printed < cpu.nout; printed++) printf(" %u", cpu.out[printed]);
        printf("\nhalted after %u instructions\n", cpu.instr_done);
        printf("final: R0=0x%04X R1=0x%04X R2=0x%04X R3=0x%04X PC=0x%02X Z=%d\n",
               cpu.reg[0], cpu.reg[1], cpu.reg[2], cpu.reg[3], cpu.pc, cpu.zf);
        return 0;
    }

    ui_init(plain);
    ui_render(&cpu, name);
    for (;;) {
        int k = ui_getkey();
        if (k == 'q' || k == 'Q' || k == EOF) break;
        if ((k == ' ' || k == 's' || k == '\r' || k == '\n') && !cpu.halted) {
            cpu_step_phase(&cpu);
            ui_render(&cpu, name);
        } else if ((k == 'r' || k == 'R') && !cpu.halted) {
            while (!cpu.halted && cpu.instr_done < MAX_INSTR) {
                cpu_step_instruction(&cpu);
                ui_render(&cpu, name);
                ui_msleep(RUN_DELAY);
            }
        } else if (k == 'p' || k == 'P') {
            /* live hand-assembly: type a word straight into memory */
            unsigned addr, val;
            char d[32];
            printf("\n");
            if (ui_read_hex(" poke slot (hex): ", &addr) && addr < MEM_SIZE &&
                ui_read_hex("   value (hex): ", &val)) {
                cpu.mem[addr] = (uint16_t)val;
                cpu.last_write = (int)addr;
                disasm(cpu.mem[addr], d, sizeof d);
                snprintf(cpu.msg, sizeof cpu.msg,
                         "POKE     mem[0x%02X] <- 0x%04X  (%s)  -- hand-assembled by a human",
                         addr, val & 0xFFFF, d);
            }
            ui_render(&cpu, name);
        }
    }
    return 0;
}
