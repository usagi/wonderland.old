// std
#include <stdexcept>
#include <iostream>

// wonderland
#include <wonder_rabbit_project/wonderland.hxx>

namespace
{
  // wonderland 
  using namespace wonder_rabbit_project::wonderland;
  using wonder_rabbit_project::memory::enable_shared_from_this_wrap;
  using wonder_rabbit_project::wonderland::object::make_shared;
  
  // Classes
  class example_world_t;
  class example_scene_a_t;
  class example_scene_b_t;
  class example_scene_c_t;
  
  // Scene-A: example_scene_a_t
  //class example_scene_a_t
  //  : public scene_t
  //{
  //public:
  //  explicit scene_a_t(const object_t& o):scene_t(o){ }
  //};
  
  // Example world: example_world_t
  class example_world_t
    : public enable_shared_from_this_wrap
        < example_world_t
        , wonderland_t<>
        >
  {
  public:
    using base_t = wonderland_t<>;
  
  private:
    scene_system_ptr_t scene_system;
    
  protected:
    auto initialize() -> void override
    {
      log(log_level::debug) << "initializing was started";
      
      // set step timing
      //step_timing(step_timing_e::non_adjusted);
      //step_timing(step_timing_e::adjusted);
      step_timing(step_timing_e::adjusted_with_skipping);
      // set target step time
      target_step_time(std::chrono::milliseconds(200));
      // set time is fixed
      time_is_fixed(true);
      // set time magnification
      time_magnification(3.);
      
      // set window
      //using wonder_rabbit_project::graphics::window;
      //window.confdigure()
      //  .size()
      //  .position()
      //  .xxx()
      //  ;
      
      // generate and push scene system
      //auto x = std::make_shared<scene_t>();
      //scene_system = std::make_shared<scene_system_t>(shared_from_this());

      //generate_and_push_scene_system();
      //auto ss = generate_and_push_scene_system();
      //auto ss = push_scene_system();
      
      // push the first scene
      //;
      
      log(log_level::debug) << "initializing will done.";
      
      base_t::initialize();
    }
    
  public:
    example_world_t()
    {
      // set logger
      using wonder_rabbit_project::log::hook_tie;
      log.hook( hook_tie(std::cerr) );
    }
    
    auto update(const update_parameter_t& dt)
      -> void override
    {
      log(log_level::debug) << "on update! " << dt.count();
    }
    
    auto render()
      -> void override
    {
      log(log_level::debug) << "on render!";
    }
    
  };
  
  //auto example_world = std::make_shared<example_world_t>();
  auto example_world = make_shared<example_world_t>();
}

// entry point
auto main()
  -> int
try
{
  example_world->invoke();
}
catch (const std::exception& e)
{
  example_world->logger()(wonder_rabbit_project::log::level_e::fatal) << "unrecoverable exception: " << e.what();
  return 2;
}
catch ( ... )
{
  example_world->logger()(wonder_rabbit_project::log::level_e::fatal) << "unknown unrecoverable exception";
  return 1;
}
