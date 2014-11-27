/* vim:ts=4
 *
 * Copyleft 2014…2015  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

// Standard:
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <iomanip>

// TinyIO:
#include <libusbcc/libusbcc.h>
#include <tinyio/control_request.h>


void
print_exception (std::exception const& e, int level = 0)
{
	std::string indent (level, ' ');
	std::cerr << indent << indent;
	if (level == 0)
		std::cerr << "Exception: ";
	else
		std::cerr << "Reason: ";
	std::cerr << e.what() << std::endl;

	try {
		std::rethrow_if_nested (e);
	}
	catch (const std::exception& e)
	{
		print_exception (e, level + 1);
	}
	catch (...)
	{
		throw;
	}
}


int main (int, char**, char**)
{
	using namespace libusb;

	try {
		libusb::Session usb;
		for (auto const& descriptor: usb.device_descriptors())
		{
			try {
				constexpr libusb::VendorID kVendorID = 0x16c0;
				constexpr libusb::ProductID kProductID = 0x05dc;
				constexpr auto kManufacturer = "mulabs.org";
				constexpr auto kProduct = "TinyIO";

				Device device = descriptor.open();

				if (device.vendor_id() == kVendorID && device.product_id() == kProductID &&
					device.manufacturer() == kManufacturer && device.product() == kProduct)
				{
					std::clog << "Found TinyIO version " << device.release_version_str();
					if (device.release_version() != 0x0100)
					{
						std::clog << " - unsupported version - skipping." << std::endl;
						continue;
					}
					else
					{
						std::clog << " - connecting." << std::endl;
						device.send (libusb::ControlTransfer (static_cast<uint8_t> (tinyio::USBControlRequest::ConfigurePins), 0, 0), 0, { 0xff, 0xff, 0xff });
						device.send (libusb::ControlTransfer (static_cast<uint8_t> (tinyio::USBControlRequest::SetPins), 0, 0), 0, { 0x55, 0x55, 0x55 });
						// TODO
					}
				}
			}
			catch (std::exception const& e)
			{
				print_exception (e);
			}
		}
	}
	catch (std::exception const& e)
	{
		print_exception (e);
	}

	return EXIT_SUCCESS;
}

