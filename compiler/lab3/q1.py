import re

code = """
int main() {
    return 0;
}
"""

pattern_keywords = re.compile(r"\b(int|return|main|if|else|for|while|class|struct)\b")
print(pattern_keywords.findall(code))