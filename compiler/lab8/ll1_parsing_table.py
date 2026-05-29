"""
LL(1) Parsing Table Generator

Generates LL(1) parsing tables from grammar strings, including:
- FIRST set computation
- FOLLOW set computation
- Parsing table construction
- LL(1) conflict detection
"""

from collections import defaultdict
from typing import Dict, List, Set, Tuple, Optional


class Grammar:
    """Represents a context-free grammar."""
    
    def __init__(self):
        self.productions: Dict[str, List[List[str]]] = defaultdict(list)
        self.non_terminals: Set[str] = set()
        self.terminals: Set[str] = set()
        self.start_symbol: Optional[str] = None
    
    def add_production(self, lhs: str, rhs_list: List[List[str]]):
        """Add a production rule A → α1 | α2 | ... | αn"""
        if self.start_symbol is None:
            self.start_symbol = lhs
        self.non_terminals.add(lhs)
        for rhs in rhs_list:
            self.productions[lhs].append(rhs)
            for symbol in rhs:
                if not symbol.isupper() and symbol != 'ε' and not symbol.endswith("'"):
                    # Check if it's a terminal (lowercase or special symbols)
                    if symbol not in self.non_terminals:
                        self.terminals.add(symbol)
    
    def __str__(self) -> str:
        result = []
        for lhs in self.productions:
            prods = [' '.join(rhs) if rhs else 'ε' for rhs in self.productions[lhs]]
            result.append(f"{lhs} → {' | '.join(prods)}")
        return '\n'.join(result)


def parse_grammar(grammar_str: str) -> Grammar:
    """Parse a grammar string into a Grammar object.
    
    Format: One or more lines of the form:
        A → α1 | α2 | ... | αn
    where symbols are space-separated and ε represents epsilon.
    """
    grammar = Grammar()
    lines = [line.strip() for line in grammar_str.strip().split('\n') if line.strip()]
    
    for line in lines:
        if '→' not in line:
            continue
        
        lhs, rhs = line.split('→', 1)
        lhs = lhs.strip()
        
        # Parse alternatives separated by |
        alternatives = rhs.split('|')
        
        for alt in alternatives:
            symbols = alt.strip().split()
            # Handle empty production (epsilon)
            if not symbols or symbols == ['ε']:
                grammar.productions[lhs].append(['ε'])
            else:
                grammar.productions[lhs].append(symbols)
        
        grammar.non_terminals.add(lhs)
    
    # Identify terminals (symbols that are not non-terminals and not epsilon)
    for lhs in grammar.productions:
        for rhs in grammar.productions[lhs]:
            for symbol in rhs:
                if symbol != 'ε' and symbol not in grammar.non_terminals:
                    grammar.terminals.add(symbol)
    
    # Set start symbol (first non-terminal in the grammar)
    if grammar.non_terminals:
        grammar.start_symbol = list(grammar.non_terminals)[0]
    
    return grammar


def compute_first_sets(grammar: Grammar) -> Dict[str, Set[str]]:
    """Compute FIRST sets for all symbols in the grammar.
    
    FIRST(X) = set of terminals that can begin strings derived from X
    """
    first: Dict[str, Set[str]] = defaultdict(set)
    
    # Initialize: FIRST(a) = {a} for terminals
    for terminal in grammar.terminals:
        first[terminal].add(terminal)
    first['ε'] = {'ε'}
    
    # Iterate until no changes
    changed = True
    while changed:
        changed = False
        
        for lhs in grammar.non_terminals:
            for rhs in grammar.productions[lhs]:
                if rhs == ['ε']:
                    if 'ε' not in first[lhs]:
                        first[lhs].add('ε')
                        changed = True
                    continue
                
                # For production X → Y1 Y2 ... Yn
                all_can_derive_epsilon = True
                
                for i, symbol in enumerate(rhs):
                    if symbol == 'ε':
                        continue
                    
                    # Add FIRST(symbol) - {ε} to FIRST(lhs)
                    for s in first[symbol]:
                        if s != 'ε' and s not in first[lhs]:
                            first[lhs].add(s)
                            changed = True
                    
                    if 'ε' not in first[symbol]:
                        all_can_derive_epsilon = False
                        break
                
                # If all symbols can derive ε, add ε to FIRST(lhs)
                if all_can_derive_epsilon and 'ε' not in first[lhs]:
                    first[lhs].add('ε')
                    changed = True
    
    return first


