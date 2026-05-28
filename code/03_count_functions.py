import re

def main():
    source = """#include <stdio.h>

void foo()
{
    int x = 0;
}

int bar(int a)
{
    return a * 2;
}

int main()
{
    foo();
    int r = bar(5);
    printf("%d\\n", r);
    return 0;
}
"""
    pattern = re.compile(r"\b[a-zA-Z_][a-zA-Z0-9_\s\*]*\b\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\([^;]*\)\s*\{")
    count = len(pattern.findall(source))
    print("Embedded Source:")
    print(source.rstrip())
    print()
    print("Functions:", count)

if __name__ == "__main__":
    main()