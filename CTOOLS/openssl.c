/*
		Project:		GAKLIB
		Module:			openssl.c
		Description:
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

#ifndef STRICT
#define STRICT 1
#endif

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#if defined( __BORLANDC__ ) && __BORLANDC__ == 0x621
// otherweise I get al lot of useless warnings about unused identifiers
#pragma option -w-use
#pragma option -w-pch
#pragma option -w-eff
#pragma option -w-8005
#endif

#include <windows.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

/* --------------------------------------------------------------------- */
/* ----- module switches ----------------------------------------------- */
/* --------------------------------------------------------------------- */

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a4
#	pragma option -pc
#endif


/* --------------------------------------------------------------------- */
/* ----- constants ----------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- type definitions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

typedef int (*fpSSL_library_init)(void);
typedef void (*fpSSL_load_error_strings)(void);
typedef SSL_METHOD * (*fpSSLv23_method)(void);
typedef SSL_CTX *(*fpSSL_CTX_new)(SSL_METHOD *meth);
typedef int	(*fpSSL_CTX_use_certificate_chain_file)(SSL_CTX *ctx, const char *file);
typedef void (*fpSSL_CTX_set_default_passwd_cb)(SSL_CTX *ctx, pem_password_cb *cb);
typedef int	(*fpSSL_CTX_use_PrivateKey_file)(SSL_CTX *ctx, const char *file, int type);
typedef int (*fpSSL_CTX_load_verify_locations)(SSL_CTX *ctx, const char *CAfile,
	const char *CApath);
typedef SSL *(*fpSSL_new)(SSL_CTX *ctx);
typedef void (*fpSSL_set_bio)(SSL *s, BIO *rbio,BIO *wbio);
typedef int (*fpSSL_connect)(SSL *ssl);
typedef int (*fpSSL_write)(SSL *ssl,const void *buf,int num);
typedef int (*fpSSL_read)(SSL *ssl,void *buf,int num);
typedef int	(*fpSSL_get_error)(const SSL *s,int ret_code);
typedef int (*fpSSL_shutdown)(SSL *s);
typedef void (*fpSSL_free)(SSL *s);
typedef void (*fpSSL_CTX_free)(SSL_CTX *ctx);

typedef BIO *(*fpBIO_new_socket)(int sock, int close_flag);
typedef char *(*fpERR_error_string)(unsigned long e,char *buf);

/* --------------------------------------------------------------------- */
/* ----- macros -------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- prototypes ---------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- imported datas ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- module statics ------------------------------------------------ */
/* --------------------------------------------------------------------- */

static HINSTANCE ssleay, libeay;

static fpSSL_library_init SSL_library_initPtr					= NULL;

static fpSSL_load_error_strings SSL_load_error_stringsPtr		= NULL;
static fpSSLv23_method SSLv23_methodPtr							= NULL;
static fpSSL_CTX_new SSL_CTX_newPtr								= NULL;
static fpSSL_CTX_use_certificate_chain_file 
	SSL_CTX_use_certificate_chain_filePtr						= NULL;
static fpSSL_CTX_set_default_passwd_cb 
	SSL_CTX_set_default_passwd_cbPtr							= NULL;
static fpSSL_CTX_use_PrivateKey_file 
	SSL_CTX_use_PrivateKey_filePtr								= NULL;
static fpSSL_CTX_load_verify_locations 
	SSL_CTX_load_verify_locationsPtr							= NULL;
static fpSSL_new SSL_newPtr										= NULL;
static fpSSL_set_bio SSL_set_bioPtr								= NULL;
static fpSSL_connect SSL_connectPtr								= NULL;
static fpSSL_write SSL_writePtr									= NULL;
static fpSSL_read SSL_readPtr									= NULL;
static fpSSL_get_error SSL_get_errorPtr							= NULL;
static fpSSL_shutdown SSL_shutdownPtr							= NULL;
static fpSSL_free SSL_freePtr									= NULL;
static fpSSL_CTX_free SSL_CTX_freePtr							= NULL;

static fpBIO_new_socket BIO_new_socketPtr						= NULL;
static fpERR_error_string ERR_error_stringPtr					= NULL;


