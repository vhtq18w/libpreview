TEMPLATE = app

CONFIG += C++11

QT += gui \
      webchannel \
      webengine \
      webenginecore \
      webenginewidgets \
      widgets

HEADERS = \
          src/pdfappbridge.h \
          src/pdfappwidget.h

SOURCES = \
          src/pdfappbridge.cc \
          src/pdfappwidget.cc \
          src/main.cc \

RESOURCES = \
            resources/pdfapp.qrc \
