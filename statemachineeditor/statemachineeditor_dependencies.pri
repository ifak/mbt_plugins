STATEMACHINEEDITOR_VERSION=0.1

include($${3RD_PATH}/mobata/libs/graphlayout/graphlayout.pri)
include($${3RD_PATH}/mobata/libs/dslparser/dslparser/dslparser.pri)

QTC_PLUGIN_NAME = statemachineeditor

QTC_LIB_DEPENDS += \
    aggregation \
    extensionsystem \
    languageutils \
    utils

QTC_PLUGIN_DEPENDS += \
    dsleditor
