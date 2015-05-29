# == Requires python 3.4! ==
# Find Unicode symbol pairs that look alike.
import pickle
from PIL import Image, ImageFont, ImageDraw, ImageChops

# If images are >= 99.5% alike then they are accepted as "visually alike".
THRESHOLD = 99.5

def findFont():
    fonts = ["Arial Unicode.ttf", "Arial.ttf"]
    for name in fonts:
        try:
            font = ImageFont.truetype(name, 32)
            return font
        except: pass
    raise Exception("Could not find any suitable fonts!")

FONT = findFont()

def renderImage(char):
    if len(char) != 1:
        raise Exception("Use only one character!")
    image = Image.new("RGB", (36, 44), (255, 255, 255))
    draw = ImageDraw.Draw(image)
    draw.text((2, 2), char, (0, 0, 0), font = FONT)
    return image

def compareChars(ch1, ch2):
    img1 = renderImage(ch1)
    img2 = renderImage(ch2)
    diff = ImageChops.difference(img1, img2)
    (w, h) = img1.size
    black = 0
    for x in range(0, w):
        for y in range(0, h):
            pix = diff.getpixel((x, y))
            if pix == (0, 0, 0):
                black += 1
    percDiff = float(black) / float(w * h) * 100.0
    return (percDiff, img1, img2)

def save(offset, matches):
    data = {"offset": offset,
            "matches": matches}
    f = open("matches.dat", mode = "wb+")
    pickle.dump(data, f)

def load():
    try:
        f = open("matches.dat", mode = "rb")
        return pickle.load(f)
    except:
        return None

if __name__ == "__main__":
    matches = {}

    # Stop when this amount of pairs have been found.
    stopAt = 5

    # The initial characteer will be "!" because below this the values are not
    # graphically representable.
    offset = ord("!")

    data = load()
    if data:
        if "offset" in data:
            offset = int(data["offset"])
        if "matches" in data:
            matches = data["matches"]

    amount = 1000
    initChr = offset
    lastChr = initChr + amount
    print("Searching pairs from code point {} to {}".format(initChr, lastChr))
    print("Stopping after {} matches".format(stopAt))

    found = 0
    for x in range(initChr, lastChr):
        for y in range(initChr, lastChr):
            if x == y: continue

            (diff, img1, img2) = compareChars(chr(x), chr(y))
            if diff < THRESHOLD: continue

            key = (x, y)
            key2 = (y, x)
            if key in matches or key2 in matches:
                continue

            matches[key] = diff
            found += 1

            print("#{}: {} and {} are {}% similar".\
                  format(found, (x, hex(x), chr(x)), (y, hex(y), chr(y)), diff))
            img1.save("test.{}.png".format(x))
            img2.save("test.{}.png".format(y))

            if found == stopAt: break
        if found == stopAt: break

        # Mark offset to which a full search has currently been done.
        offset = x

    print("Offset: {}".format(offset))
    print("{} matches found: {}".format(len(matches), matches))

    save(offset, matches)
