cmake_minimum_required(VERSION 3.16)

project(libpeony-preview)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++" )
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-O0" )
set(QT_QMAKE_EXECUTABLE "qmake")

find_package(PkgConfig REQUIRED)

pkg_check_modules(PEONYQT REQUIRED IMPORTED_TARGET peony-qt)
pkg_check_modules(POPPLER REQUIRED IMPORTED_TARGET poppler-qt5)
pkg_check_modules(QT5CORE REQUIRED IMPORTED_TARGET Qt5Core)
pkg_check_modules(QT5GUI REQUIRED IMPORTED_TARGET Qt5Gui)
pkg_check_modules(QT5WIDGETS REQUIRED IMPORTED_TARGET Qt5Widgets)

set(LIBPEONY_PREVIEW_SOURCES
  src/pdf-thumbnail.h
  src/pdf-thumbnail.cpp
  src/test.cpp
  )

add_executable(pdfpreview ${LIBPEONY_PREVIEW_SOURCES})

target_link_libraries(pdfpreview PUBLIC
  PkgConfig::PEONYQT
  PkgConfig::POPPLER
  PkgConfig::QT5CORE
  PkgConfig::QT5GUI
  PkgConfig::QT5WIDGETS
  )
