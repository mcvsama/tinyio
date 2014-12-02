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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QLineEdit>
#include <QtGui/QMouseEvent>

// Local:
#include "pin_widget.h"


namespace tinyiogui {

PinWidget::PinWidget (QWidget* parent, uint8_t pin):
	QWidget (parent),
	_pin (pin)
{
	auto layout = new QHBoxLayout (this);
	layout->setMargin (0);

	_button = new QPushButton ("", this);
	_button->setFixedSize (_button->size().height(), _button->size().height());
	QObject::connect (_button, &QPushButton::clicked, this, &PinWidget::button_pressed);

	_label = new QLineEdit (this);
	_label->setText (QString::number (pin));

	layout->addWidget (_button);
	layout->addWidget (_label);
	layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	_menu = new QMenu (this);
	_menu->addAction ("Configure as &input", this, SLOT (configure_as_input()));
	_menu->addAction ("Configure as &output", this, SLOT (configure_as_output()));
}


void
PinWidget::set_configured_pin_direction (tinyio::PinDirection direction)
{
	_configured_direction = direction;
	update_gui();
}


void
PinWidget::set_actual_pin_direction (tinyio::PinDirection direction)
{
	_actual_direction = direction;
	update_gui();
}


void
PinWidget::set_configured_pin_level (bool logic_level)
{
	_configured_level = logic_level;
	update_gui();
}


void
PinWidget::set_actual_pin_level (bool logic_level)
{
	_actual_level = logic_level;
	update_gui();
}


void
PinWidget::update_gui()
{
	QColor on, off;

	switch (_actual_direction)
	{
		case tinyio::Input:
			if (_configured_level == true)
			{
				on = { 0x00, 0xa6, 0xff };
				off = { 0x00, 0x21, 0x48 };
			}
			else
			{
				on = { 0x2a, 0xff, 0x00 };
				off = { 0x0c, 0x48, 0x00 };
			}
			break;

		case tinyio::Output:
			on = { 0xff, 0x00, 0x00 };
			off = { 0x48, 0x00, 0x00 };
			break;
	}

	QString dir_str = (_configured_direction == tinyio::Input) ? "I" : "O";
	QString level_str = _configured_level ? "↑" : "↓";
	_button->setText (dir_str + level_str);

	set_button_color (_button, _actual_level ? on : off);
}


void
PinWidget::mousePressEvent (QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		event->accept();
		_menu->popup (QCursor::pos());
	}
	else
		event->ignore();
}


void
PinWidget::configure_as_input()
{
	_configured_direction = tinyio::Input;
	set_pin_direction (_configured_direction);
}


void
PinWidget::configure_as_output()
{
	_configured_direction = tinyio::Output;
	set_pin_direction (_configured_direction);
}


void
PinWidget::button_pressed()
{
	emit flip_pin_level();
}


void
PinWidget::set_button_color (QPushButton* button, QColor const& color)
{
	QPalette p = button->palette();
	p.setColor (QPalette::Button, color);
	p.setColor (QPalette::ButtonText, Qt::black);
	button->setPalette (p);
}

} // namespace tinyiogui

