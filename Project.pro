#-------------------------------------------------
#
# Project created by QtCreator 2018-04-24T13:51:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-but-set-variable

SOURCES += main.cpp\
    Filters/allpass.cpp \
    Filters/audiofileinput.cpp \
    Filters/delay.cpp \
    Filters/etafilter.cpp \
    Filters/mixer.cpp \
    Filters/sinewavetable.cpp \
        MainWindow.cpp \
    Framework/Audio.cpp \
    Framework/AudioInput.cpp \
    Framework/AudioOutput.cpp \
    Framework/Jack.cpp \
    Framework/Properties/Properties.cpp \
    Framework/Properties/Property.cpp \
    Framework/Properties/PropertyObserver.cpp \
    Framework/Properties/TemplateProperty.cpp \
    Framework/PropertiesGUI/BooleanPropertyWidget.cpp \
    Framework/PropertiesGUI/FloatPropertyWidget.cpp \
    Framework/PropertiesGUI/IntegerPropertyWidget.cpp \
    Framework/PropertiesGUI/PropertiesWidget.cpp \
    PatchDialog.cpp \
    Framework/PropertiesGUI/OpenFilePropertyWidget.cpp \
    Filters/SineOscilator.cpp \
    Patches/SineOscilatorPatch.cpp \
    Framework/AudioFileOutput.cpp \
    Framework/PropertiesGUI/SaveFilePropertyWidget.cpp \
    Patches/allpasspatch.cpp \
    Patches/delaypatch.cpp \
    Patches/etapatch.cpp \
    Patches/feedbackcombpatch.cpp \
    Patches/feedforwardcombpatch.cpp \
    Patches/fileinputpatch.cpp \
    Patches/mixerpatch.cpp \
    Patches/reverballpasspatch.cpp \
    Patches/reverbcombpatch.cpp \
    Patches/sinewavetablepatch.cpp

HEADERS  += MainWindow.h \
    Filters/allpass.h \
    Filters/audiofileinput.h \
    Filters/delay.h \
    Filters/etafilter.h \
    Filters/mixer.h \
    Filters/sinewavetable.h \
    Framework/Audio.h \
    Framework/AudioInput.h \
    Framework/AudioIO.h \
    Framework/AudioOutput.h \
    Framework/Jack.h \
    Framework/Properties/Properties.h \
    Framework/Properties/Property.h \
    Framework/Properties/PropertyObserver.h \
    Framework/Properties/TemplateProperty.h \
    Framework/PropertiesGUI/BooleanPropertyWidget.h \
    Framework/PropertiesGUI/FloatPropertyWidget.h \
    Framework/PropertiesGUI/IntegerPropertyWidget.h \
    Framework/PropertiesGUI/PropertiesWidget.h \
    PatchDialog.h \
    Framework/PropertiesGUI/OpenFilePropertyWidget.h \
    Filters/SineOscilator.h \
    Patches/SineOscilatorPatch.h \
    Framework/AudioFileOutput.h \
    Framework/PropertiesGUI/SaveFilePropertyWidget.h \
    Patches/allpasspatch.h \
    Patches/delaypatch.h \
    Patches/etapatch.h \
    Patches/feedbackcombpatch.h \
    Patches/feedforwardcombpatch.h \
    Patches/fileinputpatch.h \
    Patches/mixerpatch.h \
    Patches/reverballpasspatch.h \
    Patches/reverbcombpatch.h \
    Patches/sinewavetablepatch.h

FORMS    += MainWindow.ui \
    Framework/PropertiesGUI/PropertiesWidget.ui \
    PatchDialog.ui

unix: LIBS += -lsndfile
unix: LIBS += -ljack

win32: INCLUDEPATH += 'C:\Program Files (x86)\Mega-Nerd\libsndfile\include'
win32: LIBS += -L'C:\Program Files (x86)\Mega-Nerd\libsndfile\lib/' -llibsndfile-1

win32: INCLUDEPATH += 'C:\Program Files (x86)\Jack\includes/'
win32: LIBS += -L'C:\Program Files (x86)\Jack\lib/' -llibjack
