file(GLOB stb_sources CONFIGURE_DEPENDS
	${stb_SOURCE_DIR_}/*.h
)

add_library(stb INTERFACE ${stb_sources})

target_include_directories(stb INTERFACE ${stb_SOURCE_DIR_})