#ifndef __stew_ui_event_hpp__
#define __stew_ui_event_hpp__

#include <optional>
#include <string>
#include <list>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace stew
{
  template <typename M>
  std::scoped_lock<M> make_scoped(M &mutex)
  {
    return std::scoped_lock<M>(mutex);
  }

  template <typename M>
  std::unique_lock<M> make_uniquelk(M &mutex)
  {
    return std::unique_lock<M>(mutex);
  }

  struct message
  {
    std::string _data;
  };

  class subscriber
  {
    std::mutex _mutex;
    std::condition_variable _empty_guard;
    std::queue<message> _messages;
    bool _closed = false;
    
  public:
    subscriber() = default;
    subscriber(const subscriber &) = delete;
    subscriber(subscriber &&o) = default;
    subscriber &operator=(const subscriber &) = delete;
    subscriber &operator=(subscriber &&) = default;

  public:
    std::optional<message> consume();

    friend class topic;
  };

  class topic
  {
    std::mutex _mutex;
    std::list<subscriber> _subscribers;

  public:
    topic() = default;
    topic(const topic&) = delete;
    topic(topic&&) = default;
    topic& operator=(const topic&) = delete;
    topic& operator=(topic&&) = default;

  public:
    subscriber &subscribe();
    void post(const message &mess);
    void close();
  };
}

#endif