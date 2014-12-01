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
			Unique<PinWidget> pin_widget = std::make_unique<PinWidget> (this, pin_i);
			QObject::connect (pin_widget.get(), &PinWidget::flip_pin_level, std::bind (&ControlWidget::flip_pin_level, this, pin_i));
			QObject::connect (pin_widget.get(), &PinWidget::set_pin_direction, std::bind (&ControlWidget::set_pin_direction, this, pin_i, std::placeholders::_1));
			_pin_widgets[pin_i] = std::move (pin_widget);
			layout->addWidget (_pin_widgets[pin_i].get(), bit, port);
		}
	}

	_refresh_timer = std::make_unique<QTimer> (this);
	_refresh_timer->setInterval (20);
	_refresh_timer->setSingleShot (false);
	QObject::connect (_refresh_timer.get(), &QTimer::timeout, this, &ControlWidget::timeout);
	_refresh_timer->start();

	get_pin_levels();
}


void
ControlWidget::set_pin_direction (uint8_t pin, tinyio::PinDirection direction)
{
	_device.configure_pin (pin, direction);
	_device.commit();
}


void
ControlWidget::flip_pin_level (uint8_t pin)
{
	_device.flip_pin_level (pin);
	_device.commit();
}


void
ControlWidget::get_pin_levels()
{
	auto levels = _device.get_pin_levels();
	for (std::size_t i = 0; i < levels.size(); ++i)
		_pin_widgets[i]->set_actual_pin_level (levels[i]);
}


void
ControlWidget::timeout()
{
	try {
		get_pin_levels();
		_failsafe = false;
	}
	catch (...)
	{
		// Idea: after first exception is throw, silence all subsequent
		// exceptions until another operation succeeds.
		if (!std::exchange (_failsafe, true))
			throw;
	}
}

} // namespace tinyiogui

