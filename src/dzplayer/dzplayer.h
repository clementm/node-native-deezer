#ifndef DZPLAYER_H
#define DZPLAYER_H

#include <nan.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include "../utils.h"

#include <deezer-connect.h>
#include <deezer-player.h>

#include "../dzconnect/dzconnect.h"

class DZPlayerHandler : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);

 private:
  explicit DZPlayerHandler(dz_connect_handle connect);
  ~DZPlayerHandler();

  static void dzPlayerEventCallback(
    dz_player_handle handle,
    dz_player_event_handle event,
    void *delegate);

  static NAN_METHOD(New);
  static NAN_METHOD(Activate);
  static NAN_METHOD(SetEventCb);
  static NAN_METHOD(SetOutputVolume);
  static NAN_METHOD(SetCrossfadingDuration);
  static NAN_METHOD(Load);
  static NAN_METHOD(Play);
  static NAN_METHOD(Stop);

  static inline Nan::Persistent<v8::Function> & constructor();

  dz_player_handle dzplayer;
};

#endif