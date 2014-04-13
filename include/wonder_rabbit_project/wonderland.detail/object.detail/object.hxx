#pragma once

#include <cstddef>
#include <memory>
#include <chrono>
#include <forward_list>

#include <wonder_rabbit_project/message.hxx>

#include "updatable.hxx"
#include "renderable.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    template <class, class, class> class wonderland_t;
    
    namespace scene
    {
      template <class> class scene_t;
      template <class> class scene_system_t;
    }

    namespace object
    {

      template<class T_update_parameter = std::chrono::nanoseconds>
      class object_t
        : public std::enable_shared_from_this<object_t<T_update_parameter>>
            , public message::messagable_t
            , public updatable_t<T_update_parameter>
            , public renderable_t
      {
          template <class> friend class scene::scene_t;
          template <class> friend class scene::scene_system_t;
          template <class, class, class> friend class wonderland_t;

        public:
          using type               = object_t<T_update_parameter>;

          using update_parameter_t = T_update_parameter;

          using shared_t           = std::shared_ptr<type>;
          using weak_t             = std::weak_ptr<type>;

        private:
          weak_t             _master;
          update_parameter_t _time;
          
        protected:
          virtual auto _update(const update_parameter_t& dt)
          -> void
          {
            _time += dt;
            this->update(dt);
          }
          
          virtual auto _render()
          -> void
          {
            this->render();
          }
          
        public:
          object_t()
          { }

          object_t( weak_t && master_ )
            : _master( std::move( master_ ) )
          { }

          template<class T = type>
          auto shared_from_master()
          -> std::shared_ptr<T>
          {
            auto p = _master.expired()
            ? std::template enable_shared_from_this<object_t<T_update_parameter>>::shared_from_this()
              : _master.lock()
              ;

            return std::dynamic_pointer_cast<T>( p );
          }

          template<class T = type>
          auto shared_from_master_until()
          -> std::shared_ptr<T>
          {
            shared_t p1;
            shared_t p2( std::template enable_shared_from_this<object_t<T_update_parameter>>::shared_from_this() );

            do
            {
              p1 = p2;
              p2 = p2->shared_from_master();
              auto dynamic_p2 = std::dynamic_pointer_cast<T>( p2 );
              if ( dynamic_p2 )
                return dynamic_p2;
            }
            while ( p1 != p2 );

            return std::shared_ptr<T>();
          }
          
          auto time()
          -> update_parameter_t
          { return _time; }
          
      };

    }
  }
}
