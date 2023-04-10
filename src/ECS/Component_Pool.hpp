//
// Created by sterr on 5/03/2023.
//

#ifndef CORE_COMPONENT_POOL_HPP
#define CORE_COMPONENT_POOL_HPP
#include "Entity_map.hpp"
#include "Types.h"
#include <functional>

namespace lightning::ECS {
    /*
     * Component_Pool holds all components of a type for all entities dat have that component
     */
    template<typename T>
    class Component_Pool {
    public:
        /// check if entity has component
        /// - input: entity to check
        /// - output: true if entity has component
        bool Has(Entity_t entity) {
            return entity_data.contains(entity);
        }

        /// add entity to pool
        /// - input: entity to add
        void AddEntity(Entity_t entity) {
            for (auto& listener : AddListeners)
                listener(entity);
            T data;
            entity_data.add(entity, data);
        }

        /// add component to entity
        /// - input: entity to add to, component to add
        void AddEntity(Entity_t entity, T& data) {
            entity_data.add(entity, data);
            for (auto& listener : AddListeners)
                listener(entity);
        }

        /// remove entity from pool
        /// - input: entity to remove
        void RemoveEntity(Entity_t entity) {
            entity_data.remove(entity);
            for (auto& listener : RemoveListeners)
                listener(entity);
        }

        /// set component for entity
        /// - input: entity to set component for, component to set
        /// - output: component of entity
        void Set(Entity_t entity, T& data) {
            entity_data.get(entity) = data;
            for (auto& listener : UpdateListeners)
                listener(entity);
        }

        /// get component for entity
        /// - input: entity to get component from
        /// - output: component of entity
        T& Get(Entity_t entity) {
            return entity_data.get(entity);
        }

        /// Get the Entity Map object
        /// - output: Entity Map
        Entity_map<T>& GetEntityMap() {
            return entity_data;
        }

        /// register callback for when entity is added
        /// - input: callback function
        void RegisterAddCallback(const std::function<void(Entity_t)>& callback) {
            AddListeners.push_back(callback);
        }

        /// register callback for when entity is removed
        /// - input: callback function
        void RegisterRemoveCallback(const std::function<void(Entity_t)>& callback) {
            RemoveListeners.push_back(callback);
        }

        /// register callback for when entity is updated
        /// - input: callback function
        /// note: this only call when the component is set whit de set function
        void RegisterUpdateCallback(const std::function<void(Entity_t)>& callback) {
            UpdateListeners.push_back(callback);
        }

    private:
        Entity_map<T> entity_data;
        std::vector<std::function<void(Entity_t)>> AddListeners;
        std::vector<std::function<void(Entity_t)>> RemoveListeners;
        std::vector<std::function<void(Entity_t)>> UpdateListeners;
    };
} // ECS


#endif //CORE_COMPONENT_POOL_HPP
