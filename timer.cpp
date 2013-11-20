// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include <Ark/HTimerWheel.h>
#include <EosSdk/timer.h>
#include <TimerSm.h>
#include <Tac/Tracing.h>
#include <Tac/Time.h>
#include <map>

DEFAULT_TRACE_HANDLE( "eos" );

namespace eos {

seconds_t now() {
   return Tac::now();
}

Ark::TimerWheel::Ptr
get_timer_wheel() {
   static Ark::TimerWheel::Ptr timer_wheel = Ark::TimerWheel::TimerWheelIs(
         0,
         Tac::activityManager()->clock(),
         1,
         0xffffffff,
         true,
         1024 );
   return timer_wheel;
}

std::map< timer_task *, TimerTaskSm::Ptr > timerTaskSm;

timer_task::timer_task() {
   // Enqueue the new timer task, with no wakeup time
   timerTaskSm[ this ] = TimerTaskSm::TimerTaskSmIs( get_timer_wheel() );
}

timer_task::~timer_task() {
   // Dequeue ourselves
   timerTaskSm.erase( this );
}

void
timer_task::wakeup_time_is( seconds_t when ) {
   TRACE9( __PRETTY_FUNCTION__ << " " << when );
   timerTaskSm[ this ]->wakeupTimeIs( when );
}

}
