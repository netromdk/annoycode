add_library(
  common

  Constants.h
  Constants.cpp

  Util.h
  Util.cpp

  Data.h
  Data.cpp
  )

qt5_use_modules(common Core Gui Widgets)

# Trainer executable.
add_executable(
  trainer

  trainer.cpp

  Job.h
  Job.cpp
  )

target_link_libraries(trainer common)

# Annoycode executable.
add_executable(
  annoycode

  annoycode.cpp
  )

target_link_libraries(annoycode common)

# Inspect executable.
add_executable(
  inspect

  inspect.cpp
  )

qt5_use_modules(inspect Core)
