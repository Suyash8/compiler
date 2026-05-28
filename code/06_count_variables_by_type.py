import sys,re
from pathlib import Path
def main():
    base = Path(__file__).resolve().parent / 'sample_inputs'
    p = Path(sys.argv[1]) if len(sys.argv) > 1 else base / '06_sample_input.c'
    try:
        text = p.read_text(encoding='utf-8', errors='ignore')
    except Exception:
        print('Error: file not found:', p)
        return
    types = ['int','char','float','double','long','short']
    print('Input file:', p)
    for t in types:
        pat = re.compile(r'\b'+t+r'\b[^;=\n]*;')
        m = pat.findall(text)
        print(t+':', len(m))
if __name__=='__main__':
    main()
