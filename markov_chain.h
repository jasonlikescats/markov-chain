#ifndef __MARKOV_CHAIN_H__
#define __MARKOV_CHAIN_H__

#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

namespace markov_chain_model
{
    template <typename T>
    struct state
    {
        const T data_;
        std::unordered_map<typename T, unsigned int> transition_tally_;

        state(const T &data)
            : data_(data)
        {
        }

        void record_transition(const T &next)
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

        bool operator==(const T& state_data)
        {
            return data_ == state_data;
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

    template<typename T>
    class markov_chain
    {
        std::vector<state<T>> states_;

    public:
        size_t record_sample(const T &sample, const size_t prev_sample_state_idx = -1)
        {
            if (prev_sample_state_idx != -1 && prev_sample_state_idx >= states_.size())
            {
                throw std::runtime_error("previous sample index out of range");
            }

            auto s = std::find(std::begin(states_), std::end(states_), sample);

            if (s == std::end(states_))
            {
                // add sample as a new state
                states_.emplace_back(sample);
                s = states_.end() - 1;
            }

            if (prev_sample_state_idx != -1)
            {
                // record the transition from the previous state
                states_.at(prev_sample_state_idx).record_transition(sample);
            }

            return std::distance(std::begin(states_), s);
        }

        // Could definitely generalize this better so it would be more useful
        void generate_data(std::ostream &os, const T &delimiter, const T &start_criteria, std::function<bool(const T&)> end_criteria)
        {
            T elem = start_criteria;

            for (;;)
            {
                std::vector<state<T>>::iterator it = std::find(std::begin(states_), std::end(states_), elem);

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
}

#endif // __MARKOV_CHAIN_H__
