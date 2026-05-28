**Concept**: Build an LL(1) parsing table from grammar, FIRST and FOLLOW
**Logic**:
- Compute FIRST(alpha) for each production
- Fill table for terminals in FIRST(alpha)
- If epsilon appears, fill table for FOLLOW(A)
**Sample Input**:
Grammar with E, E', T, T', F
**Sample Output**:
```
LL(1) parsing table:
E {'id': ['T', "E'"]}
E' {'+': ['+', 'T', "E'"], 'epsilon': ['epsilon']}
T {'id': ['F', "T'"]}
T' {'*': ['*', 'F', "T'"], 'epsilon': ['epsilon']}
F {'(': ['(', 'E', ')'], 'id': ['id']}
```
**Run**:
```bash
python3 code/14_ll1_parsing_table.py
```
