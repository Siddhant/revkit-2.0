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
 * @file add_circuit.hpp
 *
 * @brief Prepending, inserting and appending circuits to another circuit
 *
 * @author Mathias Soeken
 * @since  1.0
 */

#include <reversible/circuit.hpp>

namespace revkit
{

  /**
   * @brief Insert a circuit \p src at the end of another circuit \p circ
   *
   * @param circ Destination circuit
   * @param src  Source circuit
   * @param controls Controls, which are added to each gate in \p src (introduced in version 1.1)
   *
   * @since  1.0
   */
  void append_circuit( circuit& circ, const circuit& src, const gate::control_container& controls = gate::control_container() );

  /**
   * @brief Insert a circuit \p src at the beginning of another circuit \p circ
   *
   * @param circ Destination circuit
   * @param src  Source circuit
   * @param controls Controls, which are added to each gate in \p src (introduced in version 1.1)
   *
   * @since  1.0
   */
  void prepend_circuit( circuit& circ, const circuit& src, const gate::control_container& controls = gate::control_container() );

  /**
   * @brief Insert a circuit \p src before gate \p pos (counting from 0) of another circuit \p circ
   *
   * @param circ Destination circuit
   * @param pos  Position where to insert
   * @param src  Source circuit
   * @param controls Controls, which are added to each gate in \p src (introduced in version 1.1)
   *
   * @since  1.0
   */
  void insert_circuit( circuit& circ, unsigned pos, const circuit& src, const gate::control_container& controls = gate::control_container() );

}

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
