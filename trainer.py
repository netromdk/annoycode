# == Requires python 3.4! ==
# Find Unicode symbol pairs that look alike.
from PIL import Image, ImageFont, ImageDraw, ImageChops

# If images are >= 99.5% alike then they are accepted as "visually alike".
THRESHOLD = 99.5

FONT = ImageFont.truetype("Arial.ttf", 32)

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

if __name__ == "__main__":
    # The initial characteer will be "!" because below this the values are not
    # representable.
    initChr = ord("!")

    amount = 1000
    lastChr = initChr + amount
    for x in range(initChr, lastChr):
        for y in range(initChr, lastChr):
            if x == y: continue
            (diff, img1, img2) = compareChars(chr(x), chr(y))
            if diff < THRESHOLD: continue
            print("{} {} is {}% alike, visually similar".\
                  format((x, hex(x), chr(x)), (y, hex(y), chr(y)), diff))
            img1.save("test.{}.png".format(x))
            img1.save("test.{}.png".format(y))
