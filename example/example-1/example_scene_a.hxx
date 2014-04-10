#pragma once

#include "example_world.hxx"

namespace example
{
  class example_scene_a_t
    : public example_world_t::scene_t 
  {
  public:
    example_scene_a_t(){ }
    example_scene_a_t(weak_t&& master_): example_world_t::scene_t(std::move(master_)) { }
    
    auto update(const update_parameter_t& t) -> void override;
    auto render() -> void override;
    
  };
  
}