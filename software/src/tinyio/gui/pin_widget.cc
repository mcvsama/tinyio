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

	_button = std::make_unique<QPushButton> ("", this);
	_button->setFixedSize (_button->size().height(), _button->size().height());
	QObject::connect (_button.get(), &QPushButton::clicked, this, &PinWidget::button_pressed);

	_label = std::make_unique<QLabel> (QString::number (pin), this);

	layout->addWidget (_button.get());
	layout->addWidget (_label.get());
	layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	_menu = new QMenu (this);
	_menu->addAction ("Configure as &input", this, SLOT (configure_as_input()));
	_menu->addAction ("Configure as &output", this, SLOT (configure_as_output()));
}


void
PinWidget::set_actual_pin_level (bool logic_level)
{
	set_button_highlighted (_button.get(), _direction, logic_level);
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
	_direction = tinyio::Input;
	set_pin_direction (_direction);
}


void
PinWidget::configure_as_output()
{
	_direction = tinyio::Output;
	set_pin_direction (_direction);
}


void
PinWidget::button_pressed()
{
	emit flip_pin_level();
}


void
PinWidget::set_button_highlighted (QPushButton* button, tinyio::PinDirection direction, bool logic_level)
{
	QColor on, off;

	switch (direction)
	{
		case tinyio::Input:
			on = Qt::green;
			off = Qt::darkGreen;
			button->setText ("IN");
			break;

		case tinyio::Output:
			on = Qt::red;
			off = Qt::darkRed;
			button->setText ("OUT");
			break;
	}

	QPalette p = button->palette();
	p.setColor (QPalette::Button, logic_level ? on : off);
	p.setColor (QPalette::ButtonText, Qt::black);
	button->setPalette (p);
}

} // namespace tinyiogui

