TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(release, debug|release){
    Configuration = release
}else{
    Configuration = debug
    PrefixTag = d
}

OsgDir = C:/WtgeoProducts/ThirdParty/Osg/

INCLUDEPATH += $$OsgDir/include  \

LIBS += -L$$OsgDir/lib/$$QT_ARCH/$$Configuration \
        -losg$$PrefixTag \
        -losgViewer$$PrefixTag  \
        -losgDB$$PrefixTag

SOURCES += \
        main.cpp
