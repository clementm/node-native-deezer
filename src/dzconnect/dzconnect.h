#ifndef DZCONNECT_H
#define DZCONNECT_H

#include <nan.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include "../utils.h"
#include "../eventpool/eventpool.h"
#include "../asynchandler/asynchandler.h"

#include <deezer-connect.h>
#include <deezer-player.h>

class DZConnectHandler : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);

 dz_connect_handle getHandle();

 private:
  explicit DZConnectHandler(dz_connect_configuration configuration, Nan::Callback *onConnectEventCb);
  ~DZConnectHandler();

  static void dzConnectEventCallback(
    dz_connect_handle handle,
    dz_connect_event_handle event,
    void *delegate
  );

  static NAN_METHOD(New);
  static NAN_METHOD(DebugLogDisable);
  static NAN_METHOD(Activate);
  static NAN_METHOD(CachePathSet);
  static NAN_METHOD(SetAccessToken);
  static NAN_METHOD(OfflineMode);
  
  static NAN_METHOD(GetEventTypes);

  static inline Nan::Persistent<v8::Function> & constructor();

  dz_connect_handle dzconnect;
  ConnectEventPool *eventPool;
};

#endif