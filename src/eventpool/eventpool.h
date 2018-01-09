#ifndef EVENTPOOL_H
#define EVENTPOOL_H

#include <nan.h>
#include <queue>
#include <iostream>

using namespace Nan;

#include <deezer-connect.h>
#include <deezer-player.h>

#define REGISTER_EVENT(wrapper, event)    \
  wrapper->queue.push(event);             \
  uv_mutex_lock(wrapper->mutex);          \
  uv_cond_signal(wrapper->condition);     \
  uv_mutex_unlock(wrapper->mutex);

class EventPool : public Nan::AsyncProgressQueueWorker<char> {
 public:
  EventPool(Nan::Callback *callback, Nan::Callback *eventCb);
  ~EventPool();

  void Execute (const AsyncProgressQueueWorker::ExecutionProgress& progress);
  void HandleProgressCallback(const char *data, size_t count);

  protected:
    Nan::Callback *eventCb;
    uv_cond_t *condition;
    uv_mutex_t *mutex;
    std::queue<int> queue;
};

class ConnectEventPool : public EventPool {
  public:
    ConnectEventPool(Nan::Callback *callback, Nan::Callback *eventCb) : EventPool(callback, eventCb) {}
    static void dzConnectEventCallback(
      dz_connect_handle handle,
      dz_connect_event_handle event,
      void *delegate
    );
};

class PlayerEventPool : public EventPool {
  public:
    PlayerEventPool(Nan::Callback *callback, Nan::Callback *eventCb) : EventPool(callback, eventCb) {}
    static void dzPlayerEventCallback(
      dz_player_handle handle,
      dz_player_event_handle event,
      void *delegate
    );
};

#endif