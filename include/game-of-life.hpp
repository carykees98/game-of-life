#pragma once
/**
 * @file game-of-life.hpp
 * @author Cary Keesler
 * @brief Contains class definition for the `GameOfLife` class.
 * @version 0.1.0
 * @date 2024-07-12
 *
 * @copyright Copyright (c) 2024
 *
 */

// Standard Includes
#include <array>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <iostream>
#include <map>
#include <random>
#include <ranges>
#include <sstream>
#include <thread>
#include <utility>

/**
 * @brief Namespace that contains all code written by me for my personal C++
 * projects
 *
 */
namespace ckees
{
/**
 * @brief Class to represent the state of the Game of Life
 *
 */
template<std::integral WidthType, std::integral HeightType> class GameOfLife
{
public:
    /**
     * @brief Default constructor
     *
     */
    GameOfLife()
        : GameOfLife(10U, 10U)
    {
    }

    /**
     * @brief Parameterized Constructor
     *
     * @param width The width for the grid
     * @param height The height for the grid
     *
     */
    GameOfLife(WidthType width, HeightType height)
        : m_Width(width),
          m_Height(height),
          m_Generation(0)
    {
    }

    /**
     * @brief Copy Constructor **DELETED**
     *
     * @param game Object to copy
     *
     */
    GameOfLife(GameOfLife& game) = delete;

    /**
     * @brief Move Constructor **DELETED**
     *
     * @param game Object to move
     *
     */
    GameOfLife(GameOfLife&& game) = delete;

public:
    /**
     * @brief Method called to start the game
     *
     */
    auto run() -> void
    {
        generate_starting_cells();

        while (true)
        {
            display_grid();
            if (m_LiveCells.empty())
            {
                std::exit(EXIT_SUCCESS);
            }

            auto previousGeneration = std::move(m_LiveCells);
            m_LiveCells.clear();

            std::map<std::pair<WidthType, HeightType>, bool> cellsToCheck;

            for (const auto [coordPair, isAlive] : previousGeneration)
            {
                const std::vector<std::pair<WidthType, HeightType>> neighbors
                    = get_neighbors(coordPair, previousGeneration);

                std::uint8_t numberOfNeighbors
                    = check_neighbors(neighbors, previousGeneration);

                if (2 <= numberOfNeighbors && numberOfNeighbors <= 3)
                {
                    m_LiveCells.emplace(coordPair, isAlive);
                    cellsToCheck.try_emplace(coordPair, isAlive);
                };

                for (const auto neighbor : neighbors)
                {
                    if (!cellsToCheck.contains(neighbor))
                    {
                        cellsToCheck.emplace(neighbor, false);
                    }
                }
            }

            for (const auto [coordPair, isAlive] : cellsToCheck)
            {
                if (isAlive)
                {
                    continue;
                }

                const std::vector<std::pair<WidthType, HeightType>> neighbors
                    = get_neighbors(coordPair, previousGeneration);

                std::uint8_t numberOfNeighbors
                    = check_neighbors(neighbors, previousGeneration);

                if (numberOfNeighbors == 3)
                {
                    m_LiveCells.emplace(coordPair, true);
                }
            }

            cellsToCheck.clear();
            m_Generation++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

private:
    auto generate_starting_cells() -> void

    {
        const auto         average = std::ceil((m_Width + m_Height) * 5.0);
        std::random_device randomDevice;
        std::mt19937       generator(randomDevice());
        for (const auto _ :
             std::ranges::views::iota(0) | std::ranges::views::take(average))
        {
            m_LiveCells.emplace(
                std::make_pair(generator() % m_Width, generator() % m_Height),
                true);
        }

        /*
        constexpr std::array<std::pair<WidthType, HeightType>, 12>
            startingPositions = {
                std::pair(5, 10),  std::pair(6, 10),  std::pair(7, 10),
                std::pair(9, 8),   std::pair(9, 7),   std::pair(9, 6),
                std::pair(9, 12),  std::pair(9, 13),  std::pair(9, 14),
                std::pair(11, 10), std::pair(12, 10), std::pair(13, 10),
            };

        for (const auto position : startingPositions)
        {
            m_LiveCells.emplace(position, true);
        }
         */
    }

    auto display_grid() -> void
    {
        std::stringstream     buffer;
        constexpr const char* clearEscapeSequence = "\033c";
        buffer << clearEscapeSequence;

        for (const WidthType yCoord :
             std::ranges::views::iota(0) | std::ranges::views::take(m_Height))
        {
            for (const HeightType& xCoord :
                 std::ranges::views::iota(0)
                     | std::ranges::views::take(m_Width))
            {
                if (m_LiveCells.contains(std::pair(xCoord, yCoord)))
                {
                    buffer << '#';
                }
                else
                {
                    buffer << '~';
                }
            }

            buffer << '\n';
        }

        buffer << '\n'
               << "Generation: " << m_Generation << '\n'
               << "Live Cell Count: " << m_LiveCells.size() << '\n';

        std::cout << buffer.str();
    }

    auto get_neighbors(const std::pair<WidthType, HeightType>& coordPair,
                       const std::map<std::pair<WidthType, HeightType>, bool>&
                           previousGeneraton)
        -> const std::vector<std::pair<WidthType, HeightType>>
    {
        const auto& [xCoord, yCoord] = coordPair;

        return std::vector {
            std::pair(xCoord - 1, yCoord + 1),
            std::pair(xCoord, yCoord + 1),
            std::pair(xCoord + 1, yCoord + 1),
            std::pair(xCoord - 1, yCoord),
            std::pair(xCoord + 1, yCoord),
            std::pair(xCoord - 1, yCoord - 1),
            std::pair(xCoord, yCoord - 1),
            std::pair(xCoord + 1, yCoord - 1),
        };
    }

    auto check_neighbors(
        const std::vector<std::pair<WidthType, HeightType>>& neighbors,
        const std::map<std::pair<WidthType, HeightType>, bool>&
            previousGeneraton) -> std::uint8_t
    {
        std::uint8_t liveNeighbors = 0;

        for (auto neighborCoordPair : neighbors)
        {
            if (previousGeneraton.contains(neighborCoordPair))
            {
                liveNeighbors += 1;
            }
        }

        return liveNeighbors;
    }

private:
    /**
     * @brief Width of the grid
     *
     */
    WidthType m_Width;

    /**
     * @brief Height of the grid
     *
     */
    HeightType m_Height;

    /**
     * @brief Current generation of the game
     *
     */
    std::uint64_t m_Generation;

    /**
     * @brief `std::map` container to hold a list of the currently live cells
     *
     */
    std::map<std::pair<WidthType, HeightType>, bool> m_LiveCells;
};
} // namespace ckees
