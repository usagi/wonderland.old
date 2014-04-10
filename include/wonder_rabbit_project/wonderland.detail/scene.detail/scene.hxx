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
        : public T_object
      {
      public:
        
        using object_t           = T_object;
        using update_parameter_t = typename object_t::update_parameter_t;
        using objects_t          = std::list<typename object_t::shared_t>;
        
        scene_t()
          : object_t()
        { }
        
        scene_t(typename object_t::weak_t&& master_)
          : object_t( std::move( master_ ) )
        { }
        
        auto update(const update_parameter_t& t) -> void override
        { }
        
        auto render() -> void override
        { }
        
      };
      
    }
  }
}