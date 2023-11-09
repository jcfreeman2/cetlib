project_variable(CONFIG_DIR config CONFIG
  DOCSTRING "Directory containing config files provided by ${PROJECT_NAME}")

project_variable(PLUGINVERSIONINFO_PM_IN
  "${${PROJECT_NAME}_CONFIG_DIR}/PluginVersionInfo.pm.in"
  TYPE FILEPATH_FRAGMENT CONFIG
  DOCSTRING "Location of PluginVersionInfo.pm.in template for install_perllib()")

install(FILES PluginVersionInfo.pm.in
  DESTINATION ${${PROJECT_NAME}_CONFIG_DIR})
