/*
		Project:		GAKLIB
		Module:			crypto.cpp
		Description:	base de- and encryption
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

#include <gak/arrayFile.h>
#include <gak/stdlib.h>
#include <gak/crypto.h>
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

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

unsigned char Crypto::xData[32] =
{
	168,  43,  99,  88,
	243, 222,  47,  33,
	 31,  20,  57, 149,
	199, 172, 241,  65,
	 71,  72,  74,  82,
	 90,  96,  14,  13,
	  1,   2,   3,   5,
	 11,  13,  17,  33
};

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

size_t Crypto::decryptPrepBuffer(
	const unsigned char *crypto, unsigned char *plain, size_t size
)
{
	size_t	xi = 0;
	int		cryptoBlockSize = getCryptoBlockSize();
	int		plainBlockSize;
	size_t	result = 0;

	assert( (size % cryptoBlockSize)  == 0 );

	while( size )
	{
		plainBlockSize = decryptBlock( crypto, plain, xi );

		crypto += cryptoBlockSize;
		size   -= cryptoBlockSize;

		result += plainBlockSize;
		plain  += plainBlockSize;

		xi++;
	}

	return result;
}

void Crypto::decryptPrepBuffer(
	const unsigned char *crypto, ArrayOfData *plain, size_t size
)
{
	size_t	xi = 0;
	int		cryptoBlockSize = getCryptoBlockSize();
	int		plainBlockSize = getPlainBlockSize();

	assert( (size % cryptoBlockSize)  == 0 );

	while( size )
	{
		int newSize = decryptBlock(
			crypto,
			(unsigned char*)plain->createElements( plainBlockSize ),
			xi
		);
		if( newSize < plainBlockSize )
		{
			size_t	toRemove = plainBlockSize - newSize;
			plain->removeElementsAt( plain->size()-toRemove, toRemove );
		}

		crypto += cryptoBlockSize;
		size -= cryptoBlockSize;

		xi++;
	}
}

size_t Crypto::encryptPrepBuffer( const unsigned char *plain, unsigned char *crypto, size_t size )
{
	size_t	i;
	int		cryptoBlockSize = getCryptoBlockSize();
	int		plainBlockSize = getPlainBlockSize();
	size_t	blockCount = size/plainBlockSize;

	for( i=0; i<blockCount; i++ )
	{
		encryptBlock( plain, crypto, plainBlockSize, i );
		plain += plainBlockSize;
		crypto += cryptoBlockSize;
		size -= plainBlockSize;
	}
	if( size )
	{
		encryptBlock( plain, crypto, int(size), i );
		blockCount++;
	}

	return blockCount;
}

void Crypto::encryptPrepBuffer( const unsigned char *plain, size_t size, ArrayOfData *crypto )
{
	size_t	xi = 0;
	int		cryptoBlockSize = getCryptoBlockSize();
	int		plainBlockSize = getPlainBlockSize();
	size_t	blockCount = size/plainBlockSize;
	char	tmpCrypto[10240];

	assert( cryptoBlockSize <= 10240 );

	crypto->clear();
	for( size_t i=0; i<blockCount; i++ )
	{
		encryptBlock( plain, (unsigned char *)tmpCrypto, plainBlockSize, xi );
		crypto->addElements( tmpCrypto, cryptoBlockSize );
		plain += plainBlockSize;
		size -= plainBlockSize;

		xi++;
	}
	if( size )
	{
		encryptBlock( plain, (unsigned char *)tmpCrypto, int(size), xi );
		crypto->addElements( tmpCrypto, cryptoBlockSize );
	}
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

void Crypto::encryptFile(
	const STRING &plainFileName, const STRING &cryptoFileName
)
{
	struct stat 		statBuff;
	size_t				blockCount;
	int					cryptoBlockSize = getCryptoBlockSize();
	int					plainBlockSize = getPlainBlockSize();

	prepareEncryption();

	strStatE( plainFileName, &statBuff );

	blockCount = statBuff.st_size / plainBlockSize +1;
	STDfile fpPlain( plainFileName, "rb" );
	if( fpPlain )
	{
		Buffer<unsigned char>	plain( malloc( size_t(statBuff.st_size) ) );
		if( plain )
		{
			Buffer<CryptoHeader>	crypto(
				malloc(
					statBuff.st_size+sizeof(CryptoHeader)+blockCount*cryptoBlockSize
				)
			);
			if( crypto )
			{
				crypto->magic = EncryptionMagic;
				crypto->version = EncryptionVersion;
				crypto->headerSize = sizeof( CryptoHeader );
				crypto->plainBlockSize = uint16(plainBlockSize);
				crypto->cryptoBlockSize = uint16(cryptoBlockSize);
				crypto->fileSize = statBuff.st_size;

				if( fread( plain, 1, size_t(statBuff.st_size), fpPlain ) == size_t(statBuff.st_size) )
				{
					blockCount = encryptPrepBuffer(
						plain,
						(unsigned char *)(crypto+1),
						statBuff.st_size
					);
					STDfile fpCrypto( cryptoFileName, "wb" );
					if( fpCrypto )
					{
						if( !fwrite( crypto, sizeof(CryptoHeader), 1, fpCrypto )
						|| fwrite( crypto+1, cryptoBlockSize, blockCount, fpCrypto ) != blockCount )
/*@*/						throw WriteError( cryptoFileName );
					}
					else
