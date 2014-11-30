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
#include <utility>

// Lib:
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

// Local:
#include "control_widget.h"


namespace tinyiogui {

ControlWidget::ControlWidget (QWidget* parent, tinyio::Device&& device):
	QWidget (parent),
	_device (std::move (device))
{
	auto layout = new QGridLayout (this);
	layout->setMargin (0);

	for (int port = 0; port < 3; ++port)
	{
		for (int bit = 0; bit < 8; ++bit)
		{
			int pin_i = 8 * port + bit;
			_pin_widgets[pin_i] = std::make_unique<PinWidget> (this, pin_i);
			layout->addWidget (_pin_widgets[pin_i].get(), bit, port);
		}
	}
#if 0
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
#endif
}

} // namespace tinyiogui

