//
// Created by sterr on 8/04/2023.
//

#ifndef CORE_POOL_HOLDER_H
#define CORE_POOL_HOLDER_H
#include <unordered_map>
#include "Tag_Pool.h"
#include "Types.h"
#include "Component_Pool.hpp"
#include <string>

namespace lightning::ECS {
    class Pool_Holder {
    public:
        // tag pools
        /// get pool for tag
        Tag_Pool& Get_Pool(Tag_t tag);

        // component pools
        /// get pool for component type
        template<typename T>
        Component_Pool<T>& Get_Pool(){
            static Component_Pool<T> pool;
            return pool;
        }

    private:
        std::unordered_map<Tag_t, Tag_Pool> Tag_Pools;
    };
}
#endif //CORE_POOL_HOLDER_H
