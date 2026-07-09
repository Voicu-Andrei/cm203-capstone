#include "cpu.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#  include <windows.h>
#  include <conio.h>
#  include <io.h>
#  define ISATTY_IN() _isatty(_fileno(stdin))
#else
#  include <termios.h>
#  include <unistd.h>
#  include <stdlib.h>
#  include <time.h>
#  define ISATTY_IN() isatty(STDIN_FILENO)
static struct termios saved_tio;
static int tio_saved = 0;
static void restore_tio(void) { if (tio_saved) tcsetattr(STDIN_FILENO, TCSANOW, &saved_tio); }
#endif

static int color_on = 1;
static int tty_in = 1;

/* every color goes through cc() so --plain can turn the whole UI monochrome */
static const char *cc(const char *code) { return color_on ? code : ""; }

#define RESET  "\x1b[0m"
#define BOLD   "\x1b[1m"
#define DIM    "\x1b[2m"
#define PCROW  "\x1b[30;46m"   /* black on cyan:   the slot the PC points at   */
#define WRROW  "\x1b[30;43m"   /* black on yellow: the slot STORE last rewrote */
#define PHON   "\x1b[30;42m"   /* black on green:  the phase that just ran     */
#define CHG    "\x1b[93m"      /* bright yellow:   value changed this step     */
#define HALTB  "\x1b[97;41m"   /* white on red:    halted banner               */

void ui_init(int plain)
{
    color_on = !plain;
    tty_in = ISATTY_IN();
#ifdef _WIN32
    if (color_on) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD m = 0;
        if (GetConsoleMode(h, &m))
            SetConsoleMode(h, m | 0x0004 /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */);
    }
#else
    if (tty_in) {
        struct termios t;
        tcgetattr(STDIN_FILENO, &saved_tio);
        tio_saved = 1;
        atexit(restore_tio);
        t = saved_tio;
        t.c_lflag &= ~(ICANON | ECHO);   /* raw-ish keys; keep ISIG so Ctrl-C works */
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }
#endif
}

/* One key, no Enter needed on a terminal. From a pipe: next command character. */
int ui_getkey(void)
{
    if (!tty_in) {
        int ch;
        do ch = getchar(); while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');
        return ch == EOF ? 'q' : ch;
    }
#ifdef _WIN32
    for (;;) {
        int ch = _getch();
        if (ch == 0 || ch == 224) { _getch(); continue; }  /* swallow arrow keys etc. */
        return ch;
    }
#else
    {
        unsigned char ch;
        if (read(STDIN_FILENO, &ch, 1) != 1) return 'q';
        return ch;
    }
#endif
}

void ui_msleep(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&ts, 0);
#endif
}

/* registers as "0x0037 (55)"; big values also shown through the signed lens */
static void fmt_val(uint16_t v, char *buf, int n)
{
    if (v >= 0x8000) snprintf(buf, n, "0x%04X (%u = %d)", v, v, (int16_t)v);
    else             snprintf(buf, n, "0x%04X (%u)", v, v);
}

void ui_render(const CPU *c, const char *progname)
{
    static uint16_t prev_reg[NREGS];
    static int prev_z = -1;
    char buf[64], dis[32];
    int i;

    if (color_on) printf("\x1b[2J\x1b[H");
    else          printf("\n----------------------------------------------------------------------\n");

    printf("%s=====================  WATCH THE MACHINE THINK  =====================%s\n",
           cc(BOLD), cc(RESET));
    printf(" program: %-28s completed instructions: %u\n\n", progname, c->instr_done);

    /* the heartbeat: highlight the phase that just ran */
    {
        int just = c->phases_done ? ((int)c->next_phase + 2) % 3 : -1;
        static const char *ph[3] = { " FETCH ", " DECODE ", " EXECUTE " };
        printf(" cycle: ");
        for (i = 0; i < 3; i++) {
            if (i == just) printf("[%s%s%s]", cc(PHON), ph[i], cc(RESET));
            else           printf("[%s%s%s]", cc(DIM), ph[i], cc(RESET));
            if (i < 2) printf(" -> ");
        }
        printf("\n\n");
    }

    printf(" %s%s%s\n\n", cc(BOLD), c->msg, cc(RESET));

    printf(" REGISTERS");
    for (i = 0; i < NREGS; i++) {
        int chg = c->phases_done && c->reg[i] != prev_reg[i];
        fmt_val(c->reg[i], buf, sizeof buf);
        printf("   %sR%d = %-18s%s", chg ? cc(CHG) : "", i, buf, cc(RESET));
        if (i == 1) printf("\n          ");
    }
    {
        int zchg = prev_z >= 0 && c->zf != prev_z;
        printf("\n PC = 0x%02X    IR = 0x%04X    %sZ = %d%s\n\n",
               c->pc, c->ir, zchg ? cc(CHG) : "", c->zf, cc(RESET));
    }

    /* memory, shown through both lenses at once */
    printf(" MEMORY       addr    bits      as number   as instruction\n");
    {
        int limit = 8;
        for (i = 0; i < MEM_SIZE; i++) if (c->mem[i]) limit = i + 1;
        if (c->pc + 1 > limit) limit = c->pc + 1;
        if (c->last_write + 1 > limit) limit = c->last_write + 1;
        if (limit < 8) limit = 8;
        if (limit > 32) limit = 32;

        for (i = 0; i < limit; i++) {
            const char *row = "";
            const char *tag = "     ";
            if (i == c->last_write) { row = cc(WRROW); tag = "  *W "; }
            if (i == c->pc)         { row = cc(PCROW); tag = "  PC>"; }
            disasm(c->mem[i], dis, sizeof dis);
            printf("%s%s  0x%02X    0x%04X    %5u       %-18s%s\n",
                   tag, row, i, c->mem[i], c->mem[i], dis, cc(RESET));
        }
        printf("       %s(slots 0x%02X..0xFF not shown: all zero)%s\n", cc(DIM), limit, cc(RESET));
    }

    printf("\n OUTPUT: ");
    if (c->nout == 0) printf("%s(nothing yet)%s", cc(DIM), cc(RESET));
    for (i = 0; i < c->nout; i++) printf("%s%u%s ", cc(BOLD), c->out[i], cc(RESET));
    printf("\n\n");

    if (c->halted)
        printf(" %s*** MACHINE HALTED after %u instructions ***%s   [q] quit\n",
               cc(HALTB), c->instr_done, cc(RESET));
    else
        printf(" [space] step one phase    [r] run to halt    [q] quit\n");

    memcpy(prev_reg, c->reg, sizeof prev_reg);
    prev_z = c->zf;
    fflush(stdout);
}
