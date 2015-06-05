#include "Util.h"

#include <QPainter>

QImage Util::renderSymbol(quint16 symbol) {
  QImage img(48, 48, QImage::Format_RGB888);
  img.fill(Qt::white);

  QPainter painter;
  painter.begin(&img);

  auto font = painter.font();
  font.setPointSize(20);
  painter.setFont(font);

  painter.drawText(2, 30, QChar(symbol));
  painter.end();

  return img;
}

inline bool Util::isColor(const QRgb &color, int r, int g, int b) {
  return qRed(color) == r && qGreen(color) == g && qBlue(color) == b;
}

inline bool Util::isWhite(const QRgb &color) {
  return isColor(color, 255, 255, 255);
}

inline bool Util::isBlack(const QRgb &color) {
  return isColor(color, 0, 0, 0);
}

float Util::getImageSimilarity(const QImage &img1, const QImage &img2) {
  if (img1.size() != img2.size()) {
    return 0;
  }

  // Count how many pixels are black when taking the difference, and the white
  // pixels, too.
  int black = 0, white1 = 0, white2 = 0;
  QSize size = img1.size();
  for (int x = 0; x < size.width(); x++) {
    for (int y = 0; y < size.height(); y++) {
      QRgb pix1 = img1.pixel(x, y),
        pix2 = img2.pixel(x, y);
      if (isBlack(pix1 - pix2)) {
        black++;
      }
      if (isWhite(pix1)) {
        white1++;
      }
      if (isWhite(pix2)) {
        white2++;
      }
    }
  }

  // If any of the images are all white then ignore.
  auto total = size.width() * size.height();
  if (white1 == total || white2 == total) {
    return 0;
  }

  // If all is black but white level is about waht the invalid "square" symbol
  // is then deny it. Around 2201 whites for 2204 pixels which is ~0.955.
  if (black == total) {
    auto white1Perc = float(white1) / float(total),
      white2Perc = float(white2) / float(total);
    if ((white1Perc >= 0.94 && white1Perc <= 0.96) ||
        (white2Perc >= 0.94 && white2Perc <= 0.96)) {
      return 0;
    }
  }

  return float(black) / float(total);
}
