**Concept**: Count header includes in a C snippet
**Logic**:
- Store a sample C program inside the Python file
- Match #include lines using regex
- Count and print each include
**Sample Input**:
Embedded source used in the program:
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
Embedded Source:
#include <stdio.h>
#include "myheader.h"
#include <stdlib.h>

int main()
{
	return 0;
}

Header count: 3
#include <stdio.h>
#include "myheader.h"
#include <stdlib.h>
```
**Run**:
```bash
python3 code/05_count_headers.py
```
