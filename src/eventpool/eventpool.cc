#include "eventpool.h"

EventPool::EventPool(Nan::Callback *callback, Nan::Callback *eventCb)
  : AsyncProgressQueueWorker(callback), eventCb(eventCb) {}

EventPool::~EventPool() {
  delete eventCb;
}

void ConnectEventPool::dzConnectEventCallback(
  dz_connect_handle handle,
  dz_connect_event_handle event,
  void *delegate
) {
  ConnectEventPool* wrapper = (ConnectEventPool*)delegate;
  
  REGISTER_EVENT(wrapper, dz_connect_event_get_type(event))
}

void PlayerEventPool::dzPlayerEventCallback(
  dz_player_handle handle,
  dz_player_event_handle event,
  void *delegate
) {
  PlayerEventPool* wrapper = (PlayerEventPool*)delegate;
  
  REGISTER_EVENT(wrapper, dz_player_event_get_type(event))
}

void EventPool::Execute (const AsyncProgressQueueWorker::ExecutionProgress& progress) {
  condition = new uv_cond_t();
  mutex = new uv_mutex_t();

  uv_cond_init(condition);
  uv_mutex_init(mutex);

  while (true) {
    uv_cond_wait(condition, mutex);

    progress.Send(0, 0);
  }
}

void EventPool::HandleProgressCallback(const char* data, size_t count) {
  Nan::HandleScope scope;

  while (!queue.empty()) {
    int event = queue.front();
    queue.pop();

    v8::Local<v8::Value> argv[] = {
        Nan::New<v8::Integer>(event)
    };

    eventCb->Call(1, argv);
  }
}
