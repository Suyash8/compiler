import re

def tokenize(expression):
    spec = [
        ("NUMBER", r"\d+(?:\.\d+)?"),
        ("PLUS", r"\+"),
        ("MINUS", r"-"),
        ("MUL", r"\*"),
        ("DIV", r"/"),
        ("LPAREN", r"\("),
        ("RPAREN", r"\)"),
        ("SKIP", r"[ \t\n]+"),
        ("MISMATCH", r"."),
    ]
    pattern = re.compile("|".join("(?P<%s>%s)" % pair for pair in spec))
    tokens = []
    for match in pattern.finditer(expression):
        kind = match.lastgroup
        value = match.group()
        if kind == "SKIP":
            continue
        if kind == "MISMATCH":
            tokens.append(("UNKNOWN", value))
        else:
            tokens.append((kind, value))
    return tokens

def precedence(operator):
    if operator in ("PLUS", "MINUS"):
        return 1
    if operator in ("MUL", "DIV"):
        return 2
    return 0

def apply_operator(operator, left, right):
    if operator == "PLUS":
        return left + right
    if operator == "MINUS":
        return left - right
    if operator == "MUL":
        return left * right
    if operator == "DIV":
        return left / right
    return 0

def evaluate(tokens):
    values = []
    operators = []
    for kind, value in tokens:
        if kind == "NUMBER":
            values.append(float(value))
        elif kind == "LPAREN":
            operators.append(kind)
        elif kind == "RPAREN":
            while operators and operators[-1] != "LPAREN":
                operator = operators.pop()
                right = values.pop()
                left = values.pop()
                values.append(apply_operator(operator, left, right))
            if operators and operators[-1] == "LPAREN":
                operators.pop()
        elif kind in ("PLUS", "MINUS", "MUL", "DIV"):
            while operators and operators[-1] != "LPAREN" and precedence(operators[-1]) >= precedence(kind):
                operator = operators.pop()
                right = values.pop()
                left = values.pop()
                values.append(apply_operator(operator, left, right))
            operators.append(kind)
    while operators:
        operator = operators.pop()
        right = values.pop()
        left = values.pop()
        values.append(apply_operator(operator, left, right))
    return values[0] if values else 0

def format_token(kind, value):
    names = {
        "NUMBER": "Number",
        "PLUS": "Plus",
        "MINUS": "Minus",
        "MUL": "Multiply",
        "DIV": "Divide",
        "LPAREN": "Left Parenthesis",
        "RPAREN": "Right Parenthesis",
        "UNKNOWN": "Unknown",
    }
    return names.get(kind, kind), value

def main():
    expression = "10 + 2 * (8 - 3) / 5"
    tokens = tokenize(expression)
    print("Expression:", expression)
    print("Tokens:")
    for kind, value in tokens:
        label, text = format_token(kind, value)
        print(f"{label:<18} {text}")
    print("Result:", evaluate(tokens))

if __name__ == "__main__":
    main()