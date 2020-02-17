MBTPROJECT_VERSION=0.2

isEmpty(3RD_PATH) {
3RD_PATH = $$PWD/../../../3rd
}

MOBATA_LIBRARY_TYPE = staticlib
include($${3RD_PATH}/mobata/mobata/mobata.pri)

#MOBATAWIDGETS_LIBRARY_TYPE = staticlib
#include($${3RD_PATH}/mobata/libs/mobatawidgets/mobatawidgets.pri)

DSLPARSER_LIBRARY_TYPE = staticlib
include($${3RD_PATH}/mobata/libs/dslparser/dslparser/dslparser.pri)

MODTRACE_LIBRARY_TYPE = staticlib
include($${3RD_PATH}/mobata/libs/modtrace/modtrace/modtrace.pri)

QTC_PLUGIN_NAME = mbtproject

QTC_PLUGIN_DEPENDS += \
  projectexplorer \
  spenateditor \
  statemachineeditor \
  testcaseeditor
