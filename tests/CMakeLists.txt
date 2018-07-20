include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

macro(washipad_auto_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        add_test(${_testname} ${_testname})
        target_link_libraries(${_testname}
            Qt5::Test
            washipadlib
        )
    endforeach(_testname)
endmacro(washipad_auto_tests)

washipad_auto_tests(
    sketchmodeltest
    stroketest
)
