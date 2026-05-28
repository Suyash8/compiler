**Concept**: Compute FIRST sets for a sample grammar
**Logic**:
- Iteratively compute FIRST until stable
- Handle epsilon and terminals
**Sample Input**:
Grammar with E, E', T, T', F
**Sample Output**:
```
FIRST sets:
E []
T ['(', 'id']
E' ['+', 'epsilon']
F ['(', 'id']
T' ['*', 'epsilon']
```
**Run**:
```bash
python3 code/13_first_follow.py
```
