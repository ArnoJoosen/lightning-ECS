//
// Created by sterr on 6/04/2023.
//

#ifndef LIGHTNING_ENGINE_CORE_TAG_POOL_H
#define LIGHTNING_ENGINE_CORE_TAG_POOL_H
#include <bitset>
#include <functional>
#include <vector>
#include "Types.h"

namespace lightning::ECS {

    class Tag_Pool {
    public:
        Tag_Pool() = default;
        ~Tag_Pool() = default;

        void AddEntity(Entity_t entity);

        void RemoveEntity(Entity_t entity);

        bool HasEntity(Entity_t entity);

        void RegisterAddCallback(const std::function<void(Entity_t)>& callback);

        void RegisterRemoveCallback(const std::function<void(Entity_t)>& callback);

    private:
        std::bitset<MAX_ENTITIES> Entitys;
        std::vector<std::function<void(Entity_t)>> AddCallbacks;
        std::vector<std::function<void(Entity_t)>> RemoveCallbacks;
    };

} // namespace lightning::ECS

#endif // LIGHTNING_ENGINE_CORE_TAG_POOL_H
