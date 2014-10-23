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
 * @file functor.hpp
 *
 * @brief Generic Functor Implementation based on <a href="// http://www.boost.org/doc/libs/1_43_0/doc/html/function.html" target="_blank">Boost.Function</a>.
 *
 * @author Mathias Soeken
 * @since  1.0
 */

#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include <boost/function.hpp>

#include <core/properties.hpp>

namespace revkit
{

  /**
   * @brief Functor class for interfacing algorithms
   *
   * When interfacing an algorithm, we wanna encapsulate
   * the settings and the statistics. That is, a user can still
   * provide settings from outside, but another algorithm
   * can change settings of a functor as well. Therewith,
   * this class extends the boost::function object by adding
   * methods to access the corresponding settings and statistics
   * data from the respective algorithm.
   *
   * @since  1.0
   */
  template<typename T>
  class functor : public boost::function<T>
  {
  public:
    /**
     * @brief Default constructor
     *
     * Calls the constructor of the base class.
     *
     * @since  1.0
     */
    functor()
      : boost::function<T>() {}

    /**
     * @brief Copy constructor
     *
     * This copy constructor allows for example, the assignment
     * of other boost::function objects. Note, that the settings
     * and statistics are not set with this constructor, but
     * have to be assigned explicitly using init().
     *
     * @param f Object to be assigned
     *
     * @since  1.0
     */
    template<typename F>
    functor( F f ) : boost::function<T>( f ) {}

    /**
     * @brief Initializes the settings and statistics fields.
     *
     * @param settings Settings properties
     * @param statistics Statistics properties
     *
     * @since  1.0
     */
    void init( const properties::ptr& settings, const properties::ptr& statistics )
    {
      _settings = settings;
      _statistics = statistics;
    }

    /**
     * @brief Returns a smart pointer to the settings
     *
     * This smart pointer can be empty, if init() was never called.
     *
     * @return A smart pointer to the settings
     */
    const properties::ptr& settings() const
    {
      return _settings;
    }

    /**
     * @brief Returns a smart pointer to the statistics
     *
     * This smart pointer can be empty, if init() was never called.
     *
     * @return A smart pointer to the statistics
     */
    const properties::ptr& statistics() const
    {
      return _statistics;
    }

  private:
    /** @cond */
    properties::ptr _settings;
    properties::ptr _statistics;
    /** @endcond */
  };

}

#endif /* FUNCTOR_HPP */

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
