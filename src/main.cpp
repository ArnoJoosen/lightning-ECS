#include <iostream>
#include "ECS/registery.h"

using namespace lightning;

struct Position {
    float x, y;
};

struct Velocity {
    float x, y;
};

struct Mass {
    float mass;
};

struct Health {
    float health;
};

struct Name {
    std::string name;
};

int main() {
    ECS::registry registry;

    const ECS::Tag_t tag1 = 564154586343574;

    auto entity1 = registry.CreateEntity();
    Name name1 = {"Bob"};
    registry.Add<Name>(entity1, name1);
    registry.Add<Position>(entity1);
    registry.Add<Velocity>(entity1);
    registry.Add<Mass>(entity1);
    registry.AddTag(entity1, tag1);

    auto entity2 = registry.CreateEntity();
    Name name2 = {"Alice"};
    registry.Add<Name>(entity2, name2);
    registry.Add<Position>(entity2);
    registry.Add<Velocity>(entity2);
    registry.Add<Health>(entity2);
    registry.AddTag(entity2, tag1);

    auto entity3 = registry.CreateEntity();
    Name name3 = {"John"};
    registry.Add<Name>(entity3, name3);
    registry.Add<Position>(entity3);

    auto poolHolder = registry.Get_PoolsHolder();
    auto view = registry.view<Position, Name>({tag1}, ECS::Excludes<Health>());

    std::cout << "Before adding Health to John: \n";
    for (ECS::Entity_t entity : view){
        auto& name = registry.Get<Name>(entity);
        std::cout << "  " << name.name << std::endl;
    }

    registry.AddTag(entity3, tag1);

    std::cout << "After adding Health to John: \n";
    for (ECS::Entity_t entity : view){
        auto& name = registry.Get<Name>(entity);
        std::cout << "  " << name.name << std::endl;
    }

    registry.Add<Health>(entity3);

    std::cout << "After adding Health to John: \n";
    for (ECS::Entity_t entity : view){
        auto& name = registry.Get<Name>(entity);
        std::cout << "  " << name.name << std::endl;
    }

    registry.Remove<Health>(entity2);

    std::cout << "After removing Health from Alice: \n";
    for (ECS::Entity_t entity : view){
        auto& name = registry.Get<Name>(entity);
        std::cout << "  " << name.name << std::endl;
    }

    return 0;
}
