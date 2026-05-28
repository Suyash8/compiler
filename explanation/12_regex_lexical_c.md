**Concept**: Use C regex to find identifiers in a source file
**Logic**:
- Read the full file into memory
- Use POSIX regex to match identifier pattern
- Print each identifier
**Sample Input**:
File: code/sample_inputs/01_sample_input.c
**Sample Output**:
```
Input file: code/sample_inputs/01_sample_input.c
Identifiers:
ID:include
ID:stdio
ID:h
ID:int
ID:add
ID:int
ID:a
ID:int
ID:b
ID:return
ID:a
ID:b
ID:int
ID:main
ID:printf
ID:Line
ID:one
ID:n
ID:printf
ID:Line
ID:two
ID:n
ID:int
ID:x
ID:add
ID:printf
ID:x
ID:d
ID:n
ID:x
ID:return
```
**Run**:
```bash
gcc -std=c99 -Wall code/12_regex_lexical_c.c -o 12_regex_lexical_c
./12_regex_lexical_c code/sample_inputs/01_sample_input.c
```
