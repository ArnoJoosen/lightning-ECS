//
// Created by sterr on 5/03/2023.
//

#ifndef LIGHTNING_ENGINE_CORE_REGISTRY_H
#define LIGHTNING_ENGINE_CORE_REGISTRY_H
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <initializer_list>
#include "Types.h"
#include "View.hpp"
#include "Tag_Pool.h"
#include "Pool_Holder.h"
#include "Pool_view.h"

namespace lightning::ECS {
    /*
     * Registry is the main class for the ECS
     */

    class registry {
    public:
        /// create new entity \n
        /// - output: new entity
        Entity_t CreateEntity();

        /// destroy entity \n
        /// - input: entity to destroy
        void DestroyEntity(Entity_t entity); // TODO add global list of component pools for destruction entity

        /// check if is still active \n
        /// - input: entity to check \n
        /// - output: true if active \n
        bool IsActive(Entity_t entity);

        /// add component to entity \n
        /// - input: entity to add to
        template<typename T>
        void Add(Entity_t entity){
            Get_Pool<T>().AddEntity(entity);
        }

        /// add component to entity \n
        /// - input: entity to add to, component to add
        template<typename T>
        void Add(Entity_t entity, T& component){
            Get_Pool<T>().AddEntity(entity, component);
        }

        /// remove component from entity \n
        /// - input: entity to remove from
        template<typename T>
        void Remove(Entity_t entity){
            Get_Pool<T>().RemoveEntity(entity);
        }

        /// get component from entity \n
        /// - input: entity to get de component from \n
        /// - output: component of entity
        template<typename T>
        T& Get(Entity_t entity){
            return Get_Pool<T>().Get(entity);
        }

        /// add tag to entity \n
        /// - input: entity to add tag to, tag to add
        void AddTag(Entity_t entity, Tag_t tag);

        /// remove tag from entity \n
        /// - input: entity to remove tag from, tag to remove
        void RemoveTag(Entity_t entity, Tag_t tag);

        /// check if entity has tag \n
        /// - input: entity to check, tag to check \n
        /// - output: true if has tag
        bool HasTag(Entity_t entity, Tag_t tag);

        /// get number of entities \n
        /// - output: number of entities in registry
        size_t size() const { return Entitys.size(); }

        /// get begin iterator \n
        /// - output: begin iterator
        auto begin() { return Entitys.begin();}

        /// get end iterator \n
        /// - output: end iterator
        auto end() { return Entitys.end();}

        /// get component pool for type \n
        /// - input: type of component \n
        template<typename T>
        Component_Pool<T>& Get_Pool(){;
            return pools.Get_Pool<T>();
        }

        /// check if entity has component \n
        /// - input: entity to check \n
        template<typename T>
        bool Has(Entity_t entity){
            return Get_Pool<T>().Has(entity);
        }

        /// make view of entities with components which are not in exclude list \n
        /// - input: components to check, tags to check \n
        /// - output: view of entities
        template<typename ... Include, typename ... Exclude>
        View<Includes<Include...>, Excludes<Exclude...>>* view(std::initializer_list<Tag_t> tags = {}, Excludes<Exclude...> excludes = Excludes<>()) {
            using view_type = View<Includes<Include...>, Excludes<Exclude...>>;

            size_t id = views.GetID<Includes<Include...>, Excludes<Exclude...>>(tags);
            if (views.dus_view_exist(id))
                return (view_type*)views.get_view(id);

            view_type* ptr_view = new View<Includes<Include...>, Excludes<Exclude...>>(Entitys, pools, tags);
            views.add_view(id, (Base_View*)ptr_view);
            return ptr_view;
        }

    private:
        std::vector<Entity_t> Entitys; // list of active entities
        std::unordered_set<Entity_t> FreeEntitysSet; // set of free entities
        std::vector<Entity_t> FreeEntitys; // list of free entities
        Pool_Holder pools; // container of component and tag pools
        Pool_view views; // view of pools
        Entity_t NextEntity = 0; // next entity id
    };
} // ECS

#endif //LIGHTNING_ENGINE_CORE_REGISTRY_H
