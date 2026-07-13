# CHEATSHEET — press-by-press, no thinking required

**T1** = the show terminal (laptop) · **T2** = the board's shell (App Lab, over USB).
**T2 does nothing until the CLOSE at 7:00.** It sits parked with its command pre-typed.

---
---

## WHAT THE FIVE KEYS ACTUALLY DO

| Key | Name | In plain words |
|-----|------|----------------|
| `space` | **step** | The machine does ONE phase and stops: grab, *press*, understand, *press*, do. **Three presses = one full instruction.** You control the heartbeat. |
| `r` | **run** | The machine goes by itself, animated, until it halts. You watch, hands off. |
| `p` | **poke** | Type a number straight into a memory box. It asks which box, then what number. This is the Act 3 magic trick. |
| `n` | **next act** | Throws away the current machine, loads the next act's program, fresh and at rest. Press it between beats, never during one. |
| `1` `2` `3` `4` | **jump** | Load THAT act, fresh. Redo an act, skip ahead, or fix a mess — this is the panic button. |

(`q` quits the whole program — you should never need it during the show.)

## HOW TO READ THE SCREEN (what to point at)

- **Cyan row** = the box the PC (the "finger") points at — the next thing to run.
- **Yellow row** = a box that was just REWRITTEN. In the finale it stays lit — point at it.
- **Green box in the cycle line** = which phase just happened (FETCH / DECODE / EXECUTE).
- **Bold line under the cycle** = plain-English of what just happened. If lost, read it aloud — it IS the narration.
- **Right memory column** = the box read as an instruction · **middle column** = same box as a plain number. Same bits, two lenses — that's the thesis, permanently on screen.
- **OUTPUT** = everything the machine has printed.
- **Red banner** = HALTED. Acts end here.

---
---

## TONIGHT (✔ done 07-13 — redo only if the board gets wiped)

App Lab shell: `git clone https://github.com/Voicu-Andrei/cm203-capstone.git cm203`,
`cd cm203`, `gcc -std=c99 -Wall -Wextra -O2 -o machine main.c cpu.c ui.c programs.c`,
then `./machine A --run` → `output: 55` and `./machine B --run` → `output: 3 2 1`.
Heartbeat check: `./machine B`, three slow `space` → LED blue → green → red, `q`.

---
---

## SETUP — before the audience settles

Windows Terminal **full screen**, dark theme, `Ctrl` `+` until readable from the back.
Board connected by **USB**, App Lab shell open.

**T1:**

```
cd C:\Projects\cm203
```

```
.\machine
```

→ Screen shows **ACT 1/4 THE HEARTBEAT**, "Machine online", PC at 0x00. Touch nothing.

**T2 (App Lab shell):**

```
cd ~/cm203
```

Type `./machine B` — **do NOT press Enter.** Leave it. Back to T1.

---
---

## ACT 1 — THE HEARTBEAT (0:00 – 2:30)

1. **Press nothing.** Say the hook over the resting screen: *"Every computer runs one tiny
   routine billions of times a second — grab a number, figure out what it says, do it.
   I built a computer where you can watch."*
2. Press `space` → FETCH lights green. Say **"grab"** — point: box 00 went into the reader, finger moved to 01.
3. Press `space` → DECODE. Say **"understand"** — the number split into "put 0 in pocket 0".
4. Press `space` → EXECUTE. Say **"do"** — pocket R0 just became 0. *"That's the whole
   secret. Everything else today is this, faster."*
