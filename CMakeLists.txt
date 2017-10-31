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

set(PTESTSOURCES
${PROJECT_SOURCE_DIR}/test/ptest.c
)

source_group(thread FILES ${THREADSOURCES})
source_group(test FILES ${TESTSOURCES})
source_group(ptest FILES ${PTESTSOURCES})
include_directories(${PROJECT_SOURCE_DIR}/test)
include_directories(${PROJECT_SOURCE_DIR}/src)

find_package(Threads REQUIRED)
set(CMAKE_MACOSX_RPATH On)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

if(${COVERALLS})
ENABLE_TESTING()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

if(${WIN32})
include_directories(${PROJECT_SOURCE_DIR}/pthreadwin)
endif()

add_executable(threadtest ${TESTSOURCES} ${THREADSOURCES})
target_link_libraries(threadtest ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(threadtest PROPERTIES OUTPUT_NAME test)

add_executable(pthreadtest ${PTESTSOURCES} ${THREADSOURCES})
target_link_libraries(pthreadtest ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(pthreadtest PROPERTIES OUTPUT_NAME ptest)

add_library(threadshared SHARED ${THREADSOURCES})
target_link_libraries(threadshared ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(threadshared PROPERTIES OUTPUT_NAME thd)
set_target_properties(threadshared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(threadshared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(threadstatic STATIC ${THREADSOURCES})
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
else()
	target_link_libraries(threadstatic ${CMAKE_THREAD_LIBS_INIT})
endif()
if(${WIN32})
	set_target_properties(threadstatic PROPERTIES OUTPUT_NAME thdlib)
else()
	set_target_properties(threadstatic PROPERTIES OUTPUT_NAME thd)
endif()
set_target_properties(threadstatic PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(threadstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
