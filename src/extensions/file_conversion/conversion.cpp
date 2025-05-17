#include <napi.h>
#include <iostream>
#include "FileConversion.hpp"

void ParseFile(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 4) {
    std::string err_msg = "Wrong number of arguments, `";
    err_msg.append(std::to_string(info.Length()));
    err_msg.append("` given, `4` expected.");
    Napi::TypeError::New(env, err_msg).ThrowAsJavaScriptException();
    return;
  }

  bool firstArgCheck = info[0].IsString();
  bool secondArgCheck = info[1].IsString();
  bool thirdArgCheck = info[2].IsNumber();
  bool fourthArgCheck = info[3].IsNumber();

  if (!firstArgCheck || !secondArgCheck || !thirdArgCheck || !fourthArgCheck) {
    std::string err_msg = "Wrong argument types: \n";
    if (!firstArgCheck) err_msg.append("\tFirst argument needs to be of type: `String`\n");
    if (!secondArgCheck) err_msg.append("\tSecond argument needs to be of type: `String`\n");
    if (!thirdArgCheck) err_msg.append("\tThird argument needs to be of type: `Number`\n");
    if (!fourthArgCheck) err_msg.append("\tFourth argument needs to be of type: `Number`\n");
    Napi::TypeError::New(env, err_msg).ThrowAsJavaScriptException();
    return;
  }

  std::string file_name = info[0].As<Napi::String>().Utf8Value();
  std::string destination = info[1].As<Napi::String>().Utf8Value();
  Napi::Number thirdArg = info[2].As<Napi::Number>();
  Napi::Number fourthArg = info[3].As<Napi::Number>();
  unsigned long long char_distance_val = static_cast<unsigned long long>(thirdArg.DoubleValue());
  unsigned long long max_char_val = static_cast<unsigned long long>(fourthArg.DoubleValue());
  FileConversion converter(file_name, destination, char_distance_val, max_char_val);
  // converter.LogValues();
  converter.LoopFile();
  // Napi::String::New(env, "Hello from File Parser!");
  return;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("ParseFile", Napi::Function::New(env, ParseFile));
  return exports;
}

NODE_API_MODULE(file_conversion, Init)

