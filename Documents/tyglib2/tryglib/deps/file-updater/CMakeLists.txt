project(file-updater)

find_package(Libcurl REQUIRED)

include_directories(${LIBCURL_INCLUDE_DIRS})

if(WIN32 AND NOT MINGW)
	include_directories(../w32-pthreads)
	set(file-updater_PLATFORM_DEPS
		w32-pthreads)
endif()

set(file-updater_HEADERS
	file-updater/file-updater.h)
set(file-updater_SOURCES
	file-updater/file-updater.c)

add_library(file-updater STATIC
	${file-updater_SOURCES}
	${file-updater_HEADERS})

target_include_directories(file-updater
	PUBLIC .)

if(NOT MSVC AND NOT MINGW)
	target_compile_options(file-updater PRIVATE -fPIC)
endif()

target_link_libraries(file-updater
	${LIBCURL_LIBRARIES}
	${file-updater_PLATFORM_DEPS}
	libobs)
