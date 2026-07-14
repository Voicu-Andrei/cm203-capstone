# The acts, explained gently

The slow version, one act at a time — what is actually happening and why.
(The full-depth version with interview answers is `UNDERSTANDING.md`.)

---

## ACT 1 — adding 1+2+3, and what the pockets are for

Yes: the machine adds 1+2+3 and prints 6. (Honest detail: it adds them backwards —
3, then 2, then 1. Same answer; the why is below.)

**Why registers exist:** the machine cannot do math on memory boxes directly. The
calculator inside (the ALU) only works on the four pockets, R0–R3. So every program
has the same shape: put values into pockets, do math between pockets. The first three
instructions are just setup — filling pockets:

| Pocket | Starts as | Its job in plain words |
|--------|-----------|------------------------|
| **R0** | 0 | **The running total.** A notepad holding the sum so far. |
| **R1** | 3 | **The countdown.** How many laps are left — *and* the number being added each lap. |
| **R2** | 1 | **Just holds a 1.** SUB can only subtract pocket-from-pocket, so we park a 1 here to use every lap. |

**Two more things on the screen — the PC and the IR:**

- **PC (Program Counter) — the finger.** It holds one number: which box comes NEXT.
  It starts at 00, and the cyan row is wherever it points. Fetch moves it forward by
  one, automatically; only a jump can move it anywhere else.
- **IR (Instruction Register) — the hand holding the current line.** Fetch copies the
  finger's box into the IR; decode then reads the fields out of the IR, not out of
  memory. **PC says *where*, IR holds *what*.**

**Before the loop — the first nine presses (boxes 00 → 02):**

The machine wakes up empty: every pocket 0, note blank, finger on box 00. The first
three instructions are pure setup — filling the pockets, three presses each:

| Box | Instruction | After its "do" press |
|-----|-------------|----------------------|
| 00 | `LOAD R0, #0` | total pocket set to 0 |
| 01 | `LOAD R1, #3` | countdown pocket set to 3 |
| 02 | `LOAD R2, #1` | the spare 1 parked |

