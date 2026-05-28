def build_grammar():
    return [
        ("S", ["C", "C"]),
        ("C", ["c", "C"]),
        ("C", ["d"]),
    ]

def augment_grammar(productions):
    start_symbol = productions[0][0]
    augmented_start = start_symbol + "'"
    return [(augmented_start, [start_symbol])] + productions, augmented_start

def closure(items, productions_by_lhs):
    result = set(items)
    changed = True
    while changed:
        changed = False
        for lhs, rhs, dot in list(result):
            if dot < len(rhs):
                symbol = rhs[dot]
                if symbol in productions_by_lhs:
                    for production_rhs in productions_by_lhs[symbol]:
                        item = (symbol, tuple(production_rhs), 0)
                        if item not in result:
                            result.add(item)
                            changed = True
    return frozenset(result)

def goto(state, symbol, productions_by_lhs):
    moved = set()
    for lhs, rhs, dot in state:
        if dot < len(rhs) and rhs[dot] == symbol:
            moved.add((lhs, rhs, dot + 1))
    if not moved:
        return frozenset()
    return closure(moved, productions_by_lhs)

def item_to_string(item):
    lhs, rhs, dot = item
    parts = list(rhs)
    parts.insert(dot, "·")
    return f"{lhs} -> {' '.join(parts)}"

def state_to_string(state):
    return "\n".join(sorted(item_to_string(item) for item in state))

def build_canonical_collection(productions):
    productions_by_lhs = {}
    for lhs, rhs in productions:
        productions_by_lhs.setdefault(lhs, []).append(rhs)
    start_item = (productions[0][0], tuple(productions[0][1]), 0)
    states = [closure({start_item}, productions_by_lhs)]
    transitions = {}
    symbols = sorted({symbol for _, rhs in productions for symbol in rhs})
    index = 0
    while index < len(states):
        state = states[index]
        for symbol in symbols:
            next_state = goto(state, symbol, productions_by_lhs)
            if not next_state:
                continue
            if next_state not in states:
                states.append(next_state)
            transitions[(index, symbol)] = states.index(next_state)
        index += 1
    return states, transitions, productions_by_lhs

def build_tables(states, transitions, productions, productions_by_lhs, augmented_start):
    terminals = sorted({symbol for _, rhs in productions for symbol in rhs if symbol not in productions_by_lhs})
    terminals.append("$")
    nonterminals = sorted(symbol for symbol in productions_by_lhs.keys() if symbol != augmented_start)
    action = {i: {} for i in range(len(states))}
    goto_table = {i: {} for i in range(len(states))}
    production_numbers = {tuple((lhs, tuple(rhs))): i for i, (lhs, rhs) in enumerate(productions[1:], start=1)}
    for (state_index, symbol), target in transitions.items():
        if symbol in terminals and symbol != "$":
            action[state_index][symbol] = f"Shift {target}"
        elif symbol in nonterminals:
            goto_table[state_index][symbol] = target
    for state_index, state in enumerate(states):
        for lhs, rhs, dot in state:
            if dot == len(rhs):
                if lhs == augmented_start:
                    action[state_index]["$"] = "Accept"
                else:
                    production_index = production_numbers[(lhs, rhs)]
                    for terminal in terminals:
                        if terminal not in action[state_index]:
                            action[state_index][terminal] = f"Reduce {production_index}"
    return terminals, nonterminals, action, goto_table, production_numbers

def print_states(states):
    for index, state in enumerate(states):
        print(f"I{index}:")
        for line in sorted(item_to_string(item) for item in state):
            print(line)
        print()

def print_tables(terminals, nonterminals, action, goto_table):
    header = ["State"] + terminals + nonterminals
    widths = {column: max(len(column), 7) for column in header}
    rows = []
    for state_index in range(len(action)):
        row = [str(state_index)]
        for terminal in terminals:
            row.append(action[state_index].get(terminal, ""))
        for nonterminal in nonterminals:
            row.append(str(goto_table[state_index].get(nonterminal, "")))
        rows.append(row)
        for column, value in zip(header, row):
            widths[column] = max(widths[column], len(value))
    print("LR(0) Parsing Table")
    print()
    print("ACTION and GOTO entries")
    print()
    print(" | ".join(column.ljust(widths[column]) for column in header))
    print("-+-".join("-" * widths[column] for column in header))
    for row in rows:
        print(" | ".join(value.ljust(widths[column]) for value, column in zip(row, header)))

def main():
    productions, augmented_start = augment_grammar(build_grammar())
    states, transitions, productions_by_lhs = build_canonical_collection(productions)
    terminals, nonterminals, action, goto_table, _ = build_tables(states, transitions, productions, productions_by_lhs, augmented_start)
    print("Grammar")
    for index, (lhs, rhs) in enumerate(productions):
        print(f"{index}. {lhs} -> {' '.join(rhs)}")
    print()
    print("Canonical Collection of LR(0) Items")
    print()
    print_states(states)
    print("Transitions")
    for state_index, symbol in sorted(transitions):
        target = transitions[(state_index, symbol)]
        if symbol in {"c", "d"}:
            print(f"Shift on {symbol}: I{state_index} -> I{target}")
        else:
            print(f"GOTO on {symbol}: I{state_index} -> I{target}")
    print()
    print_tables(terminals, nonterminals, action, goto_table)

if __name__ == "__main__":
    main()
