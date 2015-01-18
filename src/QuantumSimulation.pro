#-------------------------------------------------
#
# Project created by QtCreator 2014-02-04T14:52:49
#
#-------------------------------------------------

QT       += core gui

# Add support to C++11
CONFIG += c++11

# QCustomPlot needs printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QuantumSimulation
TEMPLATE = app

SOURCES += main.cc \
    mainwindow.cc \
    chebyshevpropagator.cc \
    simulation.cc \
    ../../../qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.hh \
    chebyshevpropagator.hh \
    simenvironment.hh \
    simulation.hh \
    ../../../qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui

# Armadillo library
INCLUDEPATH += ../../../armadillo-4.000.5/include

# Optimization for the release version
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
