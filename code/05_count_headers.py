import re

def main():
    source = """#include <stdio.h>
#include "myheader.h"
#include <stdlib.h>

int main()
{
    return 0;
}
"""
    headers = re.findall(r"#\s*include\s*[<\"].+?[>\"]", source)
    print("Embedded Source:")
    print(source.rstrip())
    print()
    print("Header count:", len(headers))
    for header in headers:
        print(header)

if __name__ == "__main__":
    main()