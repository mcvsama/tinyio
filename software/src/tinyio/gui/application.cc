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

// Lib:
#include <QtCore/QTextCodec>
#include <QtWidgets/QMessageBox>

// TinyIO:
#include <tinyio/gui/main_window.h>

// Local:
#include "application.h"


namespace tinyiogui {

Application::Application (int& argc, char** argv):
	QApplication (argc, argv)
{
	// Casting QString to std::string|const char* should yield UTF-8 encoded strings.
	// Also encode std::strings and const chars* in UTF-8:
	QTextCodec::setCodecForLocale (QTextCodec::codecForName ("UTF-8"));

	_device_manager = std::make_unique<tinyio::DeviceManager>();
	_main_window = std::make_unique<MainWindow> (this);

	_main_window->show();

#if 0
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
#endif
}


bool
Application::notify (QObject* receiver, QEvent* event)
{
	try {
		return QApplication::notify (receiver, event);
	}
	catch (std::exception const& e)
	{
		QMessageBox::critical (nullptr, "Exception", QString::fromStdString (stringify_exception (e)));
	}

	return false;
}


std::string
Application::stringify_exception (std::exception const& e)
{
	std::function<void (std::exception const&, std::string&, int)> stringify_one;

	stringify_one = [&](std::exception const& e, std::string& result, int level)
	{
		std::string indent (level, ' ');
		result += indent + indent;

		if (level == 0)
			result += "Exception: ";
		else
			result += "Reason: ";

		result += e.what();
		result += '\n';

		try {
			std::rethrow_if_nested (e);
		}
		catch (const std::exception& e)
		{
			stringify_one (e, result, level + 1);
		}
		catch (...)
		{
			// TODO maybe append 'unknown exception'?
			throw;
		}
	};

	std::string result;
	stringify_one (e, result, 0);

	return result;
}

} // namespace tinyiogui

