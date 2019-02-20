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
#if ! defined(LINEBUFFER_HPP)
#define LINEBUFFER_HPP

#include "VGP_AutoArray.hpp"
#include "VGP_LineBufferException.hpp"

#include <istream>
#include <ostream>
#include <algorithm>
#include <cassert>
#include <cstring>

struct LineBuffer
{
	private:
	// input stream
	std::istream & file;

	// buffer
	AutoArray<char> buffer;
	// size of buffer
	std::size_t bufsize;
	// set to true when end of file has been observed
	bool eof;

	// start of buffer pointer
	char * bufferptra;
	// end of input data
	char * bufferptrin;
	// current read pointer
	char * bufferptrout;
	// end of buffer pointer
	char * bufferptre;

	std::size_t prevlinelen;
	std::size_t spos;

	std::streamsize read(char * p, size_t n)
	{
		file.read(p,n);

		if ( file.eof() )
			eof = true;

		if ( file.gcount() )
		{
			return file.gcount();
		}
		else if ( eof )
		{
			return 0;
		}
		else
		{
			LineBufferException LBE("LineBuffer: input error");
			throw LBE;
		}
	}

	public:
	/**
	 * construct line buffer from file and optional initial
	 * buffer size (buffer is automatically extended to the
	 * maximal length of a line in the file)
	 *
	 * @param rfile input stream
	 * @param initsize initial buffer size
	 **/
	LineBuffer(std::istream & rfile, std::size_t const initsize = 1)
	:
		file(rfile),
		buffer(std::max(static_cast<std::size_t>(1),initsize)),
		bufsize(initsize),
		eof(false),
		bufferptra(buffer.begin()),
		bufferptrin(0),
		bufferptrout(buffer.begin()),
		bufferptre(buffer.end()),
		prevlinelen(0),
		spos(0)
	{
		bufferptrin = bufferptra + read(bufferptra,bufsize);
	}

	std::size_t getPos() const
	{
		return spos;
	}

	/**
	 * return the next line. If successful the method returns true and the start and end pointers
	 * of the line are stored through the pointer a and e respectively.
	 *
	 * @param a pointer to line start pointer
	 * @param e pointer to line end pointer
	 * @return true if a line could be extracted, false otherwise
	 **/
	bool getline(
		char const ** a,
		char const ** e
	)
	{
		while ( 1 )
		{
			/* end of line pointer */
			char * lineend = bufferptrout;

			/* search for end of buffer or line end */
			while ( lineend != bufferptrin && *(lineend) != '\n' )
				++lineend;

			/* we reached the end of the data currently in memory */
			if ( lineend == bufferptrin )
			{
				/* reached end of file, return what we have */
				if ( eof )
				{
					/* this is the last line we will return */
					if ( bufferptrout != bufferptrin )
					{
						/* if file ends with a newline */
						if ( bufferptrin[-1] == '\n' )
						{
							*a = bufferptrout;
							*e = bufferptrin-1;
							bufferptrout = bufferptrin;

							std::size_t const linelen = (*e-*a)+1;
							prevlinelen = linelen;
							spos += linelen;

							return true;
						}
						/* otherwise we append an artifical newline */
						else
						{
							std::size_t const numbytes = lineend - bufferptrout;
							AutoArray<char> tmpbuf(numbytes+1);

							memcpy(tmpbuf.begin(),bufferptrout,numbytes);
							tmpbuf[numbytes] = '\n';

							buffer = tmpbuf;
							bufsize = numbytes+1;
							bufferptra = buffer.begin();
							bufferptre = buffer.begin() + this->bufsize;
							bufferptrin = bufferptre;
							bufferptrout = bufferptre;

							*a = bufferptra;
							*e = bufferptre - 1;

							std::size_t const linelen = *e-*a;
							prevlinelen = linelen;
							spos += linelen;

							return true;
						}
					}
					else
					{
						return false;
					}
				}
				/* we need to read more data */
				else
				{
					/* do we need to extend the buffer? */
					if (
						(bufferptrout == bufferptra)
						&&
						(bufferptrin == bufferptre)
					)
					{
						std::size_t const newbufsize = bufsize ? (2*bufsize) : 1;
						buffer.resize(newbufsize);

						bufferptre   = buffer.end();
						bufferptrout = buffer.begin() + (bufferptrout - bufferptra);
						bufferptrin  = buffer.begin() + (bufferptrin - bufferptra);
						bufferptra   = buffer.begin();
						bufsize      = buffer.size();
					}
					else
					{
						/* move data to front and fill rest of buffer */
						std::size_t const used   = bufferptrin  - bufferptrout;
						std::size_t const unused = bufsize - used;

						memmove(bufferptra,bufferptrout,used);

						bufferptrout = bufferptra;
						bufferptrin  = bufferptrout + used;

						bufferptrin += read(bufferptrin,unused);
					}
				}
			}
			else
			{
				*a = bufferptrout;
				*e = lineend;
				assert ( *lineend == '\n' );
				bufferptrout = lineend+1;

				std::size_t const linelen = (*e-*a)+1;
				prevlinelen = linelen;
				spos += linelen;

				return true;
			}
		}

		return false;
	}

	/**
	 * put back line starting at a. The line start pointer must have been returned by
	 * the most recent call of getline and this getline call must have returned the value true.
	 * Otherwise the behaviour is undefined.
	 *
	 * @param a line start pointer
	 **/
	void putback(char const * a, char const * /* e */)
	{
		spos -= prevlinelen;
		bufferptrout = const_cast<char *>(a);
		prevlinelen = 0;
	}
};
#endif
