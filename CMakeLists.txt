cmake_minimum_required(VERSION 3.14)

project(libpreview)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED COMPONENTS
  Core
  Gui
  WebChannel
  WebEngine
  WebEngineCore
  WebEngineWidgets
  Widgets
  )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(LIBPREVIEW_SOURCES
  src/pdfappbridge.cc
  src/pdfappwidget.cc
  src/main.cc
  )

set(LIBPREVIEW_HEADERS
  src/pdfappbridge.h
  src/pdfappwidget.h
  )

set(LIBPREVIEW_RESOURCES
  resources/pdfapp.qrc
  )

add_executable(libpreview
  ${LIBPREVIEW_SOURCES}
  ${LIBPREVIEW_HEADERS}
  ${LIBPREVIEW_RESOURCES}
  )

target_link_libraries(libpreview
  PUBLIC
  Qt5::Core
  Qt5::Gui
  Qt5::WebChannel
  Qt5::WebEngine
  Qt5::WebEngineCore
  Qt5::WebEngineWidgets
  Qt5::Widgets
  )
