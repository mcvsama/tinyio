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
#include <tinyio/gui/gui.h>

// Local:
#include "main_window.h"


namespace tinyiogui {

MainWindow::MainWindow (Application* application):
	_application (application)
{
	_select_device_widget = new SelectDeviceWidget (this, application->device_manager());

	_stack = new QStackedLayout (this);
	_stack->addWidget (_select_device_widget);
	_stack->setCurrentWidget (_select_device_widget);

	QObject::connect (_select_device_widget, &SelectDeviceWidget::selected, this, &MainWindow::connect);

	setWindowTitle ("TinyIO");
}


void
MainWindow::show_selector()
{
	_select_device_widget->refresh_list();
	_stack->setCurrentWidget (_select_device_widget);
}


void
MainWindow::show_control_widget()
{
	if (_control_widget_wrapper && _control_widget)
		_stack->setCurrentWidget (_control_widget_wrapper);
}


QWidget*
MainWindow::make_control_widget_wrapper (ControlWidget* control_widget)
{
	auto result = new QWidget (this);

	auto frame = new QFrame (result);
	frame->setFrameShadow (QFrame::Sunken);
	frame->setFrameShape (QFrame::StyledPanel);

	_disconnect_button = new QPushButton ("Disconnect", result);
	prepare_button (_disconnect_button);
	QObject::connect (_disconnect_button, &QPushButton::clicked, this, &MainWindow::disconnect);

	_hold_button = new QPushButton ("&Hold changes", result);
	prepare_button (_hold_button);
	_hold_button->setCheckable (true);
	QObject::connect (_hold_button, &QPushButton::clicked, this, &MainWindow::forward_settings);

	auto buttons_layout = new QHBoxLayout();
	buttons_layout->addWidget (_disconnect_button);
	buttons_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	buttons_layout->addWidget (_hold_button);

	auto layout = new QVBoxLayout (result);
	layout->addLayout (buttons_layout);
	layout->addWidget (frame);

	auto frame_layout = new QVBoxLayout (frame);
	frame_layout->addWidget (control_widget);

	return result;
}


void
MainWindow::connect()
{
	auto device_serial = _select_device_widget->selected_device_serial();

	if (device_serial)
	{
		try {
			_control_widget = new ControlWidget (this, _application, device_serial->toStdString());

			// Make control widget wrapper:
			_control_widget_wrapper = make_control_widget_wrapper (_control_widget);
			_stack->addWidget (_control_widget_wrapper);

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


void
MainWindow::disconnect()
{
	delete _control_widget_wrapper;
	_control_widget_wrapper = nullptr;
	show_selector();
}


void
MainWindow::forward_settings()
{
	if (_control_widget)
		_control_widget->set_hold (_hold_button->isChecked());
}

} // namespace tinyiogui

