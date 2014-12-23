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

// TinyIO:
#include <tinyio/gui/application.h>

// Local:
#include "control_widget.h"


namespace tinyiogui {

ControlWidget::ControlWidget (QWidget* parent, Application* application, std::string const& device_serial):
	QWidget (parent),
	_application (application),
	_device_serial (device_serial)
{
	for (std::size_t pin = 0; pin < tinyio::DeviceConfig::kPinsCount; ++pin)
	{
		Unique<PinWidget> pin_widget = std::make_unique<PinWidget> (this, pin);
		QObject::connect (pin_widget.get(), &PinWidget::flip_pin_level, std::bind (&ControlWidget::flip_pin_level, this, pin));
		QObject::connect (pin_widget.get(), &PinWidget::set_pin_direction, std::bind (&ControlWidget::set_pin_direction, this, pin, std::placeholders::_1));
		_pin_widgets[pin] = std::move (pin_widget);
	}

	auto info_label = new QLabel ("Green → high Z • Blue → pull-up • Red → output");

	_refresh_timer = std::make_unique<QTimer> (this);
	_refresh_timer->setInterval (10);
	_refresh_timer->setSingleShot (false);
	QObject::connect (_refresh_timer.get(), &QTimer::timeout, this, &ControlWidget::timeout);
	_refresh_timer->start();

	_reconnect_timer = std::make_unique<QTimer> (this);
	_reconnect_timer->setInterval (200);
	_reconnect_timer->setSingleShot (false);
	QObject::connect (_reconnect_timer.get(), &QTimer::timeout, this, &ControlWidget::reconnect);
	_reconnect_timer->start();

	auto grid_layout = new QGridLayout();
	grid_layout->setMargin (0);
	for (int port = 0; port < 3; ++port)
		for (int bit = 0; bit < 8; ++bit)
			grid_layout->addWidget (_pin_widgets[8 * port + bit].get(), bit, port);

	auto layout = new QVBoxLayout (this);
	layout->setMargin (0);
	layout->addLayout (grid_layout);
	layout->addWidget (info_label);

	reconnect();
	get_pin_levels();
}


void
ControlWidget::set_hold (bool enabled)
{
	_hold_enabled = enabled;
	commit();
}


void
ControlWidget::set_pin_direction (uint8_t pin, tinyio::PinDirection direction)
{
	_device_config.configure_pin (pin, direction);
	commit();
}


void
ControlWidget::flip_pin_level (uint8_t pin)
{
	_device_config.flip_pin_level (pin);
	commit();
}


void
ControlWidget::get_pin_levels()
{
	if (_device)
	{
		auto levels = _device->get_pin_levels();
		auto directions = _device->get_pin_directions();

		for (std::size_t i = 0; i < levels.size(); ++i)
		{
			_pin_widgets[i]->set_actual_pin_direction (directions[i]);
			_pin_widgets[i]->set_configured_pin_direction (_device_config.get_pin_direction (i));

			_pin_widgets[i]->set_actual_pin_level (levels[i]);
			_pin_widgets[i]->set_configured_pin_level (_device_config.get_pin_level (i));
		}
	}
}


void
ControlWidget::commit()
{
	if (_device)
		if (!_hold_enabled)
			_device->commit (_device_config);
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
		// Idea: after first exception is thrown, silence all subsequent
		// exceptions until an operation succeeds.
		if (!std::exchange (_failsafe, true))
			throw;
	}
}


void
ControlWidget::reconnect()
{
	try {
		if (!_device || !_device->good())
		{
			setEnabled (false);

			auto device_info = _application->device_manager()->find_by_serial (_device_serial);

			if (device_info)
			{
				_device = std::make_unique<tinyio::Device> (std::move (device_info->open()));
				commit();
				_failsafe = false;
			}
		}
		else if (_device->good())
			setEnabled (true);
	}
	catch (...)
	{
		if (!std::exchange (_failsafe, true))
			throw;
	}
}

} // namespace tinyiogui

