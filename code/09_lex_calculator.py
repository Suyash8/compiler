import sys,re
def tokenize(expr):
    spec=[('NUM',r'\d+(?:\.\d+)?'),('PLUS',r'\+'),('MINUS',r'-'),('MUL',r'\*'),('DIV',r'/'),('LP',r'\('),('RP',r'\)'),('WS',r'\s+')]
    rg='|'.join('(?P<%s>%s)'%p for p in spec)
    for m in re.finditer(rg,expr):
        k=m.lastgroup
        if k!='WS':
            yield (k,m.group())
def precedence(op):
    if op in ('PLUS','MINUS'):
        return 1
    if op in ('MUL','DIV'):
        return 2
    return 0
def apply_op(op,a,b):
    if op=='PLUS':
        return a+b
    if op=='MINUS':
        return a-b
    if op=='MUL':
        return a*b
    if op=='DIV':
        return a/b
    return 0
def evaluate(tokens):
    vals=[]
    ops=[]
    for k,v in tokens:
        if k=='NUM':
            vals.append(float(v))
        elif k=='LP':
            ops.append(k)
        elif k=='RP':
            while ops and ops[-1]!='LP':
                op=ops.pop()
                b=vals.pop();a=vals.pop()
                vals.append(apply_op(op,a,b))
            if ops and ops[-1]=='LP':
                ops.pop()
        else:
            while ops and ops[-1]!='LP' and precedence(ops[-1])>=precedence(k):
                op=ops.pop()
                b=vals.pop();a=vals.pop()
                vals.append(apply_op(op,a,b))
            ops.append(k)
    while ops:
        op=ops.pop()
        b=vals.pop();a=vals.pop()
        vals.append(apply_op(op,a,b))
    return vals[0] if vals else 0
def main():
    expr = sys.argv[1] if len(sys.argv)>1 else ''
    if expr=='':
        print('Usage: python 09_lex_calculator.py "expression"')
        return
    toks=list(tokenize(expr))
    print('Input:', expr)
    print('Tokens:')
    for k,v in toks:
        print(k,v)
    print('Result:', evaluate(toks))
if __name__=='__main__':
    main()
