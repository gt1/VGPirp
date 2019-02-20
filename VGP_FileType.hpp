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
#if ! defined(FILETYPE_HPP)
#define FILETYPE_HPP

#include "VGP_FileTypeException.hpp"
#include "VGP_LineBuffer.hpp"
#include "VGP_BaseOp.hpp"

struct FileType
{
	std::string filetype;
	vgp_number_type fileversion;
	vgp_number_type filesubversion;

	FileType() {}
	FileType(LineBuffer & LB) : fileversion(0), filesubversion(0)
	{
		readFileTypeLine(LB);
	}

	void readFileTypeLine(LineBuffer & LB)
	{
		char const * la = NULL;
		char const * le = NULL;

		std::size_t found = 0;

		while ( LB.getline(&la,&le) )
		{
			char const * linestart = la;

			if ( la != le && *la == '.' )
			{
				++la;

				if ( la == le || *la != ' ' )
				{
					throw FileTypeException(std::string("FileType::readFileTypeLine: no space after plus in ") + std::string(linestart,le));
				}
				la++;

				if ( la == le )
				{
					throw FileTypeException(std::string("FileType::readFileTypeLine: no line type in ") + std::string(linestart,le));
				}

				char const * typestart = la;
				while ( la != le && isalnum(*la) )
					++la;

				char const * typeend = la;

				filetype = std::string(typestart,typeend);

				if ( la == le || *la != ' ' )
				{
					throw FileTypeException(std::string("FileType::readFileTypeLine: no space after line type type in ") + std::string(linestart,le));
				}

				++la;

				fileversion = BaseOp::getNumber(la,le);

				if ( la == le || *la != '.' )
				{
					throw FileTypeException(std::string("FileType::readFileTypeLine: no dot after version in ") + std::string(linestart,le));
				}

				++la;

				filesubversion = BaseOp::getNumber(la,le);

				found += 1;
			}
			else
			{
				LB.putback(la,le);
				break;
			}
		}

		if ( found != 1 )
		{
			throw FileTypeException(std::string("FileType::readFileTypeLine: no (unique) file type line found"));
		}
	}

};

std::ostream & operator<<(std::ostream & out, FileType const & FT);
#endif
