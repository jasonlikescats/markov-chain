#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "markov_chain.h"

using namespace markov_chain_model;

void train_from_file(markov_chain<std::string> &model, const std::string &filepath)
{
    std::ifstream ifs(filepath);

    size_t last_state = -1;
    std::string word;
    while (ifs >> word)
    {
        last_state = model.record_sample(word, last_state);
    }
}

int main(int argc, char *argv[])
{
    markov_chain<std::string> model;

    for (int i = 1; i < argc; ++i)
    {
        train_from_file(model, argv[i]);
    }

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
            std::cout << "Data generation failed starting from word '" << word << "'. Reason: " << e.what() << std::endl;
        }
    }

    return 0;
}