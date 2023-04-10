//
// Created by sterr on 5/03/2023.
//

#ifndef CORE_ENTITY_MAP_HPP
#define CORE_ENTITY_MAP_HPP
#include <vector>
#include <bitset>
#include <stdexcept>

#include "Types.h"
namespace lightning {
    namespace ECS {
        /*
         * Entity_map is a map of entitys to data of type T
         * Stores data in a vector and a spare vector with the index and version of the entity
         */
        template<typename T>
        class Entity_map {
        public:
            typedef std::pair<uint32_t, Entity_Version_t> spare_pair;
            typedef std::vector<spare_pair> spare_vector;
            typedef typename std::vector<T> data_vector;
            typedef typename std::vector<T>::iterator data_iterator;
        public:
            Entity_map(){
                data.reserve(100);
                spare.reserve(100);
            }
            // add entity to map with data
            void add(Entity_t entity, T& Component) {
                Entity_ID_t id = GET_ENTITY_ID(entity);
                if (in_map.test(id)) {
                    return;
                }
                in_map.set(id);
                if (id >= spare.size()) {
                    spare.resize(id + 1);
                }
                spare[id] = std::make_pair(data.size(), GET_ENTITY_VERSION(entity));
                data.push_back(Component);
            }
            // remove entity from map
            void remove(Entity_t entity) {
                Entity_ID_t id = GET_ENTITY_ID(entity);
                if (!in_map.test(id)){
                    return;
                }
                spare_pair& pare = spare[id];
                //if (GET_ENTITY_VERSION(entity) != pare.second)
                //    throw std::runtime_error("Entity to old"); // TODO: change to logger
                in_map.reset(id);
                data.erase(data.begin() + pare.first);
                spare.erase(spare.begin() + id);
            }
            // check if entity is in map
            bool contains(Entity_t entity) {
                Entity_ID_t id = GET_ENTITY_ID(entity);
                if (id >= spare.size())
                    return false;
                if (GET_ENTITY_VERSION(entity) != spare[id].second)
                    return false;
                if (!in_map.test(id))
                    return false;
                return true;
            }
            // get data from entity
            T& get(Entity_t entity) {
                Entity_ID_t id = GET_ENTITY_ID(entity);
                spare_pair& pare = spare[id];
                //TODO LIGHTNING_ASSERT(GET_ENTITY_VERSION(entity) == pare.second, "Entity to old");
                return data[pare.first];
            }
            T& operator [](Entity_t entity) {
                return get(entity);
            }
            // set data from entity
            void set(Entity_t entity, T Component) {
                Entity_ID_t id = GET_ENTITY_ID(entity);
                if (!in_map.test(id)) {
                    return;
                }
                spare_pair& pare = spare[id];
                if (GET_ENTITY_VERSION(entity) != pare.second) {
                    return;
                }
                data[pare.first] = Component;
            }
            // clear map
            void clear() {
                in_map.reset();
                data.clear();
                spare.clear();
            }
            // get size of map
            size_t size() {
                return data.size();
            }
            // get data from index
            T& get_index_data(size_t index) {
                return data[index];
            }
            // get ind from entity
            size_t get_index(Entity_t entity) {
                Entity_ID_t id = GET_ENTITY_ID(entity);
                spare_pair& pare = spare[id];
                return pare.first;
            }
            // check if map is empty
            bool empty() {
                return data.empty();
            }
            // get begin iterator
            data_iterator begin() {
                return data.begin();
            }
            // get end iterator
            data_iterator end() {
                return data.end();
            }

        private:
            data_vector data; // vector of data from entitys
            spare_vector spare; // pare of index and entity version
            std::bitset<MAX_ENTITIES> in_map; // bitset of entitys in map
        };
    } // ECS
} // lightning
#endif //CORE_ENTITY_MAP_HPP
