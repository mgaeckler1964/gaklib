/*
		Project:		GAKLIB
		Module:			changeManager.h
		Description:	A change managers informs all connected viewers/editor 
						about changes
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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


#ifndef CHANGE_MANAGER_H
#define CHANGE_MANAGER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <stdlib.h>

#include <gak/array.h>

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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class ChangeManager;

class DocumentViewer
{
	ChangeManager *m_manager;

	public:
	DocumentViewer( ChangeManager *manager );
	virtual ~DocumentViewer();

	void setDocument( void *document );

	virtual void handlePositionChange( void *document, void *position ) = 0;
	virtual void handleChange( void *document, void *tem ) = 0;
	virtual void handleNew( void *document, void *tem ) = 0;
	virtual void handleDelete( void *document, void *tem ) = 0;

	void setPosition( void *position );
	void setChanged( void *item=nullptr );
	void setNew( void *item );
	void setDelete( void *item );

	bool operator = ( bool val );
	operator bool () const;
};

typedef ArrayOfPointer<DocumentViewer>	DocumentViewers;

class ChangeManager
{
	void			*m_theDocument;		// this is the document to observe
	bool			m_changed;

	DocumentViewers	m_theViewer;

	public:
	ChangeManager( void *document=nullptr )
	{
		doEnterFunction("ChangeManager");
		setDocument( document );
	}
	~ChangeManager();

	void setDocument( void *document )
	{
		m_theDocument = document;
		clrChanged();
	}
	bool isChanged() const
	{
		return m_changed;
	}
	void clrChanged()
	{
		m_changed = false;
	}
	void registerViewer( DocumentViewer *newViewer )
	{
		doEnterFunction("registerViewer");
		m_theViewer.addElement( newViewer );
	}
	void unregisterViewer( DocumentViewer *delViewer );

	void setPosition( DocumentViewer *source, void *position );
	void setChanged( DocumentViewer *source=nullptr, void *item=nullptr );
	void setNew( DocumentViewer *source, void *item );
	void setDelete( DocumentViewer *source, void *item );

	bool operator = ( bool val )
	{
		if( val )
		{
			setChanged();
		}
		else
		{
			m_changed = false;
		}

		return val;
	}
	operator bool () const
	{
		return m_changed;
	}
};

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

inline DocumentViewer::DocumentViewer( ChangeManager *manager )
{
	doEnterFunction("DocumentViewer::DocumentViewer");
	m_manager = manager;
	manager->registerViewer( this );
}

inline bool DocumentViewer::operator = ( bool val )
{
	if( val )
		m_manager->setChanged( this );
	else
		m_manager->clrChanged();

	return val;
}

inline void DocumentViewer::setDocument( void *document )
{
	m_manager->setDocument( document );
}

inline void DocumentViewer::setPosition( void *position )
{
	m_manager->setPosition( this, position );
}

inline void DocumentViewer::setChanged( void *item )
{
	m_manager->setChanged( this, item );
}

inline void DocumentViewer::setNew( void *item )
{
	m_manager->setNew( this, item );
}

inline void DocumentViewer::setDelete( void *item )
{
	m_manager->setDelete( this, item );
}

inline DocumentViewer::operator bool () const
{
	return m_manager->isChanged();
}

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

#endif
