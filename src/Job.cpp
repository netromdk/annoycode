#include <QDebug>
#include <QDateTime>

#include "Job.h"
#include "Util.h"
#include "Constants.h"

Job::Job(int start, int end) : start(start), end(end) { }

void Job::run() {
  auto data = Data();
  auto x = start;
  auto img1 = Util::renderSymbol(x);

  // If the symbol is not renderable by the font then stop now. So if it is not
  // 100% similar with itself then it is not renderable, i.e. the "invalid
  // square".
  auto sim = Util::getImageSimilarity(img1, img1);
  if (floatEqual(sim, 0)) {
    emit finished(start, end, data.getSubstitutions(), 0);
    return;
  }

  auto startTime = QDateTime::currentDateTime();

  //img1.save(QString("img1.%1.png").arg(x));
  for (auto y = x + 1; y <= end; y++) {
    if (data.hasSubstitution(x, y)) {
      continue;
    }

    auto img2 = Util::renderSymbol(y);

    auto sim = Util::getImageSimilarity(img1, img2);
    if (sim < Consts::similarityThreshold) continue;

    data.addSubstitution(x, y, sim);
    qDebug() << x << QChar(x) << "~" << y << QChar(y) << sim;
    //img2.save(QString("img2.%1.png").arg(y));
  }

  auto endTime = QDateTime::currentDateTime();
  auto elapsed = startTime.msecsTo(endTime);

  emit finished(start, end, data.getSubstitutions(), elapsed);
}
