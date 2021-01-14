/*
		Project:		GAKLIB
		Module:			gridRegistry.cpp
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <vcl.h>
#include <dbgrids.hpp>
#include <registry.hpp>

#include <gak/vcl_tools.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

namespace gak
{
namespace vcl
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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

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

void saveColumSettings( const TDBGrid *theGrid, const char *registryBase )
{
	TForm		*theForm;
	STRING		valueName;
	AnsiString	gridRegistry = registryBase;

	gridRegistry += '\\';
	gridRegistry += theGrid->Owner->Name;
	gridRegistry += theGrid->Name;

	TRegistry	*reg = new TRegistry;

	if( reg->OpenKey( gridRegistry, true ) )
	{
		reg->WriteInteger( "ColumnCount", theGrid->Columns->Count );
		for( int i=0; i<theGrid->Columns->Count; i++ )
		{
			valueName = "Field#";
			valueName += formatNumber( unsigned(i) );
			reg->WriteString( (const char *)valueName,theGrid->Columns->Items[i]->FieldName );
			valueName = "Width#";
			valueName += formatNumber( unsigned(i) );
			reg->WriteInteger( (const char *)valueName,theGrid->Columns->Items[i]->Width );
		}

		if( (theForm = dynamic_cast<TForm *>(theGrid->Owner)) != NULL )
		{
			reg->WriteInteger( "Left",theForm->Left );
			reg->WriteInteger( "Top",theForm->Top );
			reg->WriteInteger( "Width",theForm->Width );
			reg->WriteInteger( "Height",theForm->Height );
		}

		reg->CloseKey();
	}

	delete reg;
}

void restoreColumSettings( const TDBGrid *theGrid, const char *registryBase )
{
	TForm		*theForm;
	STRING		valueName;
	AnsiString	gridRegistry = registryBase;
	int			ColumnCount = 0;

	gridRegistry += '\\';
	gridRegistry += theGrid->Owner->Name;
	gridRegistry += theGrid->Name;

	TRegistry	*reg = new TRegistry;

	if( reg->OpenKey( gridRegistry, false ) )
	{
		if( reg->ValueExists("ColumnCount") )
			ColumnCount = reg->ReadInteger( "ColumnCount" );
		if( ColumnCount == theGrid->Columns->Count )
		{
			for( int i=0; i<theGrid->Columns->Count; i++ )
			{
				valueName = "Field#";
				valueName += formatNumber( unsigned(i) );
				if( reg->ValueExists((const char *)valueName) )
					theGrid->Columns->Items[i]->FieldName = reg->ReadString( (const char *)valueName );
				valueName = "Width#";
				valueName += formatNumber( unsigned(i) );
				if( reg->ValueExists((const char *)valueName) )
					theGrid->Columns->Items[i]->Width = reg->ReadInteger( (const char *)valueName );
			}

			if( (theForm = dynamic_cast<TForm *>(theGrid->Owner)) != NULL )
			{
				if( reg->ValueExists( "Left" ) )
					theForm->Left = reg->ReadInteger( "Left" );
				if( reg->ValueExists( "Top" ) )
					theForm->Top = reg->ReadInteger( "Top" );
				if( reg->ValueExists( "Width" ) )
					theForm->Width = reg->ReadInteger( "Width" );
				if( reg->ValueExists( "Height" ) )
				theForm->Height = reg->ReadInteger( "Height" );
			}
		}

		reg->CloseKey();
	}

	delete reg;
}

}	// namespace vcl
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

