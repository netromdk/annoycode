# Must also check validity of loaded data, e.g. if offset is an integer and
# matches a dictionary of correct format.

import pickle
from constants import *
from utils import orderTuple

class Data:
    def __init__(self):
        self.offset = INIT_SYMBOL_OFFSET

        # Matched pairs of the form (symbol 1, symbol 2) = % similarity.
        self.matches = {}

    def load(self, path = DEFAULT_DATA_FILE):
        try:
            f = open(path, mode = "rb")
            data = pickle.load(f)
            if "offset" in data:
                self.offset = int(data["offset"])
            if "matches" in data:
                self.matches = data["matches"]
        except: pass

    def save(self, path = DEFAULT_DATA_FILE):
        data = {"offset": self.offset,
                "matches": self.matches}
        f = open(path, mode = "wb+")
        pickle.dump(data, f)

    def isMatch(self, x, y):
        return orderTuple(x, y) in self.matches

    def addMatch(self, x, y, sim):
        self.matches[orderTuple(x, y)] = sim
