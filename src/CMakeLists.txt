# Trainer executable.
add_executable(
  trainer

  trainer.cpp

  Util.h
  Util.cpp
  )

qt5_use_modules(trainer Core)
qt5_use_modules(trainer Gui)
qt5_use_modules(trainer Widgets)
