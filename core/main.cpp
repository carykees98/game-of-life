/**
 * @file main.cpp
 * @author Cary Keesler
 * @brief Contains entry point for Game Of Life program
 * @version 0.1.0
 * @date 2024-07-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cstdlib>

#include <game-of-life.hpp>

/**
 * @brief Entry point for program
 *
 * @return Status code of the program
 */
auto main() -> int
{
    ckees::GameOfLife game(160, 36);

    game.run();

    std::exit(EXIT_SUCCESS);
}
