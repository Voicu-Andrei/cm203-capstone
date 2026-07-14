# UNDERSTANDING — what everything is, how to say it, and why it's true

Study order: read §1–§3 until you can say them without looking. Then §4–§8 once per act.
§9 is the interview. §10 is the last-minute cram.

---

## §1 · The one idea (memorize this paragraph)

A computer's memory holds nothing but numbers. Some of those numbers the CPU treats as
**instructions** (it fetches them and does what they say), and some it treats as **data**
(it adds them, stores them, prints them). Nothing in the memory marks which is which —
the only thing that decides is **whether the program counter arrives there**. That is the
Von Neumann design, and it is why your talk's thesis is literally true:

> **An instruction is just a number. Instructions and data share one memory. The lens decides.**

Everything in the demo exists to make that visible (Acts 1–3) and then prove it (Act 4:
a program that rewrites one of its own instructions — only possible because both live
in the same memory).

---

## §2 · The cast of characters (everything on the screen)

**MEMORY — 256 boxes of 16 bits each.**
*Say it simply:* "256 numbered boxes, each holding one number. The program lives in the
boxes AND works on the boxes. Same shelf."
*Technical:* `uint16_t mem[256]`. One address space for code and data — that IS Von Neumann.
*Why 256?* Because the operand field of an instruction is 8 bits, and 2⁸ = 256. The memory
is exactly as big as what an instruction can point at. (Defend it this way — it's a designed
choice, not a random number.)

**REGISTERS R0–R3 — the CPU's four pockets.**
*Say it simply:* "The machine's hands. It can't work on a box directly — it lifts a value
into a pocket, works on it there, puts it back."
*Technical:* four 16-bit general-purpose registers. The `rd` field in the instruction is
4 bits wide but only 2 bits are wired (4 registers) — real ISAs also reserve encoding
space they don't fully use.
*Why 4?* Enough to write real programs, few enough that the audience can watch all of them.

**PC — the program counter, the machine's finger.**
*Say it simply:* "A finger moving down a recipe. It always points at the NEXT line."
*Technical:* 8 bits on purpose — one operand's worth of address, so it can name exactly
the 256 boxes, and it wraps at 256 by itself (`uint8_t` overflow is the address bus width).
The cyan row on screen is where the finger points.

**IR — the instruction register.**
*Say it simply:* "The line the machine just picked up and is reading right now."
*Technical:* fetch copies `mem[PC]` into IR, THEN the PC advances. Decode reads fields out
of IR, never out of memory — that's why "fetch then decode" is honestly two steps.

**Z — the zero flag, one bit of memory about the last result.**
*Say it simply:* "A sticky note: 'the last answer was zero — yes/no'. It's the only way
this machine ever makes a decision."
*Technical:* ADD/SUB/CMP set it. JZ/JNZ read it. That pair — an operation that leaves a
flag, a jump that reads the flag — is exactly how x86 and ARM branch too.

**ALU — the calculator inside.**
*Technical:* add, subtract, compare on `uint16_t`, so two's-complement wraparound is the
real thing, not simulated. CMP is a SUB that throws away the result and keeps only the
flag — same trick as x86's `cmp`.

**The two-lens memory display (the most important column trick).**
Every row shows the SAME 16 bits three ways: raw hex, as a plain number, and disassembled
as an instruction. Nothing is converted — it's one value, three ways of reading it.
The screen never lies about the thesis: every box always wears both hats.

---

## §3 · One heartbeat in full: what each phase really does

The cycle is **fetch → decode → execute**. Your words: **grab → understand → do.**
Worked example — the machine is about to run box 0x03 holding `0x2001`:

1. **FETCH ("grab")** — `IR ← mem[0x03]`, so IR now holds 0x2001. PC advances to 0x04.
   *Note the order:* the PC moves forward at fetch. A jump instruction later simply
   overwrites the PC during execute — that's all a jump is.
2. **DECODE ("understand")** — the 16 bits split by position, each field one hex digit:
   `[2 | 0 | 01]` → opcode 2 = ADD, destination R0, source R1. Nothing is looked up in
   memory; the bits themselves are the meaning.
