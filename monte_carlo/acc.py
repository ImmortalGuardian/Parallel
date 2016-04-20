#!/usr/bin/python

import sys, os
from subprocess import call
#sys.stdout=open('./acc.out', 'w')
for i in range(1, 13):
    call(['./mc', str(i), str(pow(10, 9))]) 
    #os.execv('./mc', ['./mc', str(i), '1000000'])
sys.exit(0)
