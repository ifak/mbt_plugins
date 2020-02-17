include($$PWD/../mbt_config.pri)

DEFINES += IRDLEDITOR_LIBRARY

PROVIDER = ifak

include(irdleditor_dependencies.pri)
include($$APPCREATOR_PATH/src/appcreatorplugin.pri)

win32-msvc*:QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# irdleditor files


win32{
  copyexamples.commands = $(COPY_DIR) \"$$shell_path($$PWD/examples/irdl)\" \"$$shell_path($$IDE_APP_PATH/examples/irdl)\"
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
} else {
  copyexamples.commands = $(COPY_DIR) $$PWD/examples/irdl $$IDE_APP_PATH/examples
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
}

DISTFILES += \
    irdleditor.json \
    irdleditor_dependencies.pri \
    irdleditor.mimetypes.xml \
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
    examples/irdl/irdl_example_01.irdl \
    examples/irdl/irdl_simple_01.irdl \
    irdleditor.pluginspec.in

RESOURCES += \
    irdleditor.qrc

FORMS += \
    irdleditorbackgroundsettings.ui \
    irdleditorstepsettings.ui

HEADERS += \
    combuildirdloutlinemodel.hpp \
    irdlcompletionassist.hpp \
    irdldocument.hpp \
    irdleditor.hpp \
    irdleditor_global.hpp \
    irdleditorbackgroundsettingspage.hpp \
    irdleditorconstants.hpp \
    irdleditorfactory.hpp \
    irdleditoroutline.hpp \
    irdleditorplugin.hpp \
    irdleditorpreviewgraph.hpp \
    irdleditorsettingspage.hpp \
    irdleditorwidget.hpp \
    irdlgraphicdocument.hpp \
    irdlgraphiceditor.hpp \
    irdlgraphicfactory.hpp \
    irdlgraphicwidget.hpp \
    irdlhighlighter.hpp \
    irdlhoverhandler.hpp \
    irdloutlinemodel.hpp \
    irdleditorstepsettingspage.hpp

SOURCES += \
    combuildirdloutlinemodel.cpp \
    irdlcompletionassist.cpp \
    irdldocument.cpp \
    irdleditor.cpp \
    irdleditorbackgroundsettingspage.cpp \
    irdleditorfactory.cpp \
    irdleditoroutline.cpp \
    irdleditorplugin.cpp \
    irdleditorsettingspage.cpp \
    irdleditorwidget.cpp \
    irdlgraphicdocument.cpp \
    irdlgraphiceditor.cpp \
    irdlgraphicfactory.cpp \
    irdlgraphicwidget.cpp \
    irdlhighlighter.cpp \
    irdlhoverhandler.cpp \
    irdloutlinemodel.cpp \
    irdleditorstepsettingspage.cpp
