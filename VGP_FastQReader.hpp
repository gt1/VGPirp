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
#if ! defined(FASTQREADER_HPP)
#define FASTQREADER_HPP

#include "VGP_FileType.hpp"
#include "VGP_IRPReadData.hpp"
#include "VGP_IRPReaderException.hpp"
#include "VGP_IRPResult.hpp"
#include "VGP_Provenance.hpp"
#include "VGP_MaxNumberPrint.hpp"
#include "VGP_IRPHeader.hpp"
#include "VGP_BaseValid.hpp"
#include "VGP_QValid.hpp"
#include "VGP_FastQReaderException.hpp"
#include "VGP_FastQResult.hpp"
#include <sstream>
#include <iostream>
#include <map>

#include <fstream>

/*
  # x number of x lines
  @ x max list length for x
  + x total length of x entries
 */
struct FastQReader : public BaseValid, public QValid
{
	std::istream * pin0;
	std::istream * pin1;
	std::istream & in0;
	std::istream & in1;

	LineBuffer * pLB0;
	LineBuffer * pLB1;

	LineBuffer & LB0;
	LineBuffer & LB1;

	IRPReadData forward;
	IRPReadData reverse;

	FastQResult FR;

	FastQReader(::std::istream & rin, std::size_t buffersize = 1)
	:
		BaseValid(), QValid(),
		pin0(NULL),
		pin1(NULL),
		in0(rin),
		in1(rin),
		pLB0(new LineBuffer(in0,buffersize)),
		pLB1(NULL),
		LB0(*pLB0), LB1(*pLB0)
	{
	}

	FastQReader(::std::istream & rin0, ::std::istream & rin1, std::size_t buffersize = 1)
	:
		BaseValid(), QValid(),
		pin0(NULL),
		pin1(NULL),
		in0(rin0),
		in1(rin1),
		pLB0(new LineBuffer(in0,buffersize)),
		pLB1(new LineBuffer(in1,buffersize)),
		LB0(*pLB0), LB1(*pLB1)
	{
	}

	FastQReader(std::string const & rfn, std::size_t buffersize = 1)
	:
		BaseValid(), QValid(),
		pin0(new std::ifstream(rfn.c_str(),std::ios::in)),
		pin1(NULL),
		in0(*pin0),
		in1(*pin0),
		pLB0(new LineBuffer(in0,buffersize)),
		pLB1(NULL),
		LB0(*pLB0), LB1(*pLB0)
	{
	}

	~FastQReader()
	{
		delete pLB1;
		delete pLB0;
		delete pin1;
		delete pin0;
	}

	void getRead(LineBuffer & LB, IRPReadData & data)
	{
		char const * la = NULL;
		char const * le = NULL;
		std::size_t lseq = 0;
		std::size_t nseq = 0;
		std::size_t lq = 0;
		// std::size_t nq = 0;

		// read name
		if ( LB.getline(&la,&le) )
		{
			char const * linestart = la;

			if ( la != le && *la == '@' )
			{
				// skip @
				++la;

				data.N.ensureSize(le-la);
				std::copy(la,le,data.N.begin());
				data.N_o = le-la;
			}
			else
			{
				LB.putback(la,le);
				throw FastQReaderException(std::string("FastQReader::readData: no @ in ") + std::string(linestart,le));
			}
		}
		else
		{
			throw FastQReaderException(std::string("FastQReader::readData: no @ found"));
		}

		while ( LB.getline(&la,&le) )
		{
			if ( le-la && la[0] == '+' )
			{
				LB.putback(la,le);
				break;
			}
			else
			{
				std::size_t const ladd = le-la;

				data.S.ensureSize(lseq + ladd);

				std::copy(la,le,data.S.begin() + lseq);

				lseq += ladd;
				nseq += 1;
			}
		}

		if ( nseq )
		{
			data.S_o = lseq;
		}
		else
		{
			throw FastQReaderException(std::string("FastQReader::readData: no sequence data found"));
		}

		#if 0
		// sequence data
		if ( LB.getline(&la,&le) )
		{
			data.S.ensureSize(le-la);
			std::copy(la,le,data.S.begin());
			data.S_o = le-la;
		}
		else
		{
			throw FastQReaderException(std::string("FastQReader::readData: no sequence data found"));
		}
		#endif

		// plus line
		if ( LB.getline(&la,&le) )
		{
			char const * linestart = la;

			if ( la != le && *la == '+' )
			{
				// skip +
				++la;
			}
			else
			{
				LB.putback(la,le);
				throw FastQReaderException(std::string("FastQReader::readData: no plus at start of ") + std::string(linestart,le));
			}
		}
		else
		{
			throw FastQReaderException(std::string("FastQReader::readData: no plus line found"));
		}

		while ( lq < lseq )
		{
			// quality data
			if ( LB.getline(&la,&le) )
			{
				size_t const qadd = le-la;

				data.Q.ensureSize(lq + qadd);
				std::copy(la,le,data.Q.begin()+lq);

				lq += qadd;
			}
			else
			{
				throw FastQReaderException(std::string("FastQReader::readData: insufficient quality data found"));
			}
		}

		data.Q_o = lq;

		if ( lq != lseq )
		{
			throw FastQReaderException(std::string("FastQReader::readData: inconsistent quality data found"));
		}

		for ( std::size_t i = 0; i < data.S_o; ++i )
			if ( ! bValid[data.S[i]] )
			{
				throw FastQReaderException(std::string("FastQReader::readData: invalid query string ") + std::string(data.S.begin(),data.S.begin()+data.S_o));
			}
		for ( std::size_t i = 0; i < data.Q_o; ++i )
			if ( ! qValid[data.Q[i]] )
			{
				throw FastQReaderException(std::string("FastQReader::readData: invalid quality string ") + std::string(data.Q.begin(),data.Q.begin()+data.Q_o));
			}
		if ( data.S_o != data.Q_o )
		{
			throw FastQReaderException(std::string("FastQReader::readData: sequence length does not match quality length"));
		}
	}

	FastQResult const * readData()
	{
		char const * la = NULL;
		char const * le = NULL;

		//vgp_number_type fvalid = 0;
		//vgp_number_type rvalid = 0;

		forward.S_o = forward.Q_o = forward.N_o = 0;
		reverse.S_o = reverse.Q_o = reverse.N_o = 0;

		// do we have a next line?
		if ( LB0.getline(&la,&le) )
		{
			LB0.putback(la,le);
		}
		else
		{
			return NULL;
		}

		// get stream position
		vgp_number_type const spos = LB0.getPos();

		getRead(LB0,forward);
		getRead(LB1,reverse);

		FR = FastQResult(
			forward.N.begin(),
			forward.N_o,
			forward.S.begin(),
			forward.S_o,
			forward.Q.begin(),
			reverse.N.begin(),
			reverse.N_o,
			reverse.S.begin(),
			reverse.S_o,
			reverse.Q.begin(),
			spos
		);

		return &FR;
	}
};
#endif
