add_library(tinyxml2
	"include/tinyxml2/tinyxml2.h"
	"src/tinyxml2.cpp")

target_include_directories(tinyxml2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
