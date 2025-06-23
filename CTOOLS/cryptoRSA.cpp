/*
		Project:		GAKLIB
		Module:			cryptoRSA.cpp
		Description:	RSA de- and encryption
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

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

#include <assert.h>
#include <string.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <gak/rsa.h>
#include <gak/aes.h>
#include <gak/string.h>
#include <gak/stdlib.h>
#include <gak/directory.h>
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

const char file_pem[] = ".key_priv";
const char file_pem_pub[] = ".key_pub";


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

static RSA *createRSA( const char *key, bool publicKey )
{
	RSA		*rsa= NULL;
	BIO		*keybio = BIO_new_mem_buf( (void *)key, -1 );

	if( keybio )
	{
		if( publicKey )
//			rsa = PEM_read_bio_RSA_PUBKEY( keybio, &rsa, NULL, NULL );
			rsa = PEM_read_bio_RSAPublicKey( keybio, &rsa, NULL, NULL );

		else
			rsa = PEM_read_bio_RSAPrivateKey( keybio, &rsa, NULL, NULL );
	}

	return rsa;
}

static void copyBioToFile(
	BIO *bio, const STRING &fileName, time_t timeStamp
)
{
	struct stat		statBuf;

	if( strStat( fileName, &statBuf ) || statBuf.st_mtime != timeStamp )
	{
		int				keylen = BIO_pending(bio);
		Buffer<char>	pem_key( malloc( keylen+1 ) );

		if( pem_key )
		{
			BIO_read( bio, pem_key, keylen );
			pem_key[keylen] = 0;

			STDfile fp( fileName, "wb" );
			if( fp )
			{
				fprintf( fp, "%s", (char *)pem_key );
				fp.close();

				if( timeStamp )
					setModTime( fileName, timeStamp );
			}
			else
/*@*/			throw OpenWriteError( fileName );
		}
		else
/*@*/		throw AllocError();
	}
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

CryptoRSA::CryptoRSA( bool publicKey )
{
	try
	{
		timeStamp = 0;
		rsa = NULL;
		loadPersonalCypher( publicKey );
	}
	catch( ... )
	{
		timeStamp = 0;
		rsa = NULL;
	}
}

CryptoRSA::CryptoRSA( const STRING &fileName, bool publicKey )
{
	try
	{
		timeStamp = 0;
		rsa = NULL;
		if( publicKey )
			loadCypher( fileName, true );
		else
			loadCreateCypher( fileName );
	}
	catch( ... )
	{
		timeStamp = 0;
		rsa = NULL;
	}
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void CryptoRSA::makeRandomCypher( void )
{
	int				bits = 4096;
	unsigned long	exp = RSA_F4;
	RSA				*rsa = RSA_generate_key( bits, exp, NULL, NULL );

	if( rsa )
		setRsa( rsa, false );
	timeStamp = 0;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

int CryptoRSA::getPlainBlockSize( void ) const
{
	return RSA_size( rsa ) -64;
}

int CryptoRSA::getCryptoBlockSize( void ) const
{
	return RSA_size( rsa );
}

void CryptoRSA::prepareEncryption( void )
{
}

void CryptoRSA::prepareDecryption( void )
{
}

void CryptoRSA::encryptBlock( const unsigned char *plain, unsigned char *crypto, int size, size_t xi )
{
	int				result;
	unsigned char	inBuff[10240];

	assert( size <= 10240 );
	xi &= 31;

	for( int i=0; i<size; i++ )
		inBuff[i] = plain[i] ^ xData[xi];

	if( isPublic )
		result = RSA_public_encrypt( size, inBuff, crypto, rsa, padding );
	else
		result = RSA_private_encrypt( size, inBuff, crypto, rsa, padding );

	assert( result != -1 && result == getCryptoBlockSize() );
}

int CryptoRSA::decryptBlock( const unsigned char *crypto, unsigned char *plain, size_t xi )
{
	int	result;
	if( isPublic )
		result = RSA_public_decrypt( getCryptoBlockSize(), crypto, plain, rsa, padding );
	else
		result = RSA_private_decrypt( getCryptoBlockSize(), crypto, plain, rsa, padding );

	if( result != -1 )
	{
		xi &= 31;
		for( size_t i=0; i<(size_t)result; i++ )
			*plain++ ^= xData[xi];
	}

	assert( result != -1 );

	return result;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void CryptoRSA::saveCypher( const STRING &iFilename ) const
{
	if( rsa )
	{
		if( !isPublic )
		{
			BIO	*bio = BIO_new(BIO_s_mem());
			if( bio )
			{
				PEM_write_bio_RSAPrivateKey( bio, rsa, NULL, NULL, 0, NULL, NULL );

				STRING	filename = iFilename + file_pem;
				try
				{
					copyBioToFile( bio, filename, 0 );
				}
				catch( ... )
				{
					BIO_free_all(bio);
					throw;
				}
				BIO_free_all(bio);
			}
			else
/*@*/			throw AllocError();
		}

		BIO	*bio = BIO_new(BIO_s_mem());
		if( bio )
		{
			try
			{
				PEM_write_bio_RSAPublicKey( bio,rsa );

				STRING	filename = iFilename + file_pem_pub;
				copyBioToFile( bio, filename, timeStamp );
			}
			catch( ... )
			{
				BIO_free_all(bio);
/*@*/			throw;
			}

			BIO_free_all(bio);
		}
		else
/*@*/		throw AllocError();
	}
	else
/*@*/	throw AllocError();
}

void CryptoRSA::saveCryptedCypher(
	const STRING &iFileName, const STRING &txtCypher
) const
{
	saveCypher( iFileName );
	if( !isPublic )
	{
		CryptoAES		aesKey( txtCypher );
		STRING			fileName = iFileName + file_pem;

		aesKey.encryptFile( fileName, fileName );
	}
}

void CryptoRSA::savePersonalCypher( void )
{
	STRING	cypherName = getPersonalConfig() + DIRECTORY_DELIMITER_STRING ".personalRSA";

	saveCypher( cypherName );
}

void CryptoRSA::saveCryptedPersonalCypher( const STRING &txtCypher )
{
	STRING	cypherName = getPersonalConfig() + DIRECTORY_DELIMITER_STRING ".personalRSA";

	saveCryptedCypher( cypherName, txtCypher );
}

void CryptoRSA::loadCypher( const STRING &iFileName, bool publicKey )
{
	STRING	fileName = iFileName + (publicKey ? file_pem_pub : file_pem);

	struct stat		statBuf;
	strStatE( fileName, &statBuf );
	timeStamp = statBuf.st_mtime;

	STRING	pemKey;
	pemKey.readFromFile( fileName );

	if( !pemKey.isEmpty() )
	{
		RSA *rsa = createRSA( pemKey, publicKey );
		if( rsa )
			setRsa( rsa, publicKey );
		else
/*@*/		throw BadKeyError();
	}
	else
/*@*/	throw OpenReadError( fileName );
}

void CryptoRSA::loadCryptedCypher(
	const STRING &iFileName, const STRING &txtCypher
)
{
	STRING			fileName = iFileName + file_pem;
	struct stat		statBuf;

	strStatE( fileName, &statBuf );
	CryptoAES		aesKey( txtCypher );
	ArrayOfData		pemKey;

	timeStamp = statBuf.st_mtime;

	aesKey.decryptFile( fileName, &pemKey );

	RSA *rsa = createRSA( pemKey.getDataBuffer(), false );
	if( rsa )
		setRsa( rsa, false );
	else
/*@*/	throw BadKeyError();
}


void CryptoRSA::loadCreateCypher( const STRING &fileName )
{
	STRING	publicCypher = fileName + file_pem_pub;
	STRING	privateCypher = fileName + file_pem;

	if( exists( publicCypher ) && exists( privateCypher ) )
		loadCypher( fileName, false );
	else
	{
		makeRandomCypher();
		try
		{
			saveCypher( fileName );
		}
		catch( ... )
		{
			clear();
			throw;
		}
	}
}

void CryptoRSA::loadPersonalCypher( bool publicKey )
{
	STRING	cypherName = getPersonalConfig() + DIRECTORY_DELIMITER_STRING ".personalRSA";
	if( publicKey )
		loadCypher( cypherName, true );
	else
		loadCreateCypher( cypherName );
}

void CryptoRSA::loadCryptedPersonalCypher( const STRING &txtCypher )
{
	STRING	cypherName = getPersonalConfig() + DIRECTORY_DELIMITER_STRING ".personalRSA";
	loadCryptedCypher( cypherName, txtCypher );
}

void CryptoRSA::setPublicCypher( const STRING &publicPEM )
{
	setRsa( createRSA( publicPEM, true ), true );
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

