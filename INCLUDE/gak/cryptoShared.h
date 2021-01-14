/*
		Project:		GAKLIB
		Module:			cryptoShared.h
		Description:	Container file, that stores a crypto and multiple RSA
						crypted keys to encrypt the crypto.
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

#ifndef CRYPTO_SHARED_H
#define CRYPTO_SHARED_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/map.h>
#include <gak/aes.h>
#include <gak/rsa.h>

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

#define LOAD_RSA	0

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class CryptoShared
{
	size_t		fileSize;
	ArrayOfData	crypto;		// this is the encrypted file
	CryptoAES	aesKey;		// this is the AES-Key

	PairMap<STRING,ArrayOfData>	keys;

	void decryptFile(
		const STRING		&destFileName
	);
	public:
	CryptoShared()
	{
		fileSize = 0;
	}
	void init( void )
	{
		fileSize = 0;
		aesKey.makeRandomCypher();
		keys.clear();
		crypto.clear();
	}

	void decryptAesKey(
		const STRING		&identifier,	// unigue identifier (e.g. e-mail address)
		CryptoRSA			&rsaKey			// the private Key
	);
#if LOAD_RSA
	void decryptAesKey(
		const STRING &identifier	// unigue identifier (e.g. e-mail address)
	)
	{
		CryptoRSA	rsaKey( false );

		return decryptAesKey( identifier, rsaKey );
	}
#endif
	void encryptFile( const STRING &fileName );
	void encryptFile( const STRING &fileName, const STRING &identifier )
	{
		encryptFile( fileName );
		addPersonalKey( identifier );
	}
	void encryptFile(
		const STRING &fileName, const STRING &identifier,
		const STRING &pemName
	)
	{
		encryptFile( fileName );
		addPublicKeyFromFile( identifier, pemName );
	}

	void addPublicKey(
		const STRING	&identifier,	// unigue identifier (e.g. e-mail address)
		CryptoRSA		&publicKey
	);

	void addPublicKeyFromFile(
		const STRING &identifier,	// unigue identifier (e.g. e-mail address)
		const STRING &fileName
	)
	{
		CryptoRSA	publiKey( fileName, true );

		return addPublicKey( identifier, publiKey );
	}
	void addPublicKey(
		const STRING &identifier,	// unigue identifier (e.g. e-mail address)
		const STRING &publicPEM
	)
	{
		CryptoRSA	publiKey( publicPEM );

		return addPublicKey( identifier, publiKey );
	}
	void addPersonalKey(
		const STRING &identifier	// unigue identifier (e.g. e-mail address)
	)
	{
		CryptoRSA	publiKey( true );

		return addPublicKey( identifier, publiKey );
	}
	void removeKey(
		const STRING &identifier	// unigue identifier (e.g. e-mail address)
	)
	{
		keys.removeElementByKey( identifier );
	}
	Array<STRING> getKeyList() const
	{
		return keys.getKeys();
	}
	void decryptFile(
		const STRING		&identifier,	// unigue identifier (e.g. e-mail address)
		CryptoRSA			&rsaKey,		// the privateKey
		const STRING		&destFileName
	)
	{
		decryptAesKey( identifier, rsaKey );
		decryptFile( destFileName );
	}
#if LOAD_RSA
	void decryptFile(
		const STRING &identifier,	// unigue identifier (e.g. e-mail address)
		const STRING &privateKeyFileName,
		const STRING &destFileName
	)
	{
		CryptoRSA	rsaKey( privateKeyFileName, false );

		return decryptFile( identifier, rsaKey, destFileName );
	}
	void decryptFile(
		const STRING &identifier,	// unigue identifier (e.g. e-mail address)
		const STRING &destFileName
	)
	{
		CryptoRSA	rsaKey( false );

		return decryptFile( identifier, rsaKey, destFileName );
	}
#endif

	void toBinaryStream ( std::ostream &stream ) const;
	void fromBinaryStream ( std::istream &stream );
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
