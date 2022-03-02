#ifndef NET_H
#define NET_H

#include <future>
#include <string>

// Module that interacts with a node's network properties.
// https://web3js.readthedocs.io/en/v1.7.0/web3-net.html

// TODO:
// - Decide what to do with callbacks for all functions

public class Net {
  public:
    // Gets the current network ID.
    std::promise<unsigned int> getId();

    // Checks if the node is listening to peers.
    std::promise<bool> isListening();

    // Get the number of peers connected to.
    std::promise<unsigned int> getPeerCount();
};

#endif  // NET_H
