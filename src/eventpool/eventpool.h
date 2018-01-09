#ifndef EVENTPOOL_H
#define EVENTPOOL_H

#include <nan.h>
#include <queue>
#include <iostream>

using namespace Nan;

struct SignalKit {
  std::queue<int> queue;
  uv_cond_t *condition;
  uv_mutex_t *mutex;
};

class EventPool : public Nan::AsyncProgressQueueWorker<char> {
 public:
  EventPool(Nan::Callback *callback, Nan::Callback *eventCb, SignalKit *signal);
  ~EventPool();

  void Execute (const AsyncProgressQueueWorker::ExecutionProgress& progress);
  void HandleProgressCallback(const char *data, size_t count);

  private:
    Nan::Callback *eventCb;
    SignalKit *signal;
    std::queue<int> eventQueue;
};

#endif