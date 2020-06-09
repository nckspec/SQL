TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    ../includes/bptree.h \
    ../includes/condition.h \
    ../includes/map.h \
    ../includes/mmap.h \
    ../includes/parser.h \
    ../includes/record.h \
    ../includes/shaunting_yard.h \
    ../includes/stokenizer.h \
    ../includes/table.h \
    ../includes/token.h \
    ../includes/vector.h \
    sql.h \
    z_output.h \
    z_work_report.h
