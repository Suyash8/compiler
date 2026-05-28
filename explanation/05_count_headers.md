**Concept**: Count header includes in a C source file
**Logic**:
- Match #include lines using regex
- Count and print each include
**Sample Input**:
File: code/sample_inputs/05_sample_input.c
```c
#include <stdio.h>
#include "myheader.h"
#include <stdlib.h>

int main()
{
	return 0;
}
```
**Sample Output**:
```
Input file: /home/devniru2704/Personal Files/Programs/Github/cc/code/sample_inputs/05_sample_input.c
Header count: 3
#include <stdio.h>
#include "myheader.h"
#include <stdlib.h>
```
**Run**:
```bash
python3 code/05_count_headers.py
```
