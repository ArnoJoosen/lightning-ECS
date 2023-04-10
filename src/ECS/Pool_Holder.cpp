//
// Created by sterr on 8/04/2023.
//

#include "Pool_Holder.h"

namespace lightning::ECS {
    Tag_Pool &Pool_Holder::Get_Pool(Tag_t tag) {
        if (Tag_Pools.find(tag) == Tag_Pools.end()){
            Tag_Pools[tag] = Tag_Pool();
        }
        return Tag_Pools[tag];
    }
}