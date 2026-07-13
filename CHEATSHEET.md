# CHEATSHEET — every command, in order, copy-paste ready

**T1** = the show terminal (laptop) · **T2** = the board's shell (**App Lab**, exactly how you
already connect — over the USB cable, so university Wi-Fi can't hurt it).
Rule: **T2 only types during the CLOSE (7:00).** Any other time it sits parked with its command pre-typed.

Board facts (confirmed): user `arduino` · hostname `uno-q-06` · ssh running · gcc + git on board.
The board needs internet **only tonight**, for the clone. In the room it needs only the USB cable.

> The heartbeat is the board's **user RGB LED**, driven straight from the machine:
> **FETCH = blue · DECODE = green · EXECUTE = red.** It's automatic — `./machine` on the board
> just has it, no flags. (The LED *matrix* belongs to the microcontroller side and is not used —
> don't promise pixels, promise the heartbeat.)

---
---

## TONIGHT (once, at home) — put the machine on the board

**In the App Lab shell:**

```
git clone https://github.com/Voicu-Andrei/cm203-capstone.git cm203
```

```
cd cm203
```

```
gcc -std=c99 -Wall -Wextra -O2 -o machine main.c cpu.c ui.c programs.c
```

```
./machine A --run
```

```
./machine B --run
```

**Expected, exactly** (deterministic — anything else means a stale clone):

- A → `output: 55`
- B → `output: 3 2 1`

**Then the heartbeat test:**

```
./machine B
```

Press `space` three times, slowly, watching the board: the user LED goes
**blue → green → red**. Press `q` (the LED goes dark on quit).

**LED never lit?** Permissions. Try once:

```
sudo ./machine B
```

If sudo fixes it, every `./machine` in this sheet becomes `sudo ./machine`. Write that on your hand.

---
---

## SETUP — before the audience settles

Windows Terminal **full screen**, dark theme, `Ctrl` + `+` until readable from the back.
Board plugged into the laptop by **USB**, App Lab shell open.

**T1 — the show:**

```
cd C:\Projects\cm203
```

```
.\machine
```

→ opens on **ACT 1/4 THE HEARTBEAT**, machine at rest, PC at 0x00. Touch nothing.

**T2 — App Lab shell, parked:**

```
cd ~/cm203
```

Now **type `./machine B` but DO NOT press Enter.** Switch back to T1.
T2 is done until 7:00.

---
---

## THE SHOW — T1 keypresses only, five keys total

`space` step · `r` run · `p` poke · `n` next act · `1`–`4` jump to act

| Time | Beat | Keys |
|------|------|------|
| 0:00 | Hook — Act 1 at rest | — |
| 0:45 | Heartbeat — step the 1+2+3 program, ends printing 6 | `space` ×~24 |
| 2:30 | Audience number — screen asks for it | `n` → type their number → `Enter` → `r` |
| 3:15 | The reveal — same bits, two lenses | — (stay on the finished screen) |
| 4:00 | Be the assembler — pocket 0–3, digit 0–9 | `n` → `p` → slot `00` → value `1<pocket>0<digit>` (e.g. `1307`) → `space` ×3 |
| 5:15 | Finale — the program that can never stop | `n` → `space`, slowly, to the HALT |
| 7:00 | **CLOSE — switch to T2** | — |

---
---

## THE CLOSE (7:00) — T2's moment

**1 ·** Switch to the App Lab shell — the prompt is already holding `./machine B`.

**2 ·** The line, holding the board up: *"This whole machine is five hundred lines of C.
It doesn't need my laptop — this board is a computer the size of a credit card, and
those three colors are it thinking: grab, understand, do."*

**3 ·** Press `Enter`, then step a few phases with `space` — **blue, green, red** on the board,
in time with the screen. Then:

```
r
```

→ the doomed program runs its countdown, heart blinking, and writes its own ending — **on the Arduino.
The LED sticks on red: it halted on "do", and that was the last thing it ever did.**

**4 ·** Land it: *"Real CPUs run this exact loop — pipelined, cached, protected.
An instruction is just a number. The lens decides."*

---
---

## PANIC BUTTONS

**Any act misbehaves (T1)** → press that act's digit (`1`–`4`): fresh machine, same act, under a second,
deterministic — it cannot behave differently on the rerun.

**Wrong number typed at the Act 2 prompt** → `Backspace` works; or press `2` to redo the act.

**T1 dies / laptop hiccup** → new tab:

```
cd C:\Projects\cm203
```

```
.\machine
```

then press the digit of the act you were in. (~3 s total.)

**App Lab won't connect in the room** → try ssh instead — same network needed, so use your
**phone's hotspot** for laptop + board (university Wi-Fi blocks device-to-device):

```
ssh arduino@uno-q-06.local
```

```
cd ~/cm203 && ./machine B
```

then park again (don't press `r` until the close).

**Board completely dead at 7:00** → skip it and say the close on T1 — the plan's rule:
*software-only is a full talk.* The LED was theatre; the machine is the capstone.
