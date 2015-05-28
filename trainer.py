# == Requires python 3.4! ==
# Find Unicode symbol pairs that look alike.
from PIL import Image, ImageFont, ImageDraw, ImageChops

# If images are >= 99.5% alike then they are accepted as "visually alike".
THRESHOLD = 99.5

def renderImage(char):
    if len(char) != 1:
        raise Exception("Use only one character!")
    font = ImageFont.truetype("Arial.ttf", 32)
    image = Image.new("RGB", (48, 48), (255, 255, 255))
    draw = ImageDraw.Draw(image)
    draw.text((2, 2), char, (0, 0, 0), font = font)
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
    return percDiff

if __name__ == "__main__":
    lst = [("\xE5", "\x61"), ("a", "a"), ("O", "0"), ("v", "u"), ("8", "B")]
    for pair in lst:
        diff = compareChars(*pair)
        print("{} is {}% alike, visually similar = {}".\
              format(pair, diff, diff >= THRESHOLD))
