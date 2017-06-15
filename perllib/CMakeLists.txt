install(FILES PluginVersionInfo.pm.in
  DESTINATION perllib )

cet_script(cetskelgen)

add_subdirectory(CetSkel)
