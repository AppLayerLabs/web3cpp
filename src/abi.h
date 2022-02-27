#include <string>

// Module that encodes/decodes ABI parameters for function calls

// TODO:
// - Check if the following functions can use objects as parameters:
//   - encodeFunctionSignature(), encodeEventSignature(), decodeParameter()

public class abi {
  public:
    std::string encodeFunctionSignature(std::string functionName);
    std::string encodeEventSignature(std::string eventName);
    //std::string encodeParameter(string|object type, mixed parameter);
    //std::string encodeParameters(array<string|object>|object typesArray, array parameters);
    //std::string encodeFunctionCall(object jsonInterface, array parameters);
    //mixed decodeParameter(std::string type, std::string hexString);
    //object decodeParameters(array<string|object>|object typesArray, std::string hexString);
    //object decodeLog(object inputs, std::string hexString, array topics);
};
