**Concept**: Tokenize C-like source into identifiers, numbers, operators, symbols, and strings
**Logic**:
- Build a combined regex with named groups
- Iterate matches and skip whitespace
**Sample Input**:
File: code/sample_inputs/01_sample_input.c
**Sample Output**:
```
Input file: code/sample_inputs/01_sample_input.c
Tokens:
ID include
OP <
ID stdio
ID h
OP >
ID int
ID add
SYM (
ID int
ID a
SYM ,
ID int
ID b
SYM )
SYM {
ID return
ID a
OP +
ID b
SYM ;
SYM }
ID int
ID main
SYM (
SYM )
SYM {
ID printf
SYM (
STR "Line one\n"
SYM )
SYM ;
ID printf
SYM (
STR "Line two\n"
SYM )
SYM ;
ID int
ID x
OP =
ID add
SYM (
NUM 2
SYM ,
NUM 3
SYM )
SYM ;
ID printf
SYM (
STR "x=%d\n"
SYM ,
ID x
SYM )
SYM ;
ID return
NUM 0
SYM ;
SYM }
```
**Run**:
```bash
python3 code/10_lexical_analyzer.py code/sample_inputs/01_sample_input.c
```
