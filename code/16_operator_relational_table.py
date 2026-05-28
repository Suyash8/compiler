def build_grammar():
    return {
        "E": [["E", "+", "T"], ["E", "-", "T"], ["T"]],
        "T": [["T", "*", "F"], ["T", "/", "F"], ["F"]],
        "F": [["(", "E", ")"], ["i"]],
    }

def terminals_and_nonterminals(grammar):
    nonterminals = list(grammar.keys())
    terminals = []
    for productions in grammar.values():
        for production in productions:
            for symbol in production:
                if symbol not in grammar and symbol not in terminals:
                    terminals.append(symbol)
    terminals.append("$")
    return nonterminals, terminals

def firstvt(grammar):
    result = {nonterminal: set() for nonterminal in grammar}
    changed = True
    while changed:
        changed = False
        for nonterminal, productions in grammar.items():
            for production in productions:
                first = production[0]
                if first not in grammar:
                    if first not in result[nonterminal]:
                        result[nonterminal].add(first)
                        changed = True
                else:
                    if len(production) > 1:
                        second = production[1]
                        if second not in grammar and second not in result[nonterminal]:
                            result[nonterminal].add(second)
                            changed = True
                    for symbol in result[first]:
                        if symbol not in result[nonterminal]:
                            result[nonterminal].add(symbol)
                            changed = True
    return result

def lastvt(grammar):
    result = {nonterminal: set() for nonterminal in grammar}
    changed = True
    while changed:
        changed = False
        for nonterminal, productions in grammar.items():
            for production in productions:
                last = production[-1]
                if last not in grammar:
                    if last not in result[nonterminal]:
                        result[nonterminal].add(last)
                        changed = True
                else:
                    if len(production) > 1:
                        before_last = production[-2]
                        if before_last not in grammar and before_last not in result[nonterminal]:
                            result[nonterminal].add(before_last)
                            changed = True
                    for symbol in result[last]:
                        if symbol not in result[nonterminal]:
                            result[nonterminal].add(symbol)
                            changed = True
    return result

def make_table(terminals):
    return {row: {col: "" for col in terminals} for row in terminals}

def set_relation(table, a, b, relation):
    current = table[a][b]
    if current == "":
        table[a][b] = relation
    elif current != relation:
        table[a][b] = current + relation

def build_relations(grammar, first_vt, last_vt, terminals):
    table = make_table(terminals)
    for productions in grammar.values():
        for production in productions:
            for index in range(len(production) - 1):
                left = production[index]
                right = production[index + 1]
                if left not in grammar and right not in grammar:
                    set_relation(table, left, right, "=")
                if left not in grammar and right in grammar:
                    for symbol in first_vt[right]:
                        set_relation(table, left, symbol, "<")
                if left in grammar and right not in grammar:
                    for symbol in last_vt[left]:
                        set_relation(table, symbol, right, ">")
                if index + 2 < len(production):
                    middle = production[index + 1]
                    last = production[index + 2]
                    if left not in grammar and middle in grammar and last not in grammar:
                        set_relation(table, left, last, "=")
    start_symbol = next(iter(grammar))
    for symbol in first_vt[start_symbol]:
        set_relation(table, "$", symbol, "<")
    for symbol in last_vt[start_symbol]:
        set_relation(table, symbol, "$", ">")
    set_relation(table, "$", "$", "=")
    return table

def print_grammar(grammar):
    print("Grammar")
    for lhs, productions in grammar.items():
        for production in productions:
            print(f"{lhs} -> {' '.join(production)}")
    print()

def print_sets(name, sets):
    print(name)
    for nonterminal, values in sets.items():
        print(f"{nonterminal}: {sorted(values)}")
    print()

def print_table(table, terminals):
    header = [" "] + terminals
    widths = {col: max(len(col), 3) for col in header}
    for row in terminals:
        widths[row] = max(widths[row], len(row))
        for col in terminals:
            widths[col] = max(widths[col], len(table[row][col]))
    print("Operator Precedence Relation Table")
    print()
    print(" | ".join(column.ljust(widths[column]) for column in header))
    print("-+-".join("-" * widths[column] for column in header))
    for row in terminals:
        values = [row.ljust(widths[row])] + [table[row][col].ljust(widths[col]) for col in terminals]
        print(" | ".join(values))

def main():
    grammar = build_grammar()
    nonterminals, terminals = terminals_and_nonterminals(grammar)
    first_vt = firstvt(grammar)
    last_vt = lastvt(grammar)
    table = build_relations(grammar, first_vt, last_vt, terminals)
    print_grammar(grammar)
    print_sets("FIRSTVT", first_vt)
    print_sets("LASTVT", last_vt)
    print_table(table, terminals)

if __name__ == "__main__":
    main()