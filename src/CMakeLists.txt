# Trainer executable.
add_executable(
  trainer

  trainer.cpp
  )

qt5_use_modules(trainer Core)
qt5_use_modules(trainer Gui)
qt5_use_modules(trainer Widgets)
