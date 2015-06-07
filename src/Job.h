#include <QObject>
#include <QRunnable>

#include "Data.h"

class Job : public QObject, public QRunnable {
  Q_OBJECT

public:
  Job(int start, int end);

  virtual void run();

signals:
  void finished(int start, int end, SubsMap matches, int msElapsed);

private:
  int start, end;
};
