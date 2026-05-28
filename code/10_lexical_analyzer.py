import re
import sys
from pathlib import Path

def build_patterns():
    return [
        ("PREPROCESSOR", r"#[^\n]*"),
        ("BLOCK_COMMENT", r"/\*[^*]*\*+(?:[^/*][^*]*\*+)*/"),
        ("LINE_COMMENT", r"//.*"),
        ("STRING", r'"([^"\\]|\\.)*"'),
        ("NUMBER", r"\d+(?:\.\d+)?"),
        ("IDENTIFIER", r"[A-Za-z_][A-Za-z0-9_]*"),
        ("OPERATOR", r"==|!=|<=|>=|\+\+|--|&&|\|\||[+\-*/%=<>!]"),
        ("DELIMITER", r"[(){}\[\],;]"),
        ("SKIP", r"[ \t\r\n]+"),
        ("MISMATCH", r".")
    ]

def build_regex():
    return re.compile("|".join("(?P<%s>%s)" % pair for pair in build_patterns()))

def classify_identifier(value):
    keywords = {
        "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
        "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
        "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
        "union", "unsigned", "void", "volatile", "while", "main"
    }
    if value in keywords:
        return "KEYWORD"
    return "IDENTIFIER"

def scan(text):
    tokens = []
    regex = build_regex()
    for match in regex.finditer(text):
        kind = match.lastgroup
        value = match.group()
        if kind == "SKIP":
            continue
        if kind == "IDENTIFIER":
            kind = classify_identifier(value)
        elif kind == "PREPROCESSOR":
            kind = "PREPROCESSOR"
        elif kind in {"LINE_COMMENT", "BLOCK_COMMENT"}:
            kind = "COMMENT"
        elif kind == "MISMATCH":
            kind = "UNKNOWN"
        tokens.append((kind, value))
    return tokens

def main():
    default_path = Path(__file__).resolve().parent / "sample_inputs" / "01_sample_input.c"
    file_path = Path(sys.argv[1]) if len(sys.argv) > 1 else default_path
    try:
        text = file_path.read_text(encoding="utf-8", errors="ignore")
    except Exception:
        print("Error: unable to open file:", file_path)
        return
    tokens = scan(text)
    print("Input file:", file_path)
    print("Tokens:")
    for kind, value in tokens:
        print(f"{kind:<12} {value}")

if __name__ == "__main__":
    main()