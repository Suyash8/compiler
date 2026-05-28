def main():
    ops=['+','-','*','/','^']
    prec={'+':1,'-':1,'*':2,'/':2,'^':3}
    assoc={'+':'L','-':'L','*':'L','/':'L','^':'R'}
    for o in ops:
        print(o,'precedence',prec[o],'assoc',assoc[o])
if __name__=='__main__':
    main()
