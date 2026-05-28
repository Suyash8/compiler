def main():
    source = """#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int main()
{
    printf("Line one\\n");
    printf("Line two\\n");
    int x = add(2, 3);
    printf("x=%d\\n", x);
    return 0;
}
"""
    lines = source.count("\n") + (0 if source == "" or source.endswith("\n") else 1)
    words = len(source.split())
    characters = len(source)
    print("Embedded Source:")
    print(source.rstrip())
    print()
    print("Lines:", lines)
    print("Words:", words)
    print("Characters:", characters)

if __name__ == "__main__":
    main()