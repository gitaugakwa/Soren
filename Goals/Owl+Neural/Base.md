#BIND_EVENT_FN
define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#EventCallbackFn
using EventCallbackFn = std::function<void(Event&)>