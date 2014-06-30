#pragma once

#include <chrono>
#include <functional>
#include <queue>
#include <memory>

#ifdef EMSCRIPTEN
#include <mutex>
#else
#include <future>
#endif

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

#include <wonder_rabbit_project/wonderland/log.hxx>
#include <wonder_rabbit_project/time.hxx>
#include <wonder_rabbit_project/message.hxx>
#include <wonder_rabbit_project/wonderland.subsystem.hxx>

#include "wonderland.detail/object.hxx"
#include "wonderland.detail/scene.hxx"

namespace wonder_rabbit_project
{
  namespace log
  {

    class log_stream_t;
  }

  namespace wonderland
  {

#ifdef EMSCRIPTEN
    template<class T = void>
    class wonderland_main_loop_callback_templated_wrapper
    {
      public:
        static std::mutex m;
        static auto lock() -> void
        {
          if ( ! m.try_lock() )
            throw std::logic_error( "not support yet multiple instancing on emscripten version" );
        }
        static auto unlock() -> void
        { m.unlock(); }
    };

    template<class T_wonderland>
    auto wonderland_main_loop_callback( void* wonderland ) -> void
    {
      auto w = reinterpret_cast<T_wonderland*>( wonderland );

      try
      { w->_before_step_time = time::time( w->_runner ); }
      catch(scene::end_of_scene_system)
      { w->_state = T_wonderland::state_e::ending; }
      
      if ( w->_state != T_wonderland::state_e::running )
      {
        emscripten_cancel_main_loop();
        w->log( T_wonderland::log_level::debug ) << "to ending";
        w->_state = T_wonderland::state_e::ending;
      }
    }
#endif

