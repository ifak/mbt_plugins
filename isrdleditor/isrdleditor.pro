include($$PWD/../mbt_config.pri)

DEFINES += ISRDLEDITOR_LIBRARY

PROVIDER = ifak

include(isrdleditor_dependencies.pri)
include($$APPCREATOR_PATH/src/appcreatorplugin.pri)

win32-msvc*:QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# isrdleditor files


win32{
  copyexamples.commands = $(COPY_DIR) \"$$shell_path($$PWD/examples/isrdl)\" \"$$shell_path($$IDE_APP_PATH/examples/isrdl)\"
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
} else {
  copyexamples.commands = $(COPY_DIR) $$shell_path($$PWD/examples/isrdl) $$shell_path($$IDE_APP_PATH/examples)
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
}

DISTFILES += \
    isrdleditor.json \
    isrdleditor_dependencies.pri \
    isrdleditor.mimetypes.xml \
    images/attribute.png \
    images/const.png \
    images/func.png \
    images/glslfile.png \
    images/keyword.png \
    images/other.png \
    images/type.png \
    images/uniform.png \
    images/var.png \
    images/varying.png \
    examples/isrdl/isrdl_example_01.isrdl \
    examples/isrdl/isrdl_simple_01.isrdl \
    isrdleditor.pluginspec.in

RESOURCES += \
    isrdleditor.qrc

FORMS +=

HEADERS += \
    combuildisrdloutlinemodel.hpp \
    isrdlautocompleter.hpp \
    isrdlcompletionassist.hpp \
    isrdldocument.hpp \
    isrdleditor.hpp \
    isrdleditor_global.hpp \
    isrdleditorconstants.hpp \
    isrdleditorfactory.hpp \
    isrdleditoroutline.hpp \
    isrdleditorplugin.hpp \
    isrdleditorwidget.hpp \
    isrdloutlinemodel.hpp

SOURCES += \
    combuildisrdloutlinemodel.cpp \
    isrdlautocompleter.cpp \
    isrdlcompletionassist.cpp \
    isrdldocument.cpp \
    isrdleditor.cpp \
    isrdleditorfactory.cpp \
    isrdleditoroutline.cpp \
    isrdleditorplugin.cpp \
    isrdleditorwidget.cpp \
    isrdloutlinemodel.cpp
