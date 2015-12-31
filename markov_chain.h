#ifndef __MARKOV_CHAIN_H__
#define __MARKOV_CHAIN_H__

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

template<typename T>
class markov_chain
{
    struct state
    {
        const T data_;
        std::map<typename T, unsigned int> transition_tally_;

        state(const T &data)
            : data_(data)
        {
        }

        void record_next_state_sample(const T &next)
        {
            auto it = transition_tally_.find(next);

            if (it == std::end(transition_tally_))
            {
                transition_tally_.insert({ next, 1 });
            }
            else
            {
                ++it->second;
            }
        }

        std::pair<bool, const T> transition() const
        {
            if (transition_tally_.empty())
            {
                return std::make_pair(false, T());
            }

            // select a next state based on the weighted tally
            std::vector<typename T> next_options;
            std::vector<unsigned int> option_weights;

            for (const std::pair<typename T, unsigned int> &opt : transition_tally_)
            {
                next_options.push_back(opt.first);
                option_weights.push_back(opt.second);
            }

            std::random_device rd;
            std::mt19937 gen(rd());

            std::discrete_distribution<unsigned int> dd(option_weights.begin(), option_weights.end());

            const unsigned int rand_idx = dd(gen);

            if (rand_idx >= next_options.size())
            {
                throw std::logic_error("random transition index out of range");
            }

            return std::make_pair(true, next_options.at(rand_idx));
        }

        friend std::ostream& operator<<(std::ostream &os, const typename state &s)
        {
            os << s.data_ << " [";

            for (const std::pair<const typename T, long long> &tally_record : s.transition_tally_)
            {
                // TODO: fix this to not print a delimiter on the last one
                os << tally_record.first << ": " << tally_record.second << "; ";
            }

            os << "]";
            return os;
        }
    };

    std::vector<state> states_;

public:
    void train(std::function<std::pair<bool, T>()> sample_generator)
    {
        size_t last_idx = -1;
        for (;;)
        {
            const std::pair<bool, T> sample(sample_generator());
            if (!sample.first)
                break;
            
            std::vector<state>::iterator it = std::find_if(std::begin(states_), std::end(states_), [&sample](const state &s) { return sample.second == s.data_; });

            if (it == std::end(states_))
            {
                states_.emplace_back(sample.second);
                it = states_.end() - 1;
            }
 
            if (last_idx < states_.size())
            {
                states_.at(last_idx).record_next_state_sample(sample.second);
            }

            last_idx = std::distance(std::begin(states_), it);
        }
    }

    // Could definitely generalize this better so it would be more useful
    void generate_data(std::ostream &os, const T &delimiter, const T &start_criteria, std::function<bool(const T&)> end_criteria)
    {
        T elem = start_criteria;
        
        for (;;)
        {
            std::vector<state>::iterator it = std::find_if(std::begin(states_), std::end(states_), [&](const state &s) { return elem == s.data_; });

            if (it == std::end(states_))
            {
                std::stringstream ss;
                ss << "state '" << elem << "' could not be found";
                throw std::runtime_error(ss.str());
            }

            os << elem;
            
            if (end_criteria(elem))
                break;
            
            os << delimiter;

            const std::pair<bool, const T> next = it->transition();

            if (!next.first)
                break;

            elem = next.second;
        }

        os << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const markov_chain &m)
    {
        for (const markov_chain<T>::state &s : m.states_)
        {
            os << s << std::endl;
        }
        return os;
    }
};

#endif // __MARKOV_CHAIN_H__
