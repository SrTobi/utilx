# grab source
buildx_scan_here(UTILX_SOURCE "cpp;hpp")
buildx_debug("source: ${UTILX_SOURCE}" proj)
buildx_auto_group("${UTILX_SOURCE}" PREFIX source)
buildx_auto_group("${UTILX_PUBLIC_INCLUDES}")

# add library
include_directories(.)
add_library(utilx STATIC ${UTILX_SOURCE} ${UTILX_PUBLIC_INCLUDES})
buildx_export_header(utilx
					DESTINATION_FILE "UTILX_PUBLIC_INCLUDES"
					DESTINATION_DIR "UTILX_EXPORT_API_DIR"
					SUB_DIR utilx)

install(TARGETS utilx EXPORT utilxTargets
  LIBRARY DESTINATION lib/${UTILX_VERSION_DIR}
  ARCHIVE DESTINATION lib/${UTILX_VERSION_DIR}
  RUNTIME DESTINATION bin/${UTILX_VERSION_DIR}
)

buildx_target_includes(	TARGETS utilx
						FILES ${UTILX_PUBLIC_INCLUDES} ${GLM_PUBLIC_INCLUDES}
						BASE_DIRECTORIES
							${UTILX_PUBLIC_INCLUDE_DIR}
							${UTILX_EXPORT_API_DIR}
							${UTILX_GLM_INCLUDE_DIR}
						DESTINATION include/${UTILX_VERSION_DIR})

buildx_add_external_test(utilx-test ${PROJECT_SOURCE_DIR}/tests TEST_TARGETS utilx)
#buildx_add_internal_test(internal-test ${PROJECT_SOURCE_DIR}/tests/internal TEST_TARGET logx)