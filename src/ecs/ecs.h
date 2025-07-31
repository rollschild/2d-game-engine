#ifndef INCLUDED_ECS_H
#define INCLUDED_ECS_H

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

/**
 * Use a bitset (0s and 1s) to keep track of which components an entity has,
 * and also helps keep track of which entities a system is interested in.
 */
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
   protected:
    static int next_id;
};

/**
 * Assign a unique ID to a component type
 */
template <typename T>
class Component : IComponent {
    static int get_id() {
        // - all instances of the same class will share the same next_id
        // variable
        // - `id` is a _static local_ variable - initialized only once and
        // retains its value between function calls
        static auto id = next_id++;
        return id;
    }
};

class Entity {
   private:
    int id;

   public:
    Entity(int id) : id(id) {};
    Entity(const Entity& entity) = default;

    int get_id() const;

    bool operator==(const Entity& other) const { return id == other.id; }
    bool operator!=(const Entity& other) const { return id != other.id; }
    bool operator>(const Entity& other) const { return id > other.id; }
    bool operator<(const Entity& other) const { return id < other.id; }

    Entity& operator=(const Entity& other) = default;
};

/**
 * System processes entities that contain a specific signature
 */
class System {
   private:
    Signature component_signature;
    std::vector<Entity> entities;

   public:
    System() = default;
    ~System() = default;

    void add_entity_to_system(Entity entity);
    void remove_entity_from_system(Entity entity);
    std::vector<Entity> get_system_entities() const;
    const Signature& get_component_signature() const;

    /**
     * Define the component type that entities must have to be considered by the
     * system
     */
    template <typename TComponent>
    void require_component();
};

template <typename TComponent>
void System::require_component() {
    const auto component_id = Component<TComponent>::get_id();
    component_signature.set(component_id);
}

#endif
