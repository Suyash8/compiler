#!/usr/bin/env bash
set -euo pipefail
PY=python3

echo "Running example1_search.py"
$PY lab/lab2/example1_search.py
echo
echo "Running example2_match_vs_search.py"
$PY lab/lab2/example2_match_vs_search.py
echo
echo "Running example3_findall.py"
$PY lab/lab2/example3_findall.py
echo
echo "Running example4_sub.py"
$PY lab/lab2/example4_sub.py
echo
echo "Running example5_split.py"
$PY lab/lab2/example5_split.py
echo
echo "Running validators.py"
$PY lab/lab2/validators.py
echo
echo "Running lexer.py"
$PY lab/lab2/lexer.py
