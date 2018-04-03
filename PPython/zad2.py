def parse_text(text):
    words = text.split("=")
    right = words[1].strip()
    left = words[0].split("+")
    for i in range(len(left)):
        left[i] = left[i].strip()
    
    letterDict = {}
    for letter in right:
        letterDict[letter] = 0
    
    for i in range(len(left)):
        for letter in left[i]:
            letterDict[letter] = 0
    
    return (left, right, letterDict)

def getFirsts(left, right, dictionary):
    firsts = {}
    for d in dictionary:
        firsts[d] = False
        if right[0] == d:
            firsts[d] = True
            continue
        
        for word in left:
            if word[0] == d:
                firsts[d] = True
                continue

    return firsts

def eval(left, right, firsts, dictionary, index):
    if (len(dictionary) == index):
        return evaluateAll(left, right, dictionary)

    i = 0
    d = ''
    for letter in dictionary:
        if i == index:
            d = letter
            break
        i += 1

    start = 0
    if firsts[d] == True:
        start = 1
    for i in range(start, 10):
        dictionary[d] = i
        if eval(left, right, firsts, dictionary, index + 1) == True:
            return True

    return False

def wordToNumber(word, dictionary):
    number = 0
    for letter in word:
        number *= 10
        number += dictionary[letter]
    return number

def evaluateAll(left, right, dictionary):
    sumRight = wordToNumber(right, dictionary)
    sumLeft = 0
    for word in left:
        sumLeft += wordToNumber(word, dictionary)
    return sumLeft == sumRight

def evaluate(text):
    (left, right, letters) = parse_text(text)
    firsts = getFirsts(left, right, letters)
    if eval(left, right, firsts, letters, 0):
        return letters

dictionary = evaluate("SEND + MORE = MONEY")
dictionary = evaluate("CIACHO + CIACHO = NADWAGA")
print(dictionary)