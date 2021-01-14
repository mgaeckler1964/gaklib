/*
		Project:		GAKLIB
		Module:			aes.h
		Description:	AES de- and encryption
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

#ifndef GAK_AES_LIB_H
#define GAK_AES_LIB_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <openssl/aes.h>

#include <gak/crypto.h>

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

class CryptoAES : public Crypto
{
	unsigned char 	aesCypher[32];
	AES_KEY			aesKey;

	virtual int getPlainBlockSize( void ) const;
	virtual int getCryptoBlockSize( void ) const;

	virtual void prepareEncryption( void );
	virtual void prepareDecryption( void );
	virtual void encryptBlock( const unsigned char *plain, unsigned char *crypto, int size, size_t xi );
	virtual int decryptBlock( const unsigned char *crypto, unsigned char *plain, size_t xi );

	void makeAESCypher( const STRING &txtCypher );
	public:
	CryptoAES()
	{
		makeRandomCypher();
	}
	CryptoAES( const STRING &txtCypher )
	{
		makeAESCypher( txtCypher );
	}
	void makeRandomCypher( void );

	void loadCypher( const STRING &fName );
	void saveCypher( const STRING &fName ) const;

	void loadPersonalCypher( void );
	void encryptCypher( Crypto *key, ArrayOfData *crypto ) const
	{
		key->encryptBuffer( aesCypher, 32, crypto );
	}
	void decryptCypher( Crypto *key, const ArrayOfData &crypto )
	{
		char	tmpBuff[1024];
		assert( crypto.size() < 1024 ); 
		key->decryptBuffer( crypto, tmpBuff );
		memcpy( aesCypher, tmpBuff, 32 );
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

inline void aesEncryptFile(
	const STRING &cypher, const STRING &fileName, const STRING &outFileName
)
{
	CryptoAES	aesKey( cypher );

	aesKey.encryptFile( fileName, outFileName );
}

inline void aesDecryptFile(
	const STRING &cypher, const STRING &fileName, const STRING &outFileName
)
{
	CryptoAES	aesKey( cypher );

	aesKey.decryptFile( fileName, outFileName );
}

inline void aesEncryptFile(
	const STRING &fileName, const STRING &outFileName
)
{
	CryptoAES	aesKey;
	
	aesKey.loadPersonalCypher();
	aesKey.encryptFile( fileName, outFileName );
}

inline void aesDecryptFile(
	const STRING &fileName, const STRING &outFileName
)
{
	CryptoAES	aesKey;
	aesKey.loadPersonalCypher();
	aesKey.decryptFile( fileName, outFileName );
}

inline void aesEncryptString(
	const STRING &cypher,
	const STRING &text, ArrayOfData *crypto
)
{
	CryptoAES	aesKey( cypher );

	aesKey.encryptString( text, crypto );
}

inline STRING aesDecryptString(
	const STRING &cypher,
	const ArrayOfData &crypto
)
{
	CryptoAES	aesKey( cypher );

	return aesKey.decryptString( crypto );
}

inline void aesEncryptString(
	const STRING &text, ArrayOfData *crypto
)
{
	CryptoAES	aesKey;
	aesKey.loadPersonalCypher();
	aesKey.encryptString( text, crypto );
}

inline STRING aesDecryptString(
	const ArrayOfData &crypto
)
{

	CryptoAES	aesKey;
	aesKey.loadPersonalCypher();

	return aesKey.decryptString( crypto );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
