"""
Operator Precedence Relation Table Generator

This module generates operator precedence relation tables for operator grammars.
It validates operator grammars, computes FIRSTVT and LASTVT sets, and builds
precedence relations.
"""

from typing import Dict, Set, List, Tuple, Optional
from collections import defaultdict


class OperatorPrecedenceTable:
    """Generates operator precedence relation tables for operator grammars."""
    
    def __init__(self, grammar_str: str):
        """
        Initialize with a grammar string.
        
        Format: Each production on separate line or separated by |
        Example: E → E + T | T
        """
        self.grammar_str = grammar_str
        self.productions: List[Tuple[str, List[str]]] = []
        self.non_terminals: Set[str] = set()
        self.terminals: Set[str] = set()
        self.start_symbol: Optional[str] = None
        self.firstvt: Dict[str, Set[str]] = defaultdict(set)
        self.lastvt: Dict[str, Set[str]] = defaultdict(set)
        self.precedence_table: Dict[Tuple[str, str], str] = {}
        self.is_operator_grammar = True
        self.error_messages: List[str] = []
        
        self._parse_grammar()
        self._validate_operator_grammar()
        
    def _parse_grammar(self):
        """Parse the grammar string into productions."""
        lines = self.grammar_str.strip().split('\n')
        
        for line in lines:
            line = line.strip()
            if not line:
                continue
                
            # Handle different arrow symbols
            if '→' in line:
                parts = line.split('→')
            elif '->' in line:
                parts = line.split('->')
            else:
                continue
                
            if len(parts) != 2:
                continue
                
            lhs = parts[0].strip()
            rhs_str = parts[1].strip()
            
            if self.start_symbol is None:
                self.start_symbol = lhs
                
            self.non_terminals.add(lhs)
            
            # Split by | for multiple productions
            rhs_alternatives = [alt.strip() for alt in rhs_str.split('|')]
            
            for rhs in rhs_alternatives:
                # Tokenize RHS - split by spaces
                symbols = rhs.split()
                self.productions.append((lhs, symbols))
                
                # Identify terminals and non-terminals
                for symbol in symbols:
                    if symbol.isupper() and len(symbol) == 1:
                        self.non_terminals.add(symbol)
                    elif symbol not in ['ε', 'epsilon', '']:
                        self.terminals.add(symbol)
    
    def _validate_operator_grammar(self) -> bool:
        """
        Validate if the grammar is an operator grammar.
        
        Conditions:
        1. No epsilon productions
        2. No two adjacent non-terminals on RHS
        """
        self.is_operator_grammar = True
        self.error_messages = []
        
        for lhs, rhs in self.productions:
            # Check for epsilon productions
            if len(rhs) == 1 and rhs[0] in ['ε', 'epsilon', '']:
                self.is_operator_grammar = False
                self.error_messages.append(f"Epsilon production found: {lhs} → ε")
                continue
            
            # Check for adjacent non-terminals
            for i in range(len(rhs) - 1):
                if rhs[i] in self.non_terminals and rhs[i+1] in self.non_terminals:
                    self.is_operator_grammar = False
                    self.error_messages.append(
                        f"Adjacent non-terminals in production: {lhs} → {' '.join(rhs)}"
                    )
        
        return self.is_operator_grammar
    
    def compute_firstvt(self) -> Dict[str, Set[str]]:
        """
        Compute FIRSTVT sets for all non-terminals.
        
        FIRSTVT(A) = {a | A => a... or A => Ba...}
        
        Rules:
        1. If A → a... then a ∈ FIRSTVT(A)
        2. If A → Ba... then a ∈ FIRSTVT(A)
        3. If A → B... then FIRSTVT(B) ⊆ FIRSTVT(A)
        """
        if not self.is_operator_grammar:
            return {}
            
        # Initialize
        for nt in self.non_terminals:
            self.firstvt[nt] = set()
        
        # Iterate until no change
        changed = True
        while changed:
            changed = False
            for lhs, rhs in self.productions:
                if not rhs or rhs[0] in ['ε', 'epsilon', '']:
                    continue
                    
                # Rule 1: A → a... (first symbol is terminal)
                if rhs[0] in self.terminals:
                    if rhs[0] not in self.firstvt[lhs]:
                        self.firstvt[lhs].add(rhs[0])
                        changed = True
                
                # Rule 2: A → Ba... (first is non-terminal, second is terminal)
                elif rhs[0] in self.non_terminals:
                    # Add FIRSTVT(B) to FIRSTVT(A)
                    B = rhs[0]
                    for t in self.firstvt[B]:
                        if t not in self.firstvt[lhs]:
                            self.firstvt[lhs].add(t)
                            changed = True
                    
                    # If there's a terminal after B
                    if len(rhs) > 1 and rhs[1] in self.terminals:
                        if rhs[1] not in self.firstvt[lhs]:
                            self.firstvt[lhs].add(rhs[1])
                            changed = True
        
        return dict(self.firstvt)
    
    def compute_lastvt(self) -> Dict[str, Set[str]]:
        """
        Compute LASTVT sets for all non-terminals.
        
        LASTVT(A) = {a | A => ...a or A => ...aB}
        
        Rules:
        1. If A → ...a then a ∈ LASTVT(A)
        2. If A → ...aB then a ∈ LASTVT(A)
        3. If A → ...B then LASTVT(B) ⊆ LASTVT(A)
        """
        if not self.is_operator_grammar:
            return {}
            
        # Initialize
        for nt in self.non_terminals:
            self.lastvt[nt] = set()
        
        # Iterate until no change
        changed = True
        while changed:
            changed = False
            for lhs, rhs in self.productions:
                if not rhs or rhs[-1] in ['ε', 'epsilon', '']:
                    continue
                    
                last_idx = len(rhs) - 1
                
                # Rule 1: A → ...a (last symbol is terminal)
                if rhs[last_idx] in self.terminals:
                    if rhs[last_idx] not in self.lastvt[lhs]:
                        self.lastvt[lhs].add(rhs[last_idx])
                        changed = True
                
                # Rule 2: A → ...aB (last is non-terminal)
                elif rhs[last_idx] in self.non_terminals:
                    # Add LASTVT(B) to LASTVT(A)
                    B = rhs[last_idx]
                    for t in self.lastvt[B]:
                        if t not in self.lastvt[lhs]:
                            self.lastvt[lhs].add(t)
                            changed = True
                    
                    # If there's a terminal before B
                    if len(rhs) > 1 and rhs[last_idx - 1] in self.terminals:
                        if rhs[last_idx - 1] not in self.lastvt[lhs]:
                            self.lastvt[lhs].add(rhs[last_idx - 1])
                            changed = True
        
        return dict(self.lastvt)
    
    def build_precedence_table(self) -> Dict[Tuple[str, str], str]:
        """
        Build the operator precedence relation table.
        
        Relations:
        - a = b: a and b are adjacent terminals in a production
        - a < b: a is followed by non-terminal B and b ∈ FIRSTVT(B)
        - a > b: b ∈ LASTVT(B) and B is followed by terminal a
        - $ < FIRSTVT(S) and LASTVT(S) > $
        """
        if not self.is_operator_grammar:
            return {}
            
        self.compute_firstvt()
        self.compute_lastvt()
        
        self.precedence_table = {}
        
        # All symbols including $
        all_terminals = self.terminals | {'$'}
        
        # Process each production
        for lhs, rhs in self.productions:
            if not rhs or rhs[0] in ['ε', 'epsilon', '']:
                continue
            
            # Rule 1: a = b (adjacent terminals)
            for i in range(len(rhs) - 1):
                if rhs[i] in self.terminals and rhs[i+1] in self.terminals:
                    self.precedence_table[(rhs[i], rhs[i+1])] = '='
            
            # Rule 2: a < b (terminal followed by non-terminal)
            for i in range(len(rhs) - 1):
                if rhs[i] in self.terminals and rhs[i+1] in self.non_terminals:
                    B = rhs[i+1]
                    for b in self.firstvt[B]:
                        key = (rhs[i], b)
                        if key not in self.precedence_table:
                            self.precedence_table[key] = '<'
            
            # Rule 3: a > b (non-terminal followed by terminal)
            for i in range(len(rhs) - 1):
                if rhs[i] in self.non_terminals and rhs[i+1] in self.terminals:
                    B = rhs[i]
                    for a in self.lastvt[B]:
                        key = (a, rhs[i+1])
                        if key not in self.precedence_table:
                            self.precedence_table[key] = '>'
        
        # Rule 4: Handle $ end marker
        if self.start_symbol:
            # $ < FIRSTVT(S)
            for t in self.firstvt[self.start_symbol]:
                key = ('$', t)
                if key not in self.precedence_table:
                    self.precedence_table[key] = '<'
            
            # LASTVT(S) > $
            for t in self.lastvt[self.start_symbol]:
                key = (t, '$')
                if key not in self.precedence_table:
                    self.precedence_table[key] = '>'
        
        return self.precedence_table
    
    def print_table(self):
        """Print the precedence relation table in a formatted manner."""
        if not self.is_operator_grammar:
            print("Not an operator grammar!")
            print("Errors:")
            for err in self.error_messages:
                print(f"  - {err}")
            return
        
        self.build_precedence_table()
        
        # All terminals including $
        all_terminals = sorted(self.terminals | {'$'})
        
        # Print header
        print("\nOperator Precedence Relation Table:")
        print("=" * 60)
        
        # Column header
        header = "     |"
        for t in all_terminals:
            header += f"  {t}  |"
        print(header)
        print("-" * len(header))
        
        # Rows
        for row_term in all_terminals:
            row = f"  {row_term}  |"
            for col_term in all_terminals:
                relation = self.precedence_table.get((row_term, col_term), ' ')
                row += f"  {relation}  |"
            print(row)
        
        print("=" * 60)
        
        # Print FIRSTVT and LASTVT
        print("\nFIRSTVT sets:")
        for nt in sorted(self.non_terminals):
            print(f"  FIRSTVT({nt}) = {{{', '.join(sorted(self.firstvt[nt]))}}}")
        
        print("\nLASTVT sets:")
        for nt in sorted(self.non_terminals):
            print(f"  LASTVT({nt}) = {{{', '.join(sorted(self.lastvt[nt]))}}}")
    
    def get_table_string(self) -> str:
        """Return the table as a string."""
        result = []
        
        if not self.is_operator_grammar:
            result.append("Not an operator grammar!")
            result.append("Errors:")
            for err in self.error_messages:
                result.append(f"  - {err}")
            return '\n'.join(result)
        
        self.build_precedence_table()
        
        all_terminals = sorted(self.terminals | {'$'})
        
        result.append("\nOperator Precedence Relation Table:")
        result.append("=" * 60)
        
        header = "     |"
        for t in all_terminals:
            header += f"  {t}  |"
        result.append(header)
        result.append("-" * len(header))
        
        for row_term in all_terminals:
            row = f"  {row_term}  |"
            for col_term in all_terminals:
                relation = self.precedence_table.get((row_term, col_term), ' ')
                row += f"  {relation}  |"
            result.append(row)
        
        result.append("=" * 60)
        
        result.append("\nFIRSTVT sets:")
        for nt in sorted(self.non_terminals):
            result.append(f"  FIRSTVT({nt}) = {{{', '.join(sorted(self.firstvt[nt]))}}}")
        
        result.append("\nLASTVT sets:")
        for nt in sorted(self.non_terminals):
            result.append(f"  LASTVT({nt}) = {{{', '.join(sorted(self.lastvt[nt]))}}}")
        
        return '\n'.join(result)


def process_grammars(grammars: List[str]):
    """Process multiple grammars and print their precedence tables."""
    for i, grammar_str in enumerate(grammars, 1):
        print(f"\n{'#' * 70}")
        print(f"# Grammar {i}")
        print(f"{'#' * 70}")
        print("\nInput Grammar:")
        print(grammar_str)
        
        parser = OperatorPrecedenceTable(grammar_str)
        
        if parser.is_operator_grammar:
            print("\n✓ Valid operator grammar")
        else:
            print("\n✗ Invalid operator grammar")
        
        parser.print_table()


# Test grammars
GRAMMARS = [
    """E → E + T | T
T → T * F | F
F → ( E ) | id""",
    
    """S → S + A | A
A → A * B | B
B → ( S ) | id""",
    
    """E → E + E | E * E | ( E ) | id"""
]


if __name__ == "__main__":
    process_grammars(GRAMMARS)
