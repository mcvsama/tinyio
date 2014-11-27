// STD:
#include <stdio.h>
#include <string.h>

// AVR:
#include <avr/wdt.h>
#include <avr/interrupt.h>

// Mulabs:
#include <mulabs_avr/mcu.h>
#include <mulabs_avr/port.h>
#include <mulabs_avr/pin.h>

extern "C" {
#include <usbdrv/usbdrv.h>
}

// TinyIO:
#include <tinyio/control_request.h>

// Local:
#include "blinker.h"


using namespace mulabs::avr;
using tinyio::USBControlRequest;


constexpr MCU::Port	port_0_7	= MCU::c;
constexpr MCU::Port	port_15_8	= MCU::a;
constexpr MCU::Port	port_16_23	= MCU::b;

constexpr MCU::Pin	out_0		= MCU::c_0;
constexpr MCU::Pin	out_1		= MCU::c_1;
constexpr MCU::Pin	out_2		= MCU::c_2;
constexpr MCU::Pin	out_3		= MCU::c_3;
constexpr MCU::Pin	out_4		= MCU::c_4;
constexpr MCU::Pin	out_5		= MCU::c_5;
constexpr MCU::Pin	out_6		= MCU::c_6;
constexpr MCU::Pin	out_7		= MCU::c_7;

constexpr MCU::Pin	out_8		= MCU::a_7;
constexpr MCU::Pin	out_9		= MCU::a_6;
constexpr MCU::Pin	out_10		= MCU::a_5;
constexpr MCU::Pin	out_11		= MCU::a_4;
constexpr MCU::Pin	out_12		= MCU::a_3;
constexpr MCU::Pin	out_13		= MCU::a_2;
constexpr MCU::Pin	out_14		= MCU::a_1;
constexpr MCU::Pin	out_15		= MCU::a_0;

constexpr MCU::Pin	out_16		= MCU::b_0;
constexpr MCU::Pin	out_17		= MCU::b_1;
constexpr MCU::Pin	out_18		= MCU::b_2;
constexpr MCU::Pin	out_19		= MCU::b_3;
constexpr MCU::Pin	out_20		= MCU::b_4;
constexpr MCU::Pin	out_21		= MCU::b_5;
constexpr MCU::Pin	out_22		= MCU::b_6;
constexpr MCU::Pin	out_23		= MCU::b_7;

constexpr MCU::Pin	usb_dminus	= MCU::d_3;
constexpr MCU::Pin	usb_dplus	= MCU::d_2;
constexpr MCU::Pin	status_led	= MCU::d_4;

constexpr MCU::Pin all_outputs[] = {
	out_0,	out_1,	out_2,	out_3,	out_4,	out_5,	out_6,	out_7,
	out_8,	out_9,	out_10,	out_11,	out_12,	out_13,	out_14,	out_15,
	out_16,	out_17,	out_18,	out_19,	out_20,	out_21,	out_22,	out_23,
};


Blinker<100, 20> g_blinker (status_led);
USBControlRequest g_last_request;


extern "C" {

usbMsgLen_t
usbFunctionSetup (uint8_t data[8])
{
	g_blinker.blink();

	usbRequest* req = reinterpret_cast<usbRequest*> (data);
	g_last_request = static_cast<USBControlRequest> (req->bRequest);

	switch (g_last_request)
	{
		case USBControlRequest::ConfigurePins:
		case USBControlRequest::SetPins:
		{
			uint8_t len = req->wLength.word;
			// We need three bytes for all our pins:
			if (len == 3)
				// Request call to usbFunctionWrite():
				return USB_NO_MSG;
			else
				break;
		}

		case USBControlRequest::GetPins:
			// Request call to usbFunctionRead():
			return USB_NO_MSG;
	}

	return 0;
}


uint8_t
usbFunctionWrite (uint8_t* data, uint8_t len)
{
	if (len == 3)
	{
		// Note: The middle byte has all bits swapped (LSB<->MSB),
		// so port (which has also flipped outputs) can be assigned
		// directly.

		switch (g_last_request)
		{
			case USBControlRequest::ConfigurePins:
			{
				port_0_7.configure_as_output_exclusive (PinSet (data[0]));
				port_15_8.configure_as_output_exclusive (PinSet (data[0]));
				port_16_23.configure_as_output_exclusive (PinSet (data[0]));
			}

			case USBControlRequest::SetPins:
			{
				port_0_7.set_high_exclusive (PinSet (data[0]));
				port_15_8.set_high_exclusive (PinSet (data[1]));
				port_16_23.set_high_exclusive (PinSet (data[2]));
			}

			default:
				break;
		}
	}

	// Return 1 if we have all required data:
	return 1;
}


uint8_t
usbFunctionRead (uint8_t* data, uint8_t len)
{
	if (len >= 3)
	{
		return 0;
		// TODO
	}
	else
		return 0;
}

} // extern "C"


int
main()
{
	usbDeviceDisconnect();
	wdt_enable (WDTO_1S);

	for (MCU::Pin const& pin: all_outputs)
	{
		pin.configure_as_input();
		pin.set_low();
	}

	usbInit();
	MCU::sleep_ms<100>();
	usbDeviceConnect();
	sei();

	status_led.set_low();
	status_led.configure_as_output();

	g_blinker.blink (3);

	while (true)
	{
		wdt_reset();
		usbPoll();
		g_blinker.perform();
		MCU::sleep_ms<1>();
	}

	return 0;
}

