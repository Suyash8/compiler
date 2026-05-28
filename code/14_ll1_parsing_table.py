def build_grammar():
    return {
        "E": [["T", "E'"]],
        "E'": [["+", "T", "E'"], ["epsilon"]],
        "T": [["F", "T'"]],
        "T'": [["*", "F", "T'"], ["epsilon"]],
        "F": [["(", "E", ")"], ["id"]],
    }

def is_nonterminal(symbol, grammar):
    return symbol in grammar

def compute_first(grammar):
    first = {nonterminal: set() for nonterminal in grammar}
    changed = True
    while changed:
        changed = False
        for nonterminal, productions in grammar.items():
            for production in productions:
                if production == ["epsilon"]:
                    if "epsilon" not in first[nonterminal]:
                        first[nonterminal].add("epsilon")
                        changed = True
                    continue
                add_epsilon = True
                for symbol in production:
                    if not is_nonterminal(symbol, grammar):
                        if symbol not in first[nonterminal]:
                            first[nonterminal].add(symbol)
                            changed = True
                        add_epsilon = False
                        break
                    before = len(first[nonterminal])
                    first[nonterminal].update(first[symbol] - {"epsilon"})
                    if len(first[nonterminal]) != before:
                        changed = True
                    if "epsilon" not in first[symbol]:
                        add_epsilon = False
                        break
                if add_epsilon and "epsilon" not in first[nonterminal]:
                    first[nonterminal].add("epsilon")
                    changed = True
    return first

def first_of_sequence(sequence, grammar, first):
    result = set()
    if sequence == ["epsilon"]:
        result.add("epsilon")
        return result
    for symbol in sequence:
        if not is_nonterminal(symbol, grammar):
            result.add(symbol)
            return result
        result.update(first[symbol] - {"epsilon"})
        if "epsilon" not in first[symbol]:
            return result
    result.add("epsilon")
    return result

def compute_follow(grammar, first, start_symbol):
    follow = {nonterminal: set() for nonterminal in grammar}
    follow[start_symbol].add("$")
    changed = True
    while changed:
        changed = False
        for lhs, productions in grammar.items():
            for production in productions:
                for index, symbol in enumerate(production):
                    if not is_nonterminal(symbol, grammar):
                        continue
                    beta = production[index + 1:]
                    if beta:
                        first_beta = first_of_sequence(beta, grammar, first)
                        before = len(follow[symbol])
                        follow[symbol].update(first_beta - {"epsilon"})
                        if len(follow[symbol]) != before:
                            changed = True
                        if "epsilon" in first_beta:
                            before = len(follow[symbol])
                            follow[symbol].update(follow[lhs])
                            if len(follow[symbol]) != before:
                                changed = True
                    else:
                        before = len(follow[symbol])
                        follow[symbol].update(follow[lhs])
                        if len(follow[symbol]) != before:
                            changed = True
    return follow

def build_table(grammar, first, follow):
    table = {nonterminal: {} for nonterminal in grammar}
    for lhs, productions in grammar.items():
        for production in productions:
            first_alpha = first_of_sequence(production, grammar, first)
            for terminal in first_alpha - {"epsilon"}:
                table[lhs][terminal] = production
            if "epsilon" in first_alpha:
                for terminal in follow[lhs]:
                    table[lhs][terminal] = production
    return table

def table_cell(production):
    if production is None:
        return ""
    return "{} -> {}".format(production[0], " ".join(production[1]))

def print_set_map(title, values):
    print(title)
    for nonterminal in values:
        items = sorted(values[nonterminal])
        print(f"{nonterminal}: {items}")
    print()

def print_table(grammar, table, terminals):
    header = ["NT"] + terminals
    widths = {column: len(column) for column in header}
    for nonterminal in grammar:
        widths[nonterminal] = max(widths.get(nonterminal, 2), len(nonterminal))
        for terminal in terminals:
            cell = table[nonterminal].get(terminal)
            display = table_cell((nonterminal, cell)) if cell else ""
            widths[terminal] = max(widths[terminal], len(display))
    print("LL(1) Parsing Table")
    print()
    print(" | ".join(column.ljust(widths[column]) for column in header))
    print("-+-".join("-" * widths[column] for column in header))
    for nonterminal in grammar:
        row = [nonterminal.ljust(widths["NT"])]
        for terminal in terminals:
            cell = table[nonterminal].get(terminal)
            display = table_cell((nonterminal, cell)) if cell else ""
            row.append(display.ljust(widths[terminal]))
        print(" | ".join(row))

def main():
    grammar = build_grammar()
    start_symbol = next(iter(grammar))
    first = compute_first(grammar)
    follow = compute_follow(grammar, first, start_symbol)
    table = build_table(grammar, first, follow)
    terminals = []
    for productions in grammar.values():
        for production in productions:
            for symbol in production:
                if not is_nonterminal(symbol, grammar) and symbol != "epsilon" and symbol not in terminals:
                    terminals.append(symbol)
    if "$" not in terminals:
        terminals.append("$")
    print("Grammar")
    for lhs, productions in grammar.items():
        for production in productions:
            print(f"{lhs} -> {' '.join(production)}")
    print()
    print_set_map("FIRST", first)
    print_set_map("FOLLOW", follow)
    print_table(grammar, table, terminals)

if __name__ == "__main__":
    main()