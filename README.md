Markov Chain Random Data Generator
===

Overview
---

This project contains an implementation of a simple header-only trainable Markov chain.

A console client is provided in `main.cpp` which can be invoked with 'n' filepaths for training data files. Example:
    `MarkovChain.exe training_file_1.txt training_file_2.txt ... training_file_n.txt`

Some examples of sentences generated on a model trained with Shakespeare's complete works:
> - "Now I can mine eyes, even of goosequills and the world- doth not deserv'd it, the QUEEN, and dirty shoe, and a sparrow flying."
> - "Look for your succession, but well, too dear: Why I shall be up his mind? Is't come to underprop his brain, being out of my youth."
> - "Ah! that's a rascal cook? Is in them dearer than by honour and Iago."
> - "Look where all together; she's dead."

TODO List
---

### TLC
- Nothing has been optimized. Data structures should be evaluated and profiled, wasteful data copies fixed, etc. This is pretty strictly a first draft right now.
- API is awkward and not very generalized
  - Data generation should provide a single sample and allow the caller to shape the structure explicitly rather than through a tight set of function parameters
  - ~~In the spirit of the above point, training should happen on a single sample basis as well~~

### New functionality/expansion ideas
- Use to generate smart fuzz testing vectors
- Better English language dynamic text generation
  - Smarter handling of grammatical elements like capitalization and punctuation
  - Further weight selection of future states topically (could make something similar to Cleverbot)
