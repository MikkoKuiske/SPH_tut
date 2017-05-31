TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../src/calculation_area.cpp \
    ../../src/file_operations.cpp \
    ../../src/particle.cpp \
    ../../src/main.cpp \
    ../../src/sph_list.cpp

HEADERS += \
    ../../src/calculation_area.hh \
    ../../src/file_operations.hh \
    ../../src/particle.hh \
    ../../src/parameters.hh \
    ../../src/sph_list.hh
