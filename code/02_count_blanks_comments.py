import sys,re
from pathlib import Path
def main():
    base = Path(__file__).resolve().parent / 'sample_inputs'
    p = Path(sys.argv[1]) if len(sys.argv) > 1 else base / '02_sample_input.c'
    try:
        text = p.read_text(encoding='utf-8', errors='ignore')
    except Exception:
        print('Error: file not found:', p)
        return
    blanks = text.count('\n') + (0 if text == '' or text.endswith('\n') else 1)
    single = len(re.findall(r'//.*', text))
    multi = len(re.findall(r'/\*[\s\S]*?\*/', text))
    print('Input file:', p)
    print('Blank lines:', blanks)
    print('Single-line comments:', single)
    print('Multi-line comments:', multi)
if __name__=='__main__':
    main()
