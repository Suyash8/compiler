"""Example 5: Splitting text with re.split()."""
import re


def main():
    text = "one,two three:four|five"
    pattern = r"[,:\s|]+"

    split_result = re.split(pattern, text)
    print("Split result:", split_result)


if __name__ == "__main__":
    main()
