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

// Local:
#include "gui.h"


namespace tinyiogui {

void
prepare_button (QPushButton* button)
{
	QFont font = button->font();
	int h = font.pixelSize();
	if (h == -1)
		h = font.pointSize();
	button->setFixedHeight (2 * h);
}

} // namespace tinyiogui

