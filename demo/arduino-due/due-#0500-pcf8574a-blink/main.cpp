// ==========================================================================
//
// kitt on 8 LEDs on a HC595 connected to a DB103 board (LPC1114)
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using uart    = target::uart<>;

// using i2c_bus = gf::i2c_bus_bb_scl_sda< target::scl1, target::sda1, timing >;

using i2c_bus = target::i2c0<>;

using chip    = gf::pcf8574a< i2c_bus, 0x01 >;
using led     = chip::p0;

int main( void ){

   timing::init();
   //timing::ms< 2'000 >::wait();
   gf::ostream< gf::formatter< uart > > cout;
   cout << __LINE__ << "\n";
   chip::init();

   if(1)for(;;){
      cout << __LINE__ << "\n";
      gf::direct< chip >::write( 0xAA );
      cout << __LINE__ << "\n";
      timing::ms< 200 >::wait();
      gf::direct< chip >::write( 0x55 );
      cout << __LINE__ << "\n";
      timing::ms< 200 >::wait();
   }

   gf::blink< led, timing::ms< 3 >  >();
}
