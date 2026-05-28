**Concept**: Operator precedence relation table
**Logic**:
- Store an operator grammar in the program
- Compute FIRSTVT and LASTVT sets
- Fill the precedence table using <, =, and > relations
**Sample Input**:
Grammar used in the program:
```text
E -> E + T | E - T | T
T -> T * F | T / F | F
F -> ( E ) | i
```
**Sample Output**:
```
Grammar
E -> E + T
E -> E - T
E -> T
T -> T * F
T -> T / F
T -> F
F -> ( E )
F -> i

FIRSTVT
E: ['(', '*', '+', '-', '/', 'i']
T: ['(', '*', '/', 'i']
F: ['(', 'i']

LASTVT
E: [')', '*', '+', '-', '/', 'i']
T: [')', '*', '/', 'i']
F: [')', 'i']

Operator Precedence Relation Table
```
**Run**:
```bash
python3 code/16_operator_relational_table.py
```