def compute_first_of_string(symbols: List[str], first_sets: Dict[str, Set[str]]) -> Set[str]:
    """Compute FIRST set for a string of symbols (a production RHS)."""
    result = set()
    
    if not symbols or symbols == ['ε']:
        return {'ε'}
    
    for i, symbol in enumerate(symbols):
        if symbol == 'ε':
            continue
        
        # Add FIRST(symbol) - {ε}
        for s in first_sets[symbol]:
            if s != 'ε':
                result.add(s)
        
        if 'ε' not in first_sets[symbol]:
            return result
    
    # All symbols can derive ε
    result.add('ε')
    return result


def compute_follow_sets(grammar: Grammar, first_sets: Dict[str, Set[str]]) -> Dict[str, Set[str]]:
    """Compute FOLLOW sets for all non-terminals in the grammar.
    
    FOLLOW(A) = set of terminals that can appear immediately after A
    """
    follow: Dict[str, Set[str]] = defaultdict(set)
    
    # Rule 1: $ is in FOLLOW(S) where S is start symbol
    if grammar.start_symbol:
        follow[grammar.start_symbol].add('$')
    
    # Iterate until no changes
    changed = True
    while changed:
        changed = False
        
        for lhs in grammar.non_terminals:
            for rhs in grammar.productions[lhs]:
                # For each non-terminal B in the RHS
                for i, symbol in enumerate(rhs):
                    if symbol not in grammar.non_terminals:
                        continue
                    
                    # Rule 2: For A → αBβ, add FIRST(β) - {ε} to FOLLOW(B)
                    beta = rhs[i+1:] if i+1 < len(rhs) else []
                    first_beta = compute_first_of_string(beta, first_sets)
                    
                    for s in first_beta:
                        if s != 'ε' and s not in follow[symbol]:
                            follow[symbol].add(s)
                            changed = True
                    
                    # Rule 3: If β can derive ε (or β is empty), add FOLLOW(A) to FOLLOW(B)
                    if not beta or 'ε' in first_beta:
                        for s in follow[lhs]:
                            if s not in follow[symbol]:
                                follow[symbol].add(s)
                                changed = True
    
    return follow


class LL1ParsingTable:
    """Represents an LL(1) parsing table M[A, a]."""
    
    def __init__(self, grammar: Grammar):
        self.grammar = grammar
        self.table: Dict[Tuple[str, str], List[str]] = defaultdict(list)
        self.first_sets: Dict[str, Set[str]] = {}
        self.follow_sets: Dict[str, Set[str]] = {}
        self.is_ll1: bool = True
        self.conflicts: List[Tuple[str, str, List[List[str]]]] = []
    
    def build(self):
        """Build the LL(1) parsing table."""
        # Compute FIRST and FOLLOW sets
        self.first_sets = compute_first_sets(self.grammar)
        self.follow_sets = compute_follow_sets(self.grammar, self.first_sets)
        
        # Build the table
        for lhs in self.grammar.non_terminals:
            for rhs in self.grammar.productions[lhs]:
                # Compute FIRST(α) for this production
                first_alpha = compute_first_of_string(rhs, self.first_sets)
                
                # Rule 1: For each terminal a in FIRST(α), add A → α to M[A, a]
                for terminal in first_alpha:
                    if terminal != 'ε':
                        key = (lhs, terminal)
                        production = rhs if rhs != ['ε'] else ['ε']
                        
                        if self.table[key] and self.table[key] != production:
                            self.is_ll1 = False
                            self.conflicts.append((lhs, terminal, [self.table[key], production]))
                        
                        if production not in self.table[key]:
                            self.table[key].extend(production)
                        else:
                            # Store as list for conflict detection
                            pass
                
                # Rule 2: If ε in FIRST(α), add A → α to M[A, b] for each b in FOLLOW(A)
                if 'ε' in first_alpha:
                    for terminal in self.follow_sets[lhs]:
                        key = (lhs, terminal)
                        production = rhs if rhs != ['ε'] else ['ε']
                        
                        if self.table[key] and self.table[key] != production:
                            self.is_ll1 = False
                            self.conflicts.append((lhs, terminal, [self.table[key], production]))
                        
                        if production not in self.table[key]:
                            self.table[key].extend(production)
    
    def get_entry(self, non_terminal: str, terminal: str) -> Optional[List[str]]:
        """Get the production at M[non_terminal, terminal]."""
        return self.table.get((non_terminal, terminal))
    
    def __str__(self) -> str:
        """Format the parsing table as a readable string."""
        # Get all terminals (including $)
        terminals = sorted(self.grammar.terminals) + ['$']
        non_terminals = sorted(self.grammar.non_terminals)
        
        # Calculate column widths
        nt_width = max(len(nt) for nt in non_terminals) if non_terminals else 3
        nt_width = max(nt_width, 3)
        
        col_widths = {}
        for t in terminals:
            max_width = len(t)
            for nt in non_terminals:
                entry = self.get_entry(nt, t)
                if entry:
                    prod_str = f"{nt} → {' '.join(entry) if entry != ['ε'] else 'ε'}"
                    max_width = max(max_width, len(prod_str))
            col_widths[t] = max_width + 2
        
        # Build header
        header = ' ' * (nt_width + 3) + '│'
        for t in terminals:
            header += f" {t.center(col_widths[t]) }│"
        
        # Build separator
        sep = '─' * (nt_width + 3) + '┼'
        for t in terminals:
            sep += '─' * (col_widths[t] + 1) + '┼'
        
        # Build rows
        rows = []
        for nt in non_terminals:
            row = f" {nt.rjust(nt_width)} │"
            for t in terminals:
                entry = self.get_entry(nt, t)
                if entry:
                    prod_str = f"{nt} → {' '.join(entry) if entry != ['ε'] else 'ε'}"
                    row += f" {prod_str.ljust(col_widths[t])}│"
                else:
                    row += f" {' ' * col_widths[t]}│"
            rows.append(row)
        
        result = [header, sep] + rows
        return '\n'.join(result)


