TEMPLATE = app
CONFIG += console
CONFIG -= qt



SOURCES += main.cpp \
    utility.cpp \
    filter.cpp



unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

OTHER_FILES += \
    Images/IR/65.bmp \
    Images/IR/61.bmp \
    Images/IR/57.bmp \
    Images/IR/54.bmp \
    Images/IR/49.bmp \
    Images/IR/45.bmp \
    Images/IR/42.bmp \
    Images/IR/38.bmp \
    Images/IR/32.bmp \
    Images/IR/25.bmp \
    Images/IR/22.bmp \
    Images/IR/16.bmp \
    Images/IR/12.bmp \
    Images/IR/8.bmp \
    Images/IR/2.bmp \
    Images/OCT/33.bmp \
    Images/OCT/31.bmp \
    Images/OCT/29.bmp \
    Images/OCT/27.bmp \
    Images/OCT/25.bmp \
    Images/OCT/23.bmp \
    Images/OCT/21.bmp \
    Images/OCT/19.bmp \
    Images/OCT/16.bmp \
    Images/OCT/13.bmp \
    Images/OCT/11.bmp \
    Images/OCT/8.bmp \
    Images/OCT/6.bmp \
    Images/OCT/4.bmp \
    Images/OCT/1.bmp

HEADERS += \
    utility.h \
    filter.h
