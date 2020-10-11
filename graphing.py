# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

def test_write(filename, iters=1, size=[101], program_name="main", numcores=[1]):
    f = open(filename, "w")
    
    f.write("make clean\nmake all\n")
    f.write("./main 101 10 1\nmv gmon.out gmon.sum\n")
    
    for i in numcores:
        for j in size:
            f.write("./{} {} {} {}\n".format(program_name, j, iters, i))
            #f.write("gprof -s {} gmon.out gmon.sum\n".format(program_name))
            f.write('echo "{}"\n'.format("{} {} complete".format(i, j)))
    
    f.write("gprof main gmon.sum > profile.txt")
    
    f.close()

def main():
    numcores = [1, 2, 4, 8, 16, 32]
    size = [101, 201, 301, 401, 501, 601, 701, 801, 901]
    small_size = [101, 201, 301, 401, 501]
    #Small Tests
    #test_write("profile.sh", 10, size=size, numcores=numcores)
    test_write("profile.sh", 10, size=size, numcores=numcores)
    
    #Large Tests
#    test_write("profile.sh", 10, size=size, numcores=numcores)
#    test_write("naive.sh", 10, size=size, numcores=numcores)
#    
main()