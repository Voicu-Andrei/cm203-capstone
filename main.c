#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ui_init(int plain);
int  ui_getkey(void);
int  ui_read_num(const char *prompt, int base, unsigned *out);
void ui_msleep(int ms);
void ui_render(const CPU *c, const char *progname);
void ui_set_act(const char *line);
const char *load_demo(CPU *c, char which, int n);

#define MAX_INSTR 100000   /* runaway guard: no demo needs more */
#define RUN_DELAY 120      /* ms per instruction in run mode: fast, but watchable */

/*
 * Show mode — the whole talk in one command: `machine` with no arguments.
 * Four acts; [n] moves forward, [1-4] jumps anywhere with a fresh machine
 * (which is also the 3-second glitch recovery: no quitting, no retyping).
 * Program names here are audience-facing, so Act 4 keeps its secret.
 */
#define NACTS 4

static const struct {
    char prog;
    int  n;              /* Demo A's counter; -1 = ask the audience live */
    const char *title;   /* footer: the act name                         */
    const char *name;    /* top of screen: what this program claims to be */
} acts[NACTS] = {
    { 'A',  3, "THE HEARTBEAT",      "sum 1+2+3 (a loop with a branch)" },
    { 'A', -1, "YOU vs THE MACHINE", "sum 1..N, N picked by the room" },
    { 'A', 10, "BE THE ASSEMBLER",   "sum 1..10 (until a human rewrites it)" },
    { 'B',  0, "THE FINALE",         "this program can never stop" },
};

static const char *enter_act(CPU *cpu, int i)
{
    static char line[96];
    int n = acts[i].n;
    if (n < 0) {
        unsigned v = 0;
        printf("\n");
        if (!ui_read_num(" a number under 20, please: ", 10, &v) || v < 1 || v > 255)
            v = 10;
        n = (int)v;
    }
    load_demo(cpu, acts[i].prog, n);
    snprintf(line, sizeof line, "ACT %d/%d  %-22s [n] next act   [1-%d] jump to act",
             i + 1, NACTS, acts[i].title, NACTS);
    ui_set_act(line);
    return acts[i].name;
}

static int usage(void)
{
    fprintf(stderr,
        "usage: machine            the show: all four acts, [n] to advance\n"
        "       machine <A|B> [options]\n"
        "  A            demo: sum 1..N with a loop        (-n N, default 10)\n"
        "  B            demo: self-modifying code\n"
        "  -n N         set Demo A's counter (1..255); use -n 3 for live stepping\n"
        "  --run        headless: run to HALT, print output and final state\n"
        "  --plain      no colors / no screen clearing (pipes, odd projectors)\n"
        "keys: [space] one phase   [r] run to halt   [p] poke memory   [q] quit\n");
    return 2;
}

int main(int argc, char **argv)
{
    static CPU cpu;
    const char *name = 0;
    char prog = 0;
    int n = 0, headless = 0, plain = 0, show = 0, cur = 0, i;

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--run")) headless = 1;
        else if (!strcmp(argv[i], "--plain")) plain = 1;
        else if (!strcmp(argv[i], "-n") && i + 1 < argc) n = atoi(argv[++i]);
        else if (!prog && strlen(argv[i]) == 1) prog = argv[i][0];
        else return usage();
    }
    if (!prog) {
        if (headless) return usage();   /* headless needs a named demo */
        show = 1;
    } else {
        name = load_demo(&cpu, prog, n);
        if (!name) return usage();
    }

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
    if (show) name = enter_act(&cpu, cur);
    ui_render(&cpu, name);
    for (;;) {
        int k = ui_getkey();
        if (k == 'q' || k == 'Q' || k == EOF) break;
        if (show) {
            int target = -1;
            if ((k == 'n' || k == 'N') && cur < NACTS - 1) target = cur + 1;
            else if (k >= '1' && k < '1' + NACTS) target = k - '1';
            if (target >= 0) {
                cur = target;
                name = enter_act(&cpu, cur);
                ui_render(&cpu, name);
                continue;
            }
        }
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
            if (ui_read_num(" poke slot (hex): ", 16, &addr) && addr < MEM_SIZE &&
                ui_read_num("   value (hex): ", 16, &val)) {
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
