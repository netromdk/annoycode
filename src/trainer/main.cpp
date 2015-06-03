// TODO:
// - Find a way to exclude results being the invalid "square" when the symbol
//   could not be rendered.

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QApplication>

QImage renderSymbol(quint16 symbol) {
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

inline bool isColor(const QRgb &color, int r, int g, int b) {
  return qRed(color) == r && qGreen(color) == g && qBlue(color) == b;
}

inline bool isWhite(const QRgb &color) {
  return isColor(color, 255, 255, 255);
}

inline bool isBlack(const QRgb &color) {
  return isColor(color, 0, 0, 0);
}

float getImageSimilarity(const QImage &img1, const QImage &img2) {
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

  return float(black) / float(total);
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  quint16 initSym = QChar('!').unicode(),
    amount = 1000,
    end = initSym + amount;

  for (auto x = initSym; x < end; x++) {
    for (auto y = initSym; y < end; y++) {
      if (x == y) continue;

      auto img1 = renderSymbol(x),
        img2 = renderSymbol(y);

      // Disregard if <99.7% similarity.
      auto diff = getImageSimilarity(img1, img2);
      if (diff < 0.997) continue;

      qDebug() << x << QChar(x) << "~" << y << QChar(y) << diff;

      img1.save(QString("img1.%1.png").arg(x));
      img2.save(QString("img2.%1.png").arg(y));
    }
  }

  return 0;// app.exec();
}
