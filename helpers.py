from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    # TODO
    linesf1 = a.split('\n')
    linesf2 = b.split('\n')
    same = list(set(linesf1) & set(linesf2))
    return same


def sentences(a, b):
    """Return sentences in both a and b"""

    # TODO
    sentencef1 = sent_tokenize(a)
    sentencef2 = sent_tokenize(b)
    same = list(set(sentencef1) & set(sentencef2))
    return same


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # TODO
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
    return same