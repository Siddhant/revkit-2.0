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
 * @file benchmark_table.hpp
 *
 * @brief Data-structures for producing benchmark tables
 *
 * @author Mathias Soeken
 * @since  2.0
 */

#ifndef BENCHMARK_TABLE_HPP
#define BENCHMARK_TABLE_HPP

#include <iostream>
#include <iomanip>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/irange.hpp>
#include <boost/tuple/tuple.hpp>

namespace revkit
{

/* this is to format a cell */
typedef boost::optional<unsigned> length_t;

template<typename T>
std::string print_cell( const T& value, const length_t& length = length_t() )
{
  assert( 0 );
}

template<>
std::string print_cell<>( const std::string& value, const length_t& length )
{
  if ( length )
  {
    return boost::str( boost::format( "%-" + boost::lexical_cast<std::string>( *length ) + "s" ) % value );
  }
  else
  {
    return value;
  }
}

template<>
std::string print_cell<>( const int& value, const length_t& length )
{
  if ( length )
  {
    return boost::str( boost::format( "%" + boost::lexical_cast<std::string>( *length ) + "d" ) % value );
  }
  else
  {
    return boost::lexical_cast<std::string>( value );
  }
}

template<>
std::string print_cell<>( const unsigned& value, const length_t& length )
{
  if ( length )
  {
    return boost::str( boost::format( "%" + boost::lexical_cast<std::string>( *length ) + "d" ) % value );
  }
  else
  {
    return boost::lexical_cast<std::string>( value );
  }
}

template<>
std::string print_cell<>( const double& value, const length_t& length )
{
  if ( length )
  {
    return boost::str( boost::format( "%" + boost::lexical_cast<std::string>( *length ) + ".2f" ) % value );
  }
  else
  {
    return boost::str( boost::format( "%.2f" ) % value );
  }
}

template<>
std::string print_cell<>( const boost::optional<unsigned>& value, const length_t& length )
{
  std::string timeout = "T/O";
  return value ? print_cell( *value, length ) : print_cell( timeout );
}

/* this is to display one row of the result set */
template<class TypleT, int N, typename... Arguments> struct print_row_t;
template<class TupleT, int N, class T, typename... Arguments> struct print_row_t<TupleT, N, T, Arguments...>
{
  static void print( const TupleT& t, const std::vector<unsigned>& column_lengths )
  {
    std::cout << "| " << print_cell( boost::get<N>( t ), column_lengths[N] ) << " ";
    print_row_t<TupleT, N + 1, Arguments...>::print( t, column_lengths );
  }
};
template<class TupleT, int N, class T> struct print_row_t<TupleT, N, T>
{
  static void print( const TupleT& t, const std::vector<unsigned>& column_lengths )
  {
    std::cout << "| " << print_cell( boost::get<N>( t ), column_lengths[N] ) << " |" << std::endl;
  }
};

template<class TupleT, typename... Arguments>
void print_row( const TupleT& t, const std::vector<unsigned>& column_lengths )
{
  print_row_t<TupleT, 0, Arguments...>::print( t, column_lengths );
};

/* the actual benchmark table */
template<class... Arguments>
class benchmark_table
{
public:
  typedef boost::tuple<Arguments...> benchmark;

  benchmark_table( std::initializer_list<std::string> column_names, bool verbose = false ) : verbose( verbose )
  {
    using boost::adaptors::transformed;
    boost::push_back( _column_names, column_names );
    boost::push_back( lengths, _column_names | transformed( []( const std::string& s ) { return s.size(); } ) );
  }

  template<typename T>
  void add_length( const T& value )
  {
    unsigned l = print_cell( value ).size();
    if ( l > lengths[length_i] )
    {
      lengths[length_i] = l;
    }
    ++length_i;
  }

  template<typename T>
  void compute_lengths(const T& value)
  {
    add_length( value );
  }

  template<typename U, typename... T>
  void compute_lengths(const U& head, const T&... tail)
  {
    add_length( head );
    compute_lengths(tail...);
  }

  template<class... Args>
  void add(Args&&... args)
  {
    results.push_back(boost::make_tuple(std::forward<Args>(args)...));
    length_i = 0u;
    compute_lengths( args... );

    if ( verbose )
    {
      print_row<benchmark, Arguments...>( results.back(), lengths );
    }
  }

  void print() const
  {
    for ( unsigned n : boost::irange( 0u, (unsigned)_column_names.size() ) )
    {
      std::cout.setf( std::ios::left );
      std::cout << "| " << std::setw( lengths[n] ) << _column_names[n] << " ";
    }
    std::cout << "|" << std::endl;

    for ( const auto& result : results )
    {
      print_row<benchmark, Arguments...>( result, lengths );
    }
  }

private:
  bool verbose = false;
  std::vector<std::string> _column_names;
  std::vector<benchmark> results;
  std::vector<unsigned> lengths;
  unsigned length_i;
};

}

#endif

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
