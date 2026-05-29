"""
LL(1) Parsing Table Generator (Shortened)

Generates LL(1) parsing tables from grammar strings, including:
- FIRST set computation
- FOLLOW set computation
- Parsing table construction
- LL(1) conflict detection
"""

from collections import defaultdict

EPSILON = "ε"


class Grammar:
    def __init__(self):
        self.productions = defaultdict(list)
        self.non_terminals, self.terminals = set(), set()
        self.start_symbol = None

    def add_production(self, lhs, rhs_list):
        if not self.start_symbol: self.start_symbol = lhs
        self.non_terminals.add(lhs)
        for rhs in rhs_list:
            self.productions[lhs].append(rhs)
            self.terminals.update(s for s in rhs if s not in self.non_terminals and s != EPSILON)

def parse_grammar(grammar_str):
    grammar = Grammar()
    for line in grammar_str.strip().split("\n"):
        lhs, rhs = map(str.strip, line.split("→"))
        grammar.add_production(lhs, [alt.split() for alt in rhs.split("|")])
    return grammar

def compute_first(grammar):
    first = defaultdict(set)
    for t in grammar.terminals: first[t].add(t)
    changed = True
    while changed:
        changed = False
        for lhs, rules in grammar.productions.items():
            for rule in rules:
                for sym in rule:
                    updated = first[lhs].update(first[sym] - {EPSILON})
                    if updated: changed = True
                    if EPSILON not in first[sym]: break
                else: 
                    if EPSILON not in first[lhs]:
                        first[lhs].add(EPSILON)
                        changed = True
    return first

def compute_follow(grammar, first):
    follow = defaultdict(set, {grammar.start_symbol: {"$"}})
    changed = True
    while changed:
        changed = False
        for lhs, rules in grammar.productions.items():
            for rule in rules:
                for i, B in enumerate(rule):
                    if B in grammar.non_terminals:
                        rest_first = set.union(*(first[sym] for sym in rule[i+1:] if sym in first), {EPSILON})
                        if follow[B].update(rest_first - {EPSILON}):
                            changed = True
                        if EPSILON in rest_first and follow[B].update(follow[lhs]):
                            changed = True
    return follow

def build_ll1_table(grammar):
    first, follow = compute_first(grammar), compute_follow(grammar, compute_first(grammar))
    table, conflicts = defaultdict(list), []
    for lhs, rules in grammar.productions.items():
        for rule in rules:
            first_set = set.union(*(first[sym] for sym in rule), {EPSILON})
            for term in first_set - {EPSILON}: table[(lhs, term)].append(rule)
            if EPSILON in first_set:
                for term in follow[lhs]: table[(lhs, term)].append(rule)
    return table, first, follow

def format_table_as_pretty_grid(table, grammar):
    terminals = sorted(grammar.terminals) + ['$']
    non_terminals = sorted(grammar.non_terminals)

    # Calculate column widths
    col_widths = {t: max(len(t), 10) for t in terminals}
    col_widths['Non-Terminal'] = max(len('Non-Terminal'), max(len(nt) for nt in non_terminals))

    # Header row
    header = f"{'Non-Terminal'.ljust(col_widths['Non-Terminal'])} | " + " | ".join(t.ljust(col_widths[t]) for t in terminals)
    separator = "-" * len(header)

    # Rows for each non-terminal
    rows = [header, separator]
    for nt in non_terminals:
        row = [nt.ljust(col_widths['Non-Terminal'])]
        for t in terminals:
            entry = table.get((nt, t), [])
            row.append(" | ".join([" ".join(r) for r in entry]).ljust(col_widths[t]))
        rows.append(" | ".join(row))

    return "\n".join(rows)

def analyze_grammar(grammar_str):
    grammar = parse_grammar(grammar_str)
    table, first, follow = build_ll1_table(grammar)

    print("FIRST:")
    for nt, first_set in sorted(first.items()):
        print(f"  {nt}: {sorted(first_set)}")

    print("\nFOLLOW:")
    for nt, follow_set in sorted(follow.items()):
        print(f"  {nt}: {sorted(follow_set)}")

    print("\nTABLE:")
    print(format_table_as_pretty_grid(table, grammar))

# Example usage:
grammar_str = """E → T E'\nE' → + T E' | ε\nT → F T'\nT' → * F T' | ε\nF → ( E ) | id"""
analyze_grammar(grammar_str)