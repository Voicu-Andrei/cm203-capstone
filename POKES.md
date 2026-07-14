# POKES — ready-made experiments to run

**How to practice:** run `.\machine`, press `3` (fresh Act 3 machine, finger on box 00).
Press `3` again anytime to wipe and restart. Poke does not move the finger — you can
poke two boxes first, then step through both.

Reminder: `p` → slot → `Enter` → value → `Enter`. Three `space` = one instruction runs.

---

## STAGE-SAFE (digits only — these are show material)

**1 · The classic** — put a number in a pocket:

| Poke | Then | You'll see |
|------|------|-----------|
| slot `00` value `1307` | `space` ×3 | R3 = 7 |

Recipe: `1`, pocket, `0`, digit. Any combo works: `1005` → R0=5 · `1209` → R2=9 · `1108` → R1=8.

**2 · The doubler** — two hand-made instructions, chained:

| Poke | Poke | Then | You'll see |
|------|------|------|-----------|
| slot `00` value `1305` | slot `01` value `2303` | `space` ×6 | R3 = 5 … then R3 = 10 |

`2303` = ADD R3, R3 — "add the pocket to itself." You hand-wrote *math*: a 5 became a 10.
(Encore-of-the-encore material if the room is hot: "now watch my two instructions run back to back.")

---

## PLAYGROUND (practice at home — letters or drama involved)

**3 · Make it print** — your number lands in OUTPUT:

| Poke | Poke | Then | You'll see |
|------|------|------|-----------|
| slot `00` value `1307` | slot `01` value `b300` | `space` ×6 | OUTPUT: 7 |

`b300` = OUT R3 (opcode B is the printer). First hand-made program with visible *output*.

**4 · Write your own ending** — the finale's trick, by hand:

| Poke | Poke | Then | You'll see |
|------|------|------|-----------|
| slot `00` value `1307` | slot `01` value `0000` | `space` ×6 | red HALTED banner |

`0000` = HALT. You just did manually what the Act 4 program does to itself.

**5 · Infinite loop, two instructions** — ⚠ step it, NEVER press `r` on this one:

| Poke | Poke | Then | You'll see |
|------|------|------|-----------|
| slot `00` value `1301` | slot `01` value `5000` | `space`, keep going | 00 → 01 → 00 → 01 … forever |

`5000` = JMP 00. The finger bounces between two boxes until the end of time.
Escape: press `3` (fresh machine). (`r` here would animate ~100,000 instructions — hours.)

**6 · The fault** — the machine refuses rather than guesses:

| Poke | Then | You'll see |
|------|------|-----------|
| slot `00` value `f000` | `space` ×3 | "illegal opcode F — fault, machine halts" |

Opcode F isn't wired to anything. Good Q&A ammo: *"unknown numbers don't do random
things — the machine faults and stops. Real CPUs raise an exception the same way."*

**7 · Two lenses, no execution** — a number you never run still wears both hats:

| Poke | Then | You'll see |
|------|------|-----------|
| slot `10` value `2001` | nothing — just look | that row reads `8193` AND `ADD R0, R1` |

The finger never goes near box 10, yet the disassembly column decodes it anyway.
Proof that the "instruction-ness" isn't in the number — it's in whether the finger arrives.

---

## Why these are safe to memorize

Every poke above is deterministic: same poke, same result, every time. If any
experiment leaves a mess, `3` erases the world in under a second.
