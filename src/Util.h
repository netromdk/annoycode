#ifndef UTIL_H
#define UTIL_H

#include <QImage>

class Util {
public:
  static QImage renderSymbol(quint16 symbol);

  static float getImageSimilarity(const QImage &img1, const QImage &img2);

  static inline bool isColor(const QRgb &color, int r, int g, int b);
  static inline bool isWhite(const QRgb &color);
  static inline bool isBlack(const QRgb &color);

  static QString formatTime(qint64 ms);
};

inline bool floatEqual(double d1, double d2) {
  return qFuzzyCompare(d1 + 1, d2 + 1);
}

#endif // UTIL_H
