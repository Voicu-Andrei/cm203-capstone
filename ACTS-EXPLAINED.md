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

**The scale of it** (for N = 12): the loop runs 12 laps — 41 instructions, 123 phases —
and finishes in a few seconds. You spent a minute and a half doing 3 laps by hand.
And this machine is still deliberately slowed so it's watchable: a real core does
**billions** of instructions per second. Same loop. Only faster.

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

Press `p`, slot `00`, value `1307`. The poke writes your number straight into box 00 —
which until that moment held the program's real first instruction. **You just replaced
a line of the program with a number you made up on stage.** The box glows yellow and
the bold line confirms: `POKE mem[0x00] <- 0x1307 (LOAD R3, #7) -- hand-assembled by a human`.

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
