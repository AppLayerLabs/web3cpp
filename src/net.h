#include <future>
#include <string>

// Module that interacts with network properties.

// TODO:
// - Decide what to do with callbacks for all functions
// - Maybe check if ints can be unsigned?

public class net {
  public:
    std::promise<int> getId();
    std::promise<bool> isListening();
    std::promise<int> getPeerCount();
};
