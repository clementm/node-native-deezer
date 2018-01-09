#include "dzconnect.h"

NAN_METHOD(GetEventTypes) {
  const v8::Local<v8::Object> types = Nan::New<v8::Object>();

  addType(types, "DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE", DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE);
  addType(types, "DZ_CONNECT_EVENT_USER_ACCESS_TOKEN_OK", DZ_CONNECT_EVENT_USER_ACCESS_TOKEN_OK);
  addType(types, "DZ_CONNECT_EVENT_USER_ACCESS_TOKEN_FAILED", DZ_CONNECT_EVENT_USER_ACCESS_TOKEN_FAILED);
  addType(types, "DZ_CONNECT_EVENT_USER_NEW_OPTIONS", DZ_CONNECT_EVENT_USER_NEW_OPTIONS);
  addType(types, "DZ_CONNECT_EVENT_USER_LOGIN_FAIL_NETWORK_ERROR", DZ_CONNECT_EVENT_USER_LOGIN_FAIL_NETWORK_ERROR);
  addType(types, "DZ_CONNECT_EVENT_USER_LOGIN_FAIL_BAD_CREDENTIALS", DZ_CONNECT_EVENT_USER_LOGIN_FAIL_BAD_CREDENTIALS);
  addType(types, "DZ_CONNECT_EVENT_USER_LOGIN_FAIL_USER_INFO", DZ_CONNECT_EVENT_USER_LOGIN_FAIL_USER_INFO);
  addType(types, "DZ_CONNECT_EVENT_USER_LOGIN_FAIL_OFFLINE_MODE", DZ_CONNECT_EVENT_USER_LOGIN_FAIL_OFFLINE_MODE);
  addType(types, "DZ_CONNECT_EVENT_USER_LOGIN_OK", DZ_CONNECT_EVENT_USER_LOGIN_OK);
  addType(types, "DZ_CONNECT_EVENT_ADVERTISEMENT_START", DZ_CONNECT_EVENT_ADVERTISEMENT_START);
  addType(types, "DZ_CONNECT_EVENT_ADVERTISEMENT_STOP", DZ_CONNECT_EVENT_ADVERTISEMENT_STOP);
  addType(types, "DZ_CONNECT_EVENT_UNKNOWN", DZ_CONNECT_EVENT_UNKNOWN);

  info.GetReturnValue().Set(types);
}

NAN_MODULE_INIT(DZConnectHandler::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("DZConnect").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "debugLogDisable", DebugLogDisable);
  Nan::SetPrototypeMethod(tpl, "activate", Activate);
  Nan::SetPrototypeMethod(tpl, "cachePathSet", CachePathSet);
  Nan::SetPrototypeMethod(tpl, "setAccessToken", SetAccessToken);
  Nan::SetPrototypeMethod(tpl, "offlineMode", OfflineMode);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("DZConnect").ToLocalChecked(),
    Nan::GetFunction(tpl).ToLocalChecked());
  
  Nan::Set(target, Nan::New<v8::String>("getEventTypes").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GetEventTypes)->GetFunction());
}

dz_connect_handle DZConnectHandler::getHandle() {
  return dzconnect;
}

DZConnectHandler::DZConnectHandler(dz_connect_configuration configuration, Nan::Callback *onConnectEventCb) {
  configuration.connect_event_cb = DZConnectHandler::dzConnectEventCallback;
  dzconnect = dz_connect_new(&configuration);

  signal = new SignalKit();
  eventPool = new EventPool(onConnectEventCb, onConnectEventCb, signal);
  AsyncQueueWorker(eventPool);
}

DZConnectHandler::~DZConnectHandler() {}

void DZConnectHandler::dzConnectEventCallback(
  dz_connect_handle handle,
  dz_connect_event_handle event,
  void *delegate
) {
  DZConnectHandler* wrapper = (DZConnectHandler*)delegate;
  
  wrapper->signal->queue.push(dz_connect_event_get_type(event));

  uv_mutex_lock(wrapper->signal->mutex);
  uv_cond_signal(wrapper->signal->condition);
  uv_mutex_unlock(wrapper->signal->mutex);
}

