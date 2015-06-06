#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QCommandLineParser>

#include "Data.h"
#include "Constants.h"

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("Substitues Unicode symbols that look "
                                   "similar.");
  parser.addHelpOption();
  parser.addPositionalArgument("input file", "File to read values from.",
                               "<input file>");
  parser.addPositionalArgument("output file", "File to write results to.",
                               "<output file>");

  QCommandLineOption
    fileOpt(QStringList() << "f" << "file",
            QString("Use substitutions from <data file>. Will "
                    "use \"%1\" otherwise.").arg(Consts::defaultDataFile),
            "data file");
  parser.addOption(fileOpt);

  parser.process(app);

  QStringList posArgs = parser.positionalArguments();
  if (posArgs.size() != 2) {
    qCritical() << "You must specify input and output file!";
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

  QFile outFile(posArgs[1]);
  if (!outFile.open(QIODevice::WriteOnly)) {
    qCritical() << "Could not open output file for writing:" << posArgs[1];
    return -1;
  }

  QString path;
  if (parser.isSet(fileOpt)) {
    path = parser.value(fileOpt);
  }

  auto data = Data(path);
  data.load();

  int count;
  QString res = data.substitute(inText, count);

  QByteArray outData = res.toUtf8();
  outFile.write(outData);
  outFile.close();

  auto inCnt = inData.size(),
    outCnt = outData.size();
  auto incPerc = float(outCnt) / float(inCnt) * 100 - 100;

  QString msg =
    QString("%1 substitutions applied, %2 -> %3 bytes, %4%5%")
    .arg(count).arg(inCnt).arg(outCnt).arg(incPerc > 0 ? "+" : "")
    .arg(incPerc);
  qDebug() << qPrintable(msg);

  return 0;
}
