#include <QDebug>
#include <QApplication>

#include "Util.h"
#include "Data.h"
#include "Constants.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  auto data = Data();
  data.load();

  auto initSym = data.getOffset(),
    amount = 1000,
    end = initSym + amount,
    count = 0,
    stopAt = -1;

  for (auto x = initSym; x < end; x++) {
    for (auto y = initSym; y < end; y++) {
      if (x == y) continue;

      if (data.hasSubstitution(x, y)) {
        continue;
      }

      auto img1 = Util::renderSymbol(x),
        img2 = Util::renderSymbol(y);

      auto sim = Util::getImageSimilarity(img1, img2);
      if (sim < Consts::similarityThreshold) continue;

      data.addSubstitution(x, y, sim);

      count++;
      qDebug() << qPrintable(QString("#%1:").arg(count)) << x << QChar(x) << "~"
               << y << QChar(y) << sim;

      //img1.save(QString("img1.%1.png").arg(x));
      //img2.save(QString("img2.%1.png").arg(y));

      if (count == stopAt) break;
    }

    if (count == stopAt) break;

    // Mark offset to which a full search has currently been done.
    data.setOffset(x);
  }

  data.save();
  return 0;
}
