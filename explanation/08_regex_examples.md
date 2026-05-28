**Concept**: Show regex operations and simple validations
**Logic**:
- Compare re.search and re.match
- Use re.findall, re.sub, re.split
- Validate password, email, and URL formats
**Sample Input**:
String: Hello 123 world
Password: abc123
Email: a@b.com
URL: http://example.com
**Sample Output**:
```
Input string: Hello 123 world
re.search(\d+): match
re.match(\d+): no match
re.findall(\w+): ['Hello', '123', 'world']
re.split(\s+): ['Hello', '123', 'world']
re.sub(\s+): Hello 123 world
Password: abc123 => valid
Email: a@b.com => valid
URL: http://example.com => valid
```
**Run**:
```bash
python3 code/08_regex_examples.py
```
