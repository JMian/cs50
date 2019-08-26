from nltk.tokenize import sent_tokenize

a = "foobar"
b = "barfoo"
n = 2
"""Return lines in both a and b"""
substringsf1 = []
substringsf2 = []

i = 0
for i in range(len(a) - n + 1):
    substringsf1.append(a[i:i+n])
    i += 1

i = 0
for i in range(len(b) - n + 1):
    substringsf2.append(b[i:i+n])
    i += 1

same = list(set(substringsf1) & set(substringsf2))

print(substringsf1)
print(substringsf2)
print(same)

