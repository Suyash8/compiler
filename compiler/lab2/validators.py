"""Validators: password, email, and URL validators with tests."""
import re


def validate_password(password: str) -> bool:
    pattern = r"^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d@$!%*?&]{8,}$"
    return bool(re.match(pattern, password))


def validate_email(email: str) -> bool:
    pattern = r"^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$"
    return bool(re.match(pattern, email))


def validate_url(url: str) -> bool:
    pattern = (
        r"^(https?://)"      # http:// or https://
        r"(www\.)?"          # optional www.
        r"([a-zA-Z0-9-]+)"   # domain name part 1
        r"(\.[a-zA-Z0-9-]+)+"# domain dot-extension
        r"([/?].*)?$"        # optional path or parameters
    )
    return bool(re.match(pattern, url))


def _run_tests():
    print("Password tests:")
    passwords = ["mypassword", "mypassword1", "Pass123!", "p@ssw0rd?"]
    for pwd in passwords:
        print(f"{pwd}: {validate_password(pwd)}")

    print("\nEmail tests:")
    emails = [
        "john.doe@example.com",
        "john_doe123@domain.net",
        "invalid-email@.com",
        "@nodomain.com",
        "just-text",
    ]
    for e in emails:
        print(f"{e}: {validate_email(e)}")

    print("\nURL tests:")
    urls = [
        "http://www.google.com",
        "https://example.org/path/to/page?param=value",
        "ftp://not-valid.com",
        "https://domain",
        "https://sub.domain.co.uk",
    ]
    for u in urls:
        print(f"{u}: {validate_url(u)}")


if __name__ == "__main__":
    _run_tests()