3. **EXECUTE ("do")** — `R0 ← R0 + R1`, and Z is set if the result was zero.

And the reveal, always ready in your pocket: **0x2001 read as a plain number is 8193.**
Same bits. "ADD R0,R1" and "8193" were never two things.

---

## §4 · Act 1 — THE HEARTBEAT (sum 1+2+3)

The program, box by box (this is Demo A with the counter set to 3):

| box | word | meaning | plain words |
|-----|--------|---------------------|-------------|
| 00 | 0x1000 | LOAD R0, #0 | sum starts at 0 |
| 01 | 0x1103 | LOAD R1, #3 | counter starts at 3 |
| 02 | 0x1201 | LOAD R2, #1 | keep the number 1 handy |
| 03 | 0x2001 | ADD R0, R1 | sum += counter ← the loop returns HERE |
| 04 | 0x3102 | SUB R1, R2 | counter-- (this sets Z) |
| 05 | 0x7003 | JNZ 0x03 | not zero yet? finger jumps back to 03 |
| 06 | 0xB000 | OUT R0 | print the sum |
| 07 | 0x0000 | HALT | stop |

It adds 3, then 2, then 1 → prints **6**. What you're explaining while stepping:

- First instruction at full slowness: name the three phases out loud. "Grab: the finger's
  box goes into the reader, finger moves on. Understand: the number splits into 'put 0 in
  pocket 0'. Do: pocket 0 becomes 0."
- **The moment that matters:** when JNZ executes and the cyan row LEAPS from 05 back up
  to 03. Say: "Nobody told it to go back. It looked at the sticky note — 'was the last
  answer zero? no' — and *decided*. Every if-statement and every loop you have ever
  written comes down to this: an operation leaves a flag, a jump reads it."
- Last pass: SUB makes the counter 0, Z flips to 1, JNZ *falls through* — the loop ends
  the same way it lived, by reading the flag.

---

## §5 · Act 2 — YOU vs THE MACHINE (+ the reveal)

Same program, counter = their number. You press `r`: the machine does the whole
loop — three phases per instruction, every phase honestly executed — in ~2 seconds.

- The point of the race: "It took me all of Act 1 to do three loops. It just did twenty.
  And this is my machine slowed a **billion-fold** — a real core runs billions of these
  cycles every second. Same loop. Only faster."
- **The reveal (stay on the finished screen).** Point at one row of memory — say box 03:
  "Right column says ADD R0,R1. Middle column says 8193. I converted nothing. That is one
  number read through two lenses. Hold that thought — the finale runs on it."

---

## §6 · Act 3 — BE THE ASSEMBLER

What an assembler is: **a translator from words to numbers, nothing more.** Tonight you
do its job by hand, live, and the machine can't tell the difference.

The encoding, done out loud (pocket 3, digit 7 → `1307`):
- first digit `1` = the opcode LOAD, "put-a-number-in-a-pocket"
- second digit `3` = which pocket (R3)
- last two digits `07` = the number itself

Poke it into box 00, step three phases, and pocket 3 holds a 7. Land it:
"I didn't write code. I wrote a NUMBER into a box, and because the finger walked over it,
it became an instruction. That's all any program is — numbers someone put in the boxes."
(Ask for digits 0–9 only, so you never convert to hex on stage.)

---

## §7 · Act 4 — THE FINALE, line by line (know this cold)

The program as loaded — read box 07 to the audience FIRST:

| box | word | meaning | plain words |
|-----|--------|----------------------|-------------|
| 00 | 0x1200 | LOAD R2, #0 | put the number 0 in a pocket — "just a number" |
| 01 | 0x9207 | STORE R2, [0x07] | write that 0 into box 07 ← THE SABOTAGE |
| 02 | 0x1003 | LOAD R0, #3 | countdown starts at 3 |
| 03 | 0x1101 | LOAD R1, #1 | the constant 1 |
| 04 | 0xB000 | OUT R0 | print the counter ← loop returns here |
| 05 | 0x3001 | SUB R0, R1 | counter--, sets Z |
| 06 | 0x7004 | JNZ 0x04 | not zero? back to 04 |
| 07 | 0x5000 | JMP 0x00 | go back to the start — FOREVER |

