//
// Created by cartercpp on 7/22/26.
//

#ifndef MARKOVCHAIN_MARKOV_CHAIN_H
#define MARKOVCHAIN_MARKOV_CHAIN_H

#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <random>
#include <ranges>
#include <algorithm>
#include <cstddef>

template <std::size_t Order> requires (Order > 0)
class markov_chain
{
public:

    explicit markov_chain(std::string_view text)
        : m_rng{std::random_device{}()}
    {
        for (std::size_t start = 0; start + Order < text.size(); ++start)
        {
            const std::string substr{text.substr(start, Order)};
            const char succeedingChar = text[start + Order];
            ++m_transitionProbabilities[substr][succeedingChar];
        }

        for (auto& probabilities : m_transitionProbabilities | std::views::values)
        {
            double divideBy = 0;

            for (const double probability : probabilities | std::views::values)
                divideBy += probability;

            for (double& probability : probabilities | std::views::values)
                probability /= divideBy;
        }
    }

    static constexpr std::size_t order() noexcept
    {
        return Order;
    }

    std::string next_state(std::string text) const
    {
        if (text.size() < Order)
            return text;

        const std::string ending{text.substr(text.size() - Order)};

        if (const auto iter = m_transitionProbabilities.find(ending); iter != m_transitionProbabilities.end())
        {
            const std::map<char, double>& probabilities{iter->second};

            std::vector<char> chars;
            std::vector<double> weights;

            chars.reserve(probabilities.size());
            weights.reserve(probabilities.size());

            for (const auto [ch, probability] : probabilities)
            {
                chars.push_back(ch);
                weights.push_back(probability);
            }

            std::discrete_distribution<std::size_t> dist(weights.begin(), weights.end());
            const std::size_t index = dist(m_rng);

            text.push_back(chars[index]);
        }

        return text;
    }

private:

    std::map<std::string, std::map<char, double>> m_transitionProbabilities;
    mutable std::mt19937 m_rng;
};

#endif //MARKOVCHAIN_MARKOV_CHAIN_H