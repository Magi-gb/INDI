TEMPLATE    = app
QT         += opengl 

INCLUDEPATH += /usr/include/glm

INCLUDEPATH += $$PWD/assimp

QMAKE_CXXFLAGS += -isystem $$PWD/assimp/include

LIBS += $$PWD/assimp/lib/libassimp.a \
        $$PWD/assimp/lib/libdraco.a \
        $$PWD/assimp/lib/libkubazip.a \
        $$PWD/assimp/lib/libminizip.a \
        $$PWD/assimp/lib/libpoly2tri.a \
        $$PWD/assimp/lib/libpolyclipping.a \
        $$PWD/assimp/lib/libpugixml.a \
        $$PWD/assimp/lib/libz.a

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h model.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp model.cpp \
        assimp/Mesh.cpp \
        assimp/ogldev_texture.cpp \
        assimp/ogldev_util.cpp \
        assimp/3rdparty/stb_image.cpp