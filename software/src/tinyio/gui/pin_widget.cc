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

// Local:
#include "pin_widget.h"


namespace tinyiogui {

PinWidget::PinWidget (QWidget* parent, uint8_t pin):
	QWidget (parent),
	_pin (pin)
{
	auto layout = new QHBoxLayout (this);
	layout->setMargin (0);

	_button = std::make_unique<QPushButton> ("IN", this);
	_button->setFixedSize (_button->size().height(), _button->size().height());

	_label = std::make_unique<QLabel> (QString::number (pin), this);

	layout->addWidget (_button.get());
	layout->addWidget (_label.get());
	layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	// Save standard button colors:
	_std_button_bg = _button->palette().color (QPalette::Button);
	_std_button_fg = _button->palette().color (QPalette::ButtonText);

	set_button_highlighted (_button.get(), true);
}


void
PinWidget::set_button_highlighted (QPushButton* button, bool logic_level)
{
	QPalette p = button->palette();
	p.setColor (QPalette::Button, logic_level ? Qt::green : Qt::red);
	p.setColor (QPalette::ButtonText, logic_level ? Qt::black : _std_button_fg);
	button->setPalette (p);
}

} // namespace tinyiogui

