/*
		Project:		GAKLIB
		Module:			cryptoAES.cpp
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/aes.h>
#include <gak/strFiles.h>
#include <gak/directory.h>

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

void CryptoAES::makeAESCypher( const STRING &txtCypher )
{
	size_t	i, j;
	for( j=0, i=0; i<32; i++ )
	{
		aesCypher[i] = (unsigned char)txtCypher[j++];
		if( j>= txtCypher.strlen() )
			j=0;
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

int CryptoAES::getPlainBlockSize( void ) const
{
	return AES_BLOCK_SIZE;
}

int CryptoAES::getCryptoBlockSize( void ) const
{
	return AES_BLOCK_SIZE;
}

void CryptoAES::prepareEncryption( void )
{
	AES_set_encrypt_key( aesCypher, 256, &aesKey );
}

void CryptoAES::prepareDecryption( void )
{
	AES_set_decrypt_key( aesCypher, 256, &aesKey );
}

void CryptoAES::encryptBlock( const unsigned char *plain, unsigned char *crypto, int size, size_t xi )
{
	unsigned char	inBuff[AES_BLOCK_SIZE];

	assert( size >= 0 && size <= AES_BLOCK_SIZE );

	xi &= 31;

	for( int i=0; i<size; i++ )
		inBuff[i] = plain[i] ^ xData[xi];

	AES_encrypt( inBuff, crypto, &aesKey );
}

int CryptoAES::decryptBlock( const unsigned char *crypto, unsigned char *plain, size_t xi )
{
	xi &= 31;

	AES_decrypt( crypto, plain, &aesKey );

	for( size_t i=0; i<AES_BLOCK_SIZE; i++ )
		*plain++ ^= xData[xi];

	return AES_BLOCK_SIZE;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void CryptoAES::makeRandomCypher( void )
{
	srand((unsigned)time(NULL) );

	for( size_t	i=0; i<32; i++ )
	{
		aesCypher[i] = (unsigned char)randomNumber( 256 );
	}
}

void CryptoAES::loadCypher( const STRING &fName )
{
	STDfile fp( fName, "rb" );
	if( fp )
	{
		if( fread( aesCypher, 1, 32, fp ) != 32 )
/*@*/		throw ReadError( fName );
	}
	else
/*@*/	throw OpenReadError( fName );
}

void CryptoAES::saveCypher( const STRING &fName ) const
{
	STDfile fp( fName, "wb" );
	if( fp )
	{
		if( fwrite( aesCypher, 1, 32, fp ) != 32 )
/*@*/		throw WriteError( fName );
	}
	else
/*@*/	throw OpenWriteError( fName );
}

void CryptoAES::loadPersonalCypher( void )
{
	STRING	cypherName = getPersonalConfig() + DIRECTORY_DELIMITER_STRING ".personalAES.key";

	if( exists( cypherName ) )
		loadCypher( cypherName );
	else
	{
		makeRandomCypher();
		saveCypher( cypherName );
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

