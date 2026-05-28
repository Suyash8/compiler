import re

def main():
    source = """#include <stdio.h>

// this is a single line comment
int add(int a, int b) { return a + b; }

/*
   this is a
   multi-line comment
*/

int main()
{

    // blank line above
    return 0;
}
"""
    blank_lines = source.count("\n") + (0 if source == "" or source.endswith("\n") else 1)
    single_comments = len(re.findall(r"//.*", source))
    multi_comments = len(re.findall(r"/\*[\s\S]*?\*/", source))
    print("Embedded Source:")
    print(source.rstrip())
    print()
    print("Blank lines:", blank_lines)
    print("Single-line comments:", single_comments)
    print("Multi-line comments:", multi_comments)

if __name__ == "__main__":
    main()