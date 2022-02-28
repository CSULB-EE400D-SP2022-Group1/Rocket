#ifndef ROCKET_HPP_INCLUDED
#define ROCKET_HPP_INCLUDED

#include <tinyfsm.hpp>


// ----------------------------------------------------------------------------
// Event declarations
//

struct  Sensors       : tinyfsm::Event
{
  bool irq_IMU;
  bool irq_BME;
  bool irq_GPS;
};

struct green          : Sensors { };
struct red            : Sensors { };
struct reset          : tinyfsm::Event { };

struct  pad_idle      : tinyfsm::Event
{}

struct  pad_hold      : tinyfsm::Event
{}

struct  ascent        : tinyfsm::Event
{}

struct  descent       : tinyfsm::Event
{}

struct  landing       : tinyfsm::Event
{}

struct  landing_idle  : tinyfsm::Event
{}
// ----------------------------------------------------------------------------
// Rocket (FSM base class) declaration
//

class Rocket
: public tinyfsm::Fsm<Rocket>
{
  /* NOTE: react(), entry() and exit() functions need to be accessible
   * from tinyfsm::Fsm class. You might as well declare friendship to
   * tinyfsm::Fsm, and make these functions private:
   *
   * friend class Fsm;
   */
public:

  /* default reaction for unhandled events */
  void react(tinyfsm::Event const &) { };

  virtual void react(green  const &);
  virtual void react(red    const &);
  void         react(reset  const &);

  virtual void entry(void) { };  /* entry actions in some states */
  void         exit(void)  { };  /* no exit actions at all */

protected:

  static constexpr int initial_floor = 0;
  static int current_floor;
  static int dest_floor;
};


#endif
