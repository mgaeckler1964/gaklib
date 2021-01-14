/*
		Project:		GAKLIB
		Module:			UserManager.cpp
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

#include <gak/string.h>
#include <gak/md5.h>
#include <gak/vcl_tools.h>
#include <gak/sortedArray.h>


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

class UserCompare : public DynamicComparator<UserOrGroup, bool>
{
	public:
	int operator () (
		const UserOrGroup &u1, const UserOrGroup &u2
	) const
	{
		if( !sortCriteria )
			return u2.ID - u1.ID;
		else
			return strcmpi( u1.userName, u2.userName );
	}
	const UserCompare &operator = ( const bool &crit )
	{
		sortCriteria = crit;
		return *this;
	}
};
typedef SortedArray<UserOrGroup, UserCompare>	UserCache;

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

UserOrGroup		actUser;

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static UserCache userCache;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static STRING md5ToHex( unsigned char md5[16] )
{
	const char *hexDigits = "0123456789ABCDEF";

	STRING	md5Hex;

	for( size_t i=0; i<16; i++ )
	{
		md5Hex += hexDigits[md5[i]/16];
		md5Hex += hexDigits[md5[i]%16];
	}

	return md5Hex;
}

static void getMemberships( const AnsiString &database, const UserOrGroup &theUser, ArrayOfInts *groupIds )
{
	groupIds->clear();

	TQuery	*theQuery = new TQuery( NULL );
	theQuery->DatabaseName = database;

	theQuery->SQL->Add(
		"select group_id from u_member where member_id = :theUser"
	);
	theQuery->Params->Items[0]->AsInteger = theUser.ID;
	for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
	{
		groupIds->addElement( theQuery->Fields->Fields[0]->AsInteger );
	}
	theQuery->Close();

	delete theQuery;

	if( theUser.department
	&&  groupIds->findElement( theUser.department ) == groupIds->no_index )
		groupIds->addElement( theUser.department );

}

static void fillGroups( const AnsiString &database, UserOrGroup *theUser, const UserOrGroup &theGroup )
{
	ArrayOfInts	groups;
	UserOrGroup	actGroup;
	STRING		groupList = theUser->groupList;

	TQuery	*theQuery = new TQuery( NULL );
	theQuery->DatabaseName = database;

	if( theUser->ID == theGroup.ID )
	{
		theQuery->SQL->Add( "delete from u_rights where user_id = :userId" );
		theQuery->Params->Items[0]->AsInteger = theUser->ID;
		theQuery->ExecSQL();
		theQuery->SQL->Clear();
	}

	getMemberships( database, theGroup, &groups );
	theQuery->SQL->Add(
		"insert into u_rights "
		"( user_id, group_id ) "
		"values"
		"( :theUser, :theGroup )"
	);
	for(
		ArrayOfInts::const_iterator it = groups.cbegin(), endIT = groups.cend();
		it != endIT;
		++it )
	{
		const int groupId = *it;

		theQuery->Params->Items[0]->AsInteger = theUser->ID;
		theQuery->Params->Items[1]->AsInteger = groupId;

		try
		{
			theQuery->ExecSQL();

			if( !groupList.isEmpty() )
			{
				groupList += ',';
			}

			groupList += formatNumber( unsigned(groupId) );

			getUserById( database, groupId, &actGroup );
			if( actGroup.ID )
			{
				theUser->groupList = groupList;
				fillGroups( database, theUser, actGroup );
				groupList = theUser->groupList;
			}
		}
		catch( ... )
		{
			// ignore
		}
	}
	theUser->groupList = groupList;
	delete theQuery;
}

static void	readUser( TQuery *theQuery, UserOrGroup *result )
{
	result->ID = theQuery->FieldByName( "ID" )->AsInteger;
	result->type = theQuery->FieldByName( "TYPE" )->AsInteger;
	result->department = theQuery->FieldByName( "DEPARTMENT" )->AsInteger;
	result->permissions = theQuery->FieldByName( "PERMISSIONS" )->AsInteger;
#if __BORLANDC__ >= 0x620
	result->userName = AnsiString( theQuery->FieldByName( "USERNAME" )->AsString ).c_str();
	result->encryptedPassword = AnsiString( theQuery->FieldByName( "MD5_PASSWD" )->AsString ).c_str();
#else
	result->userName = theQuery->FieldByName( "USERNAME" )->AsString.c_str();
	result->encryptedPassword = theQuery->FieldByName( "MD5_PASSWD" )->AsString.c_str();
#endif
	result->groupList = "";
}

static void	fillUser( TQuery *theQuery, UserOrGroup *result )
{
	theQuery->Open();
	if( !theQuery->Eof )
	{
		readUser( theQuery, result );
	}
	else
	{
		result->ID = 0;
	}
	theQuery->Close();
}

static void fillUserCache( const AnsiString &database )
{
	userCache.clear();

	TQuery *theQuery = new TQuery( NULL );
	theQuery->DatabaseName = database;

	theQuery->SQL->Add(
		"select * "
		"from user_tab u "
		"where u.type = 0 "
		"order by username"
	);
	for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
	{
		UserOrGroup	userFound;
		readUser( theQuery, &userFound );
		userCache.addElement( userFound );
	}

	delete theQuery;
}

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

void getUserById( const AnsiString &database, int id, UserOrGroup *result )
{
	userCache.resort( false );
	result->ID = id;
	size_t pos = userCache.findElement( *result );
	if( pos != -1 )
	{
		*result = userCache[pos];
/***/	return;
	}
	TQuery *theQuery = new TQuery( NULL );
	theQuery->DatabaseName = database;
	theQuery->SQL->Add( "select * from user_tab where id = :theId" );
	theQuery->Params->Items[0]->AsInteger = id;

	fillUser( theQuery, result );
	if( result->ID )
		userCache.addElement( *result );

	delete theQuery;
}

void getUserByName( const AnsiString &database, const char *name, UserOrGroup *result )
{
	userCache.resort( true );
	result->userName = name;
	size_t pos = userCache.findElement( *result );
	if( pos != -1 )
	{
		*result = userCache[pos];
/***/	return;
	}

	TQuery *theQuery = new TQuery( NULL );
	theQuery->DatabaseName = database;
	theQuery->SQL->Add( "select * from user_tab where username = :theName" );
	theQuery->Params->Items[0]->AsString = name;

	fillUser( theQuery, result );
	if( result->ID )
		userCache.addElement( *result );

	delete theQuery;
}

const UserOrGroup &getActUser( const AnsiString &database )
{
	char	theUserName[128];

	if( !actUser.ID )
	{
		DWORD	size = sizeof( theUserName );
		GetUserName( theUserName, &size );

		getUserByName( database, theUserName, &actUser );
	}
	if( !actUser.ID && !getUserCount( database ) )
	{
		TQuery	*theQuery = new TQuery( NULL );
		theQuery->DatabaseName = database;

		theQuery->SQL->Add(
			"insert into user_tab "
			"( id, type, username, permissions ) "
			"values"
			"( 1, 0, :newUser, :allPerms )"
		);
		theQuery->Params->Items[0]->AsString = theUserName;
		theQuery->Params->Items[1]->AsInteger = -1;
		theQuery->ExecSQL();

		actUser.ID = 1;
		actUser.type = 0;
		actUser.userName = theUserName;
		actUser.permissions = -1;
		actUser.department = 0;

		delete theQuery;
	}

	if( actUser.ID )
	{
		fillGroups( database, &actUser, actUser );
	}

	return actUser;
}

void loginUser(
	const AnsiString &database,
	const AnsiString &userName,
	const AnsiString &password,
	UserOrGroup	*userSession
)
{
	userCache.clear();
	getUserByName( database, userName.c_str(), userSession );
	if( userSession->ID )
	{
		unsigned char md5Hash[16];

		md5( (unsigned char *)password.c_str(), password.Length(), md5Hash );
		STRING md5Hex = md5ToHex( md5Hash );
		if( userSession->encryptedPassword != md5Hex )
		{
			userSession->ID = 0;
		}
	}
}

void changePassword(
	const AnsiString &database,
	const AnsiString &oldPassword,
	const AnsiString &newPassword
)
{
	STRING			oldMD5Hex;
	STRING			newMD5Hex;

	if( oldPassword.Length() )
	{
		unsigned char oldMD5Hash[16];

		md5( (unsigned char *)oldPassword.c_str(), oldPassword.Length(), oldMD5Hash );
		oldMD5Hex = md5ToHex( oldMD5Hash );
	}
	if( newPassword.Length() )
	{
		unsigned char newMD5Hash[16];

		md5( (unsigned char *)newPassword.c_str(), newPassword.Length(), newMD5Hash );
		newMD5Hex = md5ToHex( newMD5Hash );
	}

	if( actUser.encryptedPassword == oldMD5Hex )
	{
		TQuery	*theQuery = new TQuery( NULL );
		theQuery->DatabaseName = database;

		theQuery->SQL->Add(
			"update user_tab "
			"set MD5_PASSWD = :newPassword "
			"where id = :user_id "
		);
		theQuery->Params->Items[0]->AsString = (const char *)newMD5Hex;
		theQuery->Params->Items[1]->AsInteger = actUser.ID;

		theQuery->ExecSQL();

		delete theQuery;

		actUser.encryptedPassword = newMD5Hex;
	}
	else
		throw Exception( "Old password does not match." );
}

size_t getUserCount( const AnsiString &database )
{
	size_t	userCount = 0;
	TQuery	*theQuery = new TQuery( NULL );
	theQuery->DatabaseName = database;

	theQuery->SQL->Add(
		"select count(*) from user_tab where type = 0"
	);
	theQuery->Open();
	if( !theQuery->Eof )
		userCount = theQuery->Fields->Fields[0]->AsInteger;

	theQuery->Close();
	delete theQuery;

	return userCount;
}

void fillUserList(
	TCustomComboBox *comboBox, const AnsiString &database, int selectedUser
)
{
	comboBox->Items->Clear();
	fillUserCache( database );

	int itemIndex = -1;
	int i = 0;

	for(
		UserCache::const_iterator it = userCache.cbegin(), endIT = userCache.cend();
		it != endIT;
		++it, ++i
	)
	{
		const UserOrGroup	&theUser = *it;
		comboBox->Items->Add( static_cast<const char *>(theUser.userName) );
		if( theUser.ID == selectedUser )
		{
			itemIndex = i;
		}
	}
	comboBox->ItemIndex = itemIndex;
}

int getUserByIndex( int index )
{
	return userCache[index].ID;
}

}	// namespace vcl
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

