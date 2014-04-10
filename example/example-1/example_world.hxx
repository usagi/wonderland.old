#pragma once

#include <wonder_rabbit_project/wonderland.hxx>

namespace example
{
  using namespace wonder_rabbit_project::wonderland;
  
  class example_world_t
    : public wonderland_t<>
  {
  public:
    using base_t = wonderland_t<>;
  
  private:
    scene_system_ptr_t scene_system;
    
  protected:
    auto initialize() -> void override;
    
  public:
    example_world_t();
    
    auto update(const update_parameter_t& dt) -> void override;
    auto render() -> void override;
    
  };
}