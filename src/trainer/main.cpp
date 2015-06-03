// TODO:
// - Find a way to exclude results being the invalid "square" when the symbol
//   could not be rendered.

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QApplication>

QImage renderSymbol(quint16 symbol) {
  QString str = QChar(symbol);
  qDebug() << str;

  QImage img(48, 48, QImage::Format_RGB888);
  img.fill(Qt::white);

  QPainter painter;
  painter.begin(&img);

  auto font = painter.font();
  font.setPointSize(20);
  painter.setFont(font);

  painter.drawText(2, 30, str);
  painter.end();

  return img;
}

float getImageSimilarity(const QImage &img1, const QImage &img2) {
  if (img1.size() != img2.size()) {
    return 0;
  }

  // Count how many pixels are black when taking the difference.
  int black = 0;
  QSize size = img1.size();
  for (int x = 0; x < size.width(); x++) {
    for (int y = 0; y < size.height(); y++) {
      if (img1.pixel(x, y) - img2.pixel(x, y) == 0) {
        black++;
      }
    }
  }

  return float(black) / (float(size.width()) * float(size.height()));
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  auto img1 = renderSymbol(0x1024),
    img2 = renderSymbol(0x1023);

  img1.save("img1.png");
  img2.save("img2.png");

  auto diff = getImageSimilarity(img1, img2);
  qDebug() << diff;

  return 0;// app.exec();
}
