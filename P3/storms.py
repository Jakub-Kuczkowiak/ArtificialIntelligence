import sys

file = open("storms.pl", "w+")

def V(i, j):
    return 'V%d_%d' % (i,j)

def domains(Vs):
    return [ q + ' in 0..1' for q in Vs ]

def row(index, columnsNumber, rowSum):
    result = ""
    for j in range(0, columnsNumber):
        if (j != columnsNumber - 1):
            result += V(index, j) + '+'
        else:
            result += V(index, j)
    
    result += " #= "
    result += str(rowSum)
    return result

def gen_rows(rowsNumber, columnsNumber, rowsDescription):
    result = ""
    for i in range(0, rowsNumber):
        result += row(i, columnsNumber, rowsDescription[i])
        result += ",\n"
    return result
    
def column(index, rowsNumber, columnSum):
    result = ""
    for i in range(0, rowsNumber):
        if (i != rowsNumber - 1):
            result += V(i, index) + '+'
        else:
            result += V(i, index)
    
    result += " #= "
    result += str(columnSum)
    return result

def gen_columns(rowsNumber, columnsNumber, columnsDescription):
    result = ""
    for j in range(0, columnsNumber):
        result += column(j, rowsNumber, columnsDescription[j])
        result += ",\n"
    return result

def rects3x1(n, m): # this cares that rects are at least 2x2
    result = ""
    for i in range(0, n):
        for j in range(1, m - 1):
            result = result + "(" + V(i, j) + " #= 1) #==> (" + V(i, j - 1) + " + " + V(i, j + 1) + " #> 0),\n"
    
    result += "\n"
    for j in range(0, m):
        for i in range(1, n - 1):
            result = result + "(" + V(i, j) + " #= 1) #==> (" + V(i - 1, j) + " + " + V(i + 1, j) + " #> 0),\n"

    return result

def rectangles(n, m): # cares that all areas are rects and not connected by corners
    result = ""
    for i in range(0, n - 1):
        for j in range(0, m - 1):
            result = result + V(i, j) + " + " + V(i, j + 1) + " + " + \
            V(i + 1, j) + " + " + V(i + 1, j + 1) + " #\\= 3,\n"

    return result

def print_constraints(Cs, indent, d):
    position = indent
    file.write((indent - 1) * ' ')
    for c in Cs:
        file.write(c + ',')
        position += len(c)
        if position > d:
            position = indent
            file.write('\n')
            file.write( (indent - 1) * ' ')

def print_listDeclaration(Vs):
    for i,j,val in Vs:
        file.write('%s #= %d, ' % (V(i,j), val))

def formatOutput(columnsNumber):
    heads = ""
    for i in range(0, columnsNumber):
        heads += ("H" + str(i))
        if (i != columnsNumber - 1):
            heads += ", "

    file.write("formatOutput([]) :- !.")
    file.write("\n")
    file.write("formatOutput(X) :-")
    file.write("[" + heads + "|T] = X,")
    file.write("write([" + heads + "]), write('\\n'),")
    file.write("formatOutput(T).")

def generate(input):
    (rows, columns, listDecl) = input
    rowsNumber = len(rows)
    columnsNumber = len(columns)

    variables = [ V(i,j) for i in range(rowsNumber) for j in range(columnsNumber)]
    constraints = domains(variables)
    rowsStr = gen_rows(rowsNumber, columnsNumber, rows)
    columnsStr = gen_columns(rowsNumber, columnsNumber, columns)
    rectanglesStr = rectangles(rowsNumber, columnsNumber)
    rects3v1Str = rects3x1(rowsNumber, columnsNumber)

    file.write(':- use_module(library(clpfd)).')
    file.write('\n')
    file.write('solve([' + ', '.join(variables) + ']) :- ')

    print_constraints(constraints, 4, 70)
    print_listDeclaration(listDecl)

    file.write('\n')
    file.write(rowsStr)
    file.write('\n')
    file.write(columnsStr)
    file.write('\n')
    file.write(rectanglesStr)
    file.write('\n')
    file.write(rects3v1Str)

    file.write('\n')
    file.write('    labeling([ff], [' +  ', '.join(variables) + ']).') 
    file.write('\n')

    file.write('\n')
    formatOutput(columnsNumber)
    file.write('\n')

    file.write(':- solve(X), formatOutput(X), nl.') 

