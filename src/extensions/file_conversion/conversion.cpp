#include <napi.h>
#include <iostream>

#include "FileConversion.hpp"
#include "font_conversion/WoffParsing.hpp"

#define EXPECTED_ARG_NUM 2
#define WOFF_EXPECTED_ARG_NUM 2

void ParseFile(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < EXPECTED_ARG_NUM) {
    std::string err_msg = "Wrong number of arguments, `";
    err_msg.append(std::to_string(info.Length()));
    err_msg.append("` given, `");
    err_msg.append(std::to_string(EXPECTED_ARG_NUM));
    err_msg.append("` expected.");
    Napi::TypeError::New(env, err_msg).ThrowAsJavaScriptException();
    return;
  }

  bool firstArgCheck = info[0].IsString();
  bool secondArgCheck = info[1].IsString();
  // bool thirdArgCheck = info[2].IsNumber();

  if (!firstArgCheck || !secondArgCheck) {
    std::string err_msg = "Wrong argument types: \n";
    if (!firstArgCheck) err_msg.append("\tFirst argument needs to be of type: `String`\n");
    if (!secondArgCheck) err_msg.append("\tSecond argument needs to be of type: `String`\n");
    // if (!thirdArgCheck) err_msg.append("\tThird argument needs to be of type: `Number`\n");
    Napi::TypeError::New(env, err_msg).ThrowAsJavaScriptException();
    return;
  }

  std::string file_name = info[0].As<Napi::String>().Utf8Value();
  std::string destination = info[1].As<Napi::String>().Utf8Value();
  // Napi::Number thirdArg = info[2].As<Napi::Number>();
  // unsigned long long char_distance_val = static_cast<unsigned long long>(thirdArg.DoubleValue());
  FileConversion converter(file_name, destination);
  // converter.LogValues();
  converter.LoopFile();
  // Napi::String::New(env, "Hello from File Parser!");
  return;
}

void ParseWoff(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < WOFF_EXPECTED_ARG_NUM) {
    std::string err_msg = "Wrong number of arguments, `";
    err_msg.append(std::to_string(info.Length()));
    err_msg.append("` given, `");
    err_msg.append(std::to_string(WOFF_EXPECTED_ARG_NUM));
    err_msg.append("` expected.");
    Napi::TypeError::New(env, err_msg).ThrowAsJavaScriptException();
  }

  bool first_arg_check = info[0].IsString();
  bool second_arg_check = info[1].IsString();

  if (!first_arg_check || !second_arg_check) {
    std::string err_msg = "Wrong argument types: \n";
    if (!first_arg_check) err_msg.append("\tFirst argument needs to be of type: `String`\n");
    if (!second_arg_check) err_msg.append("\tSecond argument needs to be of type: `String`\n");
    Napi::TypeError::New(env, err_msg).ThrowAsJavaScriptException();
    return;
  }

  std::string file_name = info[0].As<Napi::String>().Utf8Value();
  std::string destination = info[1].As<Napi::String>().Utf8Value();

  WOFFParser parser(file_name, destination);
  parser.ReadTables();
  return;

}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("ParseFile", Napi::Function::New(env, ParseFile));
  exports.Set("ParseWoff", Napi::Function::New(env, ParseWoff));
  return exports;
}

NODE_API_MODULE(file_conversion, Init)

