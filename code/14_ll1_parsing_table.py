import sys
from collections import defaultdict
def build_table(grammar,first,follow):
    table=defaultdict(dict)
    for A,prods in grammar.items():
        for prod in prods:
            first_alpha=set()
            for X in prod:
                if not X.isalpha() or X.islower():
                    first_alpha.add(X);break
                first_alpha|=(first[X]-set(['epsilon']))
                if 'epsilon' not in first[X]:break
            for a in first_alpha:
                table[A][a]=prod
            if any('epsilon' in first.get(X,set()) for X in prod) or prod==['epsilon']:
                for b in follow[A]:
                    table[A][b]=prod
    return table
def example():
    G={'E':[ ['T','E\''] ],"E\'":[['+','T','E\''],['epsilon']], 'T':[['F','T\'']],"T\'":[['*','F','T\''],['epsilon']],'F':[['(','E',')'],['id']]}
    from collections import defaultdict
    first=defaultdict(set)
    follow=defaultdict(set);follow['E'].add('$')
    for k in G:first[k].add('id')
    table=build_table(G,first,follow)
    print('LL(1) parsing table:')
    for A in table:
        print(A, table[A])
if __name__=='__main__':
    example()
