# Presentation runsheet — 10:00 timebox (aim to land at 9:45)

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
| 0:45 | **Heartbeat** — step the whole 1+2+3 program | `space`, ~42 total | First TWO instructions at full slowness: "grab… understand… do." Speed up after. When the arrow leaps back up: "it just looked at a result and *decided* to repeat." One C sentence at the leap: "in C this is `while (n != 0) { sum += n; n--; }` — same three lines." Ends printing 6: "it never knew it was summing — it read one bit and the answer fell out." |
| 3:00 | **Audience number** — Act 2 | `n`, type their number, `Enter`, then `r` | "Someone give me a number under 20." (The machine literally asks on screen.) Runs in a few seconds. "You just programmed it. Same machine, same heartbeat — I just stopped holding its hand. Real chips are another billion times faster." |
| 3:45 | **The reveal** — stay on the finished screen | — | Point at one row: instruction on the right, plain number in the middle. "I converted nothing. One thing in memory, two ways to read it. Keep that in mind for the finale." Let it breathe — this is the hinge of the talk. |
| 4:45 | **Be the assembler** — Act 3 | `n`, then `p`, slot `00`, value `1<pocket>0<digit>`, `space` ×3 | "Give me a pocket 0–3 and a digit 0–9." Encode aloud: "put-a-number is 1, pocket 3, value 07 → 1307." Poke slot 00, it glows yellow, step ×3 → pocket shows their number. "I just wrote a program by hand, as a number." **Ask for digits 0–9 only.** **Encore if the room is warm:** second poke into slot `01` (finger is already there), 3 more steps. |
| 6:30 | **Finale** — Act 4 | `n`, then `space`, slowly | Show slot 0x07: "the last step says *go back to the start* — this program can never stop. I'll bet anyone it stops anyway." Step: the program writes 0 over slot 7 (glows yellow, stays lit — "look who's holding the pen now"). Countdown prints 3…2…1 — get the room counting, full stepping, no rush. The arrow arrives at the slot the program rewrote. **HALT.** "It wrote its own ending. Instructions and data share one memory — and it's exactly what real chips forbid for safety." |
| 8:45 | **Close** — T2, the board | `Enter`, `space` ×3, `r` | LED heartbeat: "blue grab, green understand, red do — that light is the machine thinking." Countdown halts itself, LED sticks red: "it stopped on 'do'." Then: "Real CPUs run this exact loop — pipelined, cached, protected. An instruction is just a number. The lens decides." Land by 9:45. |

## Safety rules (from the plan)

- **Never cut** the stepping demo (0:45–3:00) — nothing else has meaning without it.
- **Over time?** Cut in this order: the Act 3 encore poke, then the C sentence, then all of
  "Be the assembler" (press `4` to skip from Act 2 straight to the finale), then shrink the
  reveal to one sentence. The finale stays. In the finale, `r` collapses the countdown to a second.
- **Under time?** Perfect — ending at 9:30 beats rushing the close. Never pad; silence after
  "the lens decides" is stronger than an extra sentence.
- **Anything glitches?** Press the act's number (`1`–`4`). Fresh machine, same act, under a second — and the machine is deterministic: it cannot behave differently on the rerun. Never quit, never retype a command.
- **Typed a wrong number at the Act 2 prompt?** Backspace works; or press `2` afterwards to redo the act.
- Every beat asks the audience for something: their number, their pocket, their bet, their countdown. That participation is the entertainment — don't skip the asks to save time.
