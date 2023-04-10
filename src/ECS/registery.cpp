//
// Created by sterr on 5/03/2023.
//

#include <cassert>
#include "registery.h"

namespace lightning {
    namespace ECS {
        Entity_t registry::CreateEntity() {
            assert(NextEntity < MAX_ENTITIES); // TODO change to custom assert
            if (FreeEntitys.empty()){
                NextEntity++;
                Entity_t new_entity = NextEntity;
                FreeEntitysSet.insert(new_entity);
                Entitys.push_back(new_entity);
                return new_entity;
            } else {
                Entity_t new_entity = FreeEntitys.back();
                FreeEntitys.pop_back();
                INCREASE_VERSION(new_entity);
                FreeEntitysSet.insert(new_entity);
                Entitys.push_back(new_entity);
                return new_entity;
            }
        }

        void registry::AddTag(Entity_t entity, Tag_t tag) {
            pools.Get_Pool(tag).AddEntity(entity);
        }

        void registry::RemoveTag(Entity_t entity, Tag_t tag) {
            pools.Get_Pool(tag).RemoveEntity(entity);
        }

        bool registry::HasTag(Entity_t entity, Tag_t tag) {
            return pools.Get_Pool(tag).HasEntity(entity);
        }

        void registry::DestroyEntity(Entity_t entity) {
            // TODO add global list of component pools for destruction entity
        }

        bool registry::IsActive(Entity_t entity) {
            return FreeEntitysSet.contains(entity);
        }
    } // lightning
} // ECS