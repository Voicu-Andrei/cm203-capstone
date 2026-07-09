# CM-203 Capstone One-Pager — Watch the Machine Think

**Student:** Andrei Voicu · **Module:** CM-203 Computer Organization · **Date:** 2026-07-10

## The idea

I built a complete Von Neumann machine in C — unified memory, registers, program
counter, ALU, and my own 12-instruction ISA — and I run it **one heartbeat at a
time**, so the audience watches fetch → decode → execute happen live. The talk
drives toward one claim from Ch. 5, first made visible and then *proven*:

> **An instruction is just a number. Instructions and data share one memory.
> The lens decides.**

## What exists already (working today)

- **The machine:** 256 × 16-bit words of unified memory, registers R0–R3, an 8-bit
  PC, IR, zero flag. Fixed-width instruction format `[opcode:4|rd:4|operand:8]` —
  every field is one hex digit, so instructions can be hand-assembled on stage.
  The 8-bit operand field addresses exactly the 256-slot memory: designed, not arbitrary.
- **Demo A — a loop:** sum 1..N. The PC visibly runs forward and gets thrown
  backwards by `JNZ` until the counter hits zero. Answer on stage: 55.
- **Demo B — self-modifying code:** a program that, as written, can never stop —
  its last word jumps back to the start. Mid-run it STOREs the *number* 0 over
  that word; 0 through the instruction lens is HALT. The program writes its own
  ending — possible only because instructions and data share memory.
- **The interface:** a terminal view showing every memory word through *both
  lenses at once* (as a number and as an instruction), the PC slot and the
  last-rewritten slot highlighted, and a plain-English account of each phase.

## Presentation plan (8 min)

1. The machine at rest; single-step the loop — watch the PC move and the ALU fire.
2. **The reveal:** the same word executed as an instruction, then read as a number (Ch. 4/5).
3. **Hand-assembly:** translate `ADD R0, R1` to `0x2001` live, type it into memory, run it (Ch. 6–10).
4. **Self-modifying code:** the program rewrites its own ending; why real CPUs
   forbid this (W^X, caches).
5. *Stretch:* the fetch/decode/execute heartbeat blinking on Arduino LEDs.

## Topics connected

C & the memory model (Ch. 1–3) · bits & two's-complement, "same bits, different
lens" (Ch. 4) · Von Neumann architecture, fetch–decode–execute (Ch. 5) · ISA
design, hand-assembly, x86/ARM64 parallels — my CMP is x86's CMP: a SUB that
discards its result (Ch. 6–10).

**Evidence:** public repo with working code and commit history.
