**Concept**: Lex-style calculator for arithmetic expressions
**Logic**:
- Tokenize numbers, operators, and parentheses
- Evaluate using operator precedence with two stacks
**Sample Input**:
Expression: 1+2*(3-4)
**Sample Output**:
```
Input: 1+2*(3-4)
Tokens:
NUM 1
PLUS +
NUM 2
MUL *
LP (
NUM 3
MINUS -
NUM 4
RP )
Result: -1.0
```
**Run**:
```bash
python3 code/09_lex_calculator.py "1+2*(3-4)"
```
