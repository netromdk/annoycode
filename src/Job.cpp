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
  auto startTime = QDateTime::currentDateTime();

  for (auto y = x + 1; y <= end; y++) {
    if (data.hasSubstitution(x, y)) {
      continue;
    }

    auto img2 = Util::renderSymbol(y);

    auto sim = Util::getImageSimilarity(img1, img2);
    if (sim < Consts::similarityThreshold) continue;

    data.addSubstitution(x, y, sim);
    qDebug() << x << QChar(x) << "~" << y << QChar(y) << sim;
  }

  auto endTime = QDateTime::currentDateTime();
  auto elapsed = startTime.msecsTo(endTime);

  emit finished(start, end, data.getSubstitutions(), elapsed);
}
