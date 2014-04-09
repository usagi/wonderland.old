#pragma once

#include <list>

#include "../object.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace scene
    {
      
      template<class T_object = object::object_t<>>
      class scene_t
        : memory::enable_shared_from_this_wrap
            < scene_t<T_object>
            , T_object
            >
      {
      public:
        using type               = scene_t<T_object>;
        using shared_t           = std::shared_ptr<type>;
        
        using object_t           = T_object;
        using update_parameter_t = typename object_t::update_parameter_t;
        using objects_t          = std::list<typename object_t::shared_t>;
        
        /*
        auto update(const update_parameter_t& t) -> void override
        { }
        
        auto render() -> void override
        { }
        */
      };
      
    }
  }
}