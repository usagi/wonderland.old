#include "example_scene_a.hxx"

namespace example
{
  using ::wonder_rabbit_project::wonderland::wonderland_t;

  auto example_scene_a_t::update( const update_parameter_t& t )
  -> void
  {
    LOGD << "t.count(): " << t.count();
    
    LOGD << "time().count(): " << time().count();
    
    if ( time() > std::chrono::seconds(3) )
      shared_from_master_until<example_world_t::scene_system_t>()
        -> pop();
  }

  auto example_scene_a_t::render()
  -> void
  {
    LOGD << "render! ";
  }

}
