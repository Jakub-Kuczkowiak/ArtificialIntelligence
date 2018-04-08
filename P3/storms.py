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
generate(input)

file.close()