/**
    Copyright (C) 2009-2019 German Tischler

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
#if ! defined(BASEOP_HPP)
#define BASEOP_HPP

#include "VGP_BaseOpException.hpp"
#include "VGP_AutoArray.hpp"
#include "VGP_NumberType.hpp"

struct BaseOp
{
	static vgp_number_type getNumber(char const * & pc, char const * pe)
	{
		char const * start = pc;

		vgp_number_type n = 0;
		unsigned int nc = 0;
		while ( (pc != pe) && isdigit(*pc) )
		{
			n *= 10;
			nc += 1;
			n += *(pc++)-'0';
		}

		if ( !nc )
		{
			throw BaseOpException(std::string("BaseOp::getNumber: Did not find expected number in ") + std::string(start,pe));
		}

		return n;
	}

	static std::string parseString(char const * & pc, char const * pe)
	{
		char const * start = pc;

		vgp_number_type const n = getNumber(pc,pe);

		if ( pc == pe || *pc != ' ' )
		{
			throw BaseOpException("BaseOp::parseString: No space found after string length");
		}

		pc += 1;

		if ( pe - pc < static_cast< ::std::ptrdiff_t >(n) )
		{
			throw BaseOpException(std::string("BaseOp: Insufficient string data in ") + std::string(start,pe));
		}

		std::string const s(pc,pc+n);

		pc += n;

		if ( pc != pe && *pc == ' ' )
			pc += 1;

		return s;
	}

	static vgp_number_type parseString(char const * & pc, char const * pe, AutoArray<char> & A)
	{
		char const * start = pc;

		vgp_number_type const n = getNumber(pc,pe);

		if ( pc == pe || *pc != ' ' )
		{
			throw BaseOpException("BaseOp::parseString: No space found after string length");
		}

		pc += 1;

		if ( pe - pc < static_cast< ::std::ptrdiff_t >(n) )
		{
			throw BaseOpException(std::string("BaseOp: Insufficient string data in ") + std::string(start,pe));
		}

		A.ensureSize(n);

		std::copy(pc,pc+n,A.begin());

		pc += n;

		if ( pc != pe && *pc == ' ' )
			pc += 1;

		return n;
	}

	static std::ostream & printString(std::ostream & out, std::string const & s)
	{
		out << s.size() << " " << s;
		return out;
	}

	static std::ostream & printNumberMaxSize(std::ostream & out, vgp_number_type n)
	{
		unsigned int const numlen = 20; /* >= ceil(log_10(2^64-1)) */
		unsigned int i = 0;
		char A[numlen];
		char * pa = &A[0];
		char * pc = pa;
		char * pe = &A[numlen];

		if ( ! n )
		{
			i += 1;
			out << '0';
		}
		else
		{
			while ( n && pc != pe )
			{
				*pc++ = (n % 10);
				n /= 10;
			}
			if ( n )
			{
				throw BaseOpException("BaseOp::printNumberMaxSize: number does not fit into fixed size format");
			}

			while ( pc != pa )
			{
				--pc;
				out.put('0' + *pc);
				i += 1;
			}
		}

		while ( i < numlen )
		{
			out << ' ';
			i += 1;
		}

		return out;
	}
};
#endif
