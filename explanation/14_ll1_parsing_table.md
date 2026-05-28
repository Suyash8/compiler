**Concept**: Build an LL(1) parsing table from a grammar
**Logic**:
- Store the grammar inside the program
- Compute FIRST sets for all nonterminals
- Compute FOLLOW sets for all nonterminals
- Fill the LL(1) table using FIRST and FOLLOW
**Sample Input**:
Grammar used in the program:
```text
E -> T E'
E' -> + T E' | epsilon
T -> F T'
T' -> * F T' | epsilon
F -> ( E ) | id
```
**Sample Output**:
```
Grammar
E -> T E'
E' -> + T E'
E' -> epsilon
T -> F T'
T' -> * F T'
T' -> epsilon
F -> ( E )
F -> id

FIRST
E: ['(', 'id']
E': ['+', 'epsilon']
T: ['(', 'id']
T': ['*', 'epsilon']
F: ['(', 'id']

FOLLOW
E: ['$', ')']
E': ['$', ')']
T: ['$', ')', '+']
T': ['$', ')', '+']
F: ['$', ')', '*', '+']

LL(1) Parsing Table

NT | +             | *            | (          | )             | id        | $
E  |               |              | E -> T E'  |               | E -> T E' |
E' | E' -> + T E'  |              |            | E' -> epsilon |           | E' -> epsilon
T  |               |              | T -> F T'  |               | T -> F T' |
T' | T' -> epsilon | T' -> * F T' |            | T' -> epsilon |           | T' -> epsilon
F  |               |              | F -> ( E ) |               | F -> id   |
```
**Run**:
```bash
python3 code/14_ll1_parsing_table.py
```
