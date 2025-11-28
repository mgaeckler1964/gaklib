/*
		Project:		GAKLIB
		Module:			CryptoTest.h
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/aes.h>
#include <gak/rsa.h>
#include <gak/cryptoShared.h>
#include <gak/directory.h>
#include <gak/hash.h>

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

#	pragma warn -inl
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

#if SHA_SUPPORT

class CryptoTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "CryptoTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "CryptoTest::PerformTest");
		TestScope scope( "PerformTest" );

		CryptoSharedTest();
		AesTest();
		RsaTest();
		HashTest();
	}

	template <typename HASH_T>
	void compareFiles( const STRING &srcName, const STRING &resultFile )
	{
		HASH_T	sourceHash, destHash;
		sourceHash.hash_file( srcName  );
		destHash.hash_file( resultFile );
		UT_ASSERT_EQUAL( sourceHash.getDigest(), destHash.getDigest() );
	}

	void compare( const STRING &srcName, const STRING &resultFile )
	{
		{
			TestScope scope( "MD5Hash" );
			compareFiles<MD5Hash>(srcName, resultFile);
		}
		{
			TestScope scope( "SHA224Hash" );
			compareFiles<SHA224Hash>(srcName, resultFile);
		}
		{
			TestScope scope( "SHA256Hash" );
			compareFiles<SHA256Hash>(srcName, resultFile);
		}
		{
			TestScope scope( "SHA384Hash" );
			compareFiles<SHA384Hash>(srcName, resultFile);
		}
		{
			TestScope scope( "SHA512Hash" );
			compareFiles<SHA512Hash>(srcName, resultFile);
		}
	}
	void HashTest()
	{
		STRING			hash;
		oSTRINGstream	out(hash);

		{
			SHA224Hash	sha224;

			sha224.hash_string("");
			out << sha224.getDigest();
			out.flush();
			UT_ASSERT_EQUAL( hash, STRING("d1 4a 02 8c 2a 3a 2b c9 47 61 02 bb 28 82 34 c4 15 a2 b0 1f 82 8e a6 2a c5 b3 e4 2f") );
		}
		{
			hash = "";
			SHA256Hash	sha256;

			sha256.hash_string("");
			out << sha256.getDigest();
			out.flush();
			UT_ASSERT_EQUAL( hash, STRING("e3 b0 c4 42 98 fc 1c 14 9a fb f4 c8 99 6f b9 24 27 ae 41 e4 64 9b 93 4c a4 95 99 1b 78 52 b8 55") );
		}
		{
			hash = "";
			SHA384Hash	sha384;

			sha384.hash_string("");
			out << sha384.getDigest();
			out.flush();
			UT_ASSERT_EQUAL( hash, STRING("38 b0 60 a7 51 ac 96 38 4c d9 32 7e b1 b1 e3 6a 21 fd b7 11 14 be 07 43 4c 0c c7 bf 63 f6 e1 da 27 4e de bf e7 6f 65 fb d5 1a d2 f1 48 98 b9 5b") );
		}
		{
			hash = "";
			SHA512Hash	sha512;

			sha512.hash_string("");
			out << sha512.getDigest();
			out.flush();
			UT_ASSERT_EQUAL( hash, STRING("cf 83 e1 35 7e ef b8 bd f1 54 28 50 d6 6d 80 07 d6 20 e4 05 0b 57 15 dc 83 f4 a9 21 d3 6c e9 ce 47 d0 d1 3c 5d 85 f2 b0 ff 83 18 d2 87 7e ec 2f 63 b9 31 bd 47 41 7a 81 a5 38 32 7a f9 27 da 3e") );
		}
		{
			hash = "";
			SHA512Hash	sha512;

			sha512.hash_string("Franz jagt im komplett verwahrlosten Taxi quer durch Bayern");
			out << sha512.getDigest();
			out.flush();
			UT_ASSERT_EQUAL( hash, STRING("af 9e d2 de 70 04 33 b8 03 24 0a 55 2b 41 b5 a4 72 a6 ef 3f e1 43 1a 72 2b 20 63 c7 5e 9f 07 45 1f 67 a2 8e 37 d0 9c de 76 94 24 c9 6a ea 6f 89 71 38 9d b9 e1 99 3d 6c 56 5c 3c 71 b8 55 72 3c") );
		}
	}
	void RsaTest( void )
	{
		STRING		plainText = "Hello world, the quick brown fox jumps over the lazy dog"; //key length : 2048
		ArrayOfData	encrypted;
		STRING		decrypted;

		STRING		cypherName = getTempPath() + DIRECTORY_DELIMITER_STRING "rsaKey";
/*
	create the key if not exists
*/
		CryptoRSA	myPrivateRsaKeyCreator( cypherName, false );
		if( myPrivateRsaKeyCreator.hasKey() )
			myPrivateRsaKeyCreator.saveCryptedCypher( cypherName, "MyTxtCypher" );

		CryptoRSA	myPublicRsaKey( cypherName, true );
		CryptoRSA	myPrivateRsaKey( cypherName, false );
		if( !myPrivateRsaKey.hasKey() )
			myPrivateRsaKey.loadCryptedCypher( cypherName, "MyTxtCypher" );

		myPublicRsaKey.encryptString( plainText, &encrypted );

		decrypted = myPrivateRsaKey.decryptString( encrypted );
		UT_ASSERT_EQUAL( plainText, decrypted );
		std::cout << "Decrypted Text =" << decrypted << '\n';

		STRING	srcName = __FILE__;
		STRING	secretFile = __FILE__ ".secret";
		STRING	resultFile = __FILE__ ".txt";

		myPublicRsaKey.encryptFile( srcName, secretFile );

		myPrivateRsaKey.decryptFile( secretFile, resultFile );

		compare(srcName, resultFile);

		struct stat srcStat;
		strStatE( srcName, &srcStat );
		uint32 size = Crypto::getFileSize( secretFile );
		UT_ASSERT_EQUAL( uint32(srcStat.st_size), size );

		strRemoveE( secretFile );
		strRemoveE( resultFile );

		strRemoveE( cypherName + ".key_priv" );
		strRemoveE( cypherName + ".key_pub" );
	}

	struct ArrayTEST
	{
		int		number;
		char	buffer[32];
	};

	void AesTest( void )
	{
		STRING	srcName = __FILE__;
		STRING	cypher = "The Quick Brown Fox Jumps Over The Lazy DOG";

		STRING	secretFile = __FILE__ ".secret";
		STRING	resultFile = __FILE__ ".txt";

		aesEncryptFile( cypher, srcName, secretFile );
		aesDecryptFile( cypher, secretFile, resultFile );

		compare(srcName, resultFile);

		aesEncryptFile( srcName, secretFile );
		aesDecryptFile( secretFile, resultFile );

		compare(srcName, resultFile);

		struct stat srcStat;
		strStatE( srcName, &srcStat );
		uint32 size = Crypto::getFileSize( secretFile );
		UT_ASSERT_EQUAL( size_t(srcStat.st_size), size_t(size) );

		strRemoveE( secretFile );
		strRemoveE( resultFile );

		ArrayOfData	buffer;
		aesEncryptString( cypher, cypher, &buffer );
		STRING newCypher = aesDecryptString( cypher, buffer );
		UT_ASSERT_EQUAL( cypher, newCypher );

		Array<ArrayTEST>	myArray, decodedArray;
		ArrayOfData			result;
		for( size_t i=0; i<10; i++ )
		{
			ArrayTEST	&entry = myArray.createElement();
			entry.number = i;
			sprintf( entry.buffer, "buffer%d", (int)i );
		}
	}
	void CryptoSharedTest( void )
	{
		CryptoShared	myEncryptor;
		CryptoShared	myDecryptor;

		STRING			cypherName = getTempPath() + DIRECTORY_DELIMITER_STRING "rsaKey";

		CryptoRSA		myPrivateRsaKeyCreator( cypherName, false );
		if( myPrivateRsaKeyCreator.hasKey() )
			myPrivateRsaKeyCreator.saveCryptedCypher( cypherName, "MyTxtCypher" );

		CryptoRSA	myPublicRsaKey( cypherName, true );
		CryptoRSA	myPrivateRsaKey( cypherName, false );
		if( !myPrivateRsaKey.hasKey() )
			myPrivateRsaKey.loadCryptedCypher( cypherName, "MyTxtCypher" );

		STRING	srcName = __FILE__;
		STRING	secretFile = __FILE__ ".secret";
		STRING	resultFile = __FILE__ ".txt";

		myEncryptor.encryptFile( srcName );
		myEncryptor.addPublicKeyFromFile( "support@cresd.de",  cypherName );
		myEncryptor.addPublicKeyFromFile( "root@cresd.de",  cypherName );

		writeToBinaryFile( secretFile, myEncryptor, 0, 0, owmOverwrite );

		readFromBinaryFile( secretFile, &myDecryptor, 0, 0, false );

		myDecryptor.decryptFile( "support@cresd.de", myPrivateRsaKey, resultFile );

		compare(srcName, resultFile);

		struct stat srcStat;
		strStatE( srcName, &srcStat );
		uint32 size = Crypto::getFileSize( secretFile );

		strRemoveE( secretFile );
		strRemoveE( resultFile );

		Array<STRING>	keys = myEncryptor.getKeyList();
		UT_ASSERT_EQUAL( keys.size(), size_t(2) );
		myEncryptor.removeKey( "root@cresd.de" );
		keys = myEncryptor.getKeyList();
		UT_ASSERT_EQUAL( keys.size(), size_t(1) );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static CryptoTest myCryptoTest;

#endif	// SHA_SUPPORT

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

#	pragma warn +inl
#endif