/*@*/					throw OpenWriteError( cryptoFileName );
				}
				else
/*@*/				throw ReadError( plainFileName );
			}
			else
/*@*/			throw AllocError();
		}
		else
/*@*/		throw AllocError();
	}
	else
/*@*/	throw OpenReadError( plainFileName );
}

void Crypto::decryptFile(
	const STRING &cryptoFileName, ArrayOfData *plain
)
{
	prepareDecryption();

	struct stat statBuff;
	strStatE( cryptoFileName, &statBuff );
	STDfile fpCrypto( cryptoFileName, "rb" );
	if( fpCrypto )
	{
		Buffer<CryptoHeader>crypto( malloc( size_t(statBuff.st_size) ) );
		if( crypto )
		{
			plain->setChunkSize( size_t(statBuff.st_size) );
			if( fread( crypto, 1, size_t(statBuff.st_size), fpCrypto ) == size_t(statBuff.st_size) )
			{
				if( crypto->magic == EncryptionMagic
				&& 	crypto->version == EncryptionVersion
				&&  crypto->headerSize == sizeof(CryptoHeader)
				&&  crypto->plainBlockSize == getPlainBlockSize()
				&&  crypto->cryptoBlockSize == getCryptoBlockSize() )
				{

					decryptPrepBuffer(
						reinterpret_cast<unsigned char *>(crypto+1),
						plain,
						statBuff.st_size-sizeof(CryptoHeader)
					);
					size_t size = crypto->fileSize;
					if( size < plain->size() )
						plain->removeElementsAt( size, plain->size() - 1 );
				}
				else
/*@*/				throw BadHeaderError( cryptoFileName );
			}
			else
/*@*/			throw ReadError( cryptoFileName );
		}
		else
/*@*/		throw AllocError();
	}
	else
/*@*/	throw OpenReadError( cryptoFileName );
}

void Crypto::decryptFile(
	const STRING &cryptoFileName, const STRING &plainFileName
)
{
	ArrayOfData			plain;

	decryptFile( cryptoFileName, &plain );
	writeToFile( plain, plainFileName );
}

uint32 Crypto::getFileSize( const STRING &fileName )
{
	uint32 			size = 0;
	CryptoHeader	header;

	STDfile fp( fileName, "rb" );
	if( fp )
	{
		if( fread( &header, sizeof(CryptoHeader), 1, fp )
		&&  header.magic == EncryptionMagic
		&& 	header.version == EncryptionVersion
		&&  header.headerSize == sizeof(CryptoHeader) )
		{
			size = header.fileSize;
		}
	}

	return size;
}

STRING Crypto::decryptString(
	const ArrayOfData &buffer
)
{
	STRING		result;
	size_t		size = buffer.size();
	int			cryptoBlockSize = getCryptoBlockSize();
	int			plainBlockSize = getPlainBlockSize();
	size_t		blockCount = size / cryptoBlockSize;
	char		plain[10240];
	const char	*crypto = buffer.getDataBuffer();

	assert( plainBlockSize+1 <= 10240 );

	prepareDecryption();

	plain[plainBlockSize] = 0;
	for( size_t i=0; i<blockCount; i++ )
	{
		decryptBlock(
			reinterpret_cast<const unsigned char*>(crypto), reinterpret_cast<unsigned char*>(plain), i 
		);
		crypto += cryptoBlockSize;
		result += plain;
	}

	return result;
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

