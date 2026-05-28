**Concept**: Remove extra spaces and extra blank lines from source code
**Logic**:
- Store a sample C source string inside the program
- Remove repeated spaces from each line
- Keep only one blank line when multiple blank lines appear together
**Sample Input**:
Embedded source used in the program:
```c
#include <stdio.h>


int    main( )   {

	printf(   "Hello"   );


	return   0;
}
```
**Sample Output**:
```
Original Source:
#include <stdio.h>


int    main( )   {

	printf(   "Hello"   );


	return   0;
}

Cleaned Source:
#include <stdio.h>

int main( ) {

printf( "Hello" );

return 0;
}
```
**Run**:
```bash
python3 code/07_remove_extra_space.py
```
