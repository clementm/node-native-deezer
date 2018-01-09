#include "dzplayer.h"

NAN_METHOD(DZPlayerHandler::GetEventTypes) {
  const v8::Local<v8::Object> types = Nan::New<v8::Object>();

  addType(types, "DZ_PLAYER_EVENT_UNKNOWN", DZ_PLAYER_EVENT_UNKNOWN);
  addType(types, "DZ_PLAYER_EVENT_LIMITATION_FORCED_PAUSE", DZ_PLAYER_EVENT_LIMITATION_FORCED_PAUSE);
  addType(types, "DZ_PLAYER_EVENT_QUEUELIST_LOADED", DZ_PLAYER_EVENT_QUEUELIST_LOADED);
  addType(types, "DZ_PLAYER_EVENT_QUEUELIST_NO_RIGHT", DZ_PLAYER_EVENT_QUEUELIST_NO_RIGHT);
  addType(types, "DZ_PLAYER_EVENT_QUEUELIST_TRACK_NOT_AVAILABLE_OFFLINE", DZ_PLAYER_EVENT_QUEUELIST_TRACK_NOT_AVAILABLE_OFFLINE);
  addType(types, "DZ_PLAYER_EVENT_QUEUELIST_TRACK_RIGHTS_AFTER_AUDIOADS", DZ_PLAYER_EVENT_QUEUELIST_TRACK_RIGHTS_AFTER_AUDIOADS);
  addType(types, "DZ_PLAYER_EVENT_QUEUELIST_SKIP_NO_RIGHT", DZ_PLAYER_EVENT_QUEUELIST_SKIP_NO_RIGHT);
  addType(types, "DZ_PLAYER_EVENT_QUEUELIST_TRACK_SELECTED", DZ_PLAYER_EVENT_QUEUELIST_TRACK_SELECTED);
  addType(types, "DZ_PLAYER_EVENT_QUEUELIST_NEED_NATURAL_NEXT", DZ_PLAYER_EVENT_QUEUELIST_NEED_NATURAL_NEXT);
  addType(types, "DZ_PLAYER_EVENT_MEDIASTREAM_DATA_READY", DZ_PLAYER_EVENT_MEDIASTREAM_DATA_READY);
  addType(types, "DZ_PLAYER_EVENT_MEDIASTREAM_DATA_READY_AFTER_SEEK", DZ_PLAYER_EVENT_MEDIASTREAM_DATA_READY_AFTER_SEEK);
  addType(types, "DZ_PLAYER_EVENT_RENDER_TRACK_START_FAILURE", DZ_PLAYER_EVENT_RENDER_TRACK_START_FAILURE);
  addType(types, "DZ_PLAYER_EVENT_RENDER_TRACK_START", DZ_PLAYER_EVENT_RENDER_TRACK_START);
  addType(types, "DZ_PLAYER_EVENT_RENDER_TRACK_END", DZ_PLAYER_EVENT_RENDER_TRACK_END);
  addType(types, "DZ_PLAYER_EVENT_RENDER_TRACK_PAUSED", DZ_PLAYER_EVENT_RENDER_TRACK_PAUSED);
  addType(types, "DZ_PLAYER_EVENT_RENDER_TRACK_SEEKING", DZ_PLAYER_EVENT_RENDER_TRACK_SEEKING);
  addType(types, "DZ_PLAYER_EVENT_RENDER_TRACK_UNDERFLOW", DZ_PLAYER_EVENT_RENDER_TRACK_UNDERFLOW);
  addType(types, "DZ_PLAYER_EVENT_RENDER_TRACK_RESUMED", DZ_PLAYER_EVENT_RENDER_TRACK_RESUMED);
  addType(types, "DZ_PLAYER_EVENT_RENDER_TRACK_REMOVED", DZ_PLAYER_EVENT_RENDER_TRACK_REMOVED);

  info.GetReturnValue().Set(types);
}

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

  Nan::Set(
    target->Get(Nan::New("DZPlayer").ToLocalChecked())->ToObject(),
    Nan::New<v8::String>("getEventTypes").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GetEventTypes)->GetFunction()
  );
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
  dz_error_t error = dz_player_activate(wrapper->dzplayer, wrapper->eventPool);

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::SetEventCb) {
  OBJECT_CONTEXT(DZPlayerHandler)
  dz_error_t error = dz_player_set_event_cb(wrapper->dzplayer, PlayerEventPool::dzPlayerEventCallback);

  Nan::Callback *onPlayerEventCb = new Callback(Nan::To<v8::Function>(info[0]).ToLocalChecked());
  
  wrapper->eventPool = new PlayerEventPool(onPlayerEventCb, onPlayerEventCb);

  AsyncQueueWorker(wrapper->eventPool);

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::SetOutputVolume) {
  OBJECT_CONTEXT(DZPlayerHandler)
  SETUP_ASYNC(info[1], sig, op_cb)

  dz_error_t error = dz_player_set_output_volume(wrapper->dzplayer, op_cb, sig, info[0]->Int32Value());

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::SetCrossfadingDuration) {
  OBJECT_CONTEXT(DZPlayerHandler)
  SETUP_ASYNC(info[1], sig, op_cb)

  dz_error_t error = dz_player_set_crossfading_duration(wrapper->dzplayer, op_cb, sig, info[0]->Int32Value());

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::Load) {
  OBJECT_CONTEXT(DZPlayerHandler)
  SETUP_ASYNC(info[1], sig, op_cb)

  dz_error_t error = dz_player_load(wrapper->dzplayer, op_cb, sig, ToString(info[0]->ToString()).c_str());

  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::Play) {
  OBJECT_CONTEXT(DZPlayerHandler)
  SETUP_ASYNC(info[1], sig, op_cb)

  dz_error_t error = dz_player_play(
    wrapper->dzplayer,
    op_cb,
    sig,
    dz_player_play_command_t(info[0]->Int32Value()),
    info[1]->Int32Value()
  );
  
  HANDLE_ERROR(error)
}

NAN_METHOD(DZPlayerHandler::Stop) {
  OBJECT_CONTEXT(DZPlayerHandler)
  SETUP_ASYNC(info[1], sig, op_cb)

  dz_error_t error = dz_player_stop(
    wrapper->dzplayer,
    op_cb,
    sig
  );

  HANDLE_ERROR(error)
}

Nan::Persistent<v8::Function> & DZPlayerHandler::constructor() {
  static Nan::Persistent<v8::Function> my_constructor;
  return my_constructor;
}