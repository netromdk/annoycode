set(name trainer)

add_executable(
  ${name}

  main.cpp
  )

qt5_use_modules(${name} Core)
qt5_use_modules(${name} Gui)
qt5_use_modules(${name} Widgets)
