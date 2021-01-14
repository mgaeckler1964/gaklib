/*
		Project:		GAKLIB
		Module:			rsa.h
		Description:	RSA de- and encryption
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

#ifndef GAK_RSA_LIB_H
#define GAK_RSA_LIB_H


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <openssl/rsa.h>

#include <gak/string.h>
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

const int padding = RSA_PKCS1_OAEP_PADDING;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class CryptoRSA : public Crypto
{
	bool	isPublic;
	RSA		*rsa;
	time_t	timeStamp;

	void clear( void )
	{
		if( rsa )
			RSA_free(rsa);
		rsa = NULL;
	}
	void setRsa( RSA *newRSA, bool isPublic )
	{
		clear();
		rsa = newRSA;
		this->isPublic = isPublic;
	}

	virtual int getPlainBlockSize( void ) const;
	virtual int getCryptoBlockSize( void ) const;

	virtual void prepareEncryption( void );
	virtual void prepareDecryption( void );
	virtual void encryptBlock( const unsigned char *plain, unsigned char *crypto, int size, size_t xi );
	virtual int decryptBlock( const unsigned char *crypto, unsigned char *plain, size_t xi );

	void makeRandomCypher( void );

	/*
		avoid copying
	*/
	CryptoRSA( const CryptoRSA &src );
	const CryptoRSA & operator = ( const CryptoRSA &src );

	public:

	CryptoRSA( bool publicKey = false );
	CryptoRSA( const STRING &fileName, bool publicKey );

	CryptoRSA( const STRING &publicPEM )
	{
		timeStamp = 0;
		rsa = NULL;
		setPublicCypher( publicPEM );
	}
	~CryptoRSA()
	{
		if( rsa )
			RSA_free(rsa);
	}
	bool hasKey( void ) const
	{
		return rsa ? true : false;
	}
	void saveCypher( const STRING &fileName ) const;
	void saveCryptedCypher(
		const STRING &fileName, const STRING &txtCypher
	) const;
	void savePersonalCypher( void );
	void saveCryptedPersonalCypher( const STRING &txtCypher );

	void loadCypher( const STRING &fileName, bool publicKey );
	void loadCryptedCypher(
		const STRING &iFileName, const STRING &txtCypher
	);
	void loadCreateCypher(
		const STRING &fileName
	);
	void loadPersonalCypher( bool publicKey=false );
	void loadCryptedPersonalCypher(
		const STRING &txtCypher
	);

	void setPublicCypher( const STRING &publicPEM );
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
