#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

class Consts {
public:
  static constexpr int imageWidth = 48; // px
  static constexpr int imageHeight = 48; // px
  static constexpr int imageFontSize = 20; // pt
  static constexpr int imageTextStartX = 2; // px
  static constexpr int imageTextStartY = 30; // px

  static constexpr float similarityThreshold = 0.996;

  static const QString defaultDataFile;

  // 33 is the ordinal of '!'; the first representable character.
  static constexpr int initialOffset = 33;

  static constexpr int trainerStopAt = 25;
};

#endif // CONSTANTS_H
