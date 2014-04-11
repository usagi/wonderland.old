#pragma once

#include <iostream>
#include <queue>
#include <future>
#include <stack>

#include "../object.hxx"
#include "scene.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace scene
    {

      template<class T_scene = scene_t<>>
      class scene_system_t
        : public T_scene::object_t
      {
        public:
          using type               = scene_system_t<T_scene>;
          using shared_t           = std::shared_ptr<type>;
          using scene_t            = T_scene;
          using shared_scene_t     = std::shared_ptr<scene_t>;
          using weak_scene_t       = std::weak_ptr<scene_t>;
          using object_t           = typename scene_t::object_t;
          using update_parameter_t = typename scene_t::object_t::update_parameter_t;

        private:
          using hook_t   = std::future<void>;
          using hooks_t  = std::queue<hook_t>;
          using scenes_t = std::stack<shared_scene_t>;
          using mutex_t            = std::mutex;

          hooks_t  _after_render_hooks;
          scenes_t _scenes;
          mutex_t  _scenes_mutex;

          auto after_render_hook() -> void
          {
            while ( !_after_render_hooks.empty() )
            {
              _after_render_hooks.front().get();
              _after_render_hooks.pop();
            }
          }

        protected:

          template<class T>
          auto push_after_render_hook( const T& f ) -> void
          { _after_render_hooks.push( std::async( std::launch::deferred, f ) ); }


        public:

          scene_system_t()
            : object_t( )
          { }

          scene_system_t( typename object_t::weak_t && master_ )
            : object_t( std::move( master_ ) )
          { }

          auto update( const update_parameter_t& t ) -> void override
          {
            std::cerr << "scene_system update!\n";
            _scenes.top()->update( t );
          }

          auto render() -> void override
          {
            std::cerr << "scene_system render!\n";
            _scenes.top()->render();
            after_render_hook();
          }

          auto push( const shared_scene_t& scene ) -> void
          {
            std::cerr << "ss->push: " << scene << "(" << scene->shared_from_master() << ")" << "\n";
            if ( _scenes_mutex.try_lock() )
            {
              _scenes.emplace( /*std::move(*/scene/*)*/ );
              _scenes_mutex.unlock();
            }
            else
              push_after_render_hook
              ( [this, scene]{ _scenes.emplace( /*std::move(*/scene/*)*/ ); }
            );
          }

          auto pop() -> void
          { push_after_render_hook( [this]{ _scenes.pop(); } ); }

          auto displace( shared_scene_t scene ) -> void
          {
            push_after_render_hook
            ( [this, scene]
            {
              _scenes.pop();
              _scenes.emplace( std::move( scene ) );
            }
            );
          }

      };

    }
  }
}
