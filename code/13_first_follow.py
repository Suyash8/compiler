import sys
from collections import defaultdict
def compute_first(grammar):
    first=defaultdict(set)
    changed=True
    while changed:
        changed=False
        for A,prods in grammar.items():
            for prod in prods:
                if prod==['epsilon']:
                    if 'epsilon' not in first[A]:
                        first[A].add('epsilon');changed=True
                else:
                    for X in prod:
                        before=len(first[A])
                        if X.islower() or not X.isalpha():
                            first[A].add(X);break
                        else:
                            first[A]|=(first[X]-set(['epsilon']))
                            if 'epsilon' not in first[X]:
                                break
                        if len(first[A])>before:changed=True
    return first
def example():
    G={'E':[ ['T','E\''] ],"E\'":[['+','T','E\''],['epsilon']], 'T':[['F','T\'']],"T\'":[['*','F','T\''],['epsilon']],'F':[['(','E',')'],['id']]}
    f=compute_first(G)
    print('FIRST sets:')
    for k in f:
        print(k, sorted(f[k]))
if __name__=='__main__':
    example()
