#include <QDebug>
#include <QApplication>

#include "Util.h"
#include "Constants.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  quint16 initSym = QChar('!').unicode(),
    amount = 1000,
    end = initSym + amount,
    count = 0;

  for (auto x = initSym; x < end; x++) {
    for (auto y = initSym; y < end; y++) {
      if (x == y) continue;

      auto img1 = Util::renderSymbol(x),
        img2 = Util::renderSymbol(y);

      auto diff = Util::getImageSimilarity(img1, img2);
      if (diff < Consts::SimilarityThreshold) continue;

      count++;
      qDebug() << qPrintable(QString("#%1:").arg(count)) << x << QChar(x) << "~"
               << y << QChar(y) << diff;

      //img1.save(QString("img1.%1.png").arg(x));
      //img2.save(QString("img2.%1.png").arg(y));
    }
  }

  return 0;// app.exec();
}
