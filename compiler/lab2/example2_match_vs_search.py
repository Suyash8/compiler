#!/usr/bin/env python3
"""Example 2: re.match() vs re.search() demonstration."""
import re


def main():
    text = "Hello world!"
    pattern = r"Hello"

    match_result = re.match(pattern, text)
    search_result = re.search(pattern, text)

    print("match() result:", match_result)
    print("search() result:", search_result)


if __name__ == "__main__":
    main()
