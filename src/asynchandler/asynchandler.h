#ifndef ASYNCHANDLER_H
#define ASYNCHANDLER_H

#include <nan.h>

#include <deezer-connect.h>

#define SETUP_ASYNC(arg, handler, callback)                               \
  AsyncHandler *handler = NULL;                                           \
  dz_activity_operation_callback callback = NULL;                         \
  if (arg->IsFunction()) {                                                \
    handler = new AsyncHandler(                                           \
      new Nan::Callback(Nan::To<v8::Function>(arg).ToLocalChecked())      \
    );                                                                    \
    callback = &AsyncHandler::onCallbackResponse;                         \
    AsyncQueueWorker(handler);                                            \
  }



class AsyncHandler : public Nan::AsyncWorker {
  public:
    AsyncHandler(Nan::Callback *callback);
    ~AsyncHandler();

    void Execute ();
    void HandleOKCallback ();

    static void onCallbackResponse(
      void* delegate,
      void* operation_userdata,
      dz_error_t status,
      dz_object_handle result
    );

  private:
    Nan::Callback *callback;
    uv_cond_t *condition;
    uv_mutex_t *mutex;
};

#endif