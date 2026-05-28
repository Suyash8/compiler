import sys
def example_transforms():
    grammar={'A':['Aa','b']}
    print('Input grammar:', grammar)
    grammar2={'A':['b','A\'']}
    print('Output grammar:', grammar2)
if __name__=='__main__':
    example_transforms()
