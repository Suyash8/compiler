import sys,re
def demo(s):
    print('Input string:', s)
    print('re.search(\\d+):', 'match' if re.search(r'\d+', s) else 'no match')
    print('re.match(\\d+):', 'match' if re.match(r'\d+', s) else 'no match')
    print('re.findall(\\w+):', re.findall(r'\w+', s))
    print('re.split(\\s+):', re.split(r'\s+', s))
    print('re.sub(\\s+):', re.sub(r'\s+', ' ', s))
def validate_password(p):
    print('Password:', p, '=>', 'valid' if re.match(r'^(?=.*[A-Za-z])(?=.*\d).{6,}$', p) else 'invalid')
def validate_email(e):
    print('Email:', e, '=>', 'valid' if re.match(r'^[\w\.\-]+@[\w\-]+\.[A-Za-z]{2,}$', e) else 'invalid')
def validate_url(u):
    print('URL:', u, '=>', 'valid' if re.match(r'^https?://', u) else 'invalid')
if __name__=='__main__':
    s='Hello 123 world'
    demo(s)
    validate_password('abc123')
    validate_email('a@b.com')
    validate_url('http://example.com')
