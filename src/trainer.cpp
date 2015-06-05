#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QThreadPool>
#include <QApplication>
#include <QMutexLocker>
#include <QCommandLineParser>

#include "Job.h"
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

  QCommandLineOption
    threadsOpt(QStringList() << "t" << "threads",
               "Use <num> of threads to search for mathes. Defaults to using "
               "all available ones, with value \"0\".",
               "num");
  parser.addOption(threadsOpt);

  parser.process(app);

  QString path;
  if (parser.isSet(fileOpt)) {
    path = parser.value(fileOpt);
  }

  auto data = Data(path);
  data.load();

  auto initSym = data.getOffset(),
    end = 65535, // unsigned short max - 1
    stopAt = Consts::trainerStopAt,
    threads = 0;

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
  qDebug() << "Stopping after" << stopAt << "matches are found";
  qDebug() << "Using" << threads << "threads";

  QThreadPool pool;
  pool.setMaxThreadCount(threads);

  QMutex mutex;

  for (auto x = initSym; x <= end; x++) {
    auto *job = new Job(x, end);
    job->setAutoDelete(true);
    QObject::connect(job, &Job::finished,
                     [&data, &mutex](int start, int end, SubsMap matches) {
                       QMutexLocker loacker(&mutex);

                       auto oldCount = data.getCount();
                       data.addSubstitutions(matches);
                       auto count = data.getCount();
                       if (oldCount < count) {
                         qDebug() << "Matches found:" << data.getCount();
                       }

                       data.setOffset(start);

                       // TODO: Implement support for stop-at again. then stop
                       // all threads running.
                     });
    pool.start(job);
  }

  pool.waitForDone();
  data.save();
  return 0;
}
