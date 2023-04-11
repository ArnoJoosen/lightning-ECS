//
// Created by sterr on 11/04/2023.
//

#ifndef TESTECS_POOL_VIEW_H
#define TESTECS_POOL_VIEW_H
#include <unordered_map>
#include <memory>
#include "View.hpp"

namespace lightning::ECS {
    class Pool_view {
    public:

        ~Pool_view() {
            for (auto& view : views) {
                view.second->~Base_View();
                delete view.second;
            }
        }

        template<typename Include, typename Exclude>
        size_t GetID(std::initializer_list<Tag_t> tags = {}) {
            size_t id = typeid(View<Include, Exclude>).hash_code();
            for (auto& tag : tags)
                id = id ^ tag;
            return id;
        }

        Base_View* get_view(size_t id) {
            if (views.find(id) == views.end())
                return nullptr; // TODO change to assert
            return views[id];
        }

        bool dus_view_exist(size_t id) {
            return views.find(id) != views.end();
        }

        void add_view(size_t id, Base_View* view) {
            views[id] = view;
        }

    private:
        std::unordered_map<size_t, Base_View*> views;
    };
} // lightning::ECS

#endif //TESTECS_POOL_VIEW_H
