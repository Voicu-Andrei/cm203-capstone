# Watch the Machine Think — CM-203 Capstone

A complete Von Neumann machine in ~500 lines of C: unified memory, four registers,
a program counter, an ALU with a zero flag, and a 12-instruction ISA — run **one
phase at a time** so you can watch fetch → decode → execute happen.

The thesis, made visible and then proven with self-modifying code:

> **An instruction is just a number. Instructions and data share one memory.
> The lens decides.**

## Build & run

```sh
make            # or: gcc -std=c99 -Wall -Wextra -O2 -o machine main.c cpu.c ui.c programs.c
./machine       # THE SHOW: the whole talk as four acts, one command, [n] to advance
./machine A     # demo A: sum 1..10 (a loop with a branch)
./machine A -n 3   # same loop, short enough to single-step live
./machine B     # demo B: self-modifying code — the program writes its own ending
./machine A --run  # headless: run to HALT, print output + final state
```

Keys: `space` = one phase · `r` = run to halt (animated) · `p` = poke memory ·
`q` = quit. In show mode also: `n` = next act · `1`–`4` = jump to an act with a
fresh machine (instant recovery if anything glitches mid-talk).
Portable C99 — builds on Windows (mingw), Linux, WSL, and the Arduino UNO Q's Linux side.

## The machine

| Component | Choice | Why |
|---|---|---|
| Memory | 256 × 16-bit words, instructions **and** data | the Von Neumann idea itself |
| Registers | R0–R3, PC (8-bit), IR, Z flag | enough for real programs, few enough to watch |
| Word | `uint16_t` | two's-complement wraparound is real, not simulated |

**Instruction format** — fixed width, one word per instruction, every field a whole hex digit:

```
[ opcode : 4 ][ rd : 4 ][ operand : 8 ]      e.g. 0x2001 = ADD R0, R1  (= the number 8193)
```

The 8-bit operand names exactly the 256 memory slots: the address space is
precisely what an instruction can point at, by design.

**ISA:** `HALT LOAD ADD SUB CMP JMP JZ JNZ LOADM STORE MOV OUT`
(opcodes `0x0`–`0xB`; unknown opcodes fault and halt). `CMP` is a `SUB` that
discards its result — same as x86.

## Files

- `cpu.h` / `cpu.c` — the machine: state struct, fetch/decode/execute, disassembler
- `ui.c` — the terminal UI: memory shown through *both lenses* (number + instruction), PC and last-written slots highlighted
- `programs.c` — the demo programs, hand-assembled, with the encoding worked out in comments
- `main.c` — step loop, run mode, headless mode, and show mode (the four-act talk)
- `docs/` — capstone plan and one-pager

`cpu_phase_hook` in `cpu.h` emits `'F'/'D'/'E'` once per phase. On the Arduino
UNO Q's Linux side the machine finds the board's free user RGB LED in
`/sys/class/leds` and drives it automatically — FETCH = blue, DECODE = green,
EXECUTE = red — so stepping the emulator blinks a physical heartbeat. No flag
needed; on machines without those LEDs the hook simply stays unset.
