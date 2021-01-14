/*
		Project:		GAKLIB
		Module:			fstream.h
		Description:	
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

#ifndef GAK_FSTREAM_H
#define GAK_FSTREAM_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <fstream>
#include <gak/string.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

enum OverwriteMode
{
	owmOverwrite,
	ovmShortDown,
	owmCopyOld,
	owmRenameOld,
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

namespace internal
{
	STRING prepareOpenFile( const STRING &originalName, OverwriteMode owmMode );
	void commit( const STRING &originalName, OverwriteMode owmMode );
	void rollback( const STRING &originalName, OverwriteMode owmMode );
}

template<class charT, class traits = std::char_traits<charT> >
class basic_ofstream : public std::basic_ofstream<charT, traits>
{
	typedef std::basic_ofstream<charT, traits>	Super;
	public:

	private:
	OverwriteMode	m_owmMode;
	STRING			m_originalName;

	public:
	basic_ofstream() : Super()
	{
	}

	basic_ofstream( 
		const STRING &fileName, 
		std::ios_base::openmode openMode = std::ios_base::out,
		OverwriteMode owmMode = owmOverwrite
	) : Super( internal::prepareOpenFile( fileName, owmMode ), std::ios_base::out|openMode ), 
		m_owmMode(owmMode), m_originalName(fileName)
	{
	}
	basic_ofstream( 
		const STRING &fileName, 
		OverwriteMode owmMode
	) : Super( internal::prepareOpenFile( fileName, owmMode ) ), 
		m_owmMode(owmMode), m_originalName(fileName)
	{
	}
	void open( 
		const STRING &fileName, 
		std::ios_base::openmode openMode = std::ios_base::out,
		OverwriteMode owmMode = owmOverwrite
	)
	{
		close();
		Super::open(
			internal::prepareOpenFile( fileName, owmMode ), std::ios_base::out|openMode
		);
		m_owmMode = owmMode;
		m_originalName = fileName;
	}
	void open( 
		const STRING &fileName, 
		OverwriteMode owmMode
	)
	{
		close();
		Super::open(
			internal::prepareOpenFile( fileName, owmMode )
		);
		m_owmMode = owmMode;
		m_originalName = fileName;
	}
	void close()
	{
		Super::close();
		if( !m_originalName.isEmpty() )
		{
			internal::commit(m_originalName, m_owmMode);
			m_originalName = NULL_STRING;
		}
	}
	void rollback()
	{
		if( !m_originalName.isEmpty() )
		{
			Super::close();
			internal::rollback(m_originalName, m_owmMode);
			m_originalName = NULL_STRING;
		}
	}
	~basic_ofstream()
	{
		rollback();
	}
};

typedef basic_ofstream<char>	ofstream;

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_FSTREAM_H
