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
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLayout>

// TinyIO:
#include <tinyio/gui/application.h>
#include <tinyio/gui/select_device_widget.h>

// Local:
#include "main_window.h"


namespace tinyiogui {

MainWindow::MainWindow (Application* application):
	_application (application)
{
	_select_device_widget = std::make_unique <SelectDeviceWidget> (this, application->device_manager());

	_stack = std::make_unique<QStackedLayout> (this);
	_stack->addWidget (_select_device_widget.get());
	_stack->setCurrentWidget (_select_device_widget.get());

	QObject::connect (_select_device_widget.get(), &SelectDeviceWidget::selected,
					  this, &MainWindow::device_selected);

	setWindowTitle ("TinyIO");
}


void
MainWindow::show_selector()
{
	_select_device_widget->refresh_list();
	_stack->setCurrentWidget (_select_device_widget.get());
}


void
MainWindow::show_control_widget()
{
	if (_control_widget_wrapper && _control_widget)
		_stack->setCurrentWidget (_control_widget_wrapper.get());
}


Unique<QWidget>
MainWindow::make_control_widget_wrapper (ControlWidget* control_widget)
{
	auto result = std::make_unique<QWidget> (this);

	auto disconnect_button = new QPushButton ("Disconnect", result.get());

	auto hold_button = new QPushButton ("&Hold changes", result.get());
	hold_button->setCheckable (true);

	auto buttons_layout = new QHBoxLayout();
	buttons_layout->addWidget (disconnect_button);
	buttons_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	buttons_layout->addWidget (hold_button);

	auto layout = new QVBoxLayout (result.get());
	layout->addLayout (buttons_layout);
	layout->addWidget (control_widget);

	return result;
}


void
MainWindow::device_selected()
{
	auto device_info = _select_device_widget->selected_device_info();

	if (device_info)
	{
		try {
			// Open device:
			tinyio::Device device = device_info->open();
			_control_widget = new ControlWidget (this, std::move (device));

			// Make control widget wrapper:
			_control_widget_wrapper = make_control_widget_wrapper (_control_widget);
			_stack->addWidget (_control_widget_wrapper.get());

			show_control_widget();
		}
		catch (std::exception const& e)
		{
			auto message = "Failed to open selected device:\n" + Application::stringify_exception (e);
			QMessageBox::warning (this, "Connection error", QString::fromStdString (message));
			show_selector();
		}
	}
	else
	{
		QMessageBox::warning (this, "Connection error", "Failed to find selected device.");
		show_selector();
	}
}

} // namespace tinyiogui

