**Concept**: Simple calculator using lexical analysis and expression evaluation
**Logic**:
- Store the sample expression inside the program
- Tokenize numbers, operators, and parentheses
- Evaluate the expression using operator precedence and two stacks
**Sample Input**:
Expression used in the program:
```text
10 + 2 * (8 - 3) / 5
```
**Sample Output**:
```
Expression: 10 + 2 * (8 - 3) / 5
Tokens:
Number             10
Plus               +
Number             2
Multiply           *
Left Parenthesis   (
Number             8
Minus              -
Number             3
Right Parenthesis  )
Divide             /
Number             5
Result: 12.0
```
**Run**:
```bash
python3 code/09_lex_calculator.py
```
