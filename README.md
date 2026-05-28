**Overview**
- **Repo**: contains simple programs for a Compiler Construction lab (experiments 1–16).

**Prerequisites**
- **Python**: Python 3 installed (use `python3`).
- **C compiler**: `gcc` for C examples (experiment 12).

**Run Python programs (experiments 1–7 use sample C inputs)**
- `code/01_count_lines_words_chars.py`: count lines, words, characters

Example:
```bash
python3 code/01_count_lines_words_chars.py
python3 code/02_count_blanks_comments.py
python3 code/03_count_functions.py
python3 code/04_count_loops_conditionals.py
python3 code/05_count_headers.py
python3 code/06_count_variables_by_type.py
python3 code/07_remove_extra_space.py
```

**Other Python examples (experiments 8–16)**
```bash
python3 code/08_regex_examples.py
python3 code/09_lex_calculator.py "1+2*(3-4)"
python3 code/10_lexical_analyzer.py code/sample_inputs/01_sample_input.c
python3 code/11_left_factoring_left_recursion.py
python3 code/13_first_follow.py
python3 code/14_ll1_parsing_table.py
python3 code/15_lr0_parsing_table.py
python3 code/16_operator_relational_table.py
```

**Compile and run C example (experiment 12)**
```bash
gcc -std=c99 -Wall code/12_regex_lexical_c.c -o 12_regex_lexical_c
./12_regex_lexical_c code/sample_inputs/01_sample_input.c
```

**Notes and tips**
- All sample C input files are in `code/sample_inputs/`.
- Run commands from the repository root directory.
- If Python scripts raise encoding errors, run with `python3 -X utf8` or set `PYTHONIOENCODING=utf-8`.

**Quick run-all (bash)**
```bash
python3 code/01_count_lines_words_chars.py
python3 code/02_count_blanks_comments.py
python3 code/03_count_functions.py
python3 code/04_count_loops_conditionals.py
python3 code/05_count_headers.py
python3 code/06_count_variables_by_type.py
python3 code/07_remove_extra_space.py
```
