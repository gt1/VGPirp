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
#if ! defined(IRPREADER_HPP)
#define IRPREADER_HPP

#include "VGP_FileType.hpp"
#include "VGP_IRPReadData.hpp"
#include "VGP_IRPReaderException.hpp"
#include "VGP_IRPResult.hpp"
#include "VGP_Provenance.hpp"
#include "VGP_MaxNumberPrint.hpp"
#include "VGP_IRPHeader.hpp"
#include "VGP_BaseValid.hpp"
#include "VGP_QValid.hpp"
#include <sstream>
#include <iostream>
#include <map>


/*
  # x number of x lines
  @ x max list length for x
  + x total length of x entries
 */
struct IRPReader : public BaseValid, public QValid
{
	std::istream & in;
	LineBuffer LB;

	IRPHeader header;

	ReadGroup RG;

	IRPReadData forward;
	IRPReadData reverse;

	IRPResult IR;

	IRPReader(std::istream & rin, std::size_t buffersize = 1, bool const haveheader = true)
	: BaseValid(), QValid(), in(rin), LB(in,buffersize), header()
	{
		if ( haveheader )
		{
			header = IRPHeader(LB);
		}
	}

	IRPResult const * readData()
	{
		char const * la = NULL;
		char const * le = NULL;

		vgp_number_type fvalid = 0;
		vgp_number_type rvalid = 0;

		forward.S_o = forward.Q_o = 0;
		reverse.S_o = reverse.Q_o = 0;
		bool qf = false;
		bool qr = false;

		bool newrg = false;

		// check for group lines
		while ( LB.getline(&la,&le) )
		{
			if ( la != le && *la == 'g' )
			{
				char const * linestart = la;

				if (
					RG.groupheader_o
					&&
					RG.numseen != RG.numreads
				)
				{
					throw IRPReaderException(std::string("IRPReader::readData: read group ") + std::string(RG.groupheader.begin(),RG.groupheader.begin() + RG.groupheader_o) + std::string(" is incomplete"));
				}

				++la;

				if ( la == le || *la != ' ' )
				{
					throw IRPReaderException(std::string("IRPReader::readData: no space after g in ") + std::string(linestart,le));
				}

				++la;

				RG.numreads = BaseOp::getNumber(la,le);

				if ( la == le || *la != ' ' )
				{
					throw IRPReaderException(std::string("IRPReader::readData: no space after num reads in ") + std::string(linestart,le));
				}

				++la;

				RG.groupheader_o = BaseOp::parseString(la,le,RG.groupheader);
				RG.groupfile_o = BaseOp::parseString(la,le,RG.groupfile);
				RG.numseen = 0;

				if ( RG.numreads )
					newrg = true;
			}
			else
			{
				LB.putback(la,le);
				break;
			}
		}

		// get stream position
		vgp_number_type const spos = LB.getPos();

		// try to get P line
		if ( LB.getline(&la,&le) )
		{
			if ( la != le && *la == 'P' )
			{
			}
			else
			{
				LB.putback(la,le);
				throw IRPReaderException(std::string("IRPReader::readData: expected P or G line, got ") + std::string(la,le));
			}
		}
		else
		{
			if (
				RG.groupheader_o
				&&
				RG.numseen != RG.numreads
			)
			{
				throw IRPReaderException(std::string("IRPReader::readData: read group ") + std::string(RG.groupheader.begin(),RG.groupheader.begin() + RG.groupheader_o) + std::string(" is incomplete"));
			}

			return NULL;
		}

		// try to get F line
		if ( LB.getline(&la,&le) )
		{
			if ( la != le && *la == 'S' )
			{
				char const * linestart = la;

				++la;

				if ( la == le || *la != ' ' )
				{
					throw IRPReaderException(std::string("IRPReader::readData: no space after F in ") + std::string(linestart,le));
				}

				++la;

				forward.S_o = BaseOp::parseString(la,le,forward.S);

				fvalid += 1;
			}
			else
			{
				LB.putback(la,le);
				throw IRPReaderException(std::string("IRPReader::readData: expected F line, got ") + std::string(la,le));
			}
		}

		if ( LB.getline(&la,&le) )
		{
			if ( la != le && *la == 'Q' )
			{
				char const * linestart = la;
				++la;

				if ( la == le || *la != ' ' )
				{
					throw IRPReaderException(std::string("IRPReader::readData: no space after Q in ") + std::string(linestart,le));
				}

				++la;

				forward.Q_o = BaseOp::parseString(la,le,forward.Q);
				qf = true;
			}
			else
			{
				LB.putback(la,le);
			}
		}

		if ( LB.getline(&la,&le) )
		{
			if ( la != le && *la == 'S' )
			{
				char const * linestart = la;

				++la;

				if ( la == le || *la != ' ' )
				{
					throw IRPReaderException(std::string("IRPReader::readData: no space after R in ") + std::string(linestart,le));
				}

				++la;

				reverse.S_o = BaseOp::parseString(la,le,reverse.S);

				rvalid += 1;
			}
			else
			{
				LB.putback(la,le);
				throw IRPReaderException(std::string("IRPReader::readData: expected R line, got ") + std::string(la,le));
			}
		}

		if ( LB.getline(&la,&le) )
		{
			if ( la != le && *la == 'Q' )
			{
				char const * linestart = la;
				++la;

				if ( la == le || *la != ' ' )
				{
					throw IRPReaderException(std::string("IRPReader::readData: no space after Q in ") + std::string(linestart,le));
				}

				++la;

				reverse.Q_o = BaseOp::parseString(la,le,reverse.Q);
				qr = true;
			}
			else
			{
				LB.putback(la,le);
			}
		}

		bool fsymvalid = true;
		for ( vgp_number_type i = 0; i < forward.S_o; ++i )
			fsymvalid = fsymvalid && bValid[forward.S[i]];

		if ( ! fsymvalid )
			throw IRPReaderException(std::string("IRPReader::readData: forward sequence is not valid ") + std::string(forward.S.begin(),forward.S.begin() + forward.S_o));

		bool rsymvalid = true;
		for ( vgp_number_type i = 0; i < reverse.S_o; ++i )
			rsymvalid = rsymvalid && bValid[reverse.S[i]];

		if ( ! rsymvalid )
			throw IRPReaderException(std::string("IRPReader::readData: reverse sequence is not valid ") + std::string(reverse.S.begin(),reverse.S.begin() + reverse.S_o));

		bool fqualvalid = true;
		for ( vgp_number_type i = 0; i < forward.Q_o; ++i )
			fqualvalid = fqualvalid && qValid[forward.Q[i]];

		if ( ! fqualvalid )
			throw IRPReaderException(std::string("IRPReader::readData: forward quality is not valid ") + std::string(forward.Q.begin(),forward.Q.begin() + forward.Q_o));

		bool rqualvalid = true;
		for ( vgp_number_type i = 0; i < reverse.Q_o; ++i )
			rqualvalid = rqualvalid && qValid[reverse.Q[i]];

		if ( ! rqualvalid )
			throw IRPReaderException(std::string("IRPReader::readData: reverse quality is not valid ") + std::string(reverse.Q.begin(),reverse.Q.begin() + reverse.Q_o));

		RG.numseen += 1;

		IR = IRPResult(
			forward.S.begin(),
			forward.S_o,
			reverse.S.begin(),
			reverse.S_o,
			qf ? forward.Q.begin() : NULL,
			qr ? reverse.Q.begin() : NULL,
			&RG,
			newrg,
			spos
		);

		return &IR;
	}
};
#endif
