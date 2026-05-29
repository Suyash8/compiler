"""Example 3: Using re.findall() to get all occurrences."""
import re


def main():
    text = "Cats are smarter than dogs. Dogs are amazing!"
    pattern = r"[Dd]ogs?"

    matches = re.findall(pattern, text)
    print("All matches:", matches)


if __name__ == "__main__":
    main()
