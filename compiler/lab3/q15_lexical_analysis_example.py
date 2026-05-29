import re
cpp_code = """
#include <iostream>

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    int *ptr = arr;

    for (int i = 0; i < 5; i++) {
        std::cout << *(ptr + i) << std::endl;
    }

    return 0;
}
"""
base_patterns = {
    "Keywords": re.compile(r'\b(?:int|float|double|char|void|return|if|else|for|while|include|main|std|cout|cin|endl)\b'),
    "Variables": re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_]*\b'),
    "Numbers": re.compile(r'\b\d+(?:\.\d+)?\b'),
    "Operators": re.compile(r'(\+\+|--|==|!=|<=|>=|<<|>>|[+\-*/%=<>])'),
    "Braces": re.compile(r'[{}]'),
    "Parentheses": re.compile(r'[()]'),
    "Semicolons": re.compile(r';'),
    "Comments": re.compile(r'//.*'),
    "Include Statements": re.compile(r'#include\s*<[^>]+>'),
    "Literals": re.compile(r'"[^"]*"')
}

basic_tokens = {
    category: pattern.findall(cpp_code)
    for category, pattern in base_patterns.items()
}

for category, matches in basic_tokens.items():
    print(f"{category}: {matches}")

string_literals = base_patterns["Literals"].findall(cpp_code)

improved_tokens = {
    category: pattern.findall(cpp_code)
    for category, pattern in base_patterns.items()
}

# Remove keywords from variables
improved_tokens["Variables"] = [
    var for var in improved_tokens["Variables"]
    if var not in improved_tokens["Keywords"]
]

# Remove variables inside string literals
improved_tokens["Variables"] = [
    var for var in improved_tokens["Variables"]
    if not any(var in literal for literal in string_literals)
]

for category, matches in improved_tokens.items():
    print(f"{category}: {matches}")
advanced_patterns = base_patterns.copy()

advanced_patterns.update({
    "Pointers": re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_]*\s*\*\s*[a-zA-Z_][a-zA-Z0-9_]*'),
    "Arrays": re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\[[^\]]+\]')
})

advanced_tokens = {
    category: pattern.findall(cpp_code)
    for category, pattern in advanced_patterns.items()
}

# Clean variable list again
advanced_tokens["Variables"] = [
    var for var in advanced_tokens["Variables"]
    if var not in advanced_tokens["Keywords"]
]

advanced_tokens["Variables"] = [
    var for var in advanced_tokens["Variables"]
    if not any(var in literal for literal in string_literals)
]

for category, matches in advanced_tokens.items():
    print(f"{category}: {matches}")