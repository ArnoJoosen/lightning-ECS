//
// Created by sterr on 5/03/2023.
//

#ifndef CORE_VIEW_HPP
#define CORE_VIEW_HPP
#include <vector>
#include <tuple>
#include "Component_Pool.hpp"
#include "Types.h"
#include <algorithm>
#include "Pool_Holder.h"

namespace lightning {
    namespace ECS {
        template<typename ... Ts>
        struct Type_List{
            static constexpr auto size = sizeof...(Ts);
        };

        template<typename ... T>
        using Includes = Type_List<T...>;

        template<typename ... T>
        using Excludes = Type_List<T...>;

        template<typename Include, typename Exclude>
        class View;

        template<typename ... Include, typename ... Exclude>
        class View <Includes<Include...>, Excludes<Exclude...>> {
        public:
            struct Component_Job_Base{
                virtual void Update(Entity_t, Include& ...) = 0;
            };
        public:
            View(std::vector<Entity_t>& Entity_list, Pool_Holder& pool_holder, std::initializer_list<Tag_t> tags = {})
            : include_pools(pool_holder.Get_Pool<Include>()...),
              excludes_pools(pool_holder.Get_Pool<Exclude>()...){
                // add tag pools
                std::for_each(tags.begin(), tags.end(), [&](Tag_t tag){
                    tag_pools.push_back(&pool_holder.Get_Pool(tag));
                });

                // add entities with tags
                for (auto entity : Entity_list) {
                    if (check_entity(entity))
                        Entitys.push_back(entity);
                }

                // register listeners for includes
                std::apply([&](auto& ... pools){
                    (pools.RegisterAddCallback([&](Entity_t entity){
                        if (check_entity(entity))
                            Entitys.push_back(entity);
                    }), ...);
                    (pools.RegisterRemoveCallback([&](Entity_t entity){
                        Entitys.erase(std::remove(Entitys.begin(), Entitys.end(), entity), Entitys.end());
                    }), ...);
                }, include_pools);

                // register listeners for excludes
                std::apply([&](auto& ... pools){
                    (pools.RegisterAddCallback([&](Entity_t entity){
                        Entitys.erase(std::remove(Entitys.begin(), Entitys.end(), entity), Entitys.end());
                    }), ...);
                    (pools.RegisterRemoveCallback([&](Entity_t entity){
                        if (check_entity(entity))
                            Entitys.push_back(entity);
                    }), ...);
                }, excludes_pools);

                // register listeners for tags
                std::for_each(tag_pools.begin(), tag_pools.end(), [&](auto* pool){
                    pool->RegisterAddCallback([&](Entity_t entity){
                        if (check_entity(entity))
                            Entitys.push_back(entity);
                    });
                    pool->RegisterRemoveCallback([&](Entity_t entity){
                        Entitys.erase(std::remove(Entitys.begin(), Entitys.end(), entity), Entitys.end());
                    });
                });
            }

            // get component pool for type
            template<typename T>
            Component_Pool<T>& Get_Pool(){
                return std::get<Component_Pool<T>&>(include_pools);
            }

            // get component data for entity
            template<typename T>
            T& Get(Entity_t entity){
                return Get_Pool<T>().Get(entity);
            }

            template<typename T>
            const T Get_const(Entity_t entity) {
                return Get_Pool<T>().Get(entity);
            }

            // set component data for entity
            template<typename T>
            void Set(Entity_t entity, T data){
                Get_Pool<T>().Set(entity, data);
            }

            // check if view contains entity
            bool Contains(Entity_t entity) {
                return std::find(Entitys.begin(), Entitys.end(), entity) != Entitys.end();
            }

            // get number of entities in view
            inline size_t size() const {
                return Entitys.size();
            }

            // get begin iterator
            auto begin(){
                return Entitys.begin();
            }

            // get end iterator
            auto end(){
                return Entitys.end();
            }

            // run lambda on each entity
            template<typename Func>
            void ForEach(Func func){
                auto it = Entitys.begin();
                while (it != Entitys.end()){
                    func(*it, Get<Include>(*it)...);
                    it++;
                }
            }

            // run job on each entity
            void ForEach(Component_Job_Base& job){
                auto it = Entitys.begin();
                while (it != Entitys.end()){
                    job.Update(*it, Get<Include>(*it)...);
                    it++;
                }
            }

            // Get Entitys
            const std::vector<Entity_t>& Get_Entitys() const {
                return Entitys;
            }
        private:
            bool check_entity(Entity_t entity){
                bool In = std::apply([entity](auto& ... pools) { return (pools.Has(entity) && ...); }, include_pools);
                bool Ex = std::apply([entity](auto& ... pools) { return (pools.Has(entity) && ...); }, excludes_pools);
                bool T = std::all_of(tag_pools.begin(), tag_pools.end(), [entity](Tag_Pool* pool) { return pool->HasEntity(entity); });
                return In && ((!Ex)||(sizeof...(Exclude) == 0)) && T;
            }

        private:
            std::tuple<Component_Pool<Include>&...> include_pools;
            std::tuple<Component_Pool<Exclude>&...> excludes_pools;
            std::vector<Tag_Pool*> tag_pools;
            std::vector<Entity_t> Entitys;
        };
    } // ECS
} // lightning
#endif //CORE_VIEW_HPP
