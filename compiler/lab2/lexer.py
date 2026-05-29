"""A simple regex-based lexer demonstrating tokenization.

Token types: IDENTIFIER, NUMBER, STRING_LITERAL, OPERATOR, NEWLINE, SKIP (whitespace), UNKNOWN
"""
import re
from typing import List, Tuple


Token = Tuple[str, str]


token_specification = [
    ("STRING_LITERAL", r'"([^"\\]|\\.)*"'),
    ("NUMBER", r"\d+(?:\.\d+)?"),
    ("IDENTIFIER", r"[A-Za-z_]\w*"),
    ("OPERATOR", r"[+\-*/=<>!&|]+"),
    ("NEWLINE", r"\n"),
    ("SKIP", r"[ \t]+"),
    ("MISMATCH", r".")
]

master_regex = re.compile("|".join(f"(?P<{name}>{pattern})" for name, pattern in token_specification))


def lex(code: str) -> List[Token]:
    tokens: List[Token] = []
    for mo in master_regex.finditer(code):
        kind = mo.lastgroup
        value = mo.group()
        if kind is None:
            continue
        if kind == "NEWLINE":
            tokens.append(("NEWLINE", "\\n"))
        elif kind == "SKIP":
            continue
        elif kind == "MISMATCH":
            tokens.append(("UNKNOWN", value))
        else:
            tokens.append((kind, value))
    return tokens


def _demo():
    sample = 'int count = 42;\nstring s = "hello";\ncount = count + 1;'
    print("Code:\n", sample)
    print("\nTokens:")
    for t in lex(sample):
        print(t)


if __name__ == "__main__":
    _demo()
