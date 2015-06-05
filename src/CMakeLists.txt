# Trainer executable.
add_executable(
  trainer

  trainer.cpp

  Constants.h
  Constants.cpp

  Util.h
  Util.cpp

  Data.h
  Data.cpp
  )

qt5_use_modules(trainer Core)
qt5_use_modules(trainer Gui)
qt5_use_modules(trainer Widgets)
