/* vim:ts=4
 *
 * Copyleft 2012…2014  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef BLINKER_H__INCLUDED
#define BLINKER_H__INCLUDED

// STD:
#include <stdint.h>

// Lib:
#include <mulabs_avr/mcu.h>
#include <mulabs_avr/pin.h>


template<size_t pBlinkPeriod, size_t pOnTime>
	class Blinker
	{
		static_assert (pBlinkPeriod >= pOnTime, "blink period must be >= LED on time");

		constexpr static auto kBlinkPeriod	= pBlinkPeriod;
		constexpr static auto kOnTime		= pOnTime;

	  public:
		// Ctor
		Blinker (mulabs::avr::MCU::Pin const& led_pin);

		void
		blink (size_t blinks = 1);

		void
		perform();

	  private:
		int16_t						_request_cnt = 0;
		mulabs::avr::MCU::Pin const	_led;
	};


template<size_t P, size_t O>
	inline
	Blinker<P, O>::Blinker (mulabs::avr::MCU::Pin const& led_pin):
		_led (led_pin)
	{ }


template<size_t P, size_t O>
	void
	Blinker<P, O>::blink (size_t blinks)
	{
		size_t n = blinks * kBlinkPeriod;
		if (n > _request_cnt)
			_request_cnt = n;
	}


template<size_t P, size_t O>
	void
	Blinker<P, O>::perform()
	{
		if (_request_cnt > 0)
		{
			if (_request_cnt % kBlinkPeriod > kBlinkPeriod - kOnTime)
				_led.set_high();
			else
				_led.set_low();
			--_request_cnt;
		}
	}

#endif

