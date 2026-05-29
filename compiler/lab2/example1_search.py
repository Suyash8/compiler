#!/usr/bin/env python3
"""Example 1: Basic re.search() from the PDF transcription."""
import re


def main():
    text = "Hello world!"
    pattern = r"world"

    match = re.search(pattern, text)
    if match:
        print("Pattern found at index:", match.start())
    else:
        print("Pattern not found.")


if __name__ == "__main__":
    main()
