# -*- coding: utf-8 -*-
import pickle, operator
from constants import *
from utils import orderTuple
from random import shuffle

class Data:
    def __init__(self):
        self.offset = INIT_SYMBOL_OFFSET

        # Matched pairs of the form (symbol 1, symbol 2) = % similarity.
        self.matches = {}

    # Loads data from binary file. If file not found or data is not valid then
    # defaults will be kept. The return boolean value denotes whether any
    # problems were encountered.
    def load(self, path = DEFAULT_DATA_FILE):
        data = None
        try:
            f = open(path, mode = "rb")
            data = pickle.load(f)

        except:
            print("No data file found, using defaults!")
            return False

        if not "offset" in data:
            print("No 'offset' in data!")
            return False

        if type(data["offset"]) != type(1) or data["offset"] < 0:
            print("'offset' must be a positive integer in data!")
            return False

        if not "matches" in data:
            print("No 'matches' in data!")
            return False

        if type(data["matches"]) != type({}):
            print("'matches' must be a dictionary in data!")
            return False

        self.offset = data["offset"]
        self.matches = data["matches"]
        return True

    def save(self, path = DEFAULT_DATA_FILE):
        data = {"offset": self.offset,
                "matches": self.matches}
        f = open(path, mode = "wb+")
        pickle.dump(data, f)

    def isMatch(self, x, y):
        return orderTuple(x, y) in self.matches

    def addMatch(self, x, y, sim):
        self.matches[orderTuple(x, y)] = sim

    def hasMatches(self):
        return len(self.matches) > 0

    # Returns the list of key pairs sorted by the lowest first value.
    def getSortedMatchesKeys(self):
        keys = list(self.matches.keys())
        keys.sort(key = operator.itemgetter(0))
        return keys

    # Substitutes the input character with one that looks similary to it, if
    # any. Returns None if nothing was found.
    def subMatch(self, ch):
        idx = ord(ch)

        # If there are several candidates then choose a random one.
        slist = []
        for key in self.matches:
            if key[0] == idx or key[1] == idx:
                slist.append(key)
        shuffle(slist)

        for key in slist:
            if key[0] == idx:
                return chr(key[1])
            if key[1] == idx:
                return chr(key[0])
        return None

    # Substitute similar symbols with original.
    def subMatches(self, string):
        res = []
        count = 0
        for ch in string:
            sch = self.subMatch(ch)
            if sch:
                count += 1
                res.append(sch)
            else:
                res.append(ch)
        return ("".join(res), count)
