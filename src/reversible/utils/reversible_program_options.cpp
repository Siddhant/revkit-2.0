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

#include "reversible_program_options.hpp"

#include "costs.hpp"

namespace revkit
{

  class reversible_program_options::priv
  {
  public:
    std::string in_realization;
    std::string in_specification;
    std::string out_realization;
    unsigned costs = 0u;

    bool has_in_realization = false;
    bool has_in_specification = false;
    bool has_out_realization = false;
    bool has_costs = false;
  };

  reversible_program_options::reversible_program_options( unsigned line_length )
    : program_options( line_length ),
      d( new priv() )
  {
  }

  reversible_program_options::reversible_program_options( const std::string& caption, unsigned line_length )
    : program_options( caption, line_length ),
      d( new priv() )
  {
  }

  reversible_program_options::~reversible_program_options()
  {
    delete d;
  }

  bool reversible_program_options::good() const
  {
    return ( program_options::good() && ( !d->has_in_realization || is_set( "filename" ) ) && ( !d->has_in_specification || is_set( "filename" ) ) );
  }

  reversible_program_options& reversible_program_options::add_read_realization_option()
  {
    assert( !( d->has_in_realization || d->has_in_specification ) );
    add_options()( "filename", boost::program_options::value<std::string>( &d->in_realization ), "circuit realization in RevLib *.real format" );
    d->has_in_realization = true;

    return *this;
  }

  reversible_program_options& reversible_program_options::add_read_specification_option()
  {
    assert( !( d->has_in_realization || d->has_in_specification ) );
    add_options()( "filename", boost::program_options::value<std::string>( &d->in_specification ), "circuit specification in RevLib *.spec format" );
    d->has_in_specification = true;

    return *this;
  }

  reversible_program_options& reversible_program_options::add_write_realization_option()
  {
    add_options()( "realname", boost::program_options::value<std::string>( &d->out_realization ), "output circuit realization in RevLib *.real format" );
    d->has_out_realization = true;

    return *this;
  }

  reversible_program_options& reversible_program_options::add_costs_option()
  {
    add_options()( "costs", boost::program_options::value<unsigned>( &d->costs )->default_value( 0 ), "0: Gate Costs\n1: Line Costs\n2: Transistor Costs" );
    d->has_costs = true;

    return *this;
  }

  const std::string& reversible_program_options::read_realization_filename() const
  {
    return d->in_realization;
  }

  const std::string& reversible_program_options::read_specification_filename() const
  {
    return d->in_specification;
  }

  const std::string& reversible_program_options::write_realization_filename() const
  {
    return d->out_realization;
  }

  cost_function reversible_program_options::costs() const
  {
    assert( d->has_costs );

    switch ( d->costs )
    {
    case 0:
      return costs_by_circuit_func( gate_costs() );
    case 1:
      return costs_by_circuit_func( line_costs() );
    case 2:
      return costs_by_gate_func( transistor_costs() );
    default:
      assert( false );
      return cost_function();
    }
  }

  bool reversible_program_options::is_write_realization_filename_set() const
  {
    if ( !parsed() || !d->has_out_realization )
    {
      return false;
    }

    return is_set( "realname" );
  }
}

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
