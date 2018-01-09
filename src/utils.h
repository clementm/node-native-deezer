#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <nan.h>

#include <deezer-connect.h>

#define OBJECT_CONTEXT(object) object *wrapper = Nan::ObjectWrap::Unwrap<object>(info.Holder());

#define HANDLE_ERROR(error) if (error != DZ_ERROR_NO_ERROR) return Nan::ThrowError("An error occured");

inline std::string ToString(v8::Local<v8::String> str) {
  v8::String::Utf8Value value(str);
  return std::string(*value);
}

inline std::string GetString(v8::Local<v8::Object> object, char const* key) {
  return ToString(object->Get(Nan::New(key).ToLocalChecked())->ToString());
};

inline struct dz_connect_configuration buildConfiguration(v8::Local<v8::Object> settings) {
  struct dz_connect_configuration config;

  memset(&config, 0, sizeof(struct dz_connect_configuration));

  config.app_id = GetString(settings, "appId").c_str();
  config.product_id = GetString(settings, "productId").c_str();
  config.product_build_id = GetString(settings, "productBuildId").c_str();
  config.user_profile_path = GetString(settings, "userProfilePath").c_str();

  return config;
}

inline void addType(v8::Local<v8::Object> types, const char* typeName, int typeValue) {
  types->Set(Nan::New(typeName).ToLocalChecked(), Nan::New(typeValue));
}

#endif