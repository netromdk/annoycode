#include <QPainter>

#include "Util.h"
#include "Constants.h"

QImage Util::renderSymbol(quint16 symbol) {
  QImage img(Consts::imageWidth, Consts::imageHeight, QImage::Format_RGB888);
  img.fill(Qt::white);

  QPainter painter;
  painter.begin(&img);

  auto font = painter.font();
  font.setPointSize(Consts::imageFontSize);
  painter.setFont(font);

  painter.drawText(Consts::imageTextStartX, Consts::imageTextStartY,
                   QChar(symbol));
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
  // is then deny it. Around 3996 whites for 4096 pixels which is ~0.975.
  if (black == total) {
    auto white1Perc = float(white1) / float(total),
      white2Perc = float(white2) / float(total);
    if ((white1Perc >= 0.97 && white1Perc <= 0.98) ||
        (white2Perc >= 0.97 && white2Perc <= 0.98)) {
      return 0;
    }
  }

  return float(black) / float(total);
}

QString Util::formatTime(qint64 ms) {
  constexpr qint64 Second = 1000, Minute = Second * 60, Hour = Minute * 60,
    Day = Hour * 24, Week = Day * 7;
  QString res;
  if (ms >= Week) {
    qint64 weeks = ms / Week;
    res += QString("%1w").arg(weeks);
    ms -= weeks * Week;
  }
  if (ms >= Day) {
    qint64 days = ms / Day;
    res += QString("%1%2d").arg(!res.isEmpty() ? " " : "").arg(days);
    ms -= days * Day;
  }
  if (ms >= Hour) {
    qint64 hours = ms / Hour;
    res += QString("%1%2h").arg(!res.isEmpty() ? " " : "").arg(hours);
    ms -= hours * Hour;
  }
  if (ms >= Minute) {
    qint64 minutes = ms / Minute;
    res += QString("%1%2m").arg(!res.isEmpty() ? " " : "").arg(minutes);
    ms -= minutes * Minute;
  }
  if (ms >= Second) {
    qint64 seconds = ms / Second;
    res += QString("%1%2s").arg(!res.isEmpty() ? " " : "").arg(seconds);
    ms -= seconds * Second;
  }
  if (ms > 0 || res.isEmpty()) {
    res += QString("%1%2ms").arg(!res.isEmpty() ? " " : "").arg(ms);
  }
  return res;
}
