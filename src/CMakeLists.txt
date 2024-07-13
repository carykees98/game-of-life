file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_library(game-of-life-lib)

target_sources(game-of-life-lib
PRIVATE
    game-of-life.cpp
    ${HEADER_LIST}
)

target_include_directories(game-of-life-lib
    PRIVATE
    ../include
)

target_compile_features(game-of-life-lib
PRIVATE
    cxx_std_23
)

set_target_properties(game-of-life-lib
PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
