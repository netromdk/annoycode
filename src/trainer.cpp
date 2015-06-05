#include <QDebug>
#include <QApplication>
#include <QCommandLineParser>

#include "Util.h"
#include "Data.h"
#include "Constants.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("Trains data by finding pairs of Unicode "
                                   "symbols that renders to similar text.");
  parser.addHelpOption();

  QCommandLineOption
    fileOpt(QStringList() << "f" << "file",
            QString("Use substitutions from <data file>. Will "
                    "use \"%1\" otherwise.").arg(Consts::defaultDataFile),
            "data file");
  parser.addOption(fileOpt);

  QCommandLineOption
    stopAtOpt(QStringList() << "s" << "stop",
              QString("Stop after <matches> are found. Will use \"%1\" "
                      "otherwise.").arg(Consts::trainerStopAt),
              "matches");
  parser.addOption(stopAtOpt);

  parser.process(app);

  QString path;
  if (parser.isSet(fileOpt)) {
    path = parser.value(fileOpt);
  }

  auto data = Data(path);
  data.load();

  auto initSym = data.getOffset(),
    amount = 1000,
    end = initSym + amount,
    count = 0,
    stopAt = Consts::trainerStopAt;

  if (parser.isSet(stopAtOpt)) {
    bool ok;
    QString sval = parser.value(stopAtOpt);
    int val = sval.toUInt(&ok);
    if (ok && val > 0) {
      stopAt = val;
    }
    else {
      qWarning() << "Invalid stop-at value:" << sval;
    }
  }

  qDebug() << "Searching in range" << initSym << "to" << end;
  qDebug() << "Stopping after" << stopAt << "matches are found";

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
