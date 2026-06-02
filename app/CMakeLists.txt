add_executable(chess_app
    main.cpp
    ../svg.qrc
)

target_compile_options(chess_app PRIVATE 
    -Wall -Wextra -Wuninitialized -Wdangling-gsl -Wreturn-stack-address
)
#Static Analysis
#clang++ --analyze main.cpp

target_include_directories(chess_app
    PRIVATE
    ../src
)

target_link_libraries(chess_app
    PRIVATE
    chess_lib
)

