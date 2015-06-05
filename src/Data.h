#ifndef DATA_H
#define DATA_H

#include <QMap>
#include <QPair>
#include <QString>

typedef QPair<quint16, quint16> SubsPair;
typedef QMap<SubsPair, float> SubsMap;

class Data {
public:
  Data(const QString &path = QString());

  bool load();
  bool save();

  void setOffset(int offset) { this->offset = offset; }
  int getOffset() const { return offset; }

  void addSubstitution(quint16 x, quint16 y, float similarity);
  bool hasSubstitution(quint16 x, quint16 y) const;

private:
  inline SubsPair makePair(quint16 x, quint16 y) const;

  QString path;
  int offset;
  SubsMap subs;
};

#endif // DATA_H
