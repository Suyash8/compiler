from collections import defaultdict

grammars = [
    """S → T S'
S' → + T S' | ε
T → F T'
T' → * F T' | ε
F → ( S ) | id""",

    """S → a S b
S → ε""",

    """S → S S
S → ( S )
S → ε""",

    """S → a S
S → b S
S → ε""",

    """S → 0 S 1
S → ε""",

    """S → a S a
S → b S b
S → a
S → b
S → ε""",

    """S → A a
S → b
A → A c
A → S d
A → ε""",

    """S → a A
S → b B
A → a S
A → b A A
A → a
B → b S
B → a B B
B → b""",

    """S → T S'
S' → + T S' | ε
T → F T'
T' → * F T' | ε
F → ( S ) | id""",

    """S → i E t S
S → i E t S e S
S → a
E → b"""
]

EPSILON = "ε"

def parse_grammar(grammar_str):
    productions = defaultdict(list)
    for line in grammar_str.strip().split("\n"):
        left, right = line.split("→")
        left = left.strip()
        alternatives = right.split("|")
        for alt in alternatives:
            productions[left].append(alt.strip().split())
    return productions


def compute_first(productions):
    first = defaultdict(set)

    changed = True
    while changed:
        changed = False

        for head in productions:
            for prod in productions[head]:

                # Case: ε production
                if prod == [EPSILON]:
                    if EPSILON not in first[head]:
                        first[head].add(EPSILON)
                        changed = True
                    continue

                for symbol in prod:

                    # Terminal
                    if symbol not in productions:
                        if symbol not in first[head]:
                            first[head].add(symbol)
                            changed = True
                        break

                    # Non-terminal
                    before = len(first[head])
                    first[head].update(first[symbol] - {EPSILON})
                    if len(first[head]) > before:
                        changed = True

                    # If symbol can't produce ε → stop
                    if EPSILON not in first[symbol]:
                        break

                else:
                    # All symbols can produce ε
                    if EPSILON not in first[head]:
                        first[head].add(EPSILON)
                        changed = True

    return first

def compute_follow(productions, first, start_symbol):
    follow = defaultdict(set)
    follow[start_symbol].add("$")

    changed = True
    while changed:
        changed = False

        for head in productions:
            for prod in productions[head]:
                for i, symbol in enumerate(prod):
                    if symbol not in productions:
                        continue

                    # Get rest of production
                    rest = prod[i+1:]

                    if rest:
                        rest_first = set()
                        for sym in rest:
                            sym_first = first[sym] if sym in productions else {sym}
                            rest_first.update(sym_first - {EPSILON})
                            if EPSILON not in sym_first:
                                break
                        else:
                            rest_first.add(EPSILON)

                        before = len(follow[symbol])
                        follow[symbol].update(rest_first - {EPSILON})

                        if EPSILON in rest_first:
                            follow[symbol].update(follow[head])

                        if len(follow[symbol]) > before:
                            changed = True
                    else:
                        before = len(follow[symbol])
                        follow[symbol].update(follow[head])
                        if len(follow[symbol]) > before:
                            changed = True

    return follow


for grammar in grammars:
    productions = parse_grammar(grammar)

    first = compute_first(productions)
    follow = compute_follow(productions, first, start_symbol="S")
    
    print("\n\nGRAMMAR:")
    print(grammar)
    
    print("\nFIRST sets:")
    for k, v in first.items():
        print(f"{k}: {v}")

    print("\nFOLLOW sets:")
    for k, v in follow.items():
        print(f"{k}: {v}")