5. Keep pressing `space` at talking pace. The cyan row walks down the boxes.
6. **The payoff — watch for the cyan row sitting on box 05** (`JNZ 0x03`, the jump).
   The next three presses are that jump's grab-understand-do, and on the **"do"** the
   cyan row LEAPS from 05 back up to 03. STOP. Point.
   *"Nobody told it to go back. It looked at a result and DECIDED."*
   *(If you like counting instead: it's your 18th space press — 3 presses per instruction, 6th instruction.)*
7. Keep stepping — the leap happens twice more, once per loop pass. Then OUTPUT
   shows **6**, and three presses later: red HALTED banner. Act done. ~42 presses total.
   *(Running late? At any point press `r` and it finishes itself, animated.)*

## ACT 2 — YOU vs THE MACHINE (2:30 – 4:00)

1. Ask the room: **"Someone give me a number under 20."**
2. Press `n` → the bottom of the screen literally asks: `a number under 20, please:`
3. **Type their number** (e.g. `12`), press `Enter` → fresh machine, their number loaded.
4. Press `r` → hands off. The machine loops by itself and halts in a few seconds.
   OUTPUT shows the answer. *"That was every grab-understand-do, honestly performed.
   Real chips are another billion times faster."*
5. **THE REVEAL — press nothing.** Point at the row for box 03: right column says
   `ADD R0, R1`, middle column says `8193`. *"I converted nothing. One thing in memory,
   two ways to read it. Remember that for the finale."*

## ACT 3 — BE THE ASSEMBLER (4:00 – 5:15)

1. Ask: **"Give me a pocket, 0 to 3, and a digit, 0 to 9."** (Digits only — never hex, live.)
2. Press `n` → fresh machine.
3. Press `p` → bottom of screen asks: `poke slot (hex):`
4. Type `00` and press `Enter`.
5. It asks: `value (hex):` — build the number OUT LOUD: *"put-a-number is opcode **1**,
   pocket **3**, value **07** → 1307"*. Type it (e.g. `1307`), press `Enter`.
6. Box 00 **glows yellow** and the bold line reads `POKE mem[0x00] <- 0x1307 (LOAD R3, #7)
   -- hand-assembled by a human`. Read it out.
7. Press `space` three times — grab, understand, do — pocket **R3 now holds their 7**.
   *"I just wrote a program by hand, as a number. That's all an assembler is: a translator."*

## ACT 4 — THE FINALE (5:15 – 7:00)

1. Press `n` → the finale loads. **Don't step yet.**
2. Point at box **07**: `JMP 0x00`. *"The last line says GO BACK TO THE START. This
   program can never stop. I'll bet anyone here it stops anyway."* Take the bet.
3. Press `space` **six times, slowly** — that's the first two instructions, grab-understand-do
   twice. On the sixth press (the STORE's "do"), box **07 glows yellow** and its right
   column flips from `JMP 0x00` to `HALT`. Point:
   *"The program just wrote the number 0 into one of its own boxes. Zero, read through
   the instruction lens, means HALT. It wrote its own ending — it just hasn't arrived yet."*
4. Keep a steady `space` rhythm through the countdown — OUTPUT grows **3… 2… 1** —
   get the room counting each one. The yellow box just sits there, waiting.
5. The cyan row finally lands on box 07 → it executes the HALT the program wrote →
   **red banner.** *"Instructions and data share one memory. That's the whole idea —
   and it's exactly what real chips forbid, for safety."*
   *(~36 presses of rhythm; pressing `r` finishes it in a second if time is dying.)*

## THE CLOSE (7:00) — T2's moment

1. Switch to the App Lab window — `./machine B` is already typed there.
2. Hold up / point at the board: *"Five hundred lines of C. It doesn't need my laptop —
   this is a computer the size of a credit card."*
3. Press `Enter`. Then press `space` **three times, slowly** — the board's LED goes
   **blue… green… red** in time with the screen. *"Grab. Understand. Do. That light IS
   the machine thinking."*
4. Press `r` → the doomed program counts down on the Arduino, heart flickering, and
   halts itself. **The LED sticks on red.** *"It stopped on 'do' — the last thing it ever did."*
5. Final line: *"Real CPUs run this exact loop — pipelined, cached, protected.
   An instruction is just a number. The lens decides."*

---
---

## PANIC BUTTONS

**Anything weird in any act** → press that act's digit (`1`–`4`): same act, fresh machine,
under a second. Deterministic — a rerun CANNOT behave differently.

**Typed the wrong number at the Act 2 prompt** → `Backspace` works; or press `2` and redo.

**Poke asked for a slot and you froze** → press `q` at the prompt (cancels the poke only),
then press `p` and start the poke again. Or press `3` for a whole fresh Act 3.

**T1 died / laptop hiccup** → new tab: `cd C:\Projects\cm203` then `.\machine`,
then press the digit of the act you were in. ~3 seconds.

**App Lab won't connect in the room** → phone-hotspot both devices, then in any terminal:
`ssh arduino@uno-q-06.local`, then `cd ~/cm203 && ./machine B`, park it. (University
Wi-Fi blocks device-to-device — hotspot, not eduroam.)

**Board completely dead at 7:00** → skip it. Say the close over T1's halted finale —
*software-only is a full talk.* The LED was theatre; the machine is the capstone.
