#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QCommandLineParser>

/*
  Work something like this:

% ./inspectfile.py test.txt
=== 5 raw bytes ===
b'ABC!\n'

=== 5 rendered symbols ===
ABC!

 */

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("Inspects Unicode file contents.");
  parser.addHelpOption();
  parser.addPositionalArgument("input file", "File to read values from.",
                               "<input file>");

  parser.process(app);

  QStringList posArgs = parser.positionalArguments();
  if (posArgs.size() != 1) {
    qCritical() << "You must specify input file!";
    return -1;
  }

  QFile inFile(posArgs[0]);
  if (!inFile.open(QIODevice::ReadOnly)) {
    qCritical() << "Could not read from input file:" << posArgs[0];
    return -1;
  }

  QByteArray inData = inFile.readAll();
  if (inData.isEmpty()) {
    qCritical() << "Input file is empty!";
    return -1;
  }

  QString inText = QString::fromUtf8(inData);

  qDebug() << qPrintable(QString("=== %1 raw bytes ===").arg(inData.size()));

  QStringList elms;
  foreach (const auto &ch, inText) {
    auto sym = ch.unicode();
    if (sym <= 128) {
      elms << ch;
    }
    else {
      elms << QString("\\u%1").arg(sym);
    }
  }
  qDebug() << qPrintable(elms.join(""));

  qDebug() <<
    qPrintable(QString("=== %1 rendered symbols ===").arg(inText.size()));
  qDebug() << qPrintable(inText);

  return 0;
}
