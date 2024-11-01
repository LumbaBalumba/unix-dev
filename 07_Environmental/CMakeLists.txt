cmake_minimum_required(VERSION 3.5)
project(rhasher)

option(WITH_READLINE "Собрать с поддержкой readline" ON)

add_executable(rhasher src/rhasher.c)

if(WITH_READLINE)
  find_library(READLINE_LIB readline)
  if(READLINE_LIB)
    set(READLINE_LIBS readline)
  else()
    message(WARNING "Readline не найден, сборка без поддержки readline.")
    set(READLINE_LIBS)
  endif()
else()
  message(STATUS "Сборка без поддержки readline.")
  set(READLINE_LIBS)
endif()

find_library(RHASH_LIB rhash REQUIRED)
target_link_libraries(rhasher rhash ${READLINE_LIBS})

enable_testing()

add_test(
  NAME TestMD5_String
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.sh $<TARGET_FILE:rhasher>
          MD5 "\"hello\"" 5D41402ABC4B2A76B9719D911017C592)

add_test(
  NAME TestMD5_File
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.sh $<TARGET_FILE:rhasher>
          MD5 ${CMAKE_CURRENT_SOURCE_DIR}/src/rhasher.c)

add_test(
  NAME TestSHA1_String
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.sh $<TARGET_FILE:rhasher>
          SHA1 "\"hello\"" AAF4C61DDCC5E8A2DABEDE0F3B482CD9AEA9434D)

add_test(
  NAME TestSHA1_File
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.sh $<TARGET_FILE:rhasher>
          SHA1 ${CMAKE_CURRENT_SOURCE_DIR}/src/rhasher.c)

add_test(
  NAME TestTTH_String
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.sh $<TARGET_FILE:rhasher>
          TTH "\"hello\"" 398EF4D84353CBE0F831A64418F3012A5419494727186EED)

add_test(NAME TestInvalidAlgorithm
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.sh
                 $<TARGET_FILE:rhasher> INVALID "\"hello\"" "" 1)
