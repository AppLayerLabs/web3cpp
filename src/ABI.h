#ifndef ABI_H
#define ABI_H

#include <string>

// Module that encodes/decodes ABI parameters for function calls.
// https://web3js.readthedocs.io/en/v1.7.0/web3-eth-abi.html

// TODO:
// - Implement support for JSON objects (maybe nlohmann/json?)
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
    //std::string encodeFunctionSignature(jsonObj functionObj);

    // Encodes the event name to its ABI signature (sha3 hash of the event
    // name including input types). Same details as above.
    std::string encodeEventSignature(std::string eventName);
    //std::string encodeEventSignature(jsonObj eventObj);

    // Encodes a parameter based on its type to its ABI representation.
    // See https://docs.soliditylang.org/en/develop/types.html
    // for a list of types.
    //std::string encodeParameter(std::string type, mixed parameter);
    //std::string encodeParameter(jsonObj type, mixed parameter);

    // Encodes a function's parameters based on its JSON interface object.
    // Same details as above.
    //std::string encodeParameters(std::vector<std::string> typesArray, array parameters);
    //std::string encodeParameters(std::vector<jsonObj> typesArray, array parameters);
    //std::string encodeParameters(jsonObj typesArray, array parameters);

    // Encodes a function call using its JSON interface object and given parameters.
    //std::string encodeFunctionCall(jsonObj jsonInterface, array parameters);

    // Decodes an ABI encoded parameter to its intended type.
    //mixed decodeParameter(std::string type, std::string hexString);
    //mixed decodeParameter(jsonObj type, std::string hexString);

    // Decodes ABI encoded parameters to their intended types.
    //jsonObj decodeParameters(std::vector<std::string> typesArray, std::string hexString);
    //jsonObj decodeParameters(std::vector<jsonObj> typesArray, std::string hexString);
    //jsonObj decodeParameters(jsonObj typesArray, std::string hexString);

    // Decodes ABI-encoded log data and indexed topic data.
    //jsonObj decodeLog(jsonObj inputs, std::string hexString, array topics);
};

#endif  // ABI_H
