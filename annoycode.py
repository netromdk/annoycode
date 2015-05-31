# == Requires python 3.4! ==
# Changes Unicode symbols with other symbols that looks the same.
from data import Data

if __name__ == "__main__":
    data = Data()
    if not data.load() or not data.hasMatches():
        print("Use trainer.py to generate data for use.")
        exit(-1)

    string = "ABC"
    stringEnc = string.encode("utf-8")
    (newString, subs) = data.subMatches(string)
    newStringEnc = newString.encode("utf-8")

    inCnt = len(stringEnc)
    outCnt = len(newStringEnc)
    incPerc = float(outCnt) / float(inCnt) * 100 - 100

    print("Input:  ", string)
    print("        ", stringEnc)
    print("Output: ", newString)
    print("        ", newStringEnc)
    print("{} substitutions, {} -> {} bytes, +{}%".\
          format(subs, inCnt, outCnt, incPerc))
