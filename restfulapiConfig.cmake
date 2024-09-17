get_filename_component(current_dirname ${CMAKE_CURRENT_LIST_DIR} NAME)

set(${current_dirname}_INCLUDE_DIRS "${${current_dirname}_DIR}/include")

if(MSVC)
	set(${current_dirname}_LIBS_DIR "${${current_dirname}_DIR}/lib/win64")
	file(GLOB ${current_dirname}_LIBS "${${current_dirname}_LIBS_DIR}/*.lib")
else()
	set(${current_dirname}_STATIC_LIBS_DIR "${${current_dirname}_DIR}/lib/linux64")
	file(GLOB ${current_dirname}_STATIC_LIBS "${${current_dirname}_STATIC_LIBS_DIR}/*.a")
	set(${current_dirname}_DYNAMIC_LIBS_DIR "${${current_dirname}_DIR}/bin/linux64")
	file(GLOB ${current_dirname}_DYNAMIC_LIBS "${${current_dirname}_DYNAMIC_LIBS_DIR}/*.so")
	set(${current_dirname}_LIBS ${${current_dirname}_STATIC_LIBS} ${${current_dirname}_DYNAMIC_LIBS})
endif() 

if (${current_dirname}_INCLUDE_DIRS AND ${current_dirname}_LIBS)
    set(${current_dirname}_FOUND TRUE)
endif (${current_dirname}_INCLUDE_DIRS AND ${current_dirname}_LIBS)

