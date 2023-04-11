//
// Created by sterr on 11/04/2023.
//

#include "Pool_view.h"

namespace lightning::ECS {

    Pool_view::~Pool_view() {
        for (auto& view : views) {
            view.second->~Base_View();
            delete view.second;
        }
    }

    Base_View *Pool_view::get_view(size_t id) {
        if (views.find(id) == views.end())
            return nullptr; // TODO change to assert
        return views[id];
    }

    bool Pool_view::dus_view_exist(size_t id) {
        return views.find(id) != views.end();
    }

    void Pool_view::add_view(size_t id, Base_View *view) {
        views[id] = view;
    }

} // lightning::ECS