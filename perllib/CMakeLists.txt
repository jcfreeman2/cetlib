cet_project_var(PLUGINVERSIONINFO_PM_IN
  TYPE FILEPATH
  DOCSTRING "Location of Perl PluginVersionInfo.pm.in template file"
  perllib/PluginVersionInfo.pm.in
)
install(FILES PluginVersionInfo.pm.in
  DESTINATION perllib)

cet_script(cetskelgen)

add_subdirectory(CetSkel)