input = ([4, 8, 6, 2, 5, 8, 6, 2, 6, 6],
    [4, 7, 5, 2, 3, 5, 7, 2, 6, 8, 2, 2],
    [(6, 10, 1)])

input2 = ([3, 8, 5, 3, 3, 5, 5, 5, 2, 2],
        [0, 2, 7, 7, 2, 2, 0, 7, 7, 7],
        [(0, 5, 0),
        (0, 6, 0),
        (1, 5, 1),
        (1, 6, 0)])

input3 = ([2, 2, 7, 7, 4, 4, 3, 5, 2, 8, 8, 8, 8, 6, 0],
        [4, 4, 6, 6, 2, 9, 9, 9, 2, 0, 2, 7, 7, 7, 0],
        [(13, 10, 0),
        (13, 11, 1),
        (14, 10, 0),
        (14, 11, 0),
        (3, 0, 1),
        (3, 1, 1),
        (4, 0, 1),
        (4, 1, 1),
        (3, 13, 0),
        (3, 14, 0),
        (4, 13, 1),
        (4, 14, 0),
        (9, 4, 0),
        (9, 5, 1),
        (10, 4, 0),
        (10, 5, 1),
        (2, 1, 1),
        (2, 2, 0),
        (3, 1, 1),
        (3, 2, 0)])

input4 = ([4, 4, 12, 10, 8, 0, 8, 11, 8, 7, 11, 7, 7, 5, 0],
        [10, 10, 3, 9, 9, 3, 8, 8, 5, 4, 10, 6, 9, 5, 3],
        [(13, 11, 1),
        (13, 12, 1),
        (14, 11, 0),
        (14, 12, 0),
        (3, 9, 1),
        (3, 10, 1),
        (4, 9, 0),
        (4, 10, 0),
        (2, 0, 1),
        (2, 1, 1),
        (3, 0, 0),
        (3, 1, 0),
        (11, 13, 0),
        (11, 14, 0),
        (12, 13, 0),
        (12, 14, 0),
        (7, 6, 1),
        (7, 7, 1),
        (8, 6, 1),
        (8, 7, 1)])

input5 = ([11, 11, 3, 5, 5, 11, 8, 8, 10, 8, 6, 4, 0, 7, 12, 7, 7, 7],
        [0, 5, 9, 9, 9, 4, 8, 10, 9, 11, 6, 4, 13, 13, 10, 7, 3, 0],
        [(2, 1, 0),
        (2, 2, 0),
        (3, 1, 0),
        (3, 2, 0),
        (3, 3, 0),
        (3, 4, 0),
        (4, 3, 0),
        (4, 4, 0),
        (13, 1, 0),
        (13, 2, 1),
        (14, 1, 0),
        (14, 2, 1),
        (15, 1, 0),
        (15, 2, 0),
        (16, 1, 0),
        (16, 2, 0),
        (1, 3, 1),
        (1, 4, 1),
        (2, 3, 0),
        (2, 4, 0),
        (15, 5, 0),
        (15, 6, 0),
        (16, 5, 0),
        (16, 6, 0),
        (14, 10, 0),
        (14, 11, 1),
        (15, 10, 0),
        (15, 11, 1),
        (0, 2, 1),
        (0, 3, 1),
        (1, 2, 1),
        (1, 3, 1),
        (9, 2, 1),
        (9, 3, 1),
        (10, 2, 0),
        (10, 3, 0),
        (7, 13, 0),
        (7, 14, 0),
        (8, 13, 0),
        (8, 14, 0)])

generate(input5)

file.close()