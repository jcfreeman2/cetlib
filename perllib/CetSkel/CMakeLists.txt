set(pm_files
  ${CMAKE_CURRENT_SOURCE_DIR}/BasicPluginCommon.pm
)

cet_copy(${pm_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES ${pm_files}
  DESTINATION ${product}/${version}/perllib/CetSkel/)

add_subdirectory(cetskel)
