import re

def show_search_and_match(text):
    search_result = re.search(r"\d+", text)
    match_result = re.match(r"\d+", text)
    print("Sample text:", text)
    print("re.search(\\d+):", search_result.group() if search_result else "no match")
    print("re.match(\\d+):", match_result.group() if match_result else "no match")
    print()

def show_findall_split_sub(text):
    print("Sample text:", text)
    print("re.findall(\\w+):", re.findall(r"\w+", text))
    print("re.split(\\s+):", re.split(r"\s+", text.strip()))
    print("re.sub(\\s+):", re.sub(r"\s+", " ", text).strip())
    print()

def validate_password(value):
    pattern = r"^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d]{6,}$"
    print("Password:", value, "=>", "valid" if re.match(pattern, value) else "invalid")

def validate_email(value):
    pattern = r"^[\w\.-]+@[\w\.-]+\.[A-Za-z]{2,}$"
    print("Email:", value, "=>", "valid" if re.match(pattern, value) else "invalid")

def validate_url(value):
    pattern = r"^https?://[\w\.-]+(?:/[\w\./-]*)?$"
    print("URL:", value, "=>", "valid" if re.match(pattern, value) else "invalid")

def main():
    print("Regex Operations")
    print()
    show_search_and_match("abc 123 xyz")
    show_search_and_match("123abc")
    show_findall_split_sub("Hello   world, this   is   regex")
    print("Validations")
    validate_password("abc123")
    validate_password("abc")
    validate_email("a@b.com")
    validate_email("user@site")
    validate_url("http://example.com")
    validate_url("ftp://example.com")

if __name__ == "__main__":
    main()