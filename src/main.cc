#include <nan.h>

#include "dzconnect/dzconnect.h"
#include "dzplayer/dzplayer.h"

NAN_MODULE_INIT(Init) {
  DZConnectHandler::Init(target);
  DZPlayerHandler::Init(target);
}

NODE_MODULE(main, Init)
