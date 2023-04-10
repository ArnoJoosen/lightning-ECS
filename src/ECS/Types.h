//
// Created by sterr on 5/03/2023.
//

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <cstdint>

namespace lightning {
    namespace ECS {
        // 2^20 is the max amount of entities data can be created in de engine
#define MAX_ENTITIES 104856

        typedef uint32_t Entity_t; // 20 bits for id and 12 bits for version
        typedef uint32_t Entity_ID_t; // 20 bits for id
        typedef uint32_t Entity_Version_t; // 12 bits for version

        // macro to get the id from an entity
#define GET_ENTITY_ID(entity) (entity & 0xFFFFF)
        // macro to get the version from an entity
#define GET_ENTITY_VERSION(entity) (entity >> 20) & 0xFFF
        // macro to set the version of an entity
#define SET_ENTITY_VERSION(entity,version) id | (new_version << 20)
        // macro to increase the version of an entity by 1
#define INCREASE_VERSION(entity) entity += (1 << 20)

        // tag for entitys
        typedef uint32_t Tag_t;
    } // ECS
}

#endif //CORE_TYPES_H
