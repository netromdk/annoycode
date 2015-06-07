#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

class Consts {
public:
  static constexpr int imageWidth = 64; // px
  static constexpr int imageHeight = 64; // px
  static constexpr int imageFontSize = 20; // pt
  static constexpr int imageTextStartX = 20; // px
  static constexpr int imageTextStartY = 36; // px

  static constexpr float similarityThreshold = 0.998;

  static const QString defaultDataFile;

  // 33 is the ordinal of '!'; the first representable character.
  static constexpr int initialOffset = 33;
};

#endif // CONSTANTS_H
