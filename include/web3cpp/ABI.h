#ifndef ABI_H
#define ABI_H

#include <string>

#include <nlohmann/json>

using json = nlohmann::json;

// Module that encodes/decodes ABI parameters for function calls.
// https://web3js.readthedocs.io/en/v1.7.0/web3-eth-abi.html

// TODO:
// - Check if "array" types would actually be "mixed"
//   - Docs sometimes shows examples using only strings, other times with ints mixed in

class ABI {
  public:
    // Encodes the function name to its ABI signature (first 4 bytes of the
    // sha3 hash of the function name including types).
    // If using the function name as string, it has to be in the form
    // `function(type,type,...)`, e.g. `myFunction(uint256,uint32[],bytes10,bytes)`.
    // If using a JSON interface object, please refer to
    // https://web3js.readthedocs.io/en/v1.7.0/glossary.html#json-interface
    std::string encodeFunctionSignature(std::string functionName);
    std::string encodeFunctionSignature(json functionObj);

    // Encodes the event name to its ABI signature (sha3 hash of the event
    // name including input types). Same details as above.
    std::string encodeEventSignature(std::string eventName);
    std::string encodeEventSignature(json eventObj);

    // Encodes a parameter based on its type to its ABI representation.
    // See https://docs.soliditylang.org/en/develop/types.html
    // for a list of types.
    //std::string encodeParameter(std::string type, mixed parameter);
    //std::string encodeParameter(json type, mixed parameter);

    // Encodes a function's parameters based on its JSON interface object.
    // Same details as above.
    //std::string encodeParameters(std::vector<std::string> typesArray, array parameters);
    //std::string encodeParameters(std::vector<json> typesArray, array parameters);
    //std::string encodeParameters(json typesArray, array parameters);

    // Encodes a function call using its JSON interface object and given parameters.
    //std::string encodeFunctionCall(json jsonInterface, array parameters);

    // Decodes an ABI encoded parameter to its intended type.
    //mixed decodeParameter(std::string type, std::string hexString);
    //mixed decodeParameter(json type, std::string hexString);

    // Decodes ABI encoded parameters to their intended types.
    json decodeParameters(std::vector<std::string> typesArray, std::string hexString);
    json decodeParameters(std::vector<json> typesArray, std::string hexString);
    json decodeParameters(json typesArray, std::string hexString);

    // Decodes ABI-encoded log data and indexed topic data.
    //json decodeLog(json inputs, std::string hexString, array topics);
};

#endif  // ABI_H
