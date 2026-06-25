/*
		Project:		GAKLIB
		Module:			cryptoShared.cpp
		Description:	Container file, that stores a crypto and multiple RSA
						crypted keys to encrypt the crypto.
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/cryptoShared.h>
#include <gak/arrayFile.h>
#include <gak/iostream.h>
#include <gak/rsa.h>
#include <gak/exception.h>

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

void CryptoShared::decryptFile(
	const STRING		&destFileName
)
{
	ArrayOfData		plain;
	size_t			plainSize = m_aesKey.getPlainSize( m_crypto.size() );

	m_aesKey.decryptBuffer( m_crypto, plain.createElements( plainSize ) );
	plain.removeElementsAt( m_fileSize, plainSize - m_fileSize );

	writeToFile( plain, destFileName );
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void CryptoShared::decryptAesKey(
	const STRING		&identifier,	// unigue identifier (e.g. e-mail address)
	CryptoRSA			&rsaKey			// the private Key
)
{
	if( rsaKey.hasKey() )
	{
		size_t	keyIndex = m_keys.getElementIndex( identifier );
		if( keyIndex != m_keys.no_index )
		{
			ArrayOfData	&crypto = m_keys.getValueAt(keyIndex);

			m_aesKey.decryptCypher( &rsaKey, crypto );
		}
		else
/*@*/		BadKeyError();
	}
	else
/*@*/	BadKeyError();
}

void CryptoShared::encryptFile( const STRING &fileName )
{
	ArrayOfData	plain;

	readFromFile( &plain, fileName );
	m_fileSize = plain.size();
	m_aesKey.encryptBuffer( plain.getDataBuffer(), m_fileSize, &m_crypto );
}

void CryptoShared::addPublicKey(
	const STRING	&identifier,	// unigue identifier (e.g. e-mail address)
	CryptoRSA 		&publicKey
)
{
	if( publicKey.hasKey() )
	{
		ArrayOfData	&crypto = m_keys[identifier];

		m_aesKey.encryptCypher( &publicKey, &crypto );
	}
	else
/*@*/	BadKeyError();
}

void CryptoShared::toBinaryStream ( std::ostream &stream ) const
{
	CryptoHeader	newHeader;

	newHeader.magic = EncryptionMagic;
	newHeader.version = EncryptionVersion;
	newHeader.headerSize = sizeof( CryptoHeader );
	newHeader.plainBlockSize = 0;
	newHeader.cryptoBlockSize = 0;
	if(m_fileSize > std::numeric_limits<uint32>::max())
		throw IntegerOverflowError("size_t value");
	newHeader.fileSize = uint32(m_fileSize);

	gak::toBinaryStream( stream, newHeader );

	m_crypto.toBinaryStream( stream );
	m_keys.toBinaryStream( stream );

	if( !stream )
		throw WriteError();
}

void CryptoShared::fromBinaryStream ( std::istream &stream )
{
	CryptoHeader	newHeader;

	gak::fromBinaryStream( stream, &newHeader );
	if( newHeader.magic == EncryptionMagic
	&&	newHeader.version == EncryptionVersion
	&&	newHeader.headerSize == sizeof( CryptoHeader )
	&&	newHeader.plainBlockSize == 0
	&&	newHeader.cryptoBlockSize == 0 )
	{
		m_fileSize = newHeader.fileSize;

		m_crypto.fromBinaryStream( stream );
		m_keys.fromBinaryStream( stream );

		if( !stream )
			throw ReadError();
	}
	else
/*@*/	throw BadHeaderError();
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

