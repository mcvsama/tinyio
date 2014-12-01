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
	for (std::size_t pin = 0; pin < tinyio::Device::kPinsCount; ++pin)
	{
		Unique<PinWidget> pin_widget = std::make_unique<PinWidget> (this, pin);
		QObject::connect (pin_widget.get(), &PinWidget::flip_pin_level, std::bind (&ControlWidget::flip_pin_level, this, pin));
		QObject::connect (pin_widget.get(), &PinWidget::set_pin_direction, std::bind (&ControlWidget::set_pin_direction, this, pin, std::placeholders::_1));
		_pin_widgets[pin] = std::move (pin_widget);
	}

	auto info_label = new QLabel ("Green → high Z • Blue → pull-up • Red → output");

	_refresh_timer = std::make_unique<QTimer> (this);
	_refresh_timer->setInterval (20);
	_refresh_timer->setSingleShot (false);
	QObject::connect (_refresh_timer.get(), &QTimer::timeout, this, &ControlWidget::timeout);
	_refresh_timer->start();

	auto grid_layout = new QGridLayout();
	grid_layout->setMargin (0);
	for (int port = 0; port < 3; ++port)
		for (int bit = 0; bit < 8; ++bit)
			grid_layout->addWidget (_pin_widgets[8 * port + bit].get(), bit, port);

	auto layout = new QVBoxLayout (this);
	layout->setMargin (0);
	layout->addLayout (grid_layout);
	layout->addWidget (info_label);

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
	{
		_pin_widgets[i]->set_actual_pin_level (levels[i]);
		_pin_widgets[i]->set_configured_pin_level (_device.get_pin_level (i));
	}
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

