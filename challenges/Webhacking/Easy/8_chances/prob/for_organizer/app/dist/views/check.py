import string
k1 = ['union', 'update', 'sleep', 'concat', 'like', 'set', '@', '!', '%', '_', '\t','\n','\r','\v','\f', '/', '*', '#']
k2 = string.punctuation + string.whitespace + string.digits

def waf1(data):
    for keyword in k1:
        if keyword in data.lower():
            return True
    return False

def waf2(data):
    for keyword in k2:
        if keyword in data.lower():
            return True
    return False