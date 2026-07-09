# CM-203 Capstone — "Watch the Machine Think"

Build a working CPU **in software** — memory, registers, a program counter, an ALU, and your
own instruction set — then run it **one heartbeat at a time** so the audience watches fetch →
decode → execute happen live. The core idea the whole talk drives toward:

> **An instruction is just a number. Instructions and data share one memory. The lens decides.**
> (Von Neumann, Ch. 5 — made visible, then *proven* with self-modifying code.)

**Module:** CM-203 Computer Organization
**Primary ground:** Ch. 5 (architecture) — flagged by the brief as *unassessed, prime capstone ground*
**Topics connected (the vertical slice):**
Ch. 1–3 (C, memory model, structs/arrays) · Ch. 4 (bits, two's-complement, "same bits, different lens")
· Ch. 5 (Von Neumann, fetch–decode–execute, ALU, registers, PC, bus) · Ch. 6–10 (designing an ISA,
hand-assembly, x86/ARM64 parallels)
**Environment:** C on the laptop (WSL2 Ubuntu or native). Arduino for the physical "heartbeat" (stretch).

> Deadlines are tight (one-pager Jul 10, presentation Jul 14). This plan is *brave* — it includes
> every ambitious idea — but the phases are ordered so a **complete, strong capstone exists after
> Phase 3.** Everything past that is upside. Ship the spine first; add bravery on top.

---

## Guiding principles

- **The spine must always run.** After each phase the emulator still works end-to-end. Never leave
  it half-broken chasing a stretch feature. Commit working states often.
- **Explain every design choice.** The brief's harshest anti-pattern is *"we wrote 100 lines without
  being able to say why."* Every register, every instruction, every decode step must have a reason
  you can defend in the interview. If you can't justify it, cut it.
- **Design the ISA on purpose, mirroring real hardware.** Model a few instructions on real x86-64 /
  ARM64 ones you studied in Ch. 6–10, so you can say "mine works like the real thing — here's the parallel."
- **Deterministic = your friend.** Unlike measurement projects, this has no run-to-run noise. Use that:
  the same program always produces the same trace, so live demos are safe and repeatable.
- **Software before hardware.** A polished software-only version beats a half-wired breadboard every
  time. The Arduino is theatre on top of a finished machine, never the machine itself.

---

## The architecture you're building (lock this first)

A tiny but complete Von Neumann machine. Keep it small enough to hand-trace, real enough to be honest.

| Component | Suggestion | Why |
|-----------|-----------|-----|
| **Memory** | one array (e.g. 256 slots) holding **both** instructions and data | The whole point — unified memory is the Von Neumann idea |
| **Registers** | ~4 general (R0–R3) + flags (zero, carry/overflow) | Enough to write real programs; few enough to display |
| **Program Counter (PC)** | index of the next instruction | The thing the audience watches move |
| **Instruction Register (IR)** | holds the instruction currently being decoded | Makes "fetch then decode" a visible two-step |
| **ALU** | add, sub, compare; sets flags | Where Ch. 4 two's-complement math actually happens |
| **Instruction format** | fixed-width, e.g. `[opcode | reg | reg/immediate]` | Fixed width = trivial to fetch and hand-assemble |

**Instruction set — starter menu (~8–12 ops).** Pick a set that lets you write a *loop with a branch*,
because a loop is what makes the demo feel alive:

`LOAD` (immediate → reg) · `MOV` (reg → reg) · `ADD` · `SUB` · `CMP` (sets flags) ·
`JMP` (unconditional) · `JZ` / `JNZ` (branch on zero flag) · `STORE` / `LOADM` (reg ↔ memory) ·
`HALT` · (optional) `OUT` (print a register).

> Decision to make in Phase 1: opcodes as raw numbers you decode by hand, vs. a readable format.
> Raw numbers are *braver and better here* — they make "an instruction is just a number" literally true.

---

## Phase 0 — Environment & scaffolding

**Goal:** C toolchain ready, repo standing, an empty program that compiles and runs.

- [ ] C compiler available (gcc/clang) in WSL2 Ubuntu or native Linux.
- [ ] Start the repo — this doubles as your lab notebook and one-pager evidence.
- [ ] Decide the word/slot type (a fixed-width integer type so overflow/two's-complement behave honestly).
- [ ] A `main` that compiles, prints "machine online," exits. **Commit.**

**Verify:** it builds and runs. That's all Phase 0 owes you.

---

## Phase 1 — Memory, registers, and state you can *see*

**Goal:** the machine at rest, fully inspectable, before it can do anything.

- [ ] Define the state: memory array, registers, PC, IR, flags — ideally one `struct CPU`.
- [ ] Write a **`dump_state()`** that prints everything readably: registers, PC, flags, and a
      memory view (highlight the slot PC points at). **This function is your entire UI — invest in it.**
- [ ] Hand-load a few numbers into memory and show the dump.

**Verify / first aha:** you can see the whole machine's state in one clean readout. This readout is
what the audience will stare at for 8 minutes — make it legible now, not later.

---

## Phase 2 — The fetch–decode–execute loop *(make-or-break)*

**Goal:** the heartbeat. This is the core of the entire capstone.

- [ ] **Fetch:** read `memory[PC]` into IR; advance PC.
- [ ] **Decode:** split the instruction into opcode + operands.
- [ ] **Execute:** a switch over opcodes; each op mutates registers/memory/PC/flags.
- [ ] Implement the starter instruction set (Phase-0 menu above).
- [ ] **Single-step mode:** run exactly one fetch–decode–execute cycle, then `dump_state()` and wait
      for a keypress. This is the demo's whole mechanism — the audience presses "next" and watches.
- [ ] Also a **run mode** (loop until HALT) for showing the finished program fast.

**Verify / the aha that makes the capstone real:** hand-load a 3–4 instruction program (e.g. load 5,
load 3, add, halt) and **single-step it**, narrating each phase. If you can watch PC move, the
instruction light up, the ALU fire, and a register change — **you have a working CPU.** Everything
after is depth and drama.

---

## Phase 3 — A real program: the loop that feels alive

**Goal:** a recognizable computation the machine performs by itself, using a branch.

- [ ] Write **Demo Program A** in your instruction set — something with a loop:
      **sum 1..10**, or **countdown**, or **Fibonacci**. A loop means `CMP` + `JNZ` + a jump back —
      that's when the PC starts *moving backwards* and the machine visibly "decides."
- [ ] Single-step it live; watch the loop iterate, the counter tick, the branch taken then not taken.
- [ ] Then run-mode it to show the final answer instantly.

**Verify:** the machine computes a real answer on its own, with visible control flow.
✅ **STOP POINT — a complete, defensible capstone exists here.** Commit and tag it. Everything below is bravery.

---

## Phase 4 — The reveal: "an instruction is just a number"

**Goal:** the ten-second moment that ties Ch. 5 (Von Neumann) to Ch. 4 ("same bits, different lens").

- [ ] Pick a memory slot the machine is about to execute. Narrate: "the CPU is about to run this as an
      **instruction**."
- [ ] Step — it executes.
- [ ] Now print that **same slot as a plain number** (data). Show they're the identical bits.
- [ ] Land the line: *it was never "an instruction" or "a number" — it's bits; the lens decides.*

**Verify:** you can show one value wearing both hats. This is a top-scoring conceptual-understanding beat.

---

## Phase 5 — Hand-assembly: you *are* the assembler

**Goal:** connect Ch. 6–10 (assembly & the ISA) — prove the program is bits, not magic.

- [ ] Take **one line** of your assembly (e.g. `ADD R0, R1`) and, **live and by hand**, turn it into
      the raw instruction number using your instruction format. Type that number straight into memory.
- [ ] Run it — show your hand-encoded instruction executes correctly.
- [ ] Explain the format field-by-field (opcode bits, register bits) — this is Ch. 6–10 made literal.
- [ ] *(Optional, strong):* write a tiny **assembler** — a few lines of C/Python that turn a text program
      into memory bytes — so you can say "and here's the tool that does what I just did by hand."

**Verify:** you translated human-readable assembly to raw bits with your own hands and ran it.

---

## Phase 6 — The brave finale: **self-modifying code**

**Goal:** the deepest possible proof of the Von Neumann idea — a program that **rewrites its own
instructions while running.** Very few students will show this. It's the intellectual peak of the talk.

- [ ] Write **Demo Program B**: a program that, mid-run, uses `STORE` to overwrite an instruction
      *ahead of the PC* — so when the PC reaches that slot, it executes something **different** from
      what was originally loaded.
- [ ] Single-step it: show memory, show the original instruction, watch the program **change that byte**,
      then watch the PC arrive and execute the *new* instruction.
- [ ] Land it: *because instructions and data share one memory, a program can edit itself. This is only
      possible in a Von Neumann machine — and it's why the model is so powerful and so dangerous.*
- [ ] **Interview-ready caveat:** modern CPUs make this hard on purpose (writable-XOR-executable memory,
      caches). Explain *why* real machines resist what your toy allows — that contrast is gold.

**Verify:** an instruction in memory visibly changes before it runs, and execution follows the new value.
⚠️ Genuinely tricky to get clean — if it fights you, Phase 4 already delivers the concept. This is the
*brave* version; don't let it break the spine.

---

## Phase 7 — The physical heartbeat: Arduino *(the stretch that wows)*

**Goal:** lift the invisible cycle out of the terminal and into blinking hardware.

The laptop is still the CPU; the Arduino is a **live display** driven by it over the serial cable.

- [ ] **Phase LEDs (3):** the emulator sends "fetch / decode / execute" over serial; three LEDs light in
      turn each cycle. The abstract loop becomes a literal blinking heartbeat you can see across the room.
- [ ] **Register in binary (stretch²):** a row of LEDs shows one register's value in binary, updating as
      the program runs — Ch. 4 "numbers are bits" made physically visible.
- [ ] **Single-step button (stretch³):** a physical button on the Arduino advances one cycle — you press
      real hardware and the machine ticks. Maximum theatre.
- [ ] Keep a **pure-software fallback** one keystroke away, in case hardware misbehaves live.

**Verify:** stepping the emulator visibly drives the LEDs. **Rule:** only wire this once Phases 2–5 are
rehearsed and solid. A finished software CPU with no LEDs beats a blinking half-finished one.

---

## Phase 8 — ISA parallels & polish (depth for the interview)

**Goal:** connect your toy to the real ISAs from Ch. 6–10 so the interview sees breadth-with-depth.

- [ ] For 2–3 of your instructions, name the **real x86-64 / ARM64 counterpart** and note how yours mirrors
      or simplifies it (e.g. your `CMP`+`JZ` vs. real condition flags + conditional branch).
- [ ] One honest slide/sentence on what you **abstracted away**: no pipeline, no real gates, no timing,
      fixed-width simplicity. Knowing your model's limits is a strength, not a weakness.
- [ ] Optional: a one-line nod to CISC-vs-RISC — is your fixed-width, small set more RISC-like? (It is.)

---

## The 8-minute presentation spine

Hard timebox **8 min**, live > replay, no slide-orgy. Draft shape (tune after rehearsal):

| Time | Beat | On screen / stage |
|------|------|-------------------|
| 0:00–0:45 | Hook: "every computer runs one loop, billions of times a second, invisibly. Let's slow it to one." | the state dump at rest |
| 0:45–3:00 | **Single-step the loop** (Phase 2–3): PC moves, instruction lights, ALU fires, register changes | Demo Program A running live |
| 3:00–4:00 | **The reveal** (Phase 4): same bits as instruction, then as data | one value, both hats |
| 4:00–5:15 | **Hand-assembly** (Phase 5): translate one line to raw bits, run it | you, being the assembler |
| 5:15–7:00 | **Self-modifying code** (Phase 6): the program rewrites itself | memory changes before the PC arrives |
| 7:00–8:00 | **Arduino heartbeat** (Phase 7) + what real CPUs do differently | LEDs blinking the cycle |

**If the Arduino isn't ready:** drop Phase 7, give self-modifying code more room. Still a full talk.
**If over time:** cut hand-assembly to a 20-second mention; keep the loop, the reveal, and self-mod.
**Never cut:** the single-step loop (Phase 2–3). Without it, nothing else has meaning.

---

## Interview prep — questions to be ready for

Probes conceptual understanding + soundness. Have crisp, confident answers:

- Walk me through **fetch–decode–execute** for one instruction, naming what changes where.
- Where in your machine does the **Von Neumann model** show up? What would a Harvard architecture change?
- Why can your program **modify itself**? Why do real CPUs make that hard (W^X, caches, security)?
- An **instruction vs. data** — what physically distinguishes them in memory? (Nothing — the PC decides.)
- Why did you choose **these registers / these instructions**? What's the minimum to be Turing-complete-ish?
- How does your **ALU** do subtraction — and how does **two's-complement** make add/sub the same hardware?
- What sets your **flags**, and how does a **conditional branch** read them?
- Map one of your instructions to its **real x86-64 / ARM64** equivalent.
- What did you **abstract away**, and what would it take to add a **pipeline**?
- Why is an emulator a legitimate way to demonstrate this — what does it model faithfully, what not?

---

## Build order & time budget (5 days, brave but safe)

| By | Target | Fallback if behind |
|----|--------|--------------------|
| **Jul 10 (Fri)** | **One-pager submitted.** Phase 0–1 done (state + dump). | one-pager is non-negotiable; do it first |
| **Jul 11 (Sat)** | Phase 2–3: loop works, Demo A runs live. **← complete capstone exists** | if stuck, this is still a full project — stop here and polish |
| **Jul 12 (Sun)** | Phase 4–5: reveal + hand-assembly | drop hand-assembler tool, keep by-hand encoding |
| **Jul 13 (Mon)** | Phase 6: self-modifying code + rehearse | if it won't behave, cut to Phase 4 concept, rehearse instead |
| **Jul 14 (Tue AM)** | Phase 7 Arduino **only if solid**; final rehearsal to timebox | present software-only; Arduino is pure upside |

**The one rule that saves you:** after Jul 11 you already have a strong, defensible capstone. Every
day after adds bravery you can *drop without breaking the spine*. Build in that order and you can be
ambitious without risk.

---

## Open decisions to lock before coding

- [ ] Opcode encoding: raw numbers (braver, better for "instructions are data") vs. readable — **lean raw.**
- [ ] Demo Program A: sum 1..10 vs. countdown vs. Fibonacci — pick the clearest loop.
- [ ] Instruction width & memory size — small enough to hand-trace on screen.
- [ ] Is the Arduino in scope for *this* run, or added only if Phases 2–6 finish early?
- [ ] Assembler: hand-encoding only, or the little text→bytes tool too?
