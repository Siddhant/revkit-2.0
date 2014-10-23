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
 * @file active_controls.hpp
 *
 * @brief Slot for adding control lines automatically
 *
 * @author Mathias Soeken
 * @since  1.1
 */

#ifndef ACTIVE_CONTROLS_HPP
#define ACTIVE_CONTROLS_HPP

#include <reversible/gate.hpp>

namespace revkit
{

  /**
   * @brief Slot for adding control lines automatically
   *
   * This is a slot which can be connected to the circuit::gate_added
   * signal in order to add special control lines automatically.
   * These are called active controls. For example a block of gates should
   * be added which is only active when a particular control line is
   * activated.
   *
   * This control line can be activatd with this class and then the
   * class has to be connected to the signal of the circuit.
   *
   * @section example_active_controls Example
   * The code below seems to add a CNOT and a NOT gate.
   * However, since the controller is connected and the
   * line 0 is added to the controller each time also this
   * control line is activated and, thus, a Toffoli gate
   * and a CNOT gate are added.
   * @code
   * #include <reversible/circuit.hpp>
   * #include <reversible/functions/active_controls.hpp>
   * #include <reversible/functions/add_gates.hpp>
   *
   * revkit::circuit circ( 3 );
   *
   * revkit::active_controls controller;
   * controller.add( 0u );
   *
   * circ.gate_added.connect( controller );
   *
   * revkit::append_cnot( circ, 1u, 2u );
   * revkit::append_not( circ, 1u );
   * @endcode
   */
  class active_controls
  {
  public:
    active_controls();
    ~active_controls();

    /**
     * @brief Add an active control line
     *
     * An active control line is implicitely added by every operation
     * which adds a new gate to the circuit. This is especially useful
     * in hierarchical synthesis approaches.
     *
     * This method does not add a line, it only sets an existing
     * line to be active.
     *
     * @param control Index of the line which should be active
     *
     * @since  1.1
     */
    void add( variable control );

    /**
     * @brief Removes an active control line
     *
     * An active control line is implicitely added by every operation
     * which adds a new gate to the circuit. This is especially useful
     * in hierarchical synthesis approaches.
     *
     * This method does not remove a line, it only unsets an existing
     * line to be active.
     *
     * @param control Index of the line which should be deactivated
     *
     * @since  1.1
     */
    void remove( variable control );

    /**
     * @brief Returns a list with all active lines
     *
     * An active control line is implicitely added by every operation
     * which adds a new gate to the circuit. This is especially useful
     * in hierarchical synthesis approaches.
     *
     * @return List with all active lines
     *
     * @since  1.1
     */
    const gate::control_container& controls() const;

    /**
     * @brief Operator implementation
     *
     * This operator adds the active controls to the gate
     * after it is added to a circuit.
     *
     * @param g Gate to be changed
     *
     * @since  1.1
     */
    void operator()( gate& g ) const;

  private:
    class priv;
    priv* const d;
  };

}

#endif /* ACTIVE_CONTROLS_HPP */

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
