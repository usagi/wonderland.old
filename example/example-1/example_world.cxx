#include <iostream>

#include "example_world.hxx"
#include "example_scene_a.hxx"

namespace example
{
  example_world_t::example_world_t()
  {
    // set logger
    using wonder_rabbit_project::log::hook_tie;
    logger()->hook( hook_tie( std::cerr ) );
  }

  auto example_world_t::initialize() -> void
  {
    log( log_level::debug ) << "initializing was started";

    log( log_level::debug ) << "example_world this: " << this;

    // set step timing
    //step_timing(step_timing_e::non_adjusted);
    //step_timing(step_timing_e::adjusted);
    step_timing( step_timing_e::adjusted_with_skipping );
    // set target step time
    target_step_time( std::chrono::milliseconds( 1000 ) );
    // set time is fixed
    time_is_fixed( true );
    // set time magnification
    time_magnification( 3. );

    // set window
    //using wonder_rabbit_project::graphics::window;
    //window.confdigure()
    //  .size()
    //  .position()
    //  .xxx()
    //  ;

    // scene and scene_system
    //auto scene_system = std::make_shared<scene_system_t>(shared_from_this());
    //log(log_level::debug) << "scene system is generated.";

    auto scene_a = std::make_shared<example_scene_a_t>( shared_from_this() );
    log( log_level::debug ) << "scene-A is generated      : " << scene_a << "(" << shared_from_this() << ")";
    log( log_level::debug ) << "scene-A shared_from_this  : " << scene_a->shared_from_this();
    log( log_level::debug ) << "scene-A shared_from_master: " << scene_a->shared_from_master();

    //scene_system ->
    push( scene_a );
    log( log_level::debug ) << "scene-A pushed to scene system.";

    //generate_and_push_scene_system();
    //auto ss = generate_and_push_scene_system();
    //auto ss = push_scene_system();

    // push the first scene
    //;

    log( log_level::debug ) << "initializing will done.";

    base_t::initialize();
  }

  auto example_world_t::update( const update_parameter_t& dt )
  -> void
  {
    log( log_level::debug ) << "example_world update: " << dt.count();
  }

  auto example_world_t::render()
  -> void
  {
    log( log_level::debug ) << "example_world render";
  }

}