(Why load 0 into a pocket that's already 0? Because programs never trust leftovers —
a pocket could hold anything from whatever ran before. Real code always sets its own
starting values. Say that if anyone asks; it's the honest habit, not an accident.)

Nine presses in, the finger stands on box 03 — and only now does the loop begin.

Then the loop — three instructions the machine runs over and over:

- `ADD R0, R1` — add the countdown to the total
- `SUB R1, R2` — knock 1 off the countdown
- `JNZ 03` — not done yet? go back up

Lap by lap:

| Lap | ADD does | SUB does | Then |
|-----|----------|----------|------|
| 1 | R0 = 0 + **3** = 3 | R1 = 3 − 1 = 2 | jump back to 03 |
| 2 | R0 = 3 + **2** = 5 | R1 = 2 − 1 = 1 | jump back to 03 |
| 3 | R0 = 5 + **1** = 6 | R1 = 1 − 1 = 0 | **no jump — loop over** |

On screen: **R0 climbs 0 → 3 → 5 → 6** and **R1 ticks down 3 → 2 → 1 → 0**. When the
loop ends, `OUT R0` prints the 6 and `HALT` stops the machine.

> 🎤 First instruction, one word per press: **"Grab. Understand. Do."** …then:
> *"That's the whole secret. Everything else today is this, faster."*
>
> 🎤 The leap: **"Nobody told it to go back. It looked at a result and DECIDED."**
>
> 🎤 The halt: *"It never knew it was summing. It read one bit, and the answer fell out."*

**The jump, briefly:** normally the finger (PC) just slides down the recipe one line at
a time. A jump is an instruction whose "do" step **overwrites the PC** — write 03 into
it, and the next fetch grabs box 03. That's the whole trick. `JNZ 03` does it *only if
the note says the last result wasn't zero* — the note (the **Z flag**, `Z =` on screen)
is one bit the calculator leaves behind after every ADD/SUB: "did that come out to
zero, yes or no?" Laps 1–2: not zero → leap. Lap 3: zero → the jump does nothing and
the program flows on. That one bit is the machine's entire ability to decide.

**Why backwards (3+2+1)?** R1 does two jobs — lap counter AND the number being added.
Counting up would need an extra pocket. One register, two jobs: a design choice you
can defend.

---

## ACT 2 — same program, their number, full speed

**Nothing new happens in Act 2.** That's the point of it. Same eight boxes, same three
pockets, same loop. Exactly ONE thing changes: box 01 becomes `LOAD R1, #12` (or
whatever number the room gives you) instead of `#3`. The countdown just starts higher.

**What `r` (run) really is:** the machine still does grab-understand-do, three phases
per instruction, every single one honestly performed — you just stop pressing the
button between them. Single-stepping was the artificial part; run mode is what a CPU
actually is. Nothing is skipped, nothing is faked, it's the same heartbeat without you
holding its hand.

> 🎤 After they give the number: **"You just programmed it — your number is sitting in box 01 right now."**
>
> 🎤 Before pressing `r`: **"Same machine, same heartbeat — I just stopped holding its hand."**

**The scale of it** (for N = 12): the loop runs 12 laps — 41 instructions, 123 phases —
and finishes in a few seconds. You spent a minute and a half doing 3 laps by hand.
And this machine is still deliberately slowed so it's watchable: a real core does
**billions** of instructions per second. Same loop. Only faster.

> 🎤 After the halt: **"A real chip isn't doing something different. It's doing THIS — a billion times faster."**

**Party trick (optional):** the answer is always N×(N+1)÷2 — for 12 that's 78. Someone
with a phone can check the machine live.

**The reveal (stay on the finished screen):** point at box 03. The right column reads
`ADD R0, R1`. The middle column reads `8193`. Nothing was converted — those are the
SAME 16 bits shown through two lenses. Read as fields (`2|0|01`) they mean "ADD pocket 0
and pocket 1"; read as a plain number they're 8193. The machine never marks which
reading is "true" — a word is an instruction only because the finger arrives there.
Say: *"Keep that in mind for the finale."* — Act 4 is this sentence, weaponized.

**If you want the C parallel (one spoken sentence, no slides):** the whole loop is
`while (n != 0) { sum += n; n--; }` — and it maps line for line: `sum += n` is the ADD,
`n--` is the SUB, and `while (n != 0)` **is** the JNZ — "Jump if Not Zero" is the same
words. Every while-loop ever compiled becomes exactly this shape.

---

## ACT 3 — you are the assembler

**The claim this act proves:** a program is just numbers someone put in the boxes, and
an "assembler" is nothing but a translator from words to numbers. You do the
translator's job live, by hand, and the machine cannot tell the difference — because
there is no difference.

**The mechanics.** Press `n`: a fresh copy of the sum program loads, at rest, finger on
box 00. Ask the room for a **pocket (0–3)** and a **digit (0–9)**. Now build the
instruction out loud using the format — each field is a whole hex digit on purpose:

```
[ opcode : 1 digit ][ pocket : 1 digit ][ value : 2 digits ]
     1 = "put a number       3                07
          in a pocket"
                     →  the word 1307
```

**Boxes vs pockets — the three numbers of this act live in two worlds.** Memory boxes
(256 of them, `00`–`FF`, the big list on screen) are the shelf where the program lives.
Pockets (R0–R3, the REGISTERS line) are the four hands where math happens. They're
separate places — R3 has nothing to do with box 03. In this act: slot `00` is a BOX
(where your instruction goes), `1307` is the instruction itself (the number placed in
that box), and the `3` inside it names a POCKET (where the instruction, once run, puts
the 7). You write to the shelf; your instruction writes to a hand.

**1307 is an example, not a fixed number** — it changes with their answers. The recipe
is always: **1, their pocket, 0, their digit.** (Pocket 2, digit 9 → `1209`.)

Press `p`, slot `00`, value `1307`. The poke writes your number straight into box 00 —
which until that moment held the program's real first instruction. **You just replaced
a line of the program with a number you made up on stage.** The box glows yellow —
yellow always means "this box was just written to" — and the bold line confirms:
`POKE mem[0x00] <- 0x1307 (LOAD R3, #7) -- hand-assembled by a human`.

Nothing has RUN yet — poke only placed the number. The finger still rests on box 00,
now pointing at your number. That's why the three `space` presses come next: grab
(your number enters the IR), understand (it splits into 1|3|07 → LOAD R3, #7), do
(pocket R3 gets their digit). Then STOP — payoff done; `n` when ready.

> 🎤 While typing the poke: **"I didn't write code. I wrote a number."**
>
> 🎤 Pointing at the pocket: **"Tonight, I am the assembler."**
>
> 🎤 Walking away from it: *"A program is just numbers someone put in boxes."*

**Q&A ammo:** there is no assembler program in this project, on purpose — you are the
assembler. If someone asks how the demos were written, open `programs.c`: raw numbers
on the left, human comments on the right. The machine only ever sees the numbers.

---

## ACT 4 — the program that rewrites itself (T1 only)

**The bridge from Act 3:** in Act 3, a *human* wrote a number into a box and it became
an instruction. Act 4 asks: what if the *program* did that to itself? One new
instruction makes it possible — **STORE**, the mirror of LOAD: it copies a pocket INTO
a box. It's the only instruction that writes memory. Poke was you writing a box by
hand; STORE is the program holding the pen.

The program, box by box:

| Box | Instruction | Plain words |
|-----|-------------|-------------|
| 00 | `LOAD R2, #0` | put 0 in a pocket — "just a number" |
| 01 | `STORE R2, [07]` | copy that 0 into box 07 ← **the sabotage** |
| 02 | `LOAD R0, #3` | countdown starts at 3 |
| 03 | `LOAD R1, #1` | the spare 1 |
| 04 | `OUT R0` | print the counter ← loop top |
| 05 | `SUB R0, R1` | knock 1 off, writes the note |
| 06 | `JNZ 04` | not zero? back up to 04 |
| 07 | `JMP 00` | **go back to the start — forever** |

> 🎤 The bet: **"This program can never stop. I'll bet anyone here it stops anyway."**
>
> 🎤 Press 6, box 07 yellow: **"Same yellow as my poke — but look who's holding the pen now."**
>
> 🎤 Then: **"It wrote its own ending. It just hasn't arrived there yet."**
>
> 🎤 The red banner: **"You just watched the one trick real chips are built to forbid."**

**The story, in order:**

1. **Before touching anything**, read box 07 to the room: "the last line says GO BACK
   TO THE START. This program loops forever — it can never stop. I'll bet anyone it
   stops anyway." Take the bet.
2. **Six slow presses** — that's the first two instructions. On press 6 (the STORE's
   "do"), box 07 **glows yellow** — same yellow as your Act 3 poke, but the author is
   the program — and its right column flips from `JMP 0x00` to `HALT`. Why: the number
   0, read through the instruction lens, means HALT (opcode 0). Say: *"It wrote its
   own ending. It just hasn't arrived there yet."*
3. **Steady rhythm** through the countdown — same loop shape as Act 1 (OUT, SUB, JNZ).
   OUTPUT grows **3… 2… 1** — get the room counting. The yellow box just waits.
   (~36 presses; `r` finishes it in a second if time is dying.)
4. R0 hits zero, the note flips, JNZ falls through — the finger slides onto box 07
   and executes the HALT **the program wrote for itself**. Red banner.
5. Land it: *"Instructions and data share one memory — that's the whole Von Neumann
   idea, and you just watched its deepest consequence. In a machine with separate
   code and data memories, that STORE could never have reached the program. And real
   chips forbid exactly this — memory is writable OR executable, never both — because
   data becoming code is how attacks work. My toy allows what real silicon prevents."*

---

## THE CLOSE — the two terminals (7:00)

All of Acts 1–4 happened on **T1**. T2 (the App Lab shell) has been parked the whole
time with `./machine B` typed and never entered. Now, with T1 frozen on the finale's
red HALTED banner:

1. **Switch windows to T2.** Don't touch T1 again — the red banner is your backdrop.
2. **Say the line while pointing at the board:** *"Five hundred lines of C. It doesn't
   need my laptop — this is a computer the size of a credit card."*
3. **Press `Enter`** — the same doomed program the room just watched loads on the Arduino.
4. **Press `space` three times, slowly**, eyes on the board: the LED goes
   **blue… green… red**. *"Grab. Understand. Do. That light IS the machine thinking."*
5. **Press `r`** — the countdown runs on the board, heartbeat flickering, and the
   program halts itself again. **The LED sticks on red.** *"It stopped on 'do' —
   the last thing it ever did."*
6. **Final line:** *"Real CPUs run this exact loop — pipelined, cached, protected.
   An instruction is just a number. The lens decides."* Done. Leave both screens as
   they are for Q&A.

Why T2 runs the same program: the room already knows the story — so this time they
don't watch the screen, they watch the *hardware*. Same bits, same ending, physical
heartbeat. Nothing new to explain, everything new to feel.

> 🎤 The board: **"That light is the machine thinking."**
>
> 🎤 The stuck red LED: **"It stopped on 'do' — the last thing it ever did."**
>
> 🎤 Q&A, if asked "is it a simulation?": *"It's real the way chess on a screen is real
> chess — every rule genuinely followed. I just didn't carve the pieces out of wood."*

---

## 🎤 All the lines, in firing order (last-minute cram)

1. **Hook:** "Nobody ever sees it. I built a computer where you can."
2. **Act 1, first instruction:** "Grab. Understand. Do. That's the whole secret —
   everything else today is this, faster."
3. **Act 1, the leap:** "Nobody told it to go back. It looked at a result and DECIDED."
4. **Act 1, halt:** "It never knew it was summing. It read one bit, and the answer fell out."
5. **Act 2, their number in:** "You just programmed it — your number is in box 01 right now."
6. **Act 2, before `r`:** "Same machine, same heartbeat — I just stopped holding its hand."
7. **Act 2, after halt:** "A real chip isn't doing something different. It's doing THIS —
   a billion times faster."
8. **Act 2, the reveal:** "I converted nothing. One thing in memory, two ways to read it.
   Keep that in mind for the finale."
9. **Act 3, typing the poke:** "I didn't write code. I wrote a number."
10. **Act 3, the payoff:** "Tonight, I am the assembler."
11. **Act 4, the bet:** "This program can never stop. I'll bet anyone here it stops anyway."
12. **Act 4, the yellow box:** "Same yellow as my poke — but look who's holding the pen now."
13. **Act 4, waiting:** "It wrote its own ending. It just hasn't arrived there yet."
14. **Act 4, the banner:** "You just watched the one trick real chips are built to forbid."
15. **Close, the LED:** "That light is the machine thinking."
16. **Close, stuck on red:** "It stopped on 'do' — the last thing it ever did."
17. **The last words:** "Real CPUs run this exact loop — pipelined, cached, protected.
    An instruction is just a number. **The lens decides.**"

Then three presses of `space` — grab, understand, do — and **pocket R3 holds their 7**.
The machine fetched a human-typed number and ran it as an instruction, with the exact
same three phases it uses for "real" code.

**Why slot 00:** the finger starts there, so your hand-made instruction is the very
next thing the machine runs — three presses to payoff. After the payoff, stop; don't
keep stepping (the rest is just the sum program, which is not the point). Press `n`
for the finale when ready.

**Why digits 0–9 only:** values 10–15 would need hex letters (A–F). Never convert
number systems live on stage.

**The line to land:** *"I didn't write code. I wrote a NUMBER into a box, and because
the finger walked over it, it became an instruction. That's all any program is —
numbers someone put in the boxes. The tool that normally does this translation for
you is called an assembler. Tonight, I was the assembler."*
