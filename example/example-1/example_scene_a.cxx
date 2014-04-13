#include "example_scene_a.hxx"

namespace example
{
  using ::wonder_rabbit_project::wonderland::wonderland_t;
  using ::wonder_rabbit_project::log::level_e;

  auto example_scene_a_t::update( const update_parameter_t& t )
  -> void
  {
    shared_from_master_until<example_world_t>()
      -> log( level_e::debug )
          << "example_scene_a::update! " << t.count();
    
    shared_from_master_until<example_world_t>()
      -> log( level_e::debug )
        << time().count();
    
    if ( time() > std::chrono::seconds(3) )
      shared_from_master_until<example_world_t::scene_system_t>()
        -> pop();
  }

  auto example_scene_a_t::render()
  -> void
  {
    shared_from_master_until<example_world_t>()
      ->log( level_e::debug )
          << "example_scene_a::render! ";
  }

}
