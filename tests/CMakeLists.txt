add_executable(unit_tests
    main.cpp
    test_piece.cpp
    test_piece.hpp
    test_position.cpp
    test_position.hpp
    ../svg.qrc
)

target_compile_options(unit_tests
    PRIVATE 
        -Wall
        -Wextra 
        -Wuninitialized 
        -Wdangling-gsl 
        -Wreturn-stack-address
)

target_include_directories(unit_tests
    PRIVATE
        ../src
)

target_link_libraries(unit_tests
    PRIVATE
        chess_lib
        Qt6::Test
)

