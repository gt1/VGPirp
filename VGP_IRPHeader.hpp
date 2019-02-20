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
#if ! defined(IRPHEADER_HPP)
#define IRPHEADER_HPP

#include <map>
#include "VGP_FileType.hpp"
#include "VGP_Provenance.hpp"
#include "VGP_IRPReaderException.hpp"
#include "VGP_MaxNumberPrint.hpp"

struct IRPHeader
{
	FileType FT;
	std::vector<ProvenanceStep> VP;
	std::map<char, std::map<char,vgp_number_type > > headerMap;

	IRPHeader() {}
	IRPHeader(LineBuffer & LB)
	{
		FT = FileType(LB);
		// P = Provenance(LB);
		readHeaderLines(LB);
	}

	void readHeaderLines(LineBuffer & LB)
	{
		char const * la = NULL;
		char const * le = NULL;

		while ( LB.getline(&la,&le) )
		{
			char const * linestart = la;

			if ( la != le && !isalpha(*la) )
			{
				char const category = *(la++);

				if ( la == le || *la != ' ' )
				{
					throw IRPReaderException(std::string("IRPReader::readHeaderLines: no space after plus in ") + std::string(linestart,le));
				}
				la++;

				if ( category == '!' )
				{
					ProvenanceStep PS(la,le);
					VP.push_back(PS);
				}
				else
				{
					if ( la == le )
					{
						throw IRPReaderException(std::string("IRPReader::readHeaderLines: no line type in ") + std::string(linestart,le));
					}

					char const linetype = *la++;

					if ( la == le || *la != ' ' )
					{
						throw IRPReaderException(std::string("IRPReader::readHeaderLines: no space after line type type in ") + std::string(linestart,le));
					}

					++la;

					vgp_number_type const n = BaseOp::getNumber(la,le);

					headerMap[category][linetype] = n;
				}
			}
			else
			{
				LB.putback(la,le);
				break;
			}
		}
	}

	std::ostream & printHeader(std::ostream & out) const
	{
		out << FT;

		for (
			std::map<char, std::map<char,vgp_number_type > >::const_iterator it = headerMap.begin();
			it != headerMap.end();
			++it )
		{
			char const category = it->first;

			std::map<char,vgp_number_type > const & M = it->second;

			for ( std::map<char,vgp_number_type >::const_iterator sit = M.begin(); sit != M.end(); ++sit )
			{
				out << category << ' ' << sit->first << ' ' << MaxNumberPrint(sit->second) << '\n';
			}
		}

		for ( uint64_t i = 0; i < VP.size(); ++i )
			out << '!' << ' ' << VP[i] << "\n";

		return out;
	}
};

std::ostream & operator<<(std::ostream & out, IRPHeader const & H);
#endif
