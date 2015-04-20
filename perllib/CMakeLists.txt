install(FILES PluginVersionInfo.pm.in
  DESTINATION ${product}/${version}/perllib)
cet_script(cetskel)

add_subdirectory(CetSkel)
