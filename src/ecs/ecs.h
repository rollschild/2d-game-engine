#ifndef INCLUDED_ECS_H
#define INCLUDED_ECS_H

#include <bitset>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <utility>
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

/**
 * Parent/base class of Pool
 */
class IPool {
   public:
    virtual ~IPool() {}
};

/**
 * Pool is just a vector (contiguous data) of objects of type T
 */
template <typename T>
class Pool : IPool {
   private:
    std::vector<T> data;

   public:
    Pool(int size = 100) { data.resize(size); }
    virtual ~Pool() = default;

    bool is_empty() const { return data.empty(); }
    int get_size() const { return data.size(); }
    void resize(int n) { data.resize(n); }
    void clear() { data.clear(); }
    void add(T object) { data.push_back(object); }
    void set(int index, T obj) { data[index] = obj; }
    T& get(int index) { return static_cast<T&>(data[index]); }
    T& operator[](unsigned int index) { return data[index]; }
};

/**
 * Registry manages creation/destruction of entities, systems, and components
 */
class Registry {
   private:
    int num_entities = 0;
    std::set<Entity> entities_to_be_added;
    std::set<Entity> entities_to_be_killed;

    // Vector of component pools
    // each pool contains all data for a certain component
    // pool index = entity ID
    // IPool is used because of the <Pool*> here
    std::vector<IPool*> component_pools;

    // component signature per entity
    // index is entity ID
    std::vector<Signature> entity_component_sigs;

    std::unordered_map<std::type_index, System*> systems;

   public:
    Registry() = default;
    Entity create_entity();
    void update();
    void kill_entity(Entity entity);

    template <typename T, typename... TArgs>
    void add_component(Entity ent, TArgs&&... args);

    void get_component(Entity ent);

    template <typename T>
    void remove_component(Entity ent);
    template <typename T>
    bool has_component(Entity ent) const;

    template <typename TSystem, typename... TArgs>
    void add_system(TArgs&&... args);

    template <typename TSystem>
    void remove_system();

    template <typename TSystem>
    bool has_system() const;

    template <typename TSystem>
    TSystem& get_system() const;

    void add_entity_to_systems(Entity ent);
};

template <typename TSystem, typename... TArgs>
void Registry::add_system(TArgs&&... args) {
    TSystem* new_system = new TSystem(std::forward<TArgs>(args)...);
    systems.insert(
        std::make_pair(std::type_index(typeid(TSystem)), new_system));
}

template <typename TSystem>
void Registry::remove_system() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template <typename TSystem>
bool Registry::has_system() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::get_system() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TComponent, typename... TArgs>
void Registry::add_component(Entity ent, TArgs&&... args) {
    const auto component_id = Component<TComponent>::get_id();
    const auto entity_id = ent.get_id();

    if (component_id >= component_pools.size()) {
        component_pools.resize(component_id + 1, nullptr);
    }

    if (!component_pools[component_id]) {
        Pool<TComponent>* new_component_pool = new Pool<TComponent>();
        component_pools[component_id] = new_component_pool;
    }

    Pool<TComponent>* component_pool = component_pools[component_id];

    if (entity_id >= component_pool->get_size()) {
        component_pool->resize(num_entities);
    }

    TComponent new_component(std::forward<TArgs>(args)...);

    component_pool->set(entity_id, new_component);
    entity_component_sigs[entity_id].set(component_id);
}

template <typename T>
void Registry::remove_component(Entity ent) {
    const auto component_id = Component<T>::get_id();
    const auto entity_id = ent.get_id();

    entity_component_sigs[entity_id].set(component_id, false);
}

template <typename T>
bool Registry::has_component(Entity ent) const {
    const auto component_id = Component<T>::get_id();
    const auto entity_id = ent.get_id();

    return entity_component_sigs[entity_id].test(component_id);
}

#endif
