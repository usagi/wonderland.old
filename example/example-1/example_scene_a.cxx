#include "example_scene_a.hxx"

namespace example
{
  using ::wonder_rabbit_project::wonderland::wonderland_t;
  using ::wonder_rabbit_project::log::level_e;
  
  auto example_scene_a_t::update(const update_parameter_t& t)
    -> void
  {
    if(auto w = shared_from_master_until<example_world_t>())
      w -> logger()(level_e::debug)
        << "example_scene_a::update! " << t.count();
    else
      throw std::runtime_error("（・３・）アルェー");
  }
  
  auto example_scene_a_t::render()
    -> void
  {
    if(auto w = shared_from_master_until<example_world_t>())
      w ->logger()(level_e::debug)
        << "example_scene_a::render! ";
  }
  
}