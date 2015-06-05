#include <QDebug>

#include "Job.h"
#include "Util.h"
#include "Constants.h"

Job::Job(int start, int end) : start(start), end(end) { }

void Job::run() {
  auto data = Data();

  auto x = start;
  for (auto y = x + 1; y <= end; y++) {
    if (data.hasSubstitution(x, y)) {
      continue;
    }

    auto img1 = Util::renderSymbol(x),
      img2 = Util::renderSymbol(y);

    auto sim = Util::getImageSimilarity(img1, img2);
    if (sim < Consts::similarityThreshold) continue;

    data.addSubstitution(x, y, sim);
    qDebug() << x << QChar(x) << "~" << y << QChar(y) << sim;
  }

  emit finished(start, end, data.getSubstitutions());
}
