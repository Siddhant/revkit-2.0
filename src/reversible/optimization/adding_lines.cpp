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

#include "adding_lines.hpp"

#include <boost/dynamic_bitset.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>

#include <core/functor.hpp>
#include <core/utils/timer.hpp>

#include <reversible/target_tags.hpp>
#include <reversible/functions/add_gates.hpp>
#include <reversible/functions/add_line_to_circuit.hpp>
#include <reversible/functions/copy_circuit.hpp>
#include <reversible/functions/copy_metadata.hpp>
#include <reversible/utils/costs.hpp>

namespace revkit
{

  template<typename Iterator, typename OutputIterator>
  OutputIterator _make_factor( Iterator first, Iterator last, const boost::dynamic_bitset<>& factor, OutputIterator output )
  {
    for ( Iterator it = first; it != last; ++it )
    {
      if ( factor.test( it->index() ) )
      {
        *output++ = it->value();
      }
    }

    return output;
  }

  template<typename SinglePassRange, typename OutputIterator>
  OutputIterator _make_factor( const SinglePassRange& range, const boost::dynamic_bitset<>& factor, OutputIterator output )
  {
    return _make_factor( boost::begin( range ), boost::end( range ), factor, output );
  }

  template<typename SinglePassRange, typename OutputContainer>
  void make_factor( const SinglePassRange& range, const boost::dynamic_bitset<>& factor, OutputContainer& output )
  {
    _make_factor( range | boost::adaptors::indexed( 0u ), factor, std::insert_iterator<OutputContainer>( output, output.begin() ) );
  }

  unsigned find_suitable_gates( const circuit& base, unsigned index, const gate::control_container& factor )
  {
    using boost::adaptors::transformed;

    // last gate?
    if ( index == base.num_gates() )
    {
      return index;
    }

    // only Toffoli gates
    if ( !is_toffoli( base[index] ) )
    {
      return index;
    }

    // NOTE check for helper line?
    // has target in controls?
    auto container = factor | transformed( []( variable v ) { return v.line(); } );
    if ( boost::find( container, base[index].targets().front() ) != boost::end( container ) )
    {
      return index;
    }

    // check next gate
    return find_suitable_gates( base, index + 1, factor );
  }

  int calculate_cost_reduction( const circuit& base, unsigned start, unsigned end, const gate::control_container& factor, unsigned helper_line, const cost_function& cf )
  {
    circuit tmp;
    copy_metadata( base, tmp );

    /* original costs */
    for ( circuit::const_iterator it = base.begin() + start; it != base.begin() + end; ++it )
    {
      tmp.append_gate() = *it;
    }

    unsigned original_costs = costs( tmp, cf );

    /* modify circuit */
    for ( auto& g : tmp )
    {
      if ( !boost::includes( g.controls(), factor ) ) continue;

      g.add_control( make_var( helper_line ) );
      for ( const auto& v : factor )
      {
        g.remove_control( v );
      }
    }
    prepend_toffoli( tmp, factor, helper_line );
    append_toffoli( tmp, factor, helper_line );

    unsigned new_costs = costs( tmp, cf );

    return original_costs - new_costs;
  }

  bool adding_lines( circuit& circ, const circuit& base, properties::ptr settings, properties::ptr statistics )
  {

    // Settings parsing
    unsigned additional_lines = get<unsigned>( settings, "additional_lines", 1 );
    cost_function cf = get<cost_function>( settings, "cost_function", costs_by_gate_func( transistor_costs() ) );

    // Run-time measuring
    timer<properties_timer> t;

    if ( statistics )
    {
      properties_timer rt( statistics );
      t.start( rt );
    }

    // copy circuit
    copy_circuit( base, circ );

    for ( unsigned h = 0u; h < additional_lines; ++h )
    {
      /* add one helper line */
      unsigned helper_line = add_line_to_circuit( circ, "helper", "helper", false, true );

      /* last inserted helper gate (to be removed in the end) */
      unsigned last_helper_gate_index = 0u;

      unsigned current_index = 0u;
      while ( current_index < circ.num_gates() )
      {
        /* best factor */
        unsigned best_cost_reduction = 0u;
        unsigned best_factor = 0u;
        unsigned best_j = 0u;

        /* control lines of the current gate */
        const gate& current_gate = circ[current_index];

        /* generate each factor */
        for ( unsigned F : boost::irange( 1u, 1u << current_gate.controls().size() ) )
        {
          boost::dynamic_bitset<> factor( current_gate.controls().size(), F );
          if ( factor.count() <= 1u ) continue; /* only consider factors of size > 1 */

          /* create factor */
          gate::control_container factored;
          make_factor( current_gate.controls(), factor, factored );

          /* determine upper bound */
          unsigned j = find_suitable_gates( circ, current_index, factored );

          /* calculate cost reduction */
          int cost_reduction = calculate_cost_reduction( circ, current_index, j, factored, helper_line, cf );

          /* better? */
          if ( cost_reduction > (int)best_cost_reduction )
          {
            best_cost_reduction = (unsigned)cost_reduction; /* in this case cost_reduction is greater than 0 */
            best_factor = F;
            best_j = j;
          }
        }

        /* was a factor found? */
        if ( best_factor != 0u )
        {
          gate::control_container factored;
          make_factor( current_gate.controls(), boost::dynamic_bitset<>( current_gate.controls().size(), best_factor), factored );

          /* apply factor */
          for ( unsigned i : boost::irange( current_index, best_j ) )
          {
            if ( !boost::includes( circ[i].controls(), factored ) ) continue;

            circ[i].add_control( make_var( helper_line ) );
            for ( const auto& control : factored )
            {
              circ[i].remove_control( control );
            }
          }

          /* toffoli gate at the beginning */
          insert_toffoli( circ, current_index, factored, helper_line );

          /* update best_j, since we inserted a gate before */
          ++best_j;

          /* toffoli gate at the end */
          insert_toffoli( circ, best_j, factored, helper_line );
          last_helper_gate_index = best_j;

          /* update again */
          ++best_j;

          /* update current_index */
          current_index = best_j;
        }
        else
        {
          /* check next gate */
          ++current_index;
        }
      }

      /* remove last helper gate if added */
      if ( last_helper_gate_index != 0u )
      {
        circ.remove_gate_at( last_helper_gate_index );
      }
    }

    return true;
  }

  optimization_func adding_lines_func( properties::ptr settings, properties::ptr statistics )
  {
    optimization_func f = [&settings, &statistics]( circuit& circ, const circuit& base ) {
      return adding_lines( circ, base, settings, statistics );
    };
    f.init( settings, statistics );
    return f;
  }

}

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
