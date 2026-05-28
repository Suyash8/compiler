import sys,re
from pathlib import Path
def main():
    base = Path(__file__).resolve().parent / 'sample_inputs'
    p = Path(sys.argv[1]) if len(sys.argv) > 1 else base / '03_sample_input.c'
    try:
        text = p.read_text(encoding='utf-8', errors='ignore')
    except Exception:
        print('Error: file not found:', p)
        return
    pattern = re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_\s\*]*\b\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\([^;]*\)\s*\{')
    funcs = len(pattern.findall(text))
    print('Input file:', p)
    print('Functions:', funcs)
if __name__=='__main__':
    main()