def generate_ll1_table(grammar_str: str) -> Tuple[LL1ParsingTable, Dict[str, Set[str]], Dict[str, Set[str]]]:
    """Generate an LL(1) parsing table from a grammar string.
    
    Returns:
        - The parsing table
        - FIRST sets
        - FOLLOW sets
    """
    grammar = parse_grammar(grammar_str)
    table = LL1ParsingTable(grammar)
    table.build()
    
    return table, table.first_sets, table.follow_sets


def format_set(s: Set[str], name: str) -> str:
    """Format a set for display."""
    elements = sorted(s)
    return f"{name} = {{ {', '.join(elements)} }}"


def analyze_grammar(grammar_str: str, grammar_num: int) -> str:
    """Analyze a grammar and return a formatted report."""
    result = []
    result.append("=" * 70)
    result.append(f"GRAMMAR {grammar_num}")
    result.append("=" * 70)
    
    # Parse grammar
    grammar = parse_grammar(grammar_str)
    result.append("\nGrammar:")
    result.append(str(grammar))
    
    # Generate table
    table, first_sets, follow_sets = generate_ll1_table(grammar_str)
    
    # Print FIRST sets
    result.append("\nFIRST Sets:")
    for nt in sorted(grammar.non_terminals):
        result.append(f"  {format_set(first_sets[nt], f'FIRST({nt})')}")
    
    # Print FOLLOW sets
    result.append("\nFOLLOW Sets:")
    for nt in sorted(grammar.non_terminals):
        result.append(f"  {format_set(follow_sets[nt], f'FOLLOW({nt})')}")
    
    # Print parsing table
    result.append("\nLL(1) Parsing Table:")
    result.append(str(table))
    
    # Check LL(1) property
    result.append("")
    if table.is_ll1:
        result.append("✓ Grammar IS LL(1)")
    else:
        result.append("✗ Grammar is NOT LL(1)")
        result.append("\nConflicts detected:")
        for nt, terminal, productions in table.conflicts:
            result.append(f"  M[{nt}, {terminal}] has multiple entries:")
            for prod in productions:
                result.append(f"    - {nt} → {' '.join(prod) if prod != ['ε'] else 'ε'}")
    
    result.append("")
    return '\n'.join(result)


def main():
    """Main function to demonstrate LL(1) parsing table generation."""
    
    # Test grammars
    grammars = [
        """E → T E'
E' → + T E' | ε
T → F T'
T' → * F T' | ε
F → ( E ) | id""",
        """S → A
A → a B | ε
B → b A""",
        """S → i E t S | i E t S e S | a
E → b"""
    ]
    
    print("LL(1) PARSING TABLE GENERATOR")
    print("=" * 70)
    print()
    
    for i, grammar_str in enumerate(grammars, 1):
        report = analyze_grammar(grammar_str, i)
        print(report)
    
    print("=" * 70)
    print("Analysis complete.")


if __name__ == "__main__":
    main()
