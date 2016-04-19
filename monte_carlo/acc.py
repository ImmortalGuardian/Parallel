#!/usr/bin/python

import sys, os
from subprocess import call
#sys.stdout=open('./acc.out', 'w')
for i in range(1, 13):
    call(['./mc', str(i), '1000000000']) 
    #os.execv('./mc', ['./mc', str(i), '1000000'])
sys.exit(0)
