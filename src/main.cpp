#include <iostream>
#include <vector>

#include "ECS/ECS.h"

int main()
{
    struct C1
    {
        C1(float _data = 1.0f) : data(_data) {}
        float data;
    };
   
    ECS ecs;
    uint32_t e1 = ecs.new_entity();
 
    ecs.add_component<C1>(e1, new C1(1));

    ecs.remove_entity(e1);

    e1 = ecs.new_entity();

    ecs.add_component<C1>(e1, new C1(1));
 
    auto components = ecs.get_all_components<C1>();
    for (auto c : components)
    {
        if (c == nullptr)
            std::cout << "null" << std::endl;
        else
            std::cout << c->data << std::endl;
 
    }

    return 0;
}