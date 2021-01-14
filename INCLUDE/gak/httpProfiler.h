/*
		Project:		GAKLIB
		Module:			httpProfiler.h
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

#ifndef HTTP_PROFILER_H
#define HTTP_PROFILER_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/http.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{
namespace net
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/// this class is a special HTTP client that can be used to perform a server profiling
class HTTPprofiler : public HTTPrequest
{
	Array<STRING>	script;

	void (*logFunc)( void *, const char *url, std::size_t numFiles );
	void *logData;

	private:
	size_t	GetTree(
		html::Document *baseDoc,
		const char *url,
		int buffsize,
		bool anchorTags,
		bool noExternalLinks
	);
	size_t includeData(
		html::Document *baseDoc,
		STRING &baseUrl,
		xml::Element *theElement,
		int buffSize,
		bool anchorTags,
		bool noExternalLinks
	);

	public:
	/// creates a new profiler
	HTTPprofiler()
	{
		logFunc = NULL;
		logData = NULL;
	}
	/// installs a callback that is called after each performed request
	void setLogFunc( void (*logFunc)( void *, const char *url, std::size_t numFiles), void *logData )
	{
		this->logFunc = logFunc;
		this->logData = logData;
	}
	/**
		@brief performs a new GET request

		all requests will be stored in a script buffer

		@param [in] url the start url
		@param [in] complete if true all images, scripts, frames etc. will be loaded, too
		@param [in] anchorTags if true all links withing the document will be loaded, too
		@param [in] noExternalLinks if true all links to other servers won't be loaded
		@param [in] clearExpiredOnly if false the cache will be cleared, if true expired documents will be deleted, only
		@param [in] buffsize the size of the I/O buffer to use
		@return the total number of bytes read
	*/
	size_t	Get(
		const char *url, bool complete,
		bool anchorTags, bool noExternalLinks,
		bool clearExpiredOnly, int buffsize=10240
	);
	/**
		@copydoc HTTPrequest::Get

		the URL will be added to the internal script
	*/
	size_t	Get( const char *url, int buffersize=10240  );

	/// clears all entries in the stored script
	void initScript()
	{
		script.clear();
	}

	/**
		@brief plays the recorded script
		@param [in] clearExpiredOnly if false the cache will be cleared, if true expired documents will be deleted, only
		@return the total number of bytes read
	*/
	size_t	playScript( bool clearExpiredOnly );
};

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
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

}	// namespace net
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -p.
#	pragma option -a.
#endif

#endif
