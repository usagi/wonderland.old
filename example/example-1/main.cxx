#include <stdexcept>
#include <iostream>
#include <sstream>

#include "example_world.hxx"

namespace
{
  using example::example_world_t;
  std::shared_ptr<example_world_t> example_world;
}

auto main()
-> int
try
{
  example_world = std::make_shared<example_world_t>();
  example_world->invoke();
}
catch
  ( const std::exception& e )
{
  constexpr auto r = 2;
  std::ostringstream ss;
  ss << "unrecoverable exception(abort return " << r << "): " << e.what();
  example_world->logger()( wonder_rabbit_project::log::level_e::fatal ) << ss.str();
  std::cerr << ss.str() << "\n";
  return r;
}
catch
  ( ... )
{
  constexpr auto r = 1;
  std::ostringstream ss;
  ss << "unknown unrecoverable exception(abort return " << r << ")";
  example_world->logger()( wonder_rabbit_project::log::level_e::fatal ) << ss.str();
  std::cerr << ss.str() << "\n";
  return r;
}
