import pickle

class Data:
    def __init__(self):
        # The initial character will be "!" because below this the values are
        # not graphically representable.
        self.offset = ord("!")

        # Matched pairs of the form (symbol 1, symbol 2) = % similarity.
        self.matches = {}

    def load(self, path = "matches.dat"):
        try:
            f = open(path, mode = "rb")
            data = pickle.load(f)
            if "offset" in data:
                self.offset = int(data["offset"])
            if "matches" in data:
                self.matches = data["matches"]
        except: pass

    def save(self, path = "matches.dat"):
        data = {"offset": self.offset,
                "matches": self.matches}
        f = open(path, mode = "wb+")
        pickle.dump(data, f)
