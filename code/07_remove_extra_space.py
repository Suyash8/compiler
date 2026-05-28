import sys,re
from pathlib import Path
def main():
    base = Path(__file__).resolve().parent / 'sample_inputs'
    p = Path(sys.argv[1]) if len(sys.argv) > 1 else base / '07_sample_input.c'
    try:
        text = p.read_text(encoding='utf-8', errors='ignore')
    except Exception:
        print('Error: file not found:', p)
        return
    lines = text.splitlines()
    out = []
    prev_blank = False
    for L in lines:
        s = L.strip()
        if s == '' and prev_blank:
            continue
        prev_blank = (s == '')
        s = re.sub(r"\s+"," ",s)
        out.append(s + ('\n' if s != '' else '\n'))
    print('Input file:', p)
    print('Cleaned output:')
    print(''.join(out))
if __name__=='__main__':
    main()