The story you tell:
1. "Box 07 says JMP to the start. This program is an infinite loop — it can NEVER stop.
   I'll bet anyone it stops anyway." (Take the bet. Make eye contact.)
2. Step. Instruction 2 (the STORE) fires and box 07 **glows yellow**. Point at its
   disassembly column: it said `JMP 0x00` — it now says `HALT`. "The program just wrote
   the number 0 into one of its own boxes. And 0, read through the instruction lens,
   means HALT. It has written its own ending. It just hasn't arrived there yet."
3. Countdown: 3… 2… 1… — get the room counting. The yellow box sits there the whole time.
4. Z flips, JNZ falls through, the finger lands on 07 — and executes the HALT the program
   wrote for itself. **The LED sticks on red: it stopped on "do".**
5. Why this is the proof: "In a machine where instructions and data lived in separate
   memories, this would be IMPOSSIBLE — the STORE could never reach the code. One shared
   memory is the whole Von Neumann idea, and you just watched its deepest consequence."

**The safety kicker (have it ready):** "Real chips forbid exactly this. Memory is marked
writable OR executable, never both — because a program that can rewrite instructions is
also how attackers turn injected *data* into running *code*. My toy allows what real
hardware spends silicon preventing. Same idea — they just learned to fear it."

---

## §8 · The close — why the Arduino moment is honest

- It's the **same ~500 lines of C**, compiled unchanged on the board's ARM chip. Nothing
  was ported; the machine doesn't care whose silicon it runs on.
- The three colors ARE the three phases: the emulator announces each phase (`F`/`D`/`E`)
  through one hook, and on the board that hook flips the LED — **blue grab, green
  understand, red do.** "The heartbeat you've been watching on screen is now physical."
- Final line, verbatim: "Real CPUs run this exact loop — pipelined, cached, protected.
  Everything I showed you is underneath every program you've ever run. An instruction
  is just a number. The lens decides."

---

## §9 · The interview — likely questions, crisp answers

**Walk me through fetch–decode–execute for one instruction.**
Use §3's 0x2001 example, naming state: IR gets the word, PC advances, decode splits bits
into opcode/rd/rs, execute changes R0 and maybe Z. Three phases, four possible things
mutated: registers, memory, PC, flags. That's the whole machine.

**Where is the Von Neumann model in your machine? What would Harvard change?**
One array holds code and data; the PC and a STORE can touch the same boxes. Harvard =
two separate memories; my Act 4 program becomes impossible there, and that impossibility
is the cleanest way to see the difference.

**Why can your program modify itself? Why do real CPUs resist it?**
Because STORE writes to the same memory the PC executes from. Real chips: W^X page
permissions (a page is writable or executable, not both) because self-modification is
the mechanism of code-injection attacks; also modern cores are Harvard *at the cache
level* — separate instruction and data caches — so code that edits itself needs explicit
cache synchronization. Von Neumann memory, Harvard caches: both, where each helps.

**What distinguishes an instruction from data in memory?**
Physically nothing. The only difference is whether the PC arrives. My UI proves it by
disassembling EVERY word — even your poked number, even the countdown constants.

**Why these registers / instructions? Minimum needed?**
Four registers: enough for real programs (accumulator, counter, constant, spare), few
enough to display. Twelve ops, but the real core is tiny: LOAD, ADD/SUB (which set Z),
a conditional jump, STORE, HALT. Compute + decide + touch memory — that's the minimum
for interesting programs; the rest is convenience.

**How does subtraction work? Two's complement?**
`uint16_t` arithmetic wraps mod 2¹⁶, which IS two's complement: subtracting x equals
adding its complement, so add and subtract are the same hardware. The UI shows big
values through both the unsigned and signed lens — same bits again.

