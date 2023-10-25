#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <functional>

#include "../prefs.h"

#ifdef DEBUG
#define ECS_WARN(a) std::cerr << "ECS Warning: " << a << std::endl
#else
#define ECS_WARN(a)
#endif


class ECS
{
public:
    ECS() : m_last_ent(0) {}
    ~ECS()
    {
        for (auto& m : m_component_maps)
        {
            for (auto it = m.second.begin(); it != m.second.end();)
            {
                if (it->second != nullptr)
                    delete it->second;
                it = m.second.erase(it);
            }
        }
    }
 
    uint32_t new_entity()
    {
        m_last_ent++;
        return m_last_ent;
    }

    template<typename T>
    void add_component(uint32_t entity, T* component)
    {
        if (component == nullptr)
        {
            ECS_WARN("Component can't be null when adding it to an entity.\n    Created a new component of the same type (" << typeid(T).name() << ")");
            add_component<T>(entity);
            return;
        }
       
        auto& component_map = m_component_maps[typeid(T)];
        if (component_map.empty())
        {
            component_map = std::unordered_map<uint32_t, void*>();
        }
        
        if (component_map.find(entity) != component_map.end())
        {
            ECS_WARN("Entity already has a component of type " << typeid(T).name());
            return;
        }
 
        component_map[entity] = static_cast<void*>(component);
    }

    template<typename T>
    void add_component(uint32_t entity)
    {
        T* ptr = new T();
        add_component<T>(entity, ptr);
    }
 
    // returns nullptr if component doesn't exist
    template<typename T>
    T* get_component(uint32_t entity)
    {
        auto& component_map = m_component_maps[typeid(T)];
        return static_cast<T*>(component_map[entity]);
    }

    template<typename T>
    std::vector<T*> get_all_components()
    {
        auto& component_map = m_component_maps[typeid(T)];
        std::vector<T*> output;
 
        for (auto it : component_map)
        {
            output.push_back(static_cast<T*>(it.second));
        }
 
        return output;
    }
 
    // safe to call even if component doesn't exist
    template<typename T>
    void remove_component(uint32_t entity)
    {
        auto& component_map = m_component_maps[typeid(T)];
        T* component = static_cast<T*>(component_map[entity]);
 
        auto it = component_map.find(entity);
 
        if (it != component_map.end())
        {
            delete component;
            component_map.erase(it);
        }
        else
        {
            ECS_WARN("Tried to remove a component that doesn't exist.");
        }
    }

    /*  WARNING: NOT IMPLEMENTED YET
        The actual entity id is still valid but all
        components that belong to it are deleted. */
    void remove_entity(uint32_t entity)
    {
        ECS_WARN("Function \"remove_entity(uint32_t entity)\" doesn't have an implementation yet.");
    }

    template<typename T>
    void call_for_all(std::function<void(T*)> function)
    {
        auto& component_map = m_component_maps[typeid(T)];
        for (auto& it : component_map)
        {
            function(static_cast<T*>(it.second));
        }
    }

private:
    uint32_t m_last_ent;
    std::unordered_map<std::type_index, std::unordered_map<uint32_t, void*>> m_component_maps;
};

#endif