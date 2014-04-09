#pragma once

#include <cstddef>
#include <memory>

#include "updatable.hxx"
#include "renderable.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace object
    {
      
      template<class T_update_parameter = std::chrono::nanoseconds>
      class object_t
        : public memory::enable_shared_from_this_wrap<object_t<T_update_parameter>>
        , public message::messagable_t
        , public updatable_t<T_update_parameter>
        , public renderable_t
      {
      public:
        using type               = object_t<T_update_parameter>;
        using base_t             = memory::enable_shared_from_this_wrap<object_t<T_update_parameter>>;
        
        using update_parameter_t = T_update_parameter;
        
        using shared_t           = std::shared_ptr<type>;
        using weak_t             = std::weak_ptr<type>;
        
      private:
        weak_t _master;
        
      protected:
        auto master(const shared_t& master_)
          -> void
        { _master = master_; }
        
      public:
        object_t(){ }
        object_t(const weak_t&  master_): _master(          master_ ) { }
        object_t(      weak_t&& master_): _master(std::move(master_)) { }
        
        template<class T, class ... T_params>
        static auto make_shared(T_params ... params)
          -> std::shared_ptr<T>
        {
          auto p = std::make_shared<T>(params ...);
          p->master(p->shared_from_this());
          return p;
        }
        
      };
      
      template<class T, class ... T_params>
      auto make_shared(T_params ... params)
        -> std::shared_ptr<T>
      { return T:: template make_shared<T>(params ...); }
      
    }
  }
}