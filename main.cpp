#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "markov_chain.h"

using namespace markov_chain_model;

int main()
{
    markov_chain<std::string> m;

    {
        std::ifstream ifs("training_data.txt");

        size_t last_state = -1;
        std::string word;
        while (ifs >> word)
        {
            last_state = m.record_sample(word, last_state);
        }
    }

    // Generate some sentences
    const std::string delimiter(" ");
    const std::vector<std::string> first_words = { "Now", "Then", "Whilst", "She", "He", "It", "Look", "Ah!", "Come", "The", "True", "There" };

    for (const std::string &word : first_words)
    {
        try
        {
            m.generate_data(std::cout, delimiter, word, [](const std::string &curr_word) -> bool
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