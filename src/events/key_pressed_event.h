#ifndef INCLUDED_KEY_PRESSED_EVENT_H
#define INCLUDED_KEY_PRESSED_EVENT_H

#include <SDL2/SDL_keycode.h>

#include "../ecs/ecs.h"
#include "../event_bus/event.h"

class KeyPressedEvent : public Event {
   public:
    SDL_Keycode symbol;
    KeyPressedEvent(SDL_Keycode symbol) : symbol(symbol) {}
};

#endif
