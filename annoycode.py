#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Changes Unicode symbols with other symbols that looks the same.

from utils import checkPyVer
checkPyVer()

import sys
from data import Data

def usage():
    print("usage: {} <input file> <output file>".format(sys.argv[0]))
    print("Substitutes symbols with ones that look similar.")

if __name__ == "__main__":
    if len(sys.argv) != 3:
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

    outPath = sys.argv[2]
    outFile = None
    try:
        outFile = open(outPath, mode = "w+")
    except:
        print("Could not open output file for writing: {}".format(outPath))
        exit(-1)

    data = Data()
    if not data.load() or not data.hasMatches():
        print("Use trainer.py to generate data for use.")
        exit(-1)

    (out, subs) = data.subMatches(inData)
    if subs == 0:
        print("Nothing substituted")
        exit(1)

    outFile.write(out)
    outFile.close()

    inEnc = inData.encode("utf-8")
    outEnc = out.encode("utf-8")

    inCnt = len(inEnc)
    outCnt = len(outEnc)
    incPerc = float(outCnt) / float(inCnt) * 100 - 100

    print("{} substitutions, {} -> {} bytes, {}{}%".\
          format(subs, inCnt, outCnt, "+" if incPerc > 0 else "", incPerc))