/* --------------------------------------------------------------------- */
/* ----- exported datas ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- module functions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

int SSL_library_init(void )
{
	ssleay = LoadLibrary( "SSLEAY32.DLL" );
	libeay = LoadLibrary( "LIBEAY32.DLL" );

	if( ssleay )
	{
		SSL_library_initPtr = (fpSSL_library_init)GetProcAddress( ssleay, "SSL_library_init" );
		SSL_load_error_stringsPtr = (fpSSL_load_error_strings)GetProcAddress( ssleay, "SSL_load_error_strings" );
		SSLv23_methodPtr = (fpSSLv23_method)GetProcAddress( ssleay, "SSLv23_method" );
		SSL_CTX_newPtr = (fpSSL_CTX_new)GetProcAddress( ssleay, "SSL_CTX_new" );
		SSL_CTX_use_certificate_chain_filePtr = (fpSSL_CTX_use_certificate_chain_file)GetProcAddress( ssleay, "SSL_CTX_use_certificate_chain_file" );
		SSL_CTX_set_default_passwd_cbPtr = (fpSSL_CTX_set_default_passwd_cb)GetProcAddress( ssleay, "SSL_CTX_set_default_passwd_cb" );
		SSL_CTX_use_PrivateKey_filePtr = (fpSSL_CTX_use_PrivateKey_file)GetProcAddress( ssleay, "SSL_CTX_use_PrivateKey_file" );
		SSL_CTX_load_verify_locationsPtr = (fpSSL_CTX_load_verify_locations)GetProcAddress( ssleay, "SSL_CTX_load_verify_locations" );
		SSL_newPtr = (fpSSL_new)GetProcAddress( ssleay, "SSL_new" );
		SSL_set_bioPtr = (fpSSL_set_bio)GetProcAddress( ssleay, "SSL_set_bio" );
		SSL_connectPtr = (fpSSL_connect)GetProcAddress( ssleay, "SSL_connect" );
		SSL_writePtr = (fpSSL_write)GetProcAddress( ssleay, "SSL_write" );
		SSL_readPtr = (fpSSL_read)GetProcAddress( ssleay, "SSL_read" );
		SSL_get_errorPtr = (fpSSL_get_error)GetProcAddress( ssleay, "SSL_get_error" );
		SSL_shutdownPtr = (fpSSL_shutdown)GetProcAddress( ssleay, "SSL_shutdown" );
		SSL_CTX_freePtr = (fpSSL_CTX_free)GetProcAddress( ssleay, "SSL_CTX_free" );
	}
	if( libeay )
	{
		BIO_new_socketPtr = (fpBIO_new_socket)GetProcAddress( libeay, "BIO_new_socket" );
		ERR_error_stringPtr = (fpERR_error_string)GetProcAddress( libeay, "ERR_error_string" );
	}

	if( SSL_library_initPtr )
		return SSL_library_initPtr();
	else
		return -1;
}

void SSL_load_error_strings( void )
{
	if( SSL_load_error_stringsPtr )
		SSL_load_error_stringsPtr();
}

SSL_METHOD *SSLv23_method(void)
{
	if( SSLv23_methodPtr )
		return SSLv23_methodPtr();
	else
		return NULL;
}

SSL_CTX *SSL_CTX_new(SSL_METHOD *meth)
{
	if( SSL_CTX_newPtr )
		return SSL_CTX_newPtr( meth );
	else
		return NULL;
}

int	SSL_CTX_use_certificate_chain_file(SSL_CTX *ctx, const char *file)
{
	if( SSL_CTX_use_certificate_chain_filePtr )
		return SSL_CTX_use_certificate_chain_filePtr( ctx, file );
	else
		return -1;
}

void SSL_CTX_set_default_passwd_cb(SSL_CTX *ctx, pem_password_cb *cb)
{
	if( SSL_CTX_set_default_passwd_cbPtr )
		SSL_CTX_set_default_passwd_cbPtr( ctx, cb );
}

int	SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx, const char *file, int type)
{
	if( SSL_CTX_use_PrivateKey_filePtr )
		return SSL_CTX_use_PrivateKey_filePtr( ctx, file, type );
	else
		return -1;
}

int SSL_CTX_load_verify_locations(SSL_CTX *ctx, const char *CAfile,
	const char *CApath)
{
	if( SSL_CTX_load_verify_locationsPtr )
		return SSL_CTX_load_verify_locationsPtr( ctx, CAfile, CApath );
	else
		return -1;
}

SSL *SSL_new(SSL_CTX *ctx)
{
	if( SSL_newPtr )
		return SSL_newPtr( ctx );
	else
		return NULL;
}

void SSL_set_bio(SSL *s, BIO *rbio,BIO *wbio)
{
	if( SSL_set_bioPtr )
		SSL_set_bioPtr( s, rbio, wbio );
}

int SSL_connect(SSL *ssl)
{
	if( SSL_connectPtr )
		return SSL_connectPtr( ssl );
	else
		return -1;
}

int SSL_write(SSL *ssl,const void *buf,int num)
{
	if( SSL_writePtr )
		return SSL_writePtr( ssl, buf, num );
	else
		return -1;
}

int SSL_read(SSL *ssl,void *buf,int num)
{
	if( SSL_readPtr )
		return SSL_readPtr( ssl, buf, num );
	else
		return -1;
}

int	SSL_get_error(const SSL *ssl,int ret_code)
{
	if( SSL_get_errorPtr )
		return SSL_get_errorPtr( ssl, ret_code );
	else
		return -1;
}

int SSL_shutdown(SSL *ssl)
{
	if( SSL_shutdownPtr )
		return SSL_shutdownPtr( ssl );
	else
		return -1;
}

void SSL_free(SSL *ssl)
{
	if( SSL_freePtr )
		SSL_freePtr( ssl );
}

void SSL_CTX_free(SSL_CTX *ctx)
{
	if( SSL_CTX_freePtr )
		SSL_CTX_freePtr( ctx );
}


BIO *BIO_new_socket(int sock, int close_flag)
{
	if( BIO_new_socketPtr )
		return BIO_new_socketPtr( sock, close_flag );
	else
		return NULL;
}

char *ERR_error_string(unsigned long e,char *buf)
{
	if( BIO_new_socketPtr )
		return ERR_error_stringPtr( e, buf );
	else
		return NULL;
}

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif


