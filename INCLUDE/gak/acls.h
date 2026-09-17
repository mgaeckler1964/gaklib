/*
		Project:		GAKLIB
		Module:			acls.h
		Description:	File  and OS security
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

#ifndef GAK_ACLS_H
#define GAK_ACLS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#if defined( __BORLANDC__ ) || defined( _MSC_VER )
#	ifndef STRICT
#		define STRICT 1
#	endif
#include <winerror.h>
#include <windows.h>
#include <aclapi.h>
#elif defined( __MACH__ ) || defined( __unix__ )
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#else
#error "Don't know how to get ACLs"
#endif

#include <gak/strFiles.h>
#include <gak/exception.h>
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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class FileSecurity
{
	STRING	m_fileName;

#ifdef  _Windows
	PSECURITY_DESCRIPTOR	m_pSD;
	PSID					m_pSidOwner;
	PSID					m_pSidGroup;
	PACL					m_pDACL;

	static DWORD getSidName( PSID pSid, STRING *name );

	void free()
	{
		if( m_pSD != nullptr )
		{
			LocalFree( m_pSD );
		}
		m_pSD = nullptr;
	}

#else
	struct stat				m_info;
	bool					m_dataLoaded;
#endif
	public:
	FileSecurity()
	{
#ifdef  _Windows
		m_pSD = nullptr;
		m_pSidOwner = nullptr;
		m_pSidGroup = nullptr;
		m_pDACL = nullptr;
#else
		m_dataLoaded = false;
#endif
	}
	~FileSecurity()
	{
#ifdef  _Windows
		if( m_pSD != nullptr )
		{
			LocalFree( m_pSD );
		}
#endif
	}

	void loadFromFile( const STRING &fileName )
	{
		m_fileName = fileName;
#ifdef  _Windows
		free();
		DWORD dwRtnCode = GetNamedSecurityInfo(
			fileName,
			SE_FILE_OBJECT,
			OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
			&m_pSidOwner,
			&m_pSidGroup,
			&m_pDACL,
			nullptr,
			&m_pSD
		);
		if( dwRtnCode != ERROR_SUCCESS )
			throw StatReadError( fileName );
#else
		if( !strStat(fileName, &m_info) )
		{
			m_dataLoaded = true;
		}
		else
		{
			m_dataLoaded = false;
			throw StatReadError( fileName );
		}
#endif
	}
	void saveToFile( const STRING &fileName )
	{
#ifdef  _Windows
		if( m_pSD )
		{
			DWORD dwRtnCode = SetNamedSecurityInfo(
				fileName,
				SE_FILE_OBJECT,
				OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
				m_pSidOwner,
				m_pSidGroup,
				m_pDACL,
				nullptr
			);
			if( dwRtnCode != ERROR_SUCCESS )
			{
/*@*/			throw StatWriteError( fileName ).addNTerror();
			}
		}
		else
		{
/*@*/		throw StatReadError( fileName ).addNTerror();
		}
#else
		if( m_dataLoaded )
		{
			if( chown( fileName, m_info.st_uid, m_info.st_gid ) || chmod( fileName, m_info.st_mode ) )
			{
/*@*/			throw StatWriteError( fileName ).addCerror();
			}
		}
		else
		{
/*@*/		throw StatReadError( fileName ).addCerror();
		}
#endif
	}

	STRING getOwner()
	{
		STRING ownerName;
#ifdef  _Windows
		if( m_pSD && m_pSidOwner )
		{
			DWORD errCode = getSidName( m_pSidOwner, &ownerName );
			if( errCode )
			{
/*@*/			throw StatReadError( m_fileName).addNTerror( errCode );
			}
		}
		else
		{
/*@*/		throw StatReadError( m_fileName );
		}
#else
		struct passwd *pw;
		if( m_dataLoaded && (pw = getpwuid(m_info.st_uid)) != nullptr )
		{
			ownerName = pw->pw_name;
		}
		else
		{
/*@*/		throw StatReadError( m_fileName ).addCerror();
		}
#endif
		return ownerName;
	}
	STRING getGroup()
	{
		STRING	groupName;
#ifdef  _Windows
		if( m_pSD && m_pSidGroup )
		{
			DWORD errCode = getSidName( m_pSidGroup, &groupName );
			if( errCode )
			{
/*@*/			throw StatReadError( m_fileName ).addNTerror( errCode );
			}
		}
		else
		{
/*@*/		throw StatReadError( m_fileName );
		}
#else
		struct group  *gr;
		if( m_dataLoaded && (gr = getgrgid(m_info.st_gid)) != nullptr )
		{
			groupName = gr->gr_name;
		}
		else
		{
/*@*/		throw StatReadError( m_fileName );
		}
#endif
		return groupName;
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

void getOwner( const STRING &fileName, STRING *owner, STRING *group );
void copyACLs( const STRING &src,  const STRING &dest );

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

#endif
