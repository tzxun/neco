#ifndef NECO_H_
#define NECO_H_

#include <functional>
#include <memory>
#include <queue>

namespace neco {

using Func = std::function<void()>;

class TaskQueue {
 public:
  void Run() {
    auto f = Continuation();
    f();
  }

  Func Continuation() {
    Func task = task_queue_.front();
    task_queue_.pop();
    return task;
  }

  Func Front() {
    return task_queue_.front();
  }

  void Pop() {
    task_queue_.pop();
  }

  template <class... Args>
  void AddTask(Func task, Args... args) {
    task_queue_.push(task);
    AddTask(args...);
  }

  void AddTask(Func task) {
    task_queue_.push(task);
  }

  std::queue<Func> task_queue_;
};

#define NECO_BEGIN                                                 \
  auto __co_task_queue = std::make_shared<neco::TaskQueue>();      \
  auto __self = shared_from_this();                                \
  __co_task_queue->AddTask(                                        \
    [this, __self, __NECO_CONTEXT, __callback, __co_task_queue](){

#define NECO_CONTINUATION (__co_task_queue->Continuation())

#define CALL_CC(func, ...)                                        \
  func(NECO_CONTINUATION, ##__VA_ARGS__);                         \
  },                                                              \
  [this, __self, __NECO_CONTEXT, __callback, __co_task_queue](){    

#define NECO_RETURN *__co_task_queue = neco::TaskQueue();__callback();return;

#define NECO_END                                        \
  *__co_task_queue = neco::TaskQueue();__callback();}); \
  __co_task_queue->Run();

#define NECO_FUNC(func, ...)                     \
  void func(neco::Func __callback, ##__VA_ARGS__)

#define DEFINE_NECO_VAR(NECO_VAR_REFlist)     \
  struct {                                    \
    NECO_VAR_REFlist                          \
  } *__context_type;                          \
  auto __NECO_CONTEXT = std::make_shared<std::remove_reference<decltype(*__context_type)>::type>();

#define NECO_VAR_REF(__name) (__NECO_CONTEXT->__name)

#define NECO_CONTEXT __NECO_CONTEXT

#define NECO_WHILE(__cond)                                                      \
  auto __async_while = [this, __self, __NECO_CONTEXT](neco::Func __callback) {  \
      auto __co_task_queue = std::make_shared<neco::TaskQueue>();               \
      auto __co_task_queue_backup = std::make_shared<neco::TaskQueue>();        \
      __co_task_queue->AddTask(                                                 \
      [this, __self, __NECO_CONTEXT, __callback, __co_task_queue,               \
          __co_task_queue_backup](){                                            \
      if(!(__cond)) {                                                           \
        *__co_task_queue = neco::TaskQueue();                                   \
        *__co_task_queue_backup = neco::TaskQueue();                            \
        __callback();                                                           \
        return;                                                                 \
      }

#define NECO_WHILE_END                                                                        \
        auto __continuation = __co_task_queue->Continuation();                                \
        __continuation();                                                                     \
      },                                                                                      \
      [this, __self, __NECO_CONTEXT, __callback, __co_task_queue, __co_task_queue_backup]() { \
        __co_task_queue->task_queue_ = __co_task_queue_backup->task_queue_;                   \
        __co_task_queue->Run();                                                               \
      });                                                                                     \
      __co_task_queue_backup->task_queue_ = __co_task_queue->task_queue_;                     \
      __co_task_queue->Run();                                                                 \
    };                                                                                        \
    __async_while(__co_task_queue->Continuation());                                           \
  },                                                                                          \
  [this, __self, __NECO_CONTEXT, __callback, __co_task_queue](){

#define NECO_IF(__cond)                                                          \
    auto __async_if = [this, __self, __NECO_CONTEXT](neco::Func __callback) {    \
      auto __co_task_queue = std::make_shared<neco::TaskQueue>();                \
      __co_task_queue->AddTask(                                                  \
      [this, __self, __NECO_CONTEXT, __callback, __co_task_queue](){             \
        if(!(__cond)) {                                                          \
          *__co_task_queue = neco::TaskQueue();                                  \
          __callback();                                                          \
          return;                                                                \
        }

#define NECO_IF_END                                                           \
        *__co_task_queue = neco::TaskQueue();__callback();return;             \
      });                                                                     \
      __co_task_queue->Run();                                                 \
    };                                                                        \
    __async_if(__co_task_queue->Continuation());                              \
  },                                                                          \
  [this, __self, __NECO_CONTEXT, __callback, __co_task_queue](){

}  // namespace neco

#endif
