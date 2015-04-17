install(FILES PluginVersionInfo.pm.in
  DESTINATION ${product}/${version}/tools)
cet_script(cetskel)

add_subdirectory(CetSkel)