NAN_METHOD(DZConnectHandler::New) {
  if (info.IsConstructCall()) {
    const v8::Local<v8::Object> settings = info[0]->ToObject();
    struct dz_connect_configuration configuration = buildConfiguration(settings);

    Nan::Callback *onConnectEventCb = new Callback(Nan::To<v8::Function>(settings->Get(Nan::New("connectEventCb").ToLocalChecked())).ToLocalChecked());
    onConnectEventCb->Call(0, {});
    DZConnectHandler *handle = new DZConnectHandler(configuration, onConnectEventCb);
    handle->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor());
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

NAN_METHOD(DZConnectHandler::DebugLogDisable) {
  OBJECT_CONTEXT(DZConnectHandler)
  dz_connect_debug_log_disable(wrapper->dzconnect);
}

NAN_METHOD(DZConnectHandler::Activate) {
  OBJECT_CONTEXT(DZConnectHandler)
  dz_error_t error = dz_connect_activate(wrapper->dzconnect, wrapper);

  HANDLE_ERROR(error)
}

NAN_METHOD(DZConnectHandler::CachePathSet) {
  OBJECT_CONTEXT(DZConnectHandler)
  dz_error_t error = dz_connect_cache_path_set(wrapper->dzconnect, NULL, NULL, ToString(info[0]->ToString()).c_str());

  HANDLE_ERROR(error)
}


typedef struct {
  uv_cond_t *cond;
  uv_mutex_t *mutex;
} signalisation;

class CallbackWorker : public Nan::AsyncWorker {
  public:
    CallbackWorker(Nan::Callback *callback, signalisation *sig)
      : AsyncWorker(callback), sig(sig) {}
    ~CallbackWorker() {}

    void Execute () {

      std::cout << sig << std::endl;

      sig->cond = new uv_cond_t();
      sig->mutex = new uv_mutex_t();
      
      uv_cond_init(sig->cond);
      uv_mutex_init(sig->mutex);
      std::cout << "signalisation : " << sig->cond << std::endl;


      // uv_cond_wait(cond, &mutex);

      // // uv_cond_signal(cond);

      // uv_mutex_t mutex;
      // uv_mutex_init(&mutex);

      uv_mutex_lock(sig->mutex);
      uv_cond_wait(sig->cond, sig->mutex);
      uv_mutex_unlock(sig->mutex);

      std::cout << "Hello !" << std::endl;
    }

    void HandleOKCallback () {
    }

  private:
    signalisation *sig;
};

void onCallbackResponse(
  void* delegate,
  void* operation_userdata,
  dz_error_t status,
  dz_object_handle result
) {
  std::cout << status << " " << result << std::endl;
  signalisation *sig = (signalisation*)operation_userdata;
  std::cout << "salut ! " << sig->cond << std::endl;
  uv_mutex_lock(sig->mutex);
  uv_cond_signal(sig->cond);
  uv_mutex_unlock(sig->mutex);
}

NAN_METHOD(DZConnectHandler::SetAccessToken) {
  OBJECT_CONTEXT(DZConnectHandler)
  
  signalisation *sig;
  sig = new signalisation();

  Nan::Callback *callback = new Nan::Callback(Nan::To<v8::Function>(info[1]).ToLocalChecked());

  AsyncQueueWorker(new CallbackWorker(callback, sig));
  
  dz_error_t error = dz_connect_set_access_token(
    wrapper->dzconnect,
    &onCallbackResponse,
    sig,
    ToString(info[0]->ToString()).c_str()
  );

  HANDLE_ERROR(error)
}

NAN_METHOD(DZConnectHandler::OfflineMode) {
  OBJECT_CONTEXT(DZConnectHandler)
  dz_error_t error = dz_connect_offline_mode(wrapper->dzconnect, NULL, NULL, info[0]->ToBoolean()->BooleanValue());

  HANDLE_ERROR(error)
}

Nan::Persistent<v8::Function> & DZConnectHandler::constructor() {
  static Nan::Persistent<v8::Function> my_constructor;
  return my_constructor;
}