#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Inspects a file by printing the UTF-8 encoded contents to STDOUT.

from utils import checkPyVer
checkPyVer()

import sys

def usage():
    print("usage: {} <input file>".format(sys.argv[0]))
    print("Writes contents of file as UTF-8 encoded content to STDOUT.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        usage()
        exit(-1)

    inPath = sys.argv[1]
    inData = ""
    try:
        with open(inPath, mode = "r") as f:
            inData = f.read()
    except:
        print("Could not read from input file: {}".format(inPath))
        exit(-1)

    if len(inData) == 0:
        print("Input file is empty: {}".format(inPath))
        exit(-1)

    enc = inData.encode("utf-8")
    print("=== {} raw bytes ===".format(len(enc)))
    print(enc)
    print("\n=== {} rendered symbols ===".format(len(inData)))
    print(inData)
