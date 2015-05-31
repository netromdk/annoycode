# -*- coding: utf-8 -*-
import sys

def checkPyVer():
    if sys.version_info < (3, 0, 0):
        sys.stderr.write("You need python 3 or later to run this program\n")
        exit(-1)

# Returns tuple of (x, y) where x <= y.
def orderTuple(x, y):
    return (min(x, y), max(x, y))
