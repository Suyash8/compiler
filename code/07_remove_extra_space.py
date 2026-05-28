def normalize_source(source):
    result = []
    blank_line = False
    for line in source.splitlines():
        cleaned = " ".join(line.split())
        if cleaned == "":
            if blank_line:
                continue
            blank_line = True
            result.append("")
        else:
            blank_line = False
            result.append(cleaned)
    return "\n".join(result)

def main():
    source = """#include <stdio.h>


int    main( )   {

    printf(   \"Hello\"   );


    return   0;
}
"""
    print("Original Source:")
    print(source.rstrip())
    print()
    print("Cleaned Source:")
    print(normalize_source(source))

if __name__ == "__main__":
    main()