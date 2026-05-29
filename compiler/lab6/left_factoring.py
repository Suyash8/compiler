def longest_common_prefix(rules):
    if not rules:
        return []

    prefix = rules[0][:]

    for r in rules[1:]:
        k = 0
        while k < min(len(prefix), len(r)) and prefix[k] == r[k]:
            k += 1
        prefix = prefix[:k]

        if not prefix:
            break

    return prefix


def left_factor_once(grammar, A):
    rules = grammar[A]

    if len(rules) < 2:
        return False

    buckets = {}

    for r in rules:
        key = None if r == [EPS] else r[0]
        buckets.setdefault(key, []).append(r)

    for group in buckets.values():
        if group is None or len(group) < 2:
            continue

        prefix = longest_common_prefix(group)

        if not prefix:
            continue

        A1 = fresh_nonterminal(A, grammar)

        # 🔹 New required debug prints
        debug_print(f"[Left Factoring] {A}, prefix = {' '.join(prefix)}")
        debug_print(f"Prefix length = {len(prefix)}")
        debug_print(f"Merged productions = {len(group)}")
        debug_print(f"[Create] New nonterminal {A1}")

        grammar[A1] = []

        # remove grouped rules
        grammar[A] = [r for r in rules if r not in group]

        # add factored rule
        grammar[A].append(prefix + [A1])

        # create new productions
        for r in group:
            suffix = r[len(prefix):]
            grammar[A1].append(suffix if suffix else [EPS])

        return True

    return False


def left_factor(grammar):
    g = copy_grammar(grammar)

    changed = True

    while changed:
        changed = False

        for A in list(g.keys()):
            if left_factor_once(g, A):
                changed = True

    return g

if __name__ == "__main__":
    text = """
    S -> a b c | a b d | a e | f
    """

    grammar, order = parse_grammar(text)

    print("Original Grammar:")
    print(grammar_to_string(grammar))

    print("\nApplying Left Factoring...\n")

    factored_grammar = left_factor(grammar)

    print("\nFinal Factored Grammar:")
    print(grammar_to_string(factored_grammar))