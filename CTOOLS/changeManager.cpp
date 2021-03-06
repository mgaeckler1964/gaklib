/*
		Project:		GAKLIB
		Module:			changeManager.cpp
		Description:	
		Author:			Martin G�ckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Germany, Munich ``AS IS''
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

#include <gak/ChangeManager.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
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

DocumentViewer::~DocumentViewer()
{
	doEnterFunction("DocumentViewer::~DocumentViewer");
	manager->unregisterViewer( this );
}

ChangeManager::~ChangeManager()
{
	doEnterFunction("~ChangeManager");
}

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

void ChangeManager::unregisterViewer( DocumentViewer *delViewer )
{
	doEnterFunction("ChangeManager::unregisterViewer");
	for(
		DocumentViewers::iterator it =  m_theViewer.begin(), endIT = m_theViewer.end();
		it != endIT;
		++it
	)
	{
		if( *it == delViewer )
		{
			m_theViewer.erase( it );
			break;
		}
	}
}

void ChangeManager::setPosition( DocumentViewer *source, void *position )
{
	for(
		DocumentViewers::iterator it =  m_theViewer.begin(), endIT = m_theViewer.end();
		it != endIT;
		++it
	)
	{
		DocumentViewer *theViewer = *it;
		if( theViewer != source )
		{
			theViewer->handlePositionChange( m_theDocument, position );
		}
	}
}
void ChangeManager::setChanged( DocumentViewer *source, void *item )
{
	m_changed = true;

	for(
		DocumentViewers::iterator it =  m_theViewer.begin(), endIT = m_theViewer.end();
		it != endIT;
		++it
	)
	{
		DocumentViewer *theViewer = *it;
		if( theViewer != source )
		{
			theViewer->handleChange( m_theDocument, item );
		}
	}
}
void ChangeManager::setNew( DocumentViewer *source, void *item )
{
	m_changed = true;

	for(
		DocumentViewers::iterator it =  m_theViewer.begin(), endIT = m_theViewer.end();
		it != endIT;
		++it
	)
	{
		DocumentViewer *theViewer = *it;
		if( theViewer != source )
		{
			theViewer->handleNew( m_theDocument, item );
		}
	}
}
void ChangeManager::setDelete( DocumentViewer *source, void *item )
{
	m_changed = true;

	for(
		DocumentViewers::iterator it =  m_theViewer.begin(), endIT = m_theViewer.end();
		it != endIT;
		++it
	)
	{
		DocumentViewer *theViewer = *it;
		if( theViewer != source )
		{
			theViewer->handleDelete( m_theDocument, item );
		}
	}
}
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