**What sets the flags, and how does a branch use them?**
ADD/SUB/CMP set Z from the result; CMP is SUB with the result discarded — precisely
x86's `cmp`. JZ/JNZ replace the PC with the operand if Z says so. My `SUB`+`JNZ` loop
is x86's `dec`+`jnz` loop.

**Map an instruction to a real ISA.**
`LOAD R1,#3` ≈ x86 `mov ecx,3` / ARM `MOV W1,#3`. `JNZ 0x03` ≈ x86 `jnz label`.
Fixed-width one-word instructions, few ops, load/store style — my ISA leans RISC/ARM;
x86's variable-length CISC encoding is the main thing I simplified away.

**What did you abstract away? What would a pipeline take?**
No pipeline (each instruction fully finishes), no caches, no memory protection, no
interrupts/IO, no real timing or gates. A pipeline would fetch instruction N+1 while
decoding N — and then Act 4's self-modification becomes a hazard: the machine may have
already fetched the OLD instruction. That's exactly why real self-modifying code needs
cache flushes.

**Why is an emulator legitimate?**
It faithfully implements the *architecture* — the state machine a programmer sees
(registers, memory, cycle semantics). It doesn't model the *microarchitecture* —
electricity and timing. That's the same line the industry draws between an ISA manual
and a chip design, and it's deterministic, which is why the live demo is safe.

**Is this a simulation?**
It's an emulator, which is stronger. A simulation imitates behavior approximately —
a weather simulation doesn't rain. An emulator implements the actual rules completely,
so real programs genuinely run: the 256 words are a real array, decode really splits
bits, the answers are really computed instruction by instruction. Not modeled: gates,
voltages, timing, pipeline — the electrical layer. "Real the way chess on a screen is
real chess — every rule genuinely followed; I just didn't carve the pieces."

**What language / how big?**
Strict C99, ~500 lines, nothing beyond the standard library. Four files: cpu.c (the
machine), ui.c (terminal + platform), programs.c (hand-assembled demos), main.c (the
step loop and modes). Why C beyond it being the course language: fixed-width types
make the machine honest — `uint16_t` words mean real two's-complement wraparound, a
`uint8_t` PC wraps at 256 by itself like a real 8-bit address bus — and the identical
source compiles unchanged on Windows, Linux, and the board's ARM chip.

**How did you design the interface?**
The state display IS the deliverable (the plan: "this readout is your entire UI —
invest in it"). Three rules: (1) show the whole machine at once — registers, PC, IR,
flag, memory, output — no hidden state to take on faith; (2) highlight only what just
changed — cyan = PC's row, yellow = freshly written cell, green = the phase that ran;
(3) every memory word shown through both lenses at once — plain number AND disassembled
instruction — so the UI states the thesis permanently. In short: a debugger's
register-and-memory view, redesigned for an audience.

**Laptop run — is the UNO Q involved?**
No. Two independent machines, two copies of the same program: laptop source compiled
for x86-64 runs on the laptop's CPU; the board's copy was compiled by the board's own
gcc for its ARM chip and runs entirely there. The USB cable is only a window into the
board's terminal — no computation crosses it. That's the point of the close: same 500
lines, two silicons, same ending — the machine is the rules, not the hardware.

---

## §10 · Pocket glossary (last-minute cram)

- **Memory** — 256 boxes, 16 bits each; holds code AND data. The shelf.
- **PC** — finger pointing at the next box. Cyan row.
- **IR** — the line being read right now.
- **Registers R0–R3** — pockets. Work happens in pockets, not on the shelf.
- **Z flag** — sticky note: "last answer was zero?" The machine's only way to decide.
- **Fetch / decode / execute** — grab / understand / do. One heartbeat.
- **Instruction format** — `[opcode|pocket|value]`, one hex digit / one / two.
- **Assembler** — translator from words to numbers. You, in Act 3.
- **STORE** — the only reason Act 4 can happen. Writes a pocket into a box.
- **W^X** — real chips: writable or executable, never both. Why your toy is "forbidden".
- **0x2001 = 8193** — your always-loaded example of one number, two lenses.
