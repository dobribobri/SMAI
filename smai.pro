QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

QMAKE_CFLAGS += -O2
QMAKE_CXXFLAGS += -O2

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        abox.cpp \
        attenuationmodel.cpp \
        averager.cpp \
        digitizer.cpp \
        field.cpp \
        inhomogeneity.cpp \
        main.cpp \
    measurement.cpp \
    stat.cpp \
    structuralfunction.cpp \
    variate.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    abox.h \
    attenuationmodel.h \
    averager.h \
    colormod.h \
    digitizer.h \
    field.h \
    inhomogeneity.h \
    linspace.h \
    measurement.h \
    pipegnuplotter.h \
    # plotter.h \
    rotation.h \
    stat.h \
    structuralfunction.h \
    variate.h

# DISTFILES += \
#    plotter.py
