cmake_minimum_required(VERSION 2.8)

project(thd)
option(COVERALLS "Build with coveralls" OFF)

set(THREADSOURCES
	${PROJECT_SOURCE_DIR}/src/thd.c
	${PROJECT_SOURCE_DIR}/src/thd.h
)

set(TESTSOURCES
${PROJECT_SOURCE_DIR}/test/test.c
)

source_group(thread FILES ${THREADSOURCES})
source_group(test FILES ${TESTSOURCES})
include_directories(${PROJECT_SOURCE_DIR}/test)
include_directories(${PROJECT_SOURCE_DIR}/src)

ENABLE_TESTING()
find_package(Threads REQUIRED)
set(CMAKE_MACOSX_RPATH On)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

add_executable(test ${TESTSOURCES} ${THREADSOURCES})
target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})

add_library(threadshared SHARED ${THREADSOURCES})
target_link_libraries(threadshared ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(threadshared PROPERTIES OUTPUT_NAME thd)
set_target_properties(threadshared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(threadshared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(threadstatic STATIC ${THREADSOURCES})
target_link_libraries(threadstatic ${CMAKE_THREAD_LIBS_INIT})
if(${WIN32})
	set_target_properties(threadstatic PROPERTIES OUTPUT_NAME thdlib)
else()
	set_target_properties(threadstatic PROPERTIES OUTPUT_NAME thd)
endif()
set_target_properties(threadstatic PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(threadstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
