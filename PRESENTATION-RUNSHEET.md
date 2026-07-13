# Presentation runsheet — 8:00 hard timebox

## One command, five keys

The whole talk is `.\machine` — no arguments, no tabs, no other commands.
It opens on Act 1 and the acts are the beats:

| Key | Does |
|-----|------|
| `space` | step one phase (fetch / decode / execute) |
| `r` | run to halt, animated |
| `p` | poke a word into memory (the hand-assembly bit) |
| `n` | **next act** — loads a fresh machine for the next beat |
| `1`–`4` | jump to any act, fresh machine — this is also the panic button |

## Before entering the room

- [ ] Windows Terminal, full screen, dark theme, font cranked (Ctrl and `+` until readable from the back).
- [ ] **One tab** in `C:\Projects\cm203`, running `.\machine` — at rest on Act 1, PC at 0x00. That is the entire setup.
- [ ] Phone/watch timer where you can see it.
- [ ] Arduino only if the matrix heartbeat was rehearsed. Software-only is a full talk.

## The beats

| Time | Beat | Keys | The line |
|------|------|------|----------|
| 0:00 | **Hook** — Act 1 on screen, machine at rest | — | "Every computer runs one tiny routine billions of times a second — grab a number from memory, figure out what it says, do it. Nobody ever sees it. I built a computer where you can." |
| 0:45 | **Heartbeat** — step the whole 1+2+3 program | `space` ×~24 | First instruction at full slowness: "grab… understand… do." Speed up after. When the arrow leaps back up: "it just looked at a result and *decided* to repeat." Ends printing 6. |
| 2:30 | **Audience number** — Act 2 | `n`, type their number, `Enter`, then `r` | "Someone give me a number under 20." (The machine literally asks on screen.) Answers in ~2 s. "That's me versus the machine — and real chips are another billion times faster." |
| 3:15 | **The reveal** — stay on the finished screen | — | Point at one row: instruction on the right, plain number in the middle. "I converted nothing. One thing in memory, two ways to read it. Keep that in mind for the finale." |
| 4:00 | **Be the assembler** — Act 3 | `n`, then `p`, slot `00`, value `1<pocket>0<digit>`, `space` ×3 | "Give me a pocket 0–3 and a digit 0–9." Encode aloud: "put-a-number is 1, pocket 3, value 07 → 1307." Poke slot 00, it glows yellow, step ×3 → pocket shows their number. "I just wrote a program by hand, as a number." **Ask for digits 0–9 only** (no live hex conversion). |
| 5:15 | **Finale** — Act 4 | `n`, then `space`, slowly | Show slot 0x07: "the last step says *go back to the start* — this program can never stop. I'll bet anyone it stops anyway." Step: the program writes 0 over slot 7 (glows yellow, stays lit). Countdown prints 3…2…1 — get the room counting. The arrow arrives at the slot the program rewrote. **HALT.** "It wrote its own ending. Instructions and data share one memory — that's the whole idea, and it's exactly what real chips forbid for safety." |
| 7:00 | **Close** | — | Arduino heartbeat if ready; otherwise: "Real CPUs run this exact loop — pipelined, cached, protected. Everything I showed you is underneath every program you've ever run. An instruction is just a number. The lens decides." |

## Safety rules (from the plan)

- **Never cut** the stepping demo (0:45–2:30) — nothing else has meaning without it.
- **Over time?** Cut "Be the assembler" first (press `4` to skip from Act 2 straight to the finale), then shrink the reveal to one sentence. The finale stays.
- **Anything glitches?** Press the act's number (`1`–`4`). Fresh machine, same act, under a second — and the machine is deterministic: it cannot behave differently on the rerun. Never quit, never retype a command.
- **Typed a wrong number at the Act 2 prompt?** Backspace works; or press `2` afterwards to redo the act.
- Every beat asks the audience for something: their number, their pocket, their bet, their countdown. That participation is the entertainment — don't skip the asks to save time.
