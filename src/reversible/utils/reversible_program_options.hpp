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
 * @file reversible_program_options.hpp
 *
 * @brief Easier access to program options
 *
 * @author Mathias Soeken
 * @since  1.0
 */

#ifndef REVERSIBLE_PROGRAM_OPTIONS_HPP
#define REVERSIBLE_PROGRAM_OPTIONS_HPP

#include <core/utils/program_options.hpp>

#include <reversible/utils/costs.hpp>

namespace revkit
{

  /**
   * @brief Class for program options on top of the Boost.Program_Options library
   *
   * This class can be used when writing programs for accessing algorithms.
   * It parses from C argc and argv variables and has some functions for
   * adding program options for common used parameters like input realization or
   * specification filename and output realization filename.
   *
   * @note It can be useful to check the <a href="http://www.boost.org/doc/libs/1_41_0/doc/html/program_options.html">Boost.Program_Options</a>
   *       documentation for further information.
   */
  class reversible_program_options : public program_options
  {
  public:
    /**
     * @brief Default constructor
     *
     * Calls the constructor of the boost::program_options::options_description
     * base class and adds a --help option.
     *
     * @param line_length Length of the terminal where to output
     *
     * @since  1.0
     */
    explicit reversible_program_options( unsigned line_length = m_default_line_length );

    /**
     * @brief Constructor with setting a caption for usage output
     *
     * Calls the constructor of the boost::program_options::options_description
     * base class and adds a --help option.
     *
     * @param caption     A caption  is primarily useful for output
     * @param line_length Length of the terminal where to output
     *
     * @since  1.0
     */
    explicit reversible_program_options( const std::string& caption, unsigned line_length = m_default_line_length );

    /**
     * @brief Default deconstructor
     */
    virtual ~reversible_program_options();

    /**
     * @brief Is help needed? Are all properties set properly?
     *
     * This method returns true when the --help option is not set
     * and when the --filename option is set, as far as either
     * add_read_realization_option() or add_read_specification_option()
     * was called before.
     *
     * @return true, when all properties are set properly. Otherwise false.
     *
     * @since  1.0
     */
    bool good() const;

    /**
     * @brief Adds an option for an input as RevLib realization
     *
     * This method adds an option called --filename which takes
     * a RevLib realization (*.real) file as argument.
     *
     * After calling this function, add_read_specification_option()
     * cannot be called anymore.
     *
     * @return The program_options object itself for repeatedly function calls
     *
     * @since  1.0
     */
    reversible_program_options& add_read_realization_option();

    /**
     * @brief Adds an option for an input as RevLib specification
     *
     * This method adds an option called --filename which takes
     * a RevLib specification (*.spec) file as argument.
     *
     * After calling this function, add_read_realization_option()
     * cannot be called anymore.
     *
     * @return The program_options object itself for repeatedly function calls
     *
     * @since  1.0
     */
    reversible_program_options& add_read_specification_option();

    /**
     * @brief Adds an option for an output as RevLib realization
     *
     * This method adds an option called --realname which takes
     * a RevLib realization (*.real) file as argument.
     *
     * To check whether this option was set or not, use
     * is_write_realization_filename_set().
     *
     * @return The program_options object itself for repeatedly function calls
     *
     * @since  1.0
     */
    reversible_program_options& add_write_realization_option();

    /**
     * @brief Adds an option for selecting a cost function
     *
     * This method adds an option called --costs which takes
     * an integer value as argument representing a cost function,
     * whereby 0 is gate costs, 1 is line costs, 2 is quantum costs,
     * and 3 is transistor costs, respectively.
     *
     * @return The program_options object itself for repeatedly function calls
     *
     * @since  1.0
     */
    reversible_program_options& add_costs_option();

    /**
     * @brief Returns the RevLib realization input if it was set
     *
     * This method can just be called after the option
     * was added with add_read_realization_option() and
     * good() evaluated to true.
     *
     * @return The filename set via the command line
     *
     * @since  1.0
     */
    const std::string& read_realization_filename() const;

    /**
     * @brief Returns the RevLib specification input if it was set
     *
     * This method can just be called after the option
     * was added with add_read_specification_option() and
     * good() evaluated to true.
     *
     * @return The filename set via the command line
     *
     * @since  1.0
     */
    const std::string& read_specification_filename() const;

    /**
     * @brief Returns the RevLib realization output if it was set
     *
     * This method can just be called after the option
     * was added with add_write_realization_option() and
     * is_write_realization_filename_set() evaluated to true.
     *
     * @return The filename set via the command line
     *
     * @since  1.0
     */
    const std::string& write_realization_filename() const;

    /**
     * @brief Checks whether a filename for RevLib realization output was set
     *
     * This method evaluates to true, when add_write_realization_option was
     * called before and --realname was set via the command line.
     *
     * @return True, if a realname was specified, otherwise false.
     *
     * @since  1.0
     */
    bool is_write_realization_filename_set() const;

    /**
     * @brief Returns a cost function
     *
     * Use this method together with add_costs_option() only.
     * When adding that method, costs are ensured to be selected (e.g. the default ones).
     * Then this method can create a corresponding cost function.
     *
     * @return A cost function depending on the costs option
     *
     * @since  1.0
     */
    cost_function costs() const;

  private:
    class priv;
    priv* const d;
  };

}

#endif

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
