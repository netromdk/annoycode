# Implement a Data class that saves/loads the binary data file,
#
# - set/get offset
#
# - Must also check validity of loaded data, e.g. if offset is an integer and
#   matches a dictionary of correct format.

import pickle
from constants import *

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

    def _getKey(self, x, y):
        return (min(x, y), max(x, y))

    def isMatch(self, x, y):
        return self._getKey(x, y) in self.matches

    def addMatch(self, x, y, sim):
        self.matches[self._getKey(x, y)] = sim
