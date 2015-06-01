#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Inspects a data file of substitutions, e.g. the "data.p".

from utils import checkPyVer
checkPyVer()

import sys
from data import Data

def usage():
    print("usage: {} <data file>".format(sys.argv[0]))
    print("Inspects a substitutions data file.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        usage()
        exit(-1)

    inPath = sys.argv[1]
    data = Data()
    if not data.load(inPath) or not data.hasMatches():
        print("Could not load file or invalid: {}".format(inPath))
        exit(-1)

    count = 1
    for key in data.getSortedMatchesKeys():
        sim = data.matches[key]
        print("#{}:\t'{}' ({}, {})\t  '{}' ({}, {})\t{}%".\
              format(count, chr(key[0]), key[0], hex(key[0]),\
                     chr(key[1]), key[1], hex(key[1]), sim))
        count += 1
