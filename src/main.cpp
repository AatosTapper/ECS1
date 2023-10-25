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
    uint32_t e2 = ecs.new_entity();
    uint32_t e3 = ecs.new_entity();
 
    C1* comp1 = new C1(1);
    C1* comp2 = new C1(2);
    C1* comp3 = new C1(3);
 
    ecs.add_component<C1>(e1, comp1);
    ecs.add_component<C1>(e2, comp2);
    ecs.add_component<C1>(e3, comp3);
 
 
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