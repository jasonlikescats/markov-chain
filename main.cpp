#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "markov_chain.h"

std::pair<bool, std::string> sample_generator()
{
    static std::ifstream ifs("training_data.txt");

    std::string word;
    while (ifs >> word) // will split words on whitespace
    {
        return std::make_pair(true, word);
    }

    return std::make_pair(false, std::string());
}

int main()
{
    markov_chain<std::string> model;

    model.train(sample_generator);

    // Generate some sentences
    const std::string delimiter(" ");
    const std::vector<std::string> first_words = { "Now", "Then", "Whilst", "She", "He", "It", "Look", "Ah!", "Come", "The", "True", "There" };

    for (const std::string &word : first_words)
    {
        try
        {
            model.generate_data(std::cout, delimiter, word, [](const std::string &curr_word) -> bool
            {
                // check for a period to end our sentence
                return curr_word.back() == '.';
            });
        }
        catch (std::exception &e)
        {
            std::cout << "Data generation failed starting from word '" << word << "'. Reason: " << e.what();
        }
    }

    return 0;
}