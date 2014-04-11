#pragma once

#include <chrono>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace object
    {

      template<class T_update_parameter = std::chrono::nanoseconds>
      class updatable_t
      {
        public:
          using update_parameter_t = T_update_parameter;
          auto virtual update( const update_parameter_t& delta_time ) -> void { }
          auto virtual update_priority() const
          -> unsigned
          {
            return 0;
          }
          auto virtual update_enabled() const
          -> bool
          {
            return true;
          }
      };

    }
  }
}
