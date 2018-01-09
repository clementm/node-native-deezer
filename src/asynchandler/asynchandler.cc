#include "asynchandler.h"
#include <iostream>

AsyncHandler::AsyncHandler(Nan::Callback *callback)
      : AsyncWorker(callback), callback(callback) {}

AsyncHandler::~AsyncHandler() {}

void AsyncHandler::onCallbackResponse(
  void* delegate,
  void* operation_userdata,
  dz_error_t status,
  dz_object_handle result
) {
  AsyncHandler *handler = ((AsyncHandler*)operation_userdata);

  std::cout << result << std::endl;

  uv_mutex_lock(handler->mutex);
  uv_cond_signal(handler->condition);
  uv_mutex_unlock(handler->mutex);
}

void AsyncHandler::Execute() {
  condition = new uv_cond_t;
  mutex = new uv_mutex_t;

  uv_cond_init(condition);
  uv_mutex_init(mutex);

  uv_mutex_lock(mutex);
  uv_cond_wait(condition, mutex);
  uv_mutex_unlock(mutex);
}

void AsyncHandler::HandleOKCallback()  {
  this->callback->Call(0, NULL);
}