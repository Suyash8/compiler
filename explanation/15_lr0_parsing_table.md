**Concept**: LR(0) parsing table and item sets overview
**Logic**:
- Store the grammar inside the program
- Build closure and goto for each state
- Print canonical items, Shift/GOTO transitions, and ACTION/GOTO table
**Sample Input**:
Grammar used in the program:
```text
S' -> S
S -> C C
C -> c C
C -> d
```
**Sample Output**:
```
Grammar
0. S' -> S
1. S -> C C
2. C -> c C
3. C -> d

Canonical Collection of LR(0) Items

I0:
C -> · c C
C -> · d
S -> · C C
S' -> · S

Transitions
GOTO on C: I0 -> I1
GOTO on S: I0 -> I2
Shift on c: I0 -> I3
Shift on d: I0 -> I4

LR(0) Parsing Table

State   | c        | d        | $        | C       | S
0       | Shift 3  | Shift 4  |          | 1       | 2
2       |          |          | Accept   |         |
4       | Reduce 3 | Reduce 3 | Reduce 3 |         |
```
**Run**:
```bash
python3 code/15_lr0_parsing_table.py
```
