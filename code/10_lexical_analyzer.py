import sys,re
def tokenize(text):
    specs=[('ID',r'[A-Za-z_][A-Za-z0-9_]*'),('NUM',r'\d+'),('OP',r'[+\-*/=<>]'),('SYM',r'[{}();,]'),('WS',r'\s+'),('STR',r'".*?"')]
    regex='|'.join('(?P<%s>%s)'%s for s in [(name,pat) for name,pat in specs])
def make_regex():
    specs=[('ID',r'[A-Za-z_][A-Za-z0-9_]*'),('NUM',r'\d+'),('OP',r'[+\-*/=<>]'),('SYM',r'[{}();,]'),('WS',r'\s+'),('STR',r'".*?"')]
    return '|'.join('(?P<%s>%s)'%pair for pair in specs)
def main():
    if len(sys.argv)<2:
        print('Usage: python 10_lexical_analyzer.py <file>')
        return
    p=sys.argv[1]
    with open(p,'r',errors='ignore') as f:
        text=f.read()
    regex=make_regex()
    print('Input file:', p)
    print('Tokens:')
    for m in re.finditer(regex,text):
        kind=m.lastgroup
        if kind=='WS':
            continue
        print(kind,m.group())
if __name__=='__main__':
    main()
