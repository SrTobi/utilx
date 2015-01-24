cmake_minimum_required( VERSION 3.0 )

project(utilx VERSION 0.1)
set(Project_SHORTCUT "utilx")
set(Project_PREFIX "utilx")

include(GenerateExportHeader)

add_subdirectory("extras/buildx")
buildx_enable_debug(all)
buildx_version_dir(UTILX_VERSION_DIR)
buildx_set_default_prefixes()
buildx_set_default_output_dirs()
buildx_activate_cpp11()


set(UTILX_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(UTILX_GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extras/glm-src)
buildx_scan(UTILX_PUBLIC_INCLUDES ${UTILX_PUBLIC_INCLUDE_DIR} "hpp")
buildx_scan(GLM_PUBLIC_INCLUDES ${UTILX_GLM_INCLUDE_DIR} "hpp;cpp;inl")
buildx_debug("public includes: ${UTILX_PUBLIC_INCLUDES}" proj)

add_subdirectory("src")

buildx_install_package(	EXPORT utilxTargets
						NAMESPACE x::)