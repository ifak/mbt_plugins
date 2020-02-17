exists($$PWD/../mbt_plugins.config){
  include($$PWD/../mbt_plugins.config)
}

isEmpty(MBT_MBTPROJECT_PLUGIN){
  MBT_MBTPROJECT_PLUGIN = no
}

isEmpty(MBT_SPENATEDITOR_PLUGIN){
  MBT_SPENATEDITOR_PLUGIN = yes
}

isEmpty(MBT_STATEMACHINEEDITOR_PLUGIN){
  MBT_STATEMACHINEEDITOR_PLUGIN = yes
}

isEmpty(MBT_IRDLEDITOR_PLUGIN){
  MBT_IRDLEDITOR_PLUGIN = yes
}

isEmpty(MBT_ISRDLEDITOR_PLUGIN){
  MBT_ISRDLEDITOR_PLUGIN = yes
}

isEmpty(MBT_COMMONDECLEDITOR_PLUGIN){
  MBT_COMMONDECLEDITOR_PLUGIN = yes
}

isEmpty(MBT_SPECSIM_PLUGIN){
  MBT_SPECSIM_PLUGIN = yes
}

isEmpty(MBT_MBTP_PLUGIN){
  MBT_MBTP_PLUGIN = yes
}

include(mbt_config.pri)

include($${APPCREATOR_PATH}/appcreator.pri)

TEMPLATE  = subdirs

contains(MBT_MBTPROJECT_PLUGIN, yes){
  SUBDIRS += mbtproject
  mbtproject.depends += spenateditor
  mbtproject.depends += statemachineeditor
}

contains(MBT_SPENATEDITOR_PLUGIN, yes){
  SUBDIRS += spenateditor
}

contains(MBT_STATEMACHINEEDITOR_PLUGIN, yes){
  SUBDIRS += statemachineeditor
}

contains(MBT_IRDLEDITOR_PLUGIN, yes){
  SUBDIRS += irdleditor
}

contains(MBT_ISRDLEDITOR_PLUGIN, yes){
  SUBDIRS += isrdleditor
}

contains(MBT_COMMONDECLEDITOR_PLUGIN, yes){
  SUBDIRS += commondecleditor
}

contains(MBT_SPECSIM_PLUGIN, yes){
  SUBDIRS += specsim
  specsim.depends += mbtproject
}

contains(MBT_MBTP_PLUGIN, yes){
  SUBDIRS += mbtp
}
