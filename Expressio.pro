TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    Expressio/main.cpp

HEADERS += \
    Expressio/Evaluator.hpp \
    Expressio/Expressio.hpp \
    Expressio/Function.hpp \
    Expressio/FunctionTraits.hpp

OTHER_FILES += \
    README.md \
    LICENSE.md \
    Doxyfile

