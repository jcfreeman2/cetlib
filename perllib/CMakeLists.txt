install(FILES PluginVersionInfo.pm.in
  DESTINATION ${${product}_perllib})

cet_script(cetskel)

add_subdirectory(CetSkel)
