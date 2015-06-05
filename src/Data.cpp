#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonDocument>

#include "Data.h"
#include "Constants.h"

Data::Data(const QString &path) : path(path), offset(Consts::initialOffset) {
  if (path.isEmpty()) {
    this->path = Consts::defaultDataFile;
  }
}

bool Data::load() {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) {
    qCritical() << "Could not open file for reading:" << path;
    return false;
  }

  auto data = file.readAll();
  auto doc = QJsonDocument::fromJson(data);
  if (doc.isNull()) {
    qCritical() << "Could not read/parse data file:" << path;
    return false;
  }

  auto map = doc.object().toVariantMap();

  // Check values.
  if (!map.contains("offset")) {
    qCritical() << "Invalid JSON: Expected entry: 'offset'";
    return false;
  }

  auto offVar = map["offset"];
  if (!offVar.canConvert<int>()) {
    qCritical() << "Invalid JSON: 'offset' must be an integer!";
    return false;
  }

  if (!map.contains("substitutions")) {
    qCritical() << "Invalid JSON: Expected entry: 'substitutions'";
    return false;
  }

  auto subsVar = map["substitutions"];
  if (!subsVar.canConvert<QVariantMap>()) {
    qCritical() << "Invalid JSON: 'substitutions' must be an array!";
    return false;
  }

  // Load values.
  offset = offVar.toUInt();

  auto subsMap = subsVar.toMap();
  foreach (auto &key, subsMap.keys()) {
    auto elms = key.split(" ", QString::SkipEmptyParts);
    if (elms.size() != 2) {
      qWarning() << "Invalid JSON: Invalid substitution key:" << key;
      continue;
    }

    bool ok;
    int x = elms[0].toUInt(&ok);
    if (!ok) {
      qWarning() << "Invalid JSON: Invalid substitution key value:" << elms[0];
      continue;
    }

    int y = elms[1].toUInt(&ok);
    if (!ok) {
      qWarning() << "Invalid JSON: Invalid substitution key value:" << elms[1];
      continue;
    }

    auto simVar = subsMap[key];
    if (!simVar.canConvert<float>()) {
      qWarning() << "Invalid JSON: Substitution value must be a float!";
      continue;
    }

    float sim = simVar.toFloat(&ok);
    if (!ok) {
      qWarning() << "Invalid JSON: Substitution value must be a float!";
      continue;
    }

    if (!hasSubstitution(x, y)) {
      addSubstitution(x, y, sim);
    }
  }

  qDebug() << qPrintable(QString("Loaded %1 substitutions.").arg(subs.size()));
  return true;
}

bool Data::save() {
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly)) {
    qCritical() << "Could not open file for writing:" << path;
    return false;
  }

  QVariantMap map;
  map["offset"] = offset;

  QVariantMap subsMap;
  foreach (auto &pair, subs.keys()) {
    subsMap[QString("%1 %2").arg(pair.first).arg(pair.second)] = subs[pair];
  }
  map["substitutions"] = subsMap;

  qDebug() << qPrintable(QString("Saved %1 substitutions.").arg(subs.size()));

  auto obj = QJsonObject::fromVariantMap(map);
  QJsonDocument doc(obj);

  file.write(doc.toJson());
  return true;
}

void Data::addSubstitution(quint16 x, quint16 y, float similarity) {
  subs[makePair(x, y)] = similarity;
}

bool Data::hasSubstitution(quint16 x, quint16 y) const {
  return subs.contains(makePair(x, y));
}

inline SubsPair Data::makePair(quint16 x, quint16 y) const {
  return SubsPair(std::min(x, y), std::max(x, y));
}
