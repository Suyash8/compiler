import re

class SymbolTable:
    """ A class to store tokens with their types and attributes. """

    def __init__(self):
        self.table = {}

    def insert(self, token, token_type):
        """ Insert token into the symbol table if not already present. """
        if token not in self.table:
            self.table[token] = {"Type": token_type}

    def display(self):
        """ Display the symbol table. """
        print("\n🔹 Symbol Table:")
        print("{:<20} {:<15}".format("Token", "Type"))
        print("-" * 35)
        for token, details in self.table.items():
            print("{:<20} {:<15}".format(token, details["Type"]))


class LexicalAnalyzer:

    def __init__(self, code):
        self.code = code
        self.symbol_table = SymbolTable()

        # Full list of C and C++ reserved keywords
        self.keywords = (
            "alignas|alignof|and|and_eq|asm|auto|bitand|bitor|bool|break|case|catch|char|char8_t|"
            "char16_t|char32_t|class|compl|concept|const|consteval|constexpr|constinit|continue|"
            "decltype|default|delete|do|double|dynamic_cast|else|enum|explicit|export|extern|"
            "false|float|for|friend|goto|if|inline|int|long|main|mutable|namespace|new|noexcept|"
            "not|not_eq|nullptr|operator|or|or_eq|private|protected|public|register|reinterpret_cast|"
            "requires|return|short|signed|sizeof|static|static_assert|struct|switch|template|"
            "this|thread_local|throw|true|try|typedef|typeid|typename|union|unsigned|using|"
            "virtual|void|volatile|wchar_t|while|xor|xor_eq"
        )

        # Full list of C and C++ operators
        self.operators = (
            r"\+\+|--|==|!=|<=|>=|<<|>>|&&|\|\||"
            r"\+=|-=|\*=|/=|%=|&=|\|=|\^=|"
            r"\+|-|\*|/|%|=|<|>|!|&|\||\^|~|\?|:|::|\.|"
            r"\[|\]"
        )

        # Define regex patterns for token types
        self.patterns = {
            "Keywords": re.compile(r'\b(?:' + self.keywords + r')\b'),
            "Literals": re.compile(r'"[^"]*"'),
            "Numbers": re.compile(r'\b\d+(?:\.\d+)?\b'),
            "Operators": re.compile(self.operators),
            "Braces": re.compile(r'[{}]'),
            "Parentheses": re.compile(r'[()]'),
            "Semicolons": re.compile(r';'),
            "Commas": re.compile(r','),
            "Comments": re.compile(r'//.*'),
            "Multi-line Comments": re.compile(r'/\*[\s\S]*?\*/'),
            "Include Statements": re.compile(r'#include\s*<[^>]+>'),
            "Pointers": re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_]*\s*\*\s*[a-zA-Z_][a-zA-Z0-9_]*'),
            "Arrays": re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\[[^\]]+\]'),
            "Classes": re.compile(r'\bclass\s+[a-zA-Z_][a-zA-Z0-9_]*'),
            "Objects": re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*;'),
            "Identifiers": re.compile(r'\b(?!' + self.keywords + r'\b)[a-zA-Z_][a-zA-Z0-9_]*\b')
        }

    def analyze(self):

        string_literals = self.patterns["Literals"].findall(self.code)

        tokens = {
            category: pattern.findall(self.code)
            for category, pattern in self.patterns.items()
        }

        tokens["Identifiers"] = [
            var for var in tokens["Identifiers"]
            if not any(var in lit for lit in string_literals)
        ]

        for token_type, token_list in tokens.items():
            for token in token_list:
                self.symbol_table.insert(token, token_type)

        return tokens

    def display_tokens(self):
        tokens = self.analyze()
        print("\n🔹 Token Classification:")
        for category, matches in tokens.items():
            print(f"{category}: {matches}")

        self.symbol_table.display()


# Example C++ Code
cpp_code = """
#include <iostream>
using namespace std;

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        // Last i elements are already in place
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    bubbleSort(arr, n);

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    return 0;
}
"""

lexer = LexicalAnalyzer(cpp_code)
lexer.display_tokens()
