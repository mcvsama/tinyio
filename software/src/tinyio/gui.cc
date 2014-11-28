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

// Lib:
#include <libusbcc/libusbcc.h>

// TinyIO:
#include <tinyio/tinyio/device_manager.h>


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
		tinyio::DeviceManager tinyio_manager;
		auto devices = tinyio_manager.find_devices();

		if (devices.empty())
			std::cout << "No devices found." << std::endl;
		else
		{
			std::cout << "Found " << devices.size() << " device(s):" << std::endl;

			for (auto const& device: devices)
			{
				tinyio::Device tinyio = device.open();
				std::cout << "  TinyIO version " << tinyio.release_version_str() << ", S/N: " << tinyio.serial_number() << std::endl;

				if (tinyio.release_version_str() != "1.0")
				{
					std::clog << "    Unsupported version, skipping." << std::endl;
					continue;
				}
				else
				{
					tinyio.reset();

					for (int pin = 0; pin < 24; ++pin)
						tinyio.configure_pin (pin, tinyio::Input);
					while (true)
					{
						auto in = tinyio.get_pin_levels();
						for (int i = 0; i < 24; ++i)
							std::cout << in[i];
						std::cout << std::endl;
					}


					for (int pin = 0; pin < 24; ++pin)
						tinyio.configure_pin (pin, tinyio::Output);
					for (int z = 0; z < 1000; ++z)
					{
						for (int pin = 0; pin < 24; ++pin)
						{
							for (int i = 0; i < 20; ++i)
							{
								usleep (50000);
								tinyio.set_pin_level (pin, i % 2 == 0);
								tinyio.commit();
						auto in = tinyio.get_pin_levels();
						for (int i = 0; i < 24; ++i)
							std::cout << in[i];
						std::cout << std::endl;
							}
							tinyio.set_pin_level (pin, false);
						}
					}
				}
			}
		}
	}
	catch (std::exception const& e)
	{
		print_exception (e);
	}

	return EXIT_SUCCESS;
}

