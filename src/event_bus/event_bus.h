#ifndef INCLUDED_EVENT_BUS_H
#define INCLUDED_EVENT_BUS_H

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <typeindex>
#include <utility>

#include "../logger/logger.h"
#include "event.h"

class IEventCallback {
   private:
    virtual void call(Event& e) = 0;

   public:
    virtual ~IEventCallback() = default;
    void execute(Event& e) { call(e); }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
   private:
    typedef void (TOwner::*CallbackFunction)(TEvent&);
    TOwner* owner_instance;
    CallbackFunction callback;
    virtual void call(Event& e) override {
        std::invoke(callback, owner_instance, static_cast<TEvent&>(e));
    }

   public:
    EventCallback(TOwner* owner_instance, CallbackFunction callback)
        : owner_instance(owner_instance), callback(callback) {}
    virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
   private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

   public:
    EventBus() { Logger::log("EventBus constructor called!"); }

    ~EventBus() { Logger::log("EventBus DEstructor called!"); }

    /**
     * Clears the subscribers list
     */
    void reset() { subscribers.clear(); }

    /**
     * Subscribe to an event type <T>
     * example:
     *  event_bus->subscribe_to_event<CollisionEvent>(this,
     * &Game::on_collision);
     */
    template <typename TEvent, typename TOwner>
    void subscribe_to_event(TOwner* owner_instance,
                            void (TOwner::*callback)(TEvent&)) {
        if (!subscribers[typeid(TEvent)].get()) {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(
            owner_instance, callback);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    /**
     * Emit an event type <T>
     * example:
     *  event_bus->emit_event<CollisionEvent>(player, enemy);
     */
    template <typename TEvent, typename... TArgs>
    void emit_event(TArgs&&... args) {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers) {
            for (auto it = handlers->begin(); it != handlers->end(); it++) {
                auto handler = it->get();
                TEvent e(std::forward<TArgs>(args)...);
                handler->execute(e);
            }
        }
    }
};

#endif
