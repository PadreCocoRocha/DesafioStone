QT = gui core multimedia network networkauth

TEMPLATE = app
TARGET = desafioStonePlayer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    player.cpp \
    spotifywrapper.cpp \
    searchbar.cpp \
    resultsbox.cpp \
    resultitem.cpp \
    controller.cpp \
    playlistmodel.cpp \
    playlistview.cpp

HEADERS += \
    player.h \
    spotifywrapper.h \
    clientkeys.h \
    searchbar.h \
    resultsbox.h \
    resultitem.h \
    controller.h \
    playlistmodel.h \
    playlistview.h