    template
    < class T_duration    = std::chrono::nanoseconds
    , class T_object      = object::object_t<T_duration>
    , class T_base_object = scene::scene_system_t<scene::scene_t<T_object>>
    , class T_subsystem   = subsystem::subsystem_t
    >
    class wonderland_t
      : public T_base_object
    {

#ifdef EMSCRIPTEN
        template<class T>
        friend auto wonderland_main_loop_callback( void* ) -> void;
#endif

      public:

        using type               = wonderland_t<T_duration, T_object, T_base_object>;
        using shared_t           = std::shared_ptr<type>;
        using base_object_t      = T_base_object;
        using duration_t         = T_duration;
        using object_t           = T_object;
        using subsystem_t        = T_subsystem;
        using object_ptr_t       = typename object_t::shared_t;
        using scene_t            = scene::scene_t<object_t>;
        using scene_ptr_t        = typename scene_t::shared_t;
        using scene_system_t     = scene::scene_system_t<scene_t>;
        using scene_system_ptr_t = typename scene_system_t::shared_t;
        using update_parameter_t = typename object_t::update_parameter_t;

        enum class state_e
        {
          initializing
          , running
          , suspending
          , ending
        };

        enum class step_timing_e
        {
          non_adjusted
          , adjusted
          , adjusted_with_skipping
        };

      private:
        using runner_t     = std::function<void()>;
#ifdef EMSCRIPTEN
        using step_hook_t  = std::function<void()>;
#else
        using step_hook_t  = std::future<void>;
#endif
        using step_hooks_t = std::queue<step_hook_t>;
        using delta_time_t = std::function<duration_t()>;

        state_e       _state;
        duration_t    _before_step_time;

        step_timing_e _step_timing;
        duration_t    _target_step_time;

        runner_t      _runner;
        step_hooks_t  _after_step_hooks;
        delta_time_t  _delta_time;

        bool          _time_is_fixed;
        double        _time_magnification;

        std::shared_ptr<log::log_t> _logger;
        
        std::shared_ptr<subsystem_t> _subsystem;
        
        auto generate_runner() -> runner_t
        {
          switch ( _step_timing )
          {
            case step_timing_e::non_adjusted:
              return [this]
              {
                _subsystem -> before_update();
                this->_update( this->delta_time() );
                _subsystem -> after_update();
                
                _subsystem -> before_render();
                this->_render();
                this->after_step_hook();
                _subsystem -> after_render();
              };
            case step_timing_e::adjusted:
              return [this]
              {
                time::adjust
                ( [this]
                {
                  _subsystem -> before_update();
                  this->_update( this->delta_time() );
                  _subsystem -> after_update();
                  
                  _subsystem -> before_render();
                  this->_render();
                  this->after_step_hook();
                  _subsystem -> after_render();
                }
                , _target_step_time
                );
              };
            case step_timing_e::adjusted_with_skipping:
              std::shared_ptr<duration_t> dt_bank( new duration_t( 0 ) );
              return [this, dt_bank]
              {
                if ( dt_bank->count() < 0 )
                {
                  *dt_bank += _target_step_time;
                  LOGW << "skipped a step.";
                }
                else
                {
                  auto dt = time::adjust
                  ( [this]
                  {
                    _subsystem -> before_update();
                    this->_update( this->delta_time() );
                    _subsystem -> after_update();
                    
                    _subsystem -> before_render();
                    this->_render();
                    this->after_step_hook();
                    _subsystem -> after_render();
                  }
                  , _target_step_time
                  );

                  if ( dt.count() < 0 )
                    *dt_bank += dt;
                }
              };
          }

          throw std::logic_error( "unknown step_timing_e value." );
        }

        auto after_step_hook() -> void
        {
          while ( !_after_step_hooks.empty() )
          {
            LOGD << _after_step_hooks.size();
#ifdef EMSCRIPTEN
            _after_step_hooks.front()();
#else
            _after_step_hooks.front().get();
#endif
            _after_step_hooks.pop();
          }
        }

      protected:

        template<class T>
        auto push_after_step_hook( T && f ) -> void
        {
#ifdef EMSCRIPTEN
          _after_step_hooks.push( std::move( f ) );
#else
          _after_step_hooks.push( std::async( std::launch::deferred, std::move( f ) ) );
#endif
        }

        auto delta_time() -> duration_t
        {
          return std::chrono::duration_cast
          <duration_t>
          ( std::chrono::duration
          < decltype( _time_magnification )
          , typename duration_t::period
          >
          ( _time_magnification * _delta_time().count() )
          );
        }

        auto step_timing( const step_timing_e step_timing_ )
        -> void
        {
#ifdef EMSCRIPTEN
          if ( step_timing_ == step_timing_e::adjusted_with_skipping )
            log( log_level::warn ) << "step_timing::adjusted_with_skipping is not support in emscripten version.";
#endif
          if ( _state != state_e::initializing )
            push_after_step_hook
            ( [this, &step_timing_]
          {
            this->_step_timing = step_timing_;
            this->_runner      = this->generate_runner();
          }
          );
          else
            _step_timing = step_timing_;
        }

        auto target_step_time( const duration_t& time )
        -> void
        {
          if ( _state != state_e::initializing )
            push_after_step_hook
            ( [this, &time]
          { this->_target_step_time = time; }
          );
          else
            _target_step_time = time;
        }

        auto time_is_fixed( const bool value ) -> void
        {
          if ( _state != state_e::initializing )
            push_after_step_hook
            ( [this, value]
          { this->_time_is_fixed = value; }
          );
          else
            _time_is_fixed = value;
        }

        auto time_magnification( const double value ) -> void
        {
          if ( _state != state_e::initializing )
            push_after_step_hook
            ( [this, value]
          { this->_time_magnification = value; }
          );
          else
            _time_magnification = value;
        }

        auto virtual initialize
        ( typename subsystem_t::initialize_params_t&& subsystem_initialize_params )
          -> void
        {
          _state = state_e::initializing;

          _runner = generate_runner();

          if ( _time_is_fixed )
            _delta_time = [this]{ return this->_target_step_time; };
          else
            _delta_time = [this]{ return this->_before_step_time; };
          
          _subsystem -> initialize( std::move(subsystem_initialize_params) );
        }
        
        auto virtual initialize() -> void
        { initialize( _subsystem -> default_initialize_params() ); }

        auto virtual run() -> void
        {
          LOGD << "to runing";
          _state = state_e::running;

#ifdef EMSCRIPTEN
          const int fps = _step_timing == step_timing_e::non_adjusted
          ? 0
          : duration_t::period::den / duration_t::period::num / _target_step_time.count()
          ;
          constexpr int is_infinite_simulate = 1;
          emscripten_set_main_loop_arg
          ( wonderland_main_loop_callback<type>
          , static_cast<void*>( this )
          , fps
          , is_infinite_simulate
          );
#else
          do
            try
            { _before_step_time = time::time( _runner ); }
            catch(scene::end_of_scene_system)
            { _state = state_e::ending; }
          while ( _state == state_e::running );

          LOGD << "to ending";
          _state = state_e::ending;
#endif
        }

      public:

        wonderland_t()
          : base_object_t()
          , _state( state_e::initializing )
          , _before_step_time( 0 )
          , _target_step_time( 30 )
          , _time_is_fixed( false )
          , _time_magnification( 1. )
          , _subsystem(std::make_shared<subsystem_t>())
        {
#ifdef EMSCRIPTEN
          wonderland_main_loop_callback_templated_wrapper<>::lock();
#endif
        }

        wonderland_t( typename object_t::weak_t && master_ )
          : base_object_t( std::move( master_ ) )
          , _state( state_e::initializing )
          , _before_step_time( 0 )
          , _target_step_time( 30 )
          , _time_is_fixed( false )
          , _time_magnification( 1. )
          , _subsystem(std::make_shared<subsystem_t>())
        { }

        virtual ~wonderland_t()
        {
          if ( _state != state_e::ending )
          {
            auto to_string = []( const state_e & a )
            {
              switch ( a )
              {
                case state_e::initializing:
                  return "initializing";
                case state_e::running     :
                  return "running";
                case state_e::suspending  :
                  return "suspending";
                case state_e::ending      :
                  return "ending";
                default
                    :
                  return "**unknown**";
              }
            };

            LOGF << "wonderland_t(" << this << ") to destruct emmergency."
                    " current state is [" << to_string( _state ) << "]."
                 ;
          }
#ifdef EMSCRIPTEN
          wonderland_main_loop_callback_templated_wrapper<>::unlock();
#endif
        }

        auto virtual invoke()
        -> void
        {
          initialize();
          run();
        }
        
        auto virtual subsystem()
          -> std::shared_ptr<subsystem::subsystem_base_t>
        { return _subsystem -> shared_from_this(); }
        
    protected:
        auto _update( const update_parameter_t& t )
        -> void override
        {
          LOGD << "wonderland update: " << t.count();
          base_object_t::_update( t );
        }

        auto _render()
        -> void override
        {
          LOGD << "wonderland render";
          base_object_t::_render();
        }

    };

  }
}
