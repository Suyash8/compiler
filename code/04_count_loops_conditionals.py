import re

def main():
    source = """#include <stdio.h>

int main()
{
    int i;
    for (i = 0; i < 3; i++)
    {
    }
    i = 0;
    while (i < 2)
    {
        i++;
    }
    int j = 0;
    do
    {
        j++;
    } while (j < 1);

    if (i > 0)
    {
    }
    else if (i == 0)
    {
    }
    else
    {
    }

    switch (i)
    {
    case 0:
        break;
    default:
        break;
    }

    return 0;
}
"""
    loops = len(re.findall(r"\bfor\b|\bwhile\b|\bdo\b", source))
    conditionals = len(re.findall(r"\bif\b|\belse if\b|\bswitch\b", source))
    print("Embedded Source:")
    print(source.rstrip())
    print()
    print("Loops:", loops)
    print("Conditionals:", conditionals)

if __name__ == "__main__":
    main()