cmake_minimum_required(VERSION 2.8)

project(thread)
option(COVERALLS "Build with coveralls" OFF)

set(THREADSOURCES
	${PROJECT_SOURCE_DIR}/src/thd_def.h
	${PROJECT_SOURCE_DIR}/src/thd_mutex.c
	${PROJECT_SOURCE_DIR}/src/thd_mutex.h
	${PROJECT_SOURCE_DIR}/src/thd_thread.c
	${PROJECT_SOURCE_DIR}/src/thd_thread.h
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
add_executable(threadtest ${TESTSOURCES} ${THREADSOURCES})
target_link_libraries(threadtest ${CMAKE_THREAD_LIBS_INIT})
