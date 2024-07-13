file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_executable(game-of-life)

target_sources(game-of-life
PRIVATE
    main.cpp
    ${HEADER_LIST}
)

target_include_directories(game-of-life
PRIVATE
    ../include
)

target_link_libraries(game-of-life
PRIVATE 
    game-of-life-lib
)

target_compile_features(game-of-life
PRIVATE 
    cxx_std_23
)

set_target_properties(game-of-life
PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
