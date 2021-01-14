/*
		Project:		GAKLIB
		Module:			crypto.h
		Description:	base de- and encryption
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

#ifndef GAK_CYPHER_H
#define GAK_CYPHER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <stdlib.h>

#include <gak/types.h>
#include <gak/string.h>
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

const int EncryptionMagic = 0xAF62;
const int EncryptionVersion = 0x0001;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct CryptoHeader
{
	uint16	magic;
	uint16	version;

	uint16	headerSize;
	int		plainBlockSize;
	int		cryptoBlockSize;

	uint32	fileSize;

	void toBinaryStream( std::ostream &stream ) const
	{
		binaryToBinaryStream( stream, *this );
	}
	void fromBinaryStream( std::istream &stream )
	{
		binaryFromBinaryStream( stream, this );
	}
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class Crypto
{
	protected:
	static unsigned char xData[32];

	private:
	virtual int getPlainBlockSize( void ) const = 0;
	virtual int getCryptoBlockSize( void ) const = 0;

	virtual void prepareEncryption( void ) = 0;
	virtual void prepareDecryption( void ) = 0;
	virtual void encryptBlock( const unsigned char *plain, unsigned char *crypto, int size, size_t xi ) = 0;
	virtual int decryptBlock( const unsigned char *crypto, unsigned char *plain, size_t xi ) = 0;

	size_t encryptPrepBuffer( const unsigned char *plain, unsigned char *crypto, size_t size );
	void encryptPrepBuffer( const unsigned char *plain, size_t size, ArrayOfData *crypto );
	size_t decryptPrepBuffer( const unsigned char *crypto, unsigned char *plain, size_t size );
	void decryptPrepBuffer(
		const unsigned char *crypto, ArrayOfData *plain, size_t size
	);

	public:
	void encryptFile(
		const STRING &plainFileName, const STRING &cryptoFileName
	);
	void decryptFile(
		const STRING &cryptoFileName, const STRING &plainFileName
	);
	void decryptFile(
		const STRING &cryptoFileName, ArrayOfData *plain
	);

	void encryptBuffer(
		const void *plain, size_t size,
		ArrayOfData *result
	)
	{
		prepareEncryption();
		encryptPrepBuffer( (const unsigned char *)plain, size, result );
	}
	size_t decryptBuffer(
		void *inOut, size_t size
	)
	{
		prepareDecryption();
		size_t result = decryptPrepBuffer(
			(const unsigned char *)inOut, (unsigned char *)inOut,
			size
		);

		assert( result <= size );

		return result;
	}
	size_t decryptBuffer(
		const ArrayOfData &crypto, void *plain
	)
	{
		prepareDecryption();
		return decryptPrepBuffer(
			(const unsigned char *)crypto.getDataBuffer(), (unsigned char *)plain,
			crypto.size()
		);
	}

	void encryptString(
		const STRING &text, ArrayOfData *buffer
	)
	{
		size_t				size = text.strlen()+1;
		const char			*cp = text;

		prepareEncryption();
		encryptPrepBuffer( (const unsigned char *)cp, size, buffer );
	}
	STRING decryptString( const ArrayOfData &buffer );

	size_t	getPlainSize( size_t cryptoSize ) const
	{
		int	cryptoBlockSize = getCryptoBlockSize();
		int	plainBlockSize = getPlainBlockSize();
		size_t	blockCount = cryptoSize / cryptoBlockSize;

		assert( !(cryptoSize % cryptoBlockSize) );

		return blockCount * plainBlockSize;
	}
	static uint32 getFileSize( const STRING &fileName );
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
