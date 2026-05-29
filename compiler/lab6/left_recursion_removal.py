from collections import OrderedDict

EPS = "Є"
DEBUG = True


def debug_print(msg):
    if DEBUG:
        print(msg)


def parse_grammar(text):
    grammar = OrderedDict()

    for line in text.strip().splitlines():
        line = line.strip()
        if not line:
            continue

        if "->" not in line:
            raise ValueError(f"Invalid production (missing '->'): {line}")

        lhs, rhs = line.split("->", 1)
        lhs = lhs.strip()
        grammar.setdefault(lhs, [])

        for alt in rhs.split("|"):
            alt = alt.strip()

            if alt == "" or alt.lower() == "epsilon" or alt == EPS:
                grammar[lhs].append([EPS])
            else:
                grammar[lhs].append(alt.split())

    return grammar, list(grammar.keys())


def grammar_to_string(grammar):
    out = []

    for A, rules in grammar.items():
        parts = []
        for r in rules:
            parts.append(" ".join(r) if r != [EPS] else EPS)
        out.append(f"{A} -> " + " | ".join(parts))

    return "\n".join(out)


def copy_grammar(grammar):
    return OrderedDict((A, [r[:] for r in rules]) for A, rules in grammar.items())


def grammars_equal(g1, g2):
    if g1.keys() != g2.keys():
        return False
    for A in g1:
        if g1[A] != g2[A]:
            return False
    return True


def fresh_nonterminal(A, grammar):
    if A + "'" not in grammar:
        return A + "'"

    i = 1
    while f"{A}{i}" in grammar:
        i += 1

    return f"{A}{i}"


def substitute(grammar, Ai, Aj):
    new_rules = []
    changed = False

    for alt in grammar[Ai]:
        if alt and alt[0] == Aj:
            gamma = alt[1:]
            debug_print(f"[Substitute] {Ai} -> {Aj} {' '.join(gamma)}")

            for delta in grammar[Aj]:
                if delta == [EPS]:
                    new_alt = gamma[:] if gamma else [EPS]
                else:
                    new_alt = delta + gamma

                new_rules.append(new_alt)
                debug_print(
                    f"Expanded: {Ai} -> {' '.join(new_alt) if new_alt != [EPS] else EPS}"
                )

            changed = True
        else:
            new_rules.append(alt)

    if changed:
        grammar[Ai] = new_rules


def remove_immediate_left_recursion(grammar, A):
    rec, nonrec = [], []

    for alt in grammar[A]:
        if alt and alt[0] == A:
            rec.append(alt[1:])   # alpha
        else:
            nonrec.append(alt)   # beta

    if not rec:
        return

    debug_print(f"[Immediate LR] Found in {A}")

    A1 = fresh_nonterminal(A, grammar)
    debug_print(f"[Create] New nonterminal {A1}")

    grammar[A] = []
    grammar[A1] = []

    for beta in nonrec:
        if beta == [EPS]:
            grammar[A].append([A1])
        else:
            grammar[A].append(beta + [A1])

    for alpha in rec:
        grammar[A1].append((alpha if alpha else [EPS]) + [A1])

    grammar[A1].append([EPS])


def remove_left_recursion(grammar, order):
    g = copy_grammar(grammar)
    original = copy_grammar(grammar)

    for i, Ai in enumerate(order):
        debug_print(f"\n=== Processing {Ai} ===")

        for j in range(i):
            Aj = order[j]
            substitute(g, Ai, Aj)

        remove_immediate_left_recursion(g, Ai)

    if grammars_equal(original, g):
        print("No left recursion detected.")

    return g


# Example usage
if __name__ == "__main__":
    text = """
    S -> S a | b
    """

    grammar, order = parse_grammar(text)

    print("Original Grammar:")
    print(grammar_to_string(grammar))

    new_grammar = remove_left_recursion(grammar, order)

    print("\nTransformed Grammar:")
    print(grammar_to_string(new_grammar))