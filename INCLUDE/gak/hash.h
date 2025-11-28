/*
		Project:		GAKLIB
		Module:			hash.h
		Description:	
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

#ifndef GAK_HASH_H
#define GAK_HASH_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#if defined( __WIN64__ ) || (defined( __MACH__) && !defined(__LP64__)) || __GNUC__ > 4
#	define	SHA_SUPPORT	0
#else
#	define	SHA_SUPPORT	1
#endif

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iomanip>
#include <fstream>
#include <gak/stringStream.h>

#if SHA_SUPPORT
#include <openssl/sha.h>
#endif

#include <gak/md5.h>

#include <gak/string.h>
#include <gak/wideString.h>
#include <gak/fixedArray.h>
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

class Hash
{
	virtual void init() = 0;
	virtual void update( const void *data, size_t len ) = 0;
	virtual void finish() = 0;
	public:
	bool m_storeStream;
	ArrayOfData m_streamData;
	Hash() : m_storeStream(false) {}

	void hash_stream( std::istream &str );
	void hash_file( const STRING &fName )
	{
		std::ifstream	in;

#ifdef _MSC_VER
		if( fName.getCharSet() == STR_UTF8 )
		{
			in.open( uSTRING().decodeUTF8( fName ), std::ios_base::in|std::ios_base::binary );

		}
		else
#endif
		{
			in.open( fName, std::ios_base::in|std::ios_base::binary );
		}

		if( !in.is_open() )
		{
			throw OpenReadError( fName );
		}
		hash_stream( in );
	}
	void hash_data( void *data, size_t len )
	{
		init();
		update( data, len );
		finish();
	}
	void hash_data( const ArrayOfData &data )
	{
		init();
		update( data.getDataBuffer(), data.size() );
		finish();
	}
	void hash_string( const STRING &data )
	{
		init();
		update( static_cast<const char*>(data), data.strlen() );
		finish();
	}
};

class MD5Hash : public Hash
{
	public:
	typedef FixedArray<unsigned char, 16>	Digest;

	private:
	md5_context		ctx;
	Digest			digest;

	virtual void init();
	virtual void update( const void *data, size_t len );
	virtual void finish();

	public:
	const Digest &getDigest() const
	{
		return digest;
	}
	MD5Hash()
	{
		memset( digest.getDataBuffer(), 0, 16 );
	}
};

#if	SHA_SUPPORT
class SHA224Hash : public Hash
{
	public:
	typedef FixedArray<unsigned char, SHA224_DIGEST_LENGTH>	Digest;

	private:
	SHA256_CTX		ctx;
	Digest			digest;

	virtual void init();
	virtual void update( const void *data, size_t len );
	virtual void finish();

	public:
	const Digest &getDigest() const
	{
		return digest;
	}
};

class SHA256Hash : public Hash
{
	public:
	typedef FixedArray<unsigned char, SHA256_DIGEST_LENGTH>	Digest;

	private:
	SHA256_CTX		ctx;
	Digest			digest;

	virtual void init();
	virtual void update( const void *data, size_t len );
	virtual void finish();

	public:
	const Digest &getDigest() const
	{
		return digest;
	}
};

class SHA384Hash : public Hash
{
	public:
	typedef FixedArray<unsigned char, SHA384_DIGEST_LENGTH>	Digest;

	private:
	SHA512_CTX		ctx;
	Digest			digest;

	virtual void init();
	virtual void update( const void *data, size_t len );
	virtual void finish();

	public:
	const Digest &getDigest() const
	{
		return digest;
	}
};

class SHA512Hash : public Hash
{
	public:
	typedef FixedArray<unsigned char, SHA512_DIGEST_LENGTH>	Digest;

	private:
	SHA512_CTX		ctx;
	Digest			digest;

	virtual void init();
	virtual void update( const void *data, size_t len );
	virtual void finish();

	public:
	const Digest &getDigest() const
	{
		return digest;
	}
};
#endif	// SHA_SUPPORT

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

template <typename DIGEST_T>
void printDigest( std::ostream &out, const DIGEST_T &digest )
{
	for( size_t i=0; i<digest.size(); ++i )
	{
		if( i )
			out << ' ';
		out << std::hex << std::setw(2) << std::setfill('0') << int(digest[i]);
	}
}

inline std::ostream &operator << ( std::ostream &out, const MD5Hash::Digest &digest )
{
	printDigest( out, digest );
	return out;
}

#if SHA_SUPPORT
inline std::ostream &operator << ( std::ostream &out, const SHA224Hash::Digest &digest )
{
	printDigest( out, digest );
	return out;
}

inline std::ostream &operator << ( std::ostream &out, const SHA256Hash::Digest &digest )
{
	printDigest( out, digest );
	return out;
}

inline std::ostream &operator << ( std::ostream &out, const SHA512Hash::Digest &digest ){
	printDigest( out, digest );
	return out;
}

inline std::ostream &operator << ( std::ostream &out, const SHA384Hash::Digest &digest ){
	printDigest( out, digest );
	return out;
}
#endif	// SHA_SUPPORT

template <typename DIGEST_T>
STRING digestStr( const DIGEST_T &digest )
{
	STRING resultString;
	oSTRINGstream str(resultString);

	str << digest;
	str.flush();

	return resultString;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif  // GAK_HASH_H