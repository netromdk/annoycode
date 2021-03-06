#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QThreadPool>
#include <QApplication>
#include <QMutexLocker>
#include <QCommandLineParser>

#include "Job.h"
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
    threadsOpt(QStringList() << "t" << "threads",
               "Use <num> of threads to search for mathes. Defaults to using "
               "all available ones, with value \"0\".",
               "num");
  parser.addOption(threadsOpt);

  QCommandLineOption
    incSaveOpt(QStringList() << "i" << "inc-save",
               "Incrementally save results to disk.");
  parser.addOption(incSaveOpt);

  parser.process(app);

  QString path;
  if (parser.isSet(fileOpt)) {
    path = parser.value(fileOpt);
  }

  auto data = Data(path);
  data.load();

  auto initSym = data.getOffset(),
    end = 65535, // unsigned short max - 1
    threads = 0;
  auto incSave = parser.isSet(incSaveOpt);

  if (parser.isSet(threadsOpt)) {
    bool ok;
    QString sval = parser.value(threadsOpt);
    int val = sval.toUInt(&ok);
    if (ok) {
      threads = val;
    }
    else {
      qWarning() << "Invalid threads value:" << sval;
    }
  }

  if (threads == 0) {
    threads = QThread::idealThreadCount();
  }

  qDebug() << "Searching in range" << initSym << "to" << end;
  qDebug() << "Using" << threads << "threads";

  QThreadPool pool;
  pool.setMaxThreadCount(threads);

  QMutex mutex;
  qint64 minElapsed = 0, maxElapsed = 0, avgElapsed = 0;

  auto onJobFinished =
  [&data, &mutex, incSave, &minElapsed, &maxElapsed, &avgElapsed]
  (int start, int end, SubsMap matches, qint64 msElapsed) {
    QMutexLocker loacker(&mutex);

    if (msElapsed > 0) {
      if (msElapsed < minElapsed || minElapsed == 0) {
        minElapsed = msElapsed;
      }
      if (msElapsed > maxElapsed) {
        maxElapsed = msElapsed;
      }
      avgElapsed = (minElapsed + maxElapsed) / 2;
    }

    auto remPerc = 100.0 - float(end - start) / float(end) * 100.0;
    auto timeLeft = (end - start) * avgElapsed;

    QString msg =
      QString("Finished offset: %1 in %2, %3 remaining, %4% done, %5 left")
      .arg(start).arg(Util::formatTime(msElapsed)).arg(end - start)
      .arg(remPerc).arg(Util::formatTime(timeLeft));
    qDebug() << qPrintable(msg);

    auto oldCount = data.getCount();
    data.addSubstitutions(matches);
    auto count = data.getCount();

    data.setOffset(start);

    if (count > 0 && count > oldCount) {
      if (incSave) {
        data.save();
      }
      else {
        qDebug() << "Matches found:" << data.getCount();
      }
    }
  };

  for (auto x = initSym; x <= end; x++) {
    auto *job = new Job(x, end);
    job->setAutoDelete(true);
    QObject::connect(job, &Job::finished, onJobFinished);
    pool.start(job);
  }

  pool.waitForDone();
  data.save();
  return 0;
}
