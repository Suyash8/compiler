"""
Operator Precedence Relation Table Generator (Shortened)

Generates operator precedence relation tables for operator grammars.
"""

from collections import defaultdict

EPSILON = "ε"

class OperatorPrecedenceTable:
    def __init__(self, grammar_str):
        self.productions, self.non_terminals, self.terminals = [], set(), set()
        self.firstvt, self.lastvt = defaultdict(set), defaultdict(set)
        self.precedence_table = {}
        self._parse_grammar(grammar_str)

    def _parse_grammar(self, grammar_str):
        for line in grammar_str.strip().split("\n"):
            lhs, rhs = map(str.strip, line.split("→"))
            self.non_terminals.add(lhs)
            for alt in rhs.split("|"):
                symbols = alt.split()
                self.productions.append((lhs, symbols))
                self.terminals.update(s for s in symbols if s not in self.non_terminals)

    def compute_first_last_vt(self):
        for lhs, rhs in self.productions:
            if rhs[0] in self.terminals: self.firstvt[lhs].add(rhs[0])
            if rhs[-1] in self.terminals: self.lastvt[lhs].add(rhs[-1])
        changed = True
        while changed:
            changed = False
            for lhs, rhs in self.productions:
                for i, sym in enumerate(rhs):
                    if sym in self.non_terminals:
                        if i > 0 and rhs[i-1] in self.terminals:
                            if rhs[i-1] not in self.firstvt[sym]:
                                self.firstvt[sym].add(rhs[i-1])
                                changed = True
                        if i < len(rhs)-1 and rhs[i+1] in self.terminals:
                            if rhs[i+1] not in self.lastvt[sym]:
                                self.lastvt[sym].add(rhs[i+1])
                                changed = True

    def build_precedence_table(self):
        self.compute_first_last_vt()
        for lhs, rhs in self.productions:
            for i in range(len(rhs)-1):
                if rhs[i] in self.terminals and rhs[i+1] in self.terminals:
                    self.precedence_table[(rhs[i], rhs[i+1])] = "="
                if rhs[i] in self.terminals and rhs[i+1] in self.non_terminals:
                    for t in self.firstvt[rhs[i+1]]:
                        self.precedence_table[(rhs[i], t)] = "<"
                if rhs[i] in self.non_terminals and rhs[i+1] in self.terminals:
                    for t in self.lastvt[rhs[i]]:
                        self.precedence_table[(t, rhs[i+1])] = ">"

def format_precedence_table_as_pretty_grid(table, terminals):
    col_width = max(len(t) for t in terminals) + 2

    # Header row
    header = f"{'':<{col_width}}" + "".join(f"{t:<{col_width}}" for t in terminals)
    separator = "-" * len(header)

    # Rows for each terminal
    rows = [header, separator]
    for a in terminals:
        row = [f"{a:<{col_width}}"]
        for b in terminals:
            entry = table.get((a, b), "")
            row.append(f"{entry:<{col_width}}")
        rows.append("".join(row))

    return "\n".join(rows)

def analyze_operator_grammar(grammar_str):
    opt = OperatorPrecedenceTable(grammar_str)
    opt.build_precedence_table()

    print("FIRSTVT:")
    for nt, firstvt_set in sorted(opt.firstvt.items()):
        print(f"  {nt}: {sorted(firstvt_set)}")

    print("\nLASTVT:")
    for nt, lastvt_set in sorted(opt.lastvt.items()):
        print(f"  {nt}: {sorted(lastvt_set)}")

    print("\nTABLE:")
    terminals = sorted(opt.terminals)
    print(format_precedence_table_as_pretty_grid(opt.precedence_table, terminals))

# Example usage:
grammar_str = """E → E + T | T\nT → T * F | F\nF → ( E ) | id"""
analyze_operator_grammar(grammar_str)