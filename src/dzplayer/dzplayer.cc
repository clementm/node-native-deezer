#include "dzplayer.h"

NAN_MODULE_INIT(DZPlayerHandler::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("DZPlayer").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "activate", Activate);
  Nan::SetPrototypeMethod(tpl, "setEventCb", SetEventCb);
  Nan::SetPrototypeMethod(tpl, "setOutputVolume", SetOutputVolume);
  Nan::SetPrototypeMethod(tpl, "setCrossfadingDuration", SetCrossfadingDuration);
  Nan::SetPrototypeMethod(tpl, "load", Load);
  Nan::SetPrototypeMethod(tpl, "play", Play);
  Nan::SetPrototypeMethod(tpl, "stop", Stop);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("DZPlayer").ToLocalChecked(),
    Nan::GetFunction(tpl).ToLocalChecked());
}

DZPlayerHandler::DZPlayerHandler(dz_connect_handle connect) {
  dzplayer = dz_player_new(connect);
}
DZPlayerHandler::~DZPlayerHandler() {}

void DZPlayerHandler::dzPlayerEventCallback(
  dz_player_handle handle,
  dz_player_event_handle event,
  void *delegate
) {
  DZPlayerHandler* wrapper = (DZPlayerHandler*)delegate;
  std::cout << wrapper << " : " << dz_player_event_get_type(event) << std::endl;
}

NAN_METHOD(DZPlayerHandler::New) {
  if (info.IsConstructCall()) {
    DZConnectHandler *dzconnect = ObjectWrap::Unwrap<DZConnectHandler>(info[0]->ToObject());
    DZPlayerHandler *handle = new DZPlayerHandler(dzconnect->getHandle());

    handle->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor());
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

NAN_METHOD(DZPlayerHandler::Activate) {
  OBJECT_CONTEXT(DZPlayerHandler)
  dz_error_t error = dz_player_activate(wrapper->dzplayer, wrapper);

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::SetEventCb) {
  OBJECT_CONTEXT(DZPlayerHandler)
  dz_error_t error = dz_player_set_event_cb(wrapper->dzplayer, DZPlayerHandler::dzPlayerEventCallback);

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::SetOutputVolume) {
  OBJECT_CONTEXT(DZPlayerHandler)
  dz_error_t error = dz_player_set_output_volume(wrapper->dzplayer, NULL, NULL, info[0]->Int32Value());

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::SetCrossfadingDuration) {
  OBJECT_CONTEXT(DZPlayerHandler)
  dz_error_t error = dz_player_set_crossfading_duration(wrapper->dzplayer, NULL, NULL, info[0]->Int32Value());

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::Load) {
  OBJECT_CONTEXT(DZPlayerHandler)
  dz_error_t error = dz_player_load(wrapper->dzplayer, NULL, NULL, ToString(info[0]->ToString()).c_str());

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::Play) {
  OBJECT_CONTEXT(DZPlayerHandler)
  dz_error_t error = dz_player_play(
    wrapper->dzplayer,
    NULL,
    NULL,
    dz_player_play_command_t(info[0]->Int32Value()),
    info[1]->Int32Value()
  );
  
  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::Stop) {
  OBJECT_CONTEXT(DZPlayerHandler)
  dz_error_t error = dz_player_stop(
    wrapper->dzplayer,
    NULL,
    NULL
  );

  HANDLE_ERROR(error)
}

Nan::Persistent<v8::Function> & DZPlayerHandler::constructor() {
  static Nan::Persistent<v8::Function> my_constructor;
  return my_constructor;
}