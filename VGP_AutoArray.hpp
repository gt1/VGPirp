/**
    Copyright (C) 2009-2019 German Tischler
    Copyright (C) 2011-2014 Genome Research Limited

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
#if ! defined(AUTOARRAY_HPP)
#define AUTOARRAY_HPP

#include <cstdlib>
#include <algorithm>

template<typename _type>
struct AutoArray
{
	private:
	typedef _type type;

	type * A;
	std::size_t n;
	
	
	public:
	AutoArray() : A(NULL), n(0) {}
	AutoArray(std::size_t const rn) : n(rn)
	{
		A = new type[n];
	}
	AutoArray(AutoArray<type> & O)
	: A(NULL), n(0)
	{
		*this = O;
	}
	
	std::size_t size() const
	{
		return n;
	}
	
	type & operator[](std::size_t const i)
	{
		return A[i];
	}

	type const & operator[](std::size_t const i) const
	{
		return A[i];
	}
	
	AutoArray<type> & operator=(AutoArray<type> & O)
	{
		delete [] A;
		A = NULL;
		
		A = O.A;
		n = O.n;
		
		O.A = NULL;
		O.n = 0;
		
		return *this;
	}
	
	void resize(std::size_t const rn)
	{
		type * NA = new type[rn];
		
		std::size_t const tocopy = std::min(n,rn);
		
		std::copy(A,A+tocopy,NA);
		
		delete [] A;
		
		A = NA;
		n = rn;
	}
	
	void ensureSize(std::size_t const rn)
	{
		while ( size() < rn )
			bump();
	}
	
	void bump()
	{
		if ( size() )
			resize(2*size());
		else
			resize(1);
	}
	
	type const * begin() const
	{
		return A;
	}

	type const * end() const
	{
		return A + n;
	}

	type * begin()
	{
		return A;
	}

	type * end()
	{
		return A + n;
	}
};
#endif
