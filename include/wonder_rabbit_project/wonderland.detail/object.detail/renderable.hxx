#pragma once

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace object
    {

      class renderable_t
      {
        public:
          auto virtual render() -> void { }
          auto virtual render_priority() const
          -> unsigned
          {
            return 0;
          }
          auto virtual render_enabled() const
          -> bool
          {
            return true;
          }
      };

    }
  }
}
