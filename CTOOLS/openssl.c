/*
		Project:		GAKLIB
		Module:			openssl.c
		Description:	The open SSL loader
						For other compilers I like to use the import
						libraries.
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#ifndef STRICT
#define STRICT 1
#endif

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

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

// otherweise I get al lot of useless warnings about unused identifiers
#	pragma option -w-use		// xxx declared but never used
#	pragma option -w-par		// parameter xxx never used

#	if __BORLANDC__ == 0x621
#		pragma option -w-pch
#		pragma option -w-eff
#		pragma option -w-8005
#	endif
#endif

#ifdef _MSC_VER

#	pragma warning( disable: 4100 )	// 'xxx': Unreferenzierter formaler Parameter

#endif

/* --------------------------------------------------------------------- */
/* ----- constants ----------------------------------------------------- */
/* --------------------------------------------------------------------- */

#define nullptr ((void *)0)

/* --------------------------------------------------------------------- */
/* ----- type definitions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

typedef int (*fpSSL_library_init)(void);
typedef int (*fpOPENSSL_init_ssl)(void);

typedef void (*fpSSL_load_error_strings)(void);
typedef SSL_METHOD * (*fpSSLv23_method)(void);
typedef SSL_METHOD * (*fpTLS_method)(void);

typedef SSL_CTX *(*fpSSL_CTX_new)(const SSL_METHOD *meth);
typedef int	(*fpSSL_CTX_use_certificate_chain_file)(SSL_CTX *ctx, const char *file);
typedef void (*fpSSL_CTX_set_default_passwd_cb)(SSL_CTX *ctx, pem_password_cb *cb);
typedef int	(*fpSSL_CTX_use_PrivateKey_file)(SSL_CTX *ctx, const char *file, int type);
typedef int (*fpSSL_CTX_load_verify_locations)(SSL_CTX *ctx, const char *CAfile,
	const char *CApath);
typedef SSL *(*fpSSL_new)(const SSL_CTX *ctx);
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

typedef unsigned long (*fpERR_get_error_line_data)(const char **file, int *line, const char **data, int *flags);

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

static HINSTANCE s_ssleay = NULL, s_libeay = NULL;

static fpSSL_library_init SSL_library_initPtr					= NULL;
static fpOPENSSL_init_ssl OPENSSL_init_sslPtr					= NULL;

static fpSSL_load_error_strings SSL_load_error_stringsPtr		= NULL;
static fpSSLv23_method SSLv23_methodPtr							= NULL;
static fpTLS_method TLS_methodPtr								= NULL;
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
static fpERR_get_error_line_data ERR_get_error_line_dataPtr		= NULL;

/* --------------------------------------------------------------------- */
/* ----- exported datas ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- module functions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

static void initFunctionPointers( void )
{
	if( s_ssleay )
	{
		SSL_library_initPtr = (fpSSL_library_init)GetProcAddress( s_ssleay, "SSL_library_init" );
		OPENSSL_init_sslPtr = (fpOPENSSL_init_ssl)GetProcAddress( s_ssleay, "OPENSSL_init_ssl" );


		SSL_load_error_stringsPtr = (fpSSL_load_error_strings)GetProcAddress( s_ssleay, "SSL_load_error_strings" );
		SSLv23_methodPtr = (fpSSLv23_method)GetProcAddress( s_ssleay, "SSLv23_method" );
		TLS_methodPtr = (fpTLS_method)GetProcAddress( s_ssleay, "TLS_method" );
		SSL_CTX_newPtr = (fpSSL_CTX_new)GetProcAddress( s_ssleay, "SSL_CTX_new" );
		SSL_CTX_use_certificate_chain_filePtr = (fpSSL_CTX_use_certificate_chain_file)GetProcAddress( s_ssleay, "SSL_CTX_use_certificate_chain_file" );
		SSL_CTX_set_default_passwd_cbPtr = (fpSSL_CTX_set_default_passwd_cb)GetProcAddress( s_ssleay, "SSL_CTX_set_default_passwd_cb" );
		SSL_CTX_use_PrivateKey_filePtr = (fpSSL_CTX_use_PrivateKey_file)GetProcAddress( s_ssleay, "SSL_CTX_use_PrivateKey_file" );
		SSL_CTX_load_verify_locationsPtr = (fpSSL_CTX_load_verify_locations)GetProcAddress( s_ssleay, "SSL_CTX_load_verify_locations" );
		SSL_newPtr = (fpSSL_new)GetProcAddress( s_ssleay, "SSL_new" );
		SSL_set_bioPtr = (fpSSL_set_bio)GetProcAddress( s_ssleay, "SSL_set_bio" );
		SSL_connectPtr = (fpSSL_connect)GetProcAddress( s_ssleay, "SSL_connect" );
		SSL_writePtr = (fpSSL_write)GetProcAddress( s_ssleay, "SSL_write" );
		SSL_readPtr = (fpSSL_read)GetProcAddress( s_ssleay, "SSL_read" );
		SSL_get_errorPtr = (fpSSL_get_error)GetProcAddress( s_ssleay, "SSL_get_error" );
		SSL_shutdownPtr = (fpSSL_shutdown)GetProcAddress( s_ssleay, "SSL_shutdown" );
		SSL_CTX_freePtr = (fpSSL_CTX_free)GetProcAddress( s_ssleay, "SSL_CTX_free" );
	}
	if( s_libeay )
	{
		BIO_new_socketPtr = (fpBIO_new_socket)GetProcAddress( s_libeay, "BIO_new_socket" );
		ERR_error_stringPtr = (fpERR_error_string)GetProcAddress( s_libeay, "ERR_error_string" );
		ERR_get_error_line_dataPtr = (fpERR_get_error_line_data)GetProcAddress( s_libeay, "ERR_get_error_line_data" );
	}
}

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

#ifndef SSL_library_init
int SSL_library_init( void )
{
	if( !s_ssleay && !s_libeay )
	{
		s_ssleay = LoadLibrary( "SSLEAY32.DLL" );
		s_libeay = LoadLibrary( "LIBEAY32.DLL" );

		initFunctionPointers();
	}
	if( SSL_library_initPtr )
		return SSL_library_initPtr();
	else
		return -1;
}
#endif

#ifndef SSL_load_error_strings
void SSL_load_error_strings( void )
{
	if( SSL_load_error_stringsPtr )
		SSL_load_error_stringsPtr();
}
#endif

#ifndef SSLv23_method
SSL_METHOD *SSLv23_method(void)
{
	if( SSLv23_methodPtr )
		return SSLv23_methodPtr();
	else
		return NULL;
}
#endif

SSL_CTX *SSL_CTX_new(const SSL_METHOD *meth)
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
	if( ERR_error_stringPtr )
		return ERR_error_stringPtr( e, buf );
	else
		return NULL;
}

unsigned long ERR_get_error_line_data(const char **file, int *line,
                                      const char **data, int *flags)
{
	if( ERR_get_error_line_dataPtr )
		return ERR_get_error_line_dataPtr( file, line, data, flags );
	else
		return 0;
}

#if OPENSSL_VERSION_MAJOR >= 3
int OPENSSL_sk_num(const OPENSSL_STACK *stack)
{
	return 0;
}

void *OPENSSL_sk_value(const OPENSSL_STACK *stack, int val)
{
	return nullptr;
}

OPENSSL_STACK *OPENSSL_sk_new(OPENSSL_sk_compfunc cmp)
{
	return nullptr;
}

OPENSSL_STACK *OPENSSL_sk_new_null(void)
{
	return nullptr;
}

OPENSSL_STACK *OPENSSL_sk_new_reserve(OPENSSL_sk_compfunc c, int n)
{
	return nullptr;
}

int OPENSSL_sk_reserve(OPENSSL_STACK *st, int n)
{
	return 0;
}

void OPENSSL_sk_free(OPENSSL_STACK *stack)
{
}

void *OPENSSL_sk_delete(OPENSSL_STACK *st, int loc)
{
	return nullptr;
}

int OPENSSL_sk_push(OPENSSL_STACK *st, const void *data)
{
	return 0;
}

void OPENSSL_sk_zero(OPENSSL_STACK *st)
{
}

void OPENSSL_sk_pop_free(OPENSSL_STACK *st, OPENSSL_sk_freefunc func)
{
}

void *OPENSSL_sk_delete_ptr(OPENSSL_STACK *st, const void *p)
{
	return nullptr;
}

int OPENSSL_sk_unshift(OPENSSL_STACK *st, const void *data)
{
	return 0;
}

void *OPENSSL_sk_shift(OPENSSL_STACK *st)
{
	return nullptr;
}

void *OPENSSL_sk_pop(OPENSSL_STACK *st)
{
	return nullptr;
}

void *OPENSSL_sk_set(OPENSSL_STACK *st, int i, const void *data)
{
	return nullptr;
}

OPENSSL_STACK *OPENSSL_sk_set_thunks(OPENSSL_STACK *st, OPENSSL_sk_freefunc_thunk f_thunk)
{
	return nullptr;
}

int OPENSSL_sk_insert(OPENSSL_STACK *sk, const void *data, int where)
{
	return 0;
}

int OPENSSL_sk_find(OPENSSL_STACK *st, const void *data)
{
	return 0;
}

int OPENSSL_sk_find_ex(OPENSSL_STACK *st, const void *data)
{
	return 0;
}

int OPENSSL_sk_find_all(OPENSSL_STACK *st, const void *data, int *pnum)
{
	return 0;
}

void OPENSSL_sk_sort(OPENSSL_STACK *st)
{
}

OPENSSL_STACK *OPENSSL_sk_deep_copy(
	const OPENSSL_STACK *s, OPENSSL_sk_copyfunc c, OPENSSL_sk_freefunc f
)
{
	return nullptr;
}

OPENSSL_sk_compfunc OPENSSL_sk_set_cmp_func(
	OPENSSL_STACK *sk, OPENSSL_sk_compfunc cmp
)
{
	return nullptr;
}

OPENSSL_STACK *OPENSSL_sk_dup(const OPENSSL_STACK *st)
{
	return nullptr;
}

int OPENSSL_sk_is_sorted(const OPENSSL_STACK *st)
{
	return 0;
}

__owur const SSL_METHOD *TLS_method(void)
{
	if( TLS_methodPtr )
		return TLS_methodPtr();
	else
		return nullptr;
}

int OPENSSL_init_ssl(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings)
{
	if( !s_ssleay && !s_libeay )
	{
		s_ssleay = LoadLibrary( "libssl-3.dll" );
		s_libeay = LoadLibrary( "libcrypto-3.DLL" );
		initFunctionPointers();
	}

	if( OPENSSL_init_sslPtr )
		return OPENSSL_init_sslPtr();
	else
		return -1;
}

#endif	// #if OPENSSL_VERSION_MAJOR >= 3

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif
