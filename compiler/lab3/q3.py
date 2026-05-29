import re

code = """
int x = 42;
double pi = 3.14159;
"""

pattern_numbers = re.compile(r'\b\d+(?:\.\d+)?\b')

# findall() now correctly returns full matches
matches = pattern_numbers.findall(code)

print("Matched Numbers:", matches)
