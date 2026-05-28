import re

def main():
    source = """#include <stdio.h>

int a;
char c;
float f;
double d;
long l;
short s;

int main()
{
    int x;
    char y;
    return 0;
}
"""
    types = ["int", "char", "float", "double", "long", "short"]
    print("Embedded Source:")
    print(source.rstrip())
    print()
    for data_type in types:
        pattern = re.compile(r"\b" + data_type + r"\b[^;=\n]*;")
        matches = pattern.findall(source)
        print(f"{data_type}: {len(matches)}")

if __name__ == "__main__":
    main()