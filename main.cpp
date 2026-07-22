#include <iostream>
#include <iomanip>
#include <string>
#include <format>
#include <utility>
#include <thread>
#include <chrono>
#include "markov_chain.h"

int main()
{
    auto moveCursor = [](std::size_t row, std::size_t column)
    {
        std::cout << std::format("\033[{};{}H", row + 1, column + 1);
    };

    std::string input;
    std::cout << "\033[?25l";
    std::cout << "\033[2J";
    moveCursor(0, 0);
    std::cout << "> ";
    std::getline(std::cin, input);
    std::cout << "\033[2J";
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    if (!input.empty())
    {
        markov_chain<2> chain{input};
        std::string sequence{input.substr(0, chain.order())};

        while (true)
        {
            moveCursor(0, 0);
            for (std::size_t i = 0; i < sequence.size(); ++i)
                std::cout << sequence[i] << std::flush;

            if (sequence.size() >= input.size())
                break;

            std::string newSequence{chain.next_state(sequence)};
            if (newSequence == sequence)
                break;

            std::swap(sequence, newSequence);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    std::cout << "\033[?25h";
    moveCursor(1, 0);
}