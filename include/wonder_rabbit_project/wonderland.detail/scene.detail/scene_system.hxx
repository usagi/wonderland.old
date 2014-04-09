#pragma once

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
        : public memory::enable_shared_from_this_wrap
            < scene_system_t<T_scene>
            , typename T_scene::object_t
            >
      {
      public:
        using type               = scene_system_t<T_scene>;
        using shared_t           = std::shared_ptr<type>;
        using scene_t            = T_scene;
        using object_t           = typename scene_t::object_t;
        using update_parameter_t = typename scene_t::object_t::update_parameter_t;
        
      private:
        using hook_t   = std::future<void>;
        using hooks_t  = std::queue<hook_t>;
        using scenes_t = std::stack<scene_t>;
        
        //hooks_t  _after_render_hooks;
        //scenes_t _scenes;
        /*
        auto after_render_hook() -> void
        {
          //while(!_after_render_hooks.empty())
          {
            //_after_render_hooks.front().get();
            //_after_render_hooks.pop();
          }
        }
        */
      protected:
        /*
        template<class T>
        auto push_after_render_hook(const T& f) -> void
        { _after_render_hooks.push(std::async(std::launch::deferred, f)); }
        */
        /*
        auto push(scene_t&& scene) -> void
        {
          push_after_render_hook
          ( [this]{ _scenes.emplace(std::move(scene)); }
          );
        }
        
        auto pop() -> void
        { push_after_render_hook( [this]{ _scenes.pop(); } ); }
        
        auto displace(scene_t&& scene) -> void
        {
          push_after_render_hook
          ( [this]
            {
              _scenes.pop();
              _scenes.emplace(std::move(scene));
            }
          );
        }
        
      public:
        auto update(const update_parameter_t& t) -> void override
        { _scenes.top().update(t); }
        
        auto render() -> void override
        {
          _scenes.top().render();
          after_render_hook();
        }
        */
      };
      
    }
  }
}