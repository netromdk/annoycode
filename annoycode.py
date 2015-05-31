# == Requires python 3.4! ==
# Changes Unicode symbols with other symbols that looks the same.
from data import Data

if __name__ == "__main__":
    data = Data()
    if not data.load() or not data.hasMatches():
        print("Use trainer.py to generate data for use.")
        exit(-1)

    string = "A B C ! -"
    (newString, subs) = data.subMatches(string)

    print("Input:  ", string)
    print("        ", string.encode("utf-8"))
    print("Output: ", newString)
    print("        ", newString.encode("utf-8"))
    print("{} substitutions".format(subs))
