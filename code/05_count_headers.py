import sys,re
from pathlib import Path
def main():
    base = Path(__file__).resolve().parent / 'sample_inputs'
    p = Path(sys.argv[1]) if len(sys.argv) > 1 else base / '05_sample_input.c'
    try:
        text = p.read_text(encoding='utf-8', errors='ignore')
    except Exception:
        print('Error: file not found:', p)
        return
    incs = re.findall(r'#\s*include\s*[<\"].+?[>\"]', text)
    print('Input file:', p)
    print('Header count:', len(incs))
    for h in incs:
        print(h)
if __name__=='__main__':
    main()
