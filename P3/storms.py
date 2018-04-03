import sys

file = open("storms.pl", "w+")

def V(i, j):
    return 'V%d_%d' % (i,j)

def domains(Vs):
    return [ q + ' in 0..1' for q in Vs ]

def rowSum(index, rows):
    return []

def columnSum(index, column):
    return []

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

def generate(input):
    (rows, columns, listDecl) = input
    rowsNumber = len(rows)
    columnsNumber = len(columns)

    variables = [ V(i,j) for i in range(rowsNumber) for j in range(columnsNumber)]
    domain = domains(variables)

    file.write(':- use_module(library(clpfd)).')
    file.write('\n')
    file.write('solve([' + ', '.join(variables) + ']) :- ')

    print_constraints(domain, 4, 70)

input = ([4, 8, 6, 2, 5, 8, 6, 2, 6, 6],
    [4, 7, 5, 2, 3, 5, 7, 2, 6, 8, 2, 2],
    [(6, 10, 1)])
generate(input)

file.close()