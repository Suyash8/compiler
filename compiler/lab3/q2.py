import re

code = """
int myVariable = 10;
float computeValue(int x) {
    return x * 2.5;
}
"""

# List of C/C++ keywords
cpp_keywords = {
    "int", "float", "double", "char", "void", "return", "if", "else", "while", "for",
    "do", "switch", "case", "default", "break", "continue", "typedef", "struct",
    "class", "namespace", "const", "static", "public", "private", "protected",
    "virtual", "new", "delete", "operator", "template", "friend", "inline", "try",
    "catch", "throw", "using", "long", "short", "signed", "unsigned", "bool", "enum",
    "this", "goto", "sizeof"
}

# Match all identifiers
pattern_identifiers = re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_]*\b')

# Find all identifiers
all_identifiers = pattern_identifiers.findall(code)

# Filter out keywords
variable_names = [
    identifier for identifier in all_identifiers
    if identifier not in cpp_keywords
]

print("Matched Variables:", variable_names)
