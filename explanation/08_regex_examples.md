**Concept**: Regular expression operations and validations
**Logic**:
- Compare re.search and re.match on two different strings
- Use re.findall, re.split, and re.sub on a sample sentence
- Validate password, email, and URL values using regex
**Sample Input**:
Embedded samples used in the program:
```text
abc 123 xyz
123abc
Hello   world, this   is   regex
abc123
abc
a@b.com
user@site
http://example.com
ftp://example.com
```
**Sample Output**:
```
Regex Operations

Sample text: abc 123 xyz
re.search(\d+): 123
re.match(\d+): no match

Sample text: 123abc
re.search(\d+): 123
re.match(\d+): 123

Sample text: Hello   world, this   is   regex
re.findall(\w+): ['Hello', 'world', 'this', 'is', 'regex']
re.split(\s+): ['Hello', 'world,', 'this', 'is', 'regex']
re.sub(\s+): Hello world, this is regex

Validations
Password: abc123 => valid
Password: abc => invalid
Email: a@b.com => valid
Email: user@site => invalid
URL: http://example.com => valid
URL: ftp://example.com => invalid
```
**Run**:
```bash
python3 code/08_regex_examples.py
```
