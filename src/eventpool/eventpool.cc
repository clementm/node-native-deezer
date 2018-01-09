#include "eventpool.h"

EventPool::EventPool(Nan::Callback *callback, Nan::Callback *eventCb, SignalKit *signal)
  : AsyncProgressQueueWorker(callback), eventCb(eventCb), signal(signal) {}

EventPool::~EventPool() {
  delete eventCb;
}

void EventPool::Execute (const AsyncProgressQueueWorker::ExecutionProgress& progress) {
  signal->condition = new uv_cond_t();
  signal->mutex = new uv_mutex_t();

  uv_cond_init(signal->condition);
  uv_mutex_init(signal->mutex);

  while (true) {
    uv_mutex_lock(signal->mutex);
    uv_cond_wait(signal->condition, signal->mutex);
    uv_mutex_unlock(signal->mutex);

    progress.Send(0, 0);
  }
}

void EventPool::HandleProgressCallback(const char* data, size_t count) {
  Nan::HandleScope scope;

  while (!signal->queue.empty()) {
    int event = signal->queue.front();
    signal->queue.pop();

    v8::Local<v8::Value> argv[] = {
        Nan::New<v8::Integer>(event)
    };

    std::cout << "event" << eventCb << std::endl;

    eventCb->Call(1, argv);
  }
}
