//
// Created by sterr on 6/04/2023.
//

#include "Tag_Pool.h"

namespace lightning::ECS {

    void Tag_Pool::AddEntity(Entity_t entity)  {
        Entitys.set(entity);
        for (auto& callback : AddCallbacks)
            callback(entity);
    }

    void Tag_Pool::RemoveEntity(Entity_t entity)  {
        Entitys.reset(entity);
        for (auto& callback : RemoveCallbacks)
            callback(entity);
    }

    bool Tag_Pool::HasEntity(Entity_t entity)  {
        return Entitys.test(entity);
    }

    void Tag_Pool::RegisterAddCallback(const std::function<void(Entity_t)>& callback) {
        AddCallbacks.push_back(callback);
    }

    void Tag_Pool::RegisterRemoveCallback(const std::function<void(Entity_t)>& callback) {
        RemoveCallbacks.push_back(callback);
    }

} // namespace lightning::ECS