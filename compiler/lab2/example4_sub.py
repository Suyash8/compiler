"""Example 4: Substituting text using re.sub()."""
import re


def main():
    text = "My phone number is 123-456-7890."
    pattern = r"\d"

    result = re.sub(pattern, "X", text)
    print("Substituted text:", result)


if __name__ == "__main__":
    main()
