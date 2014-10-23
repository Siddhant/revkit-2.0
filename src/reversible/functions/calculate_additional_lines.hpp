/* RevKit (www.revkit.org)
 * Copyright (C) 2009-2014  University of Bremen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file calculate_additional_lines.hpp
 *
 * @brief Calculates the minimal number of additional lines using BDDs
 *
 * @author Mathias Soeken
 * @since  2.0
 */

#ifndef CALCULATE_ADDITIONAL_LINES_HPP
#define CALCULATE_ADDITIONAL_LINES_HPP

#include <string>

#include <core/properties.hpp>

namespace revkit
{

unsigned calculate_additional_lines( const std::string& filename,
                                     properties::ptr settings = properties::ptr(),
                                     properties::ptr statistics = properties::ptr() );

}

#endif

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
