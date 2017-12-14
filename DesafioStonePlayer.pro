QT = gui core multimedia network networkauth

TEMPLATE = app
TARGET = desafioStonePlayer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    player.cpp \
    spotifywrapper.cpp

HEADERS += \
    player.h \
    spotifywrapper.h \
    clientkeys.h

