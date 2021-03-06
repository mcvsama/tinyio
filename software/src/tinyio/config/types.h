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

#ifndef TINYIO__CONFIG__TYPES_H__INCLUDED
#define TINYIO__CONFIG__TYPES_H__INCLUDED

// Standard:
#include <cstddef>
#include <memory>

// Lib:
#include <boost/optional.hpp>


template<class T>
	using Optional = boost::optional<T>;


template<class T>
	using Unique = std::unique_ptr<T>;

#endif

