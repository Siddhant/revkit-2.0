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
 * @author Mathias Soeken
 * @author Oliver Keszöcze
 * @since  2.0
 */

#include <iostream>

#if ADDON_FORMAL

#include <reversible/truth_table.hpp>
#include <reversible/functions/circuit_to_truth_table.hpp>
#include <reversible/io/print_circuit.hpp>
#include <reversible/io/print_statistics.hpp>
#include <reversible/io/read_specification.hpp>
#include <reversible/io/write_realization.hpp>
#include <reversible/simulation/simple_simulation.hpp>
#include <reversible/synthesis/exact_synthesis.hpp>
#include <reversible/utils/reversible_program_options.hpp>

using namespace revkit;

int main( int argc, char ** argv )
{
  unsigned max_depth = 20u;

  reversible_program_options opts;
  opts.add_read_specification_option();
  opts.add_write_realization_option();
  opts.add_options()
    ( "max_depth",           value_with_default( &max_depth ), "Maximum search depth" )
    ( "print_circuit,c",                                       "Prints the circuit" )
    ( "print_truth_table,t",                                   "Prints the truth table of the circuit" )
    ( "negative,n",                                            "Allow negative control lines" )
    ( "multiple,m",                                            "Allow multiple target lines" )
    ( "verbose,v",                                             "Be verbose" )
    ;

  opts.parse( argc, argv );

  if ( !opts.good() )
  {
    std::cout << opts << std::endl;
    return 1;
  }

  binary_truth_table spec;
  read_specification( spec, opts.read_specification_filename() );

  circuit circ;
  properties::ptr settings( new properties );
  settings->set( "max_depth", max_depth );
  settings->set( "negative", opts.is_set( "negative" ) );
  settings->set( "multiple", opts.is_set( "multiple" ) );
  settings->set( "verbose", opts.is_set( "verbose" ) );
  properties::ptr statistics( new properties );

  exact_synthesis( circ, spec, settings, statistics );

  if ( opts.is_set( "print_circuit" ) )
  {
    std::cout << circ << std::endl;
  }

  if ( opts.is_set( "print_truth_table" ) )
  {
    binary_truth_table spec2;
    circuit_to_truth_table( circ, spec2, simple_simulation_func() );
    std::cout << spec2 << std::endl;
  }

  if ( opts.is_write_realization_filename_set() )
  {
    write_realization( circ, opts.write_realization_filename() );
  }

  print_statistics( circ, statistics->get<double>( "runtime" ) );

  return 0;
}

#else

int main( int argc, char ** argv )
{
  std::cout << "[E] Addon `formal' is not installed." << std::endl;
  return 1;
}

#endif

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
