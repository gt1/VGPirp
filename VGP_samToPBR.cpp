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
#include "VGP_SAMReader.hpp"
#include "VGP_BRPHeader.hpp"
#include "VGP_GroupLineTools.hpp"
#include "VGP_HeaderTools.hpp"
#include <fstream>

int main(int argc, char * argv[])
{
	try
	{
		if ( argc < 2 )
		{
			std::cerr << "usage: " << argv[0] << " <out.brp> < in.sam\n";
			return EXIT_FAILURE;
		}

		BRPHeader header;

		header.FT.filetype = "seq";
		header.FT.fileversion = 1;
		header.FT.filesubversion = 0;
		
		header.FT.subfiletype = "pbr";

		std::ostringstream comostr;
		for ( int i = 0; i < argc; ++i )
		{
			if ( i )
				comostr << ' ';
			comostr << HeaderTools::quote(argv[i]);
		}
		std::string const com = comostr.str();

		header.VP.push_back(
			ProvenanceStep(
				argv[0],
				"0.0",
				com,
				"now"
			)
		);

		header.headerMap['#']['L'] = 0;
		header.headerMap['#']['S'] = 0;
		header.headerMap['#']['A'] = 0;
		header.headerMap['#']['N'] = 0;
		header.headerMap['@']['S'] = 0;
		header.headerMap['@']['A'] = 0;
		header.headerMap['+']['S'] = 0;
		header.headerMap['+']['A'] = 0;
		header.headerMap['#']['!'] = 1;

		#if 0
		header.headerMap['#']['P'] = 0;
		header.headerMap['#']['S'] = 0;
		header.headerMap['@']['S'] = 0;
		header.headerMap['+']['S'] = 0;
		#endif

		std::fstream out(argv[1], std::ios::in | std::ios::out | std::ios::trunc);

		out << header;

		// GroupLineTools::getGroupLineMax();

		SAMReader SR(std::cin);

		vgp_number_type numL = 0;
		vgp_number_type numS = 0;
		vgp_number_type numA = 0;
		vgp_number_type numN = 0;
		vgp_number_type maxS = 0;
		vgp_number_type maxA = 0;
		vgp_number_type sumS = 0;
		vgp_number_type sumA = 0;

		PBRRecord * R = NULL;
		while ( (R=SR.readData()) )
		{
			out
				<< 'L'
				<< ' ' << R->well
				<< ' ' << R->firstpulse
				<< ' ' << R->lastpulse
				<< ' ' << R->score
				<< '\n';
			numL += 1;
			out << 'S'
				<< ' ' << R->sequence_o
				<< ' '
				;
			out.write(
				R->sequence,
				R->sequence_o
			);
			out.put('\n');
			numS += 1;
			maxS = std::max(maxS,static_cast<vgp_number_type>(R->sequence_o));
			sumS += R->sequence_o;

			if ( R->N_o )
			{
				out << 'N' << ' ';
				out.write(
					R->N,
					R->N_o
				);

				out.put('\n');

				numN += 1;
			}

			if ( R->PW )
			{
				out << 'A' << ' ' << R->PW_o << ' ';
				out.write(
					R->PW,
					R->PW_o
				);
				out << '\n';

				numA += 1;
				sumA += R->PW_o;
				maxA = std::max(maxA,static_cast<vgp_number_type>(R->PW_o));
			}

			// qs,qr,rq
		}

		header.headerMap['#']['L'] = numL;
		header.headerMap['#']['S'] = numS;
		header.headerMap['#']['N'] = numA;
		header.headerMap['#']['A'] = numN;
		header.headerMap['@']['S'] = maxS;
		header.headerMap['@']['A'] = maxA;
		header.headerMap['+']['S'] = sumS;
		header.headerMap['+']['A'] = sumA;

		out.seekp(0);
		out << header;

		out.flush();
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}
