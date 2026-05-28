import sys
from pathlib import Path
def main():
    base = Path(__file__).resolve().parent / 'sample_inputs'
    p = Path(sys.argv[1]) if len(sys.argv) > 1 else base / '01_sample_input.c'
    try:
        text = p.read_text(encoding='utf-8', errors='ignore')
    except Exception:
        print('Error: file not found:', p)
        return
    lines = text.count('\n') + (0 if text == '' or text.endswith('\n') else 1)
    words = len(text.split())
    chars = len(text)
    print('Input file:', p)
    print('Lines:', lines)
    print('Words:', words)
    print('Characters:', chars)
if __name__=='__main__':
    main()
