from collections import defaultdict
from pathlib import Path

EPSILON = "ε"

def load_grammars(filename="grammars.txt"):
    path = Path(__file__).parent / filename
    return [g.strip() for g in path.read_text().split("---") if g.strip()]

def update_set(dst, values):
    before = len(dst)
    dst.update(values)
    return len(dst) > before

def first_of_sequence(seq, productions, first):
    result = set()
    for sym in seq:
        sym_first = first[sym] if sym in productions else {sym}
        result.update(sym_first - {EPSILON})
        if EPSILON not in sym_first:
            return result
    result.add(EPSILON)
    return result

def parse_grammar(grammar_str):
    productions = defaultdict(list)
    for line in grammar_str.strip().split("\n"):
        left, right = line.split("→")
        for alt in right.split("|"):
            productions[left].append(alt.strip().split())
    return productions

def compute_first(productions):
    first = defaultdict(set)

    changed = True
    while changed:
        changed = False

        for head, rules in productions.items():
            for prod in rules:
                if prod == [EPSILON]:
                    changed |= update_set(first[head], {EPSILON})
                    continue

                nullable = True
                for symbol in prod:
                    if symbol not in productions:
                        changed |= update_set(first[head], {symbol})
                        nullable = False
                        break

                    changed |= update_set(first[head], first[symbol] - {EPSILON})
                    if EPSILON not in first[symbol]:
                        nullable = False
                        break

                if nullable:
                    changed |= update_set(first[head], {EPSILON})

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

                    rest = prod[i+1:]
                    if rest:
                        rest_first = first_of_sequence(rest, productions, first)
                        changed |= update_set(follow[symbol], rest_first - {EPSILON})
                        if EPSILON in rest_first:
                            changed |= update_set(follow[symbol], follow[head])
                    else:
                        changed |= update_set(follow[symbol], follow[head])

    return follow


def print_sets(title, data):
    print(f"\n{title}:")
    for k, v in data.items():
        print(f"{k}: {v}")

for grammar in load_grammars():
    productions = parse_grammar(grammar)
    first = compute_first(productions)
    follow = compute_follow(productions, first, start_symbol="S")
    print("\n\nGRAMMAR:")
    print(grammar)
    print_sets("FIRST sets", first)
    print_sets("FOLLOW sets", follow)
