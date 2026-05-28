import sys,re
from pathlib import Path
def main():
    base = Path(__file__).resolve().parent / 'sample_inputs'
    p = Path(sys.argv[1]) if len(sys.argv) > 1 else base / '04_sample_input.c'
    try:
        text = p.read_text(encoding='utf-8', errors='ignore')
    except Exception:
        print('Error: file not found:', p)
        return
    loops = len(re.findall(r'\bfor\b|\bwhile\b|\bdo\b', text))
    conditionals = len(re.findall(r'\bif\b|\belse if\b|\bswitch\b', text))
    print('Input file:', p)
    print('Loops:', loops)
    print('Conditionals:', conditionals)
if __name__=='__main__':
    main()
