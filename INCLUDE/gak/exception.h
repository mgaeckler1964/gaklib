/*
		Project:		GAKLIB
		Module:			exception.h
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

#ifndef GAK_EXCEPTIONS_H
#define GAK_EXCEPTIONS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <errno.h>

#include <exception>

#include <gak/string.h>

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

/// @brief Base class for all exceptions thrown by the library
class LibraryException : public std::exception
{
	private:
	STRING			m_errorText;

	public:
	/// error text that can be used if a NULL_STRING caused the error
	static const STRING NULL_STRING_ERROR;
	/// error text that can be used if an EMPTY_STRING caused the error
	static const STRING EMPTY_STRING_ERROR;

	protected:
	public:
	LibraryException( const STRING &text ) : m_errorText( text )
	{
	}
	virtual ~LibraryException() throw();

	public:
	/**
		@brief adds an error text to the error message
		@param [in] iErrText the error text to add
		@return this 
	*/
	LibraryException &addErrorText( const STRING &iErrText )
	{
		STRING errText = iErrText;
		errText.stripBlanks();

		if( !errText.isEmpty() )
		{
			m_errorText += ": " + errText;
		}
		return *this;
	}
	LibraryException &addErrorText( const std::exception &cause )
	{
		return addErrorText( STRING("Caused by: ") + cause.what() );
	}

#ifdef _Windows
	/**
		@brief adds an error text to the error message
		@param [in] errCode the Windows error code
		@return this 
	*/
	LibraryException &addNTerror( unsigned long errCode=GetLastError() )
	{
		char			*lpMsgBuf;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		addErrorText( STRING("Caused by: ") + lpMsgBuf );
		LocalFree( (LPVOID)lpMsgBuf );

		return *this;
	}
#endif
	/**
		@brief adds an error text to the error message
		@param [in] errCode the C-standard error code
		@return this 
	*/
	LibraryException &addCerror( int errCode=errno )
	{
		return addErrorText( STRING("Caused by: ") + strerror( errCode ) );
	}

	/// returns the error message of this exception
	virtual const char * what() const throw();
};

/// @brief Base class for memory errors
class MemoryError : public LibraryException
{
	protected:
	MemoryError( const STRING &errText ) : LibraryException( errText )
	{
	}
};

/// @brief Exception thrown if memory allocation failed
class AllocError : public MemoryError
{
	public:
	AllocError() : MemoryError( "Memory Allocation Failed" ) 
	{
	}
};

/// @brief Exception thrown if an array index was out of bounds
class IndexError : public MemoryError
{
	public:
	IndexError() : MemoryError( "Array Index Range Error" ) 
	{
	}
};

/// @brief Exception thrown when trying to pop from an empty Queue
class QueueEmptyError : public MemoryError
{
	public:
	QueueEmptyError() : MemoryError( "Queue Empty Error" ) 
	{
	}
};

/// @brief Exception thrown when trying to pop from an empty Stack
class StackEmptyError : public MemoryError
{
	public:
	StackEmptyError() : MemoryError( "Stack Empty Error" ) 
	{
	}
};

/// @brief Exception thrown when trying to access a link that does not exist in the Graph
class LinkNotFoundError : public MemoryError
{
	public:
	LinkNotFoundError() : MemoryError( "Link Not Found Error" ) 
	{
	}
};

/// @brief Exception thrown when trying to addd a link that exists in the Graph, allready
class LinkExistsError : public MemoryError
{
	public:
	LinkExistsError() : MemoryError( "Link Allready Exists" ) 
	{
	}
};

/// @brief Exception thrown when trying to access a node that does not exist in the Graph
class NodeNotFoundError : public MemoryError
{
	public:
	NodeNotFoundError() : MemoryError( "Node Not Found Error" ) 
	{
	}
};

/// @brief Exception thrown when trying to addd a node that exists in the Graph, allready
class NodeExistsError : public MemoryError
{
	public:
	NodeExistsError() : MemoryError( "Node Allready Exists" ) 
	{
	}
};

/// @brief Exception thrown when trying to retrieve a value from an empty Optional
class OptionalError : public MemoryError
{
	public:
	OptionalError() : MemoryError( "Optional Error" ) 
	{
	}
};

/// @brief Exception thrown when trying to read data from a file with an unkown or invalid header (magic)
class BadHeaderError : public LibraryException
{
	public:
	BadHeaderError( const STRING &fileName=NULL_STRING ) : LibraryException( "Bad Header" )
	{
		if( !fileName.isEmpty() )
		{
			addErrorText( fileName );
		}
	}
};

/**
	@brief Exception thrown when an invalid RSA key was used
	@see CryptoRSA, CryptoShared
*/
class BadKeyError : public LibraryException
{
	public:
	BadKeyError() : LibraryException( "Bad Key" )
	{
	}
};

/// @brief Base class for all network errors
class NETerror : public LibraryException
{
	protected:
	NETerror( const STRING &errText, const STRING &url ) : LibraryException( errText )
	{
		if( !url.isEmpty() )
			addErrorText( url );
		if( errno )
			addCerror();
	}
};

/// @brief Base class for all I/O errors
class IOerror : public LibraryException
{
	protected:
	IOerror( const STRING &errText, const STRING &fileName ) : LibraryException( errText )
	{
		if( !fileName.isEmpty() )
			addErrorText( fileName );
		if( errno )
			addCerror();
	}
	IOerror( const STRING &errText, const STRING &fileName1, const STRING &fileName2 ) : LibraryException( errText )
	{
		if( !fileName1.isEmpty() )
			addErrorText( fileName1 );
		if( !fileName2.isEmpty() )
			addErrorText( fileName2 );
		if( errno )
			addCerror();
	}
};

/**
	@brief Exception thrown if a hard link could not be created
	@see flink, dlink
*/
class LinkError : public IOerror
{
	public:
	LinkError( const STRING &source, const STRING &target ) 
	: IOerror( "File Link Creation Failed", source + "->" + target )
	{
#ifdef _Windows
		addNTerror();
#endif
	}
};

/**
	@brief Exception thrown if file attributes could not be read
	@see GetAttributes
*/
class AttributeReadError : public IOerror
{
	public:
	AttributeReadError( const STRING &file ) : IOerror( "File Attributes Read Failed", file )
	{
#ifdef _Windows
		addNTerror();
#endif
	}
};

/**
	@brief Exception thrown if file attributes could not be written
	@see SetAttributes
*/
class AttributeWriteError : public IOerror
{
	public:
	AttributeWriteError( const STRING &file ) : IOerror( "File Attributes Write Failed", file )
	{
#ifdef _Windows
		addNTerror();
#endif
	}
};

/**
	@brief Exception thrown if a directory could not be created
	@see makeDirectory
*/
class MakeDirectoryError : public IOerror
{
	public:
	MakeDirectoryError( const STRING &dirName ) : IOerror( "Directory Creation Failed", dirName )
	{
	}
};

///	@brief Exception thrown if file status could not be read
class StatReadError : public IOerror
{
	public:
	StatReadError( const STRING &fileName ) : IOerror( "File Info For Read Failed", fileName )
	{
	}
};

///	@brief Exception thrown if file status could not be written
class StatWriteError : public IOerror
{
	public:
	StatWriteError( const STRING &fileName ) : IOerror( "File Info For Write Failed", fileName )
	{
	}
};

///	@brief Exception thrown if a file could not be open for reading
class OpenReadError : public IOerror
{
	public:
	OpenReadError( const STRING &fileName ) : IOerror( "File Open For Read Failed", fileName )
	{
	}
};

///	@brief Exception thrown if a file could not be open for writting
class OpenWriteError : public IOerror
{
	public:
	OpenWriteError( const STRING &fileName ) : IOerror( "File Open For Write Failed", fileName )
	{
	}
};

///	@brief Exception thrown if a file could not be written
class WriteError : public IOerror
{
	public:
	WriteError( const STRING &fileName=NULL_STRING ) : IOerror( "File Write Failed", fileName )
	{
	}
};

///	@brief Exception thrown if a file could not be read
class ReadError : public IOerror
{
	public:
	ReadError( const STRING &fileName=NULL_STRING ) : IOerror( "File Read Failed", fileName )
	{
	}
};

///	@brief Exception thrown if a file date could not be read or written
class DateError : public IOerror
{
	public:
	DateError( const STRING &fileName=NULL_STRING ) : IOerror( "File Date Read/Write Failed", fileName )
	{
	}
};

///	@brief Exception thrown if a file could not be removed
class RemoveError : public IOerror
{
	public:
	RemoveError( const STRING &fileName=NULL_STRING ) : IOerror( "Cannot remove file", fileName )
	{
	}
};

///	@brief Exception thrown if a file could not be removed
class RenameError : public IOerror
{
	public:
	RenameError( const STRING &fileName1=NULL_STRING, const STRING &fileName2=NULL_STRING  ) : IOerror( "Cannot rename file", fileName1, fileName2 )
	{
	}
};

///	@brief Base class for mathematic errors
class MathError : public LibraryException
{
	protected:
	MathError( const STRING &errText ) : LibraryException( errText )
	{
	}
};

/**
	@brief Exception thrown in case of an operator error
	@see ExpressionEvaluator
*/
class OperatorError : public MathError
{
	public:
	OperatorError() : MathError( "Operator Error" ) 
	{
	}
};

/**
	@brief Exception thrown in case of an illegal parenthesis
	@see ExpressionEvaluator
*/
class UnexpectedParenthesisError : public MathError
{
	public:
	UnexpectedParenthesisError() : MathError( "Unexpected ')'" ) 
	{
	}
};

/**
	@brief Exception thrown in case of a missing parenthesis
	@see ExpressionEvaluator
*/
class MissingParenthesisError : public MathError
{
	public:
	MissingParenthesisError() : MathError( "Missing ')'" ) 
	{
	}
};

/**
	@brief Exception thrown in case of an unknown function
	@see ExpressionEvaluator
*/
class UnknownFunctionError : public MathError
{
	public:
	UnknownFunctionError() : MathError( "Unknown Function" ) 
	{
	}
};

/**
	@brief Exception thrown in case of a bad function call
	@see ExpressionEvaluator
*/
class IllegalNumberOfParamsError : public MathError
{
	public:
	IllegalNumberOfParamsError() : MathError( "Illegal Number of Function Parameters" ) 
	{
	}
};

/**
	@brief Exception thrown in case of a division by zero
	@see ExpressionEvaluator
*/
class DivisionByZeroError : public MathError
{
	public:
	DivisionByZeroError() : MathError( "Division By Zero" ) 
	{
	}
};

/**
	@brief Exception thrown in case of an invalid result
	@see ExpressionEvaluator
*/
class InvalidResultError : public MathError
{
	public:
	InvalidResultError() : MathError( "Invalid Result" ) 
	{
	}
};

/**
	@brief Exception thrown in case of a bad number format
	@see getValue
*/
class BadNumericFormatError : public MathError
{
	public:
	BadNumericFormatError( const STRING &intString, const STRING &errText = "Bad Numeric Format" ) 
		: MathError( errText ) 
	{
		addErrorText( intString );
	}
};

/**
	@brief Exception thrown in case of an integer overflow
	@see getValue
*/
class IntegerOverflowError : public BadNumericFormatError
{
	public:
	IntegerOverflowError( const STRING &intString ) : BadNumericFormatError( intString, "Integer Overflow" ) 
	{
	}
};

/**
	@brief Exception thrown in case of an integer underflow
	@see getValue
*/
class IntegerUnderflowError : public BadNumericFormatError
{
	public:
	IntegerUnderflowError( const STRING &intString ) 
	: BadNumericFormatError( intString, "Integer Underflow" ) 
	{
	}
};

/**
	@brief Exception thrown in case of an exponent overflow
	@see getValue
*/
class ExponentOverflowError : public BadNumericFormatError
{
	public:
	ExponentOverflowError( const STRING &intString ) 
	: BadNumericFormatError( intString, "Exponent Overflow" ) 
	{
	}
};

/**
	@brief Exception thrown in case of an exponent underflow
	@see getValue
*/
class ExponentUnderflowError : public BadNumericFormatError
{
	public:
	ExponentUnderflowError( const STRING &intString ) 
	: BadNumericFormatError( intString, "Exponent Underflow" ) 
	{
	}
};

/**
	@brief Exception thrown in case of a float overflow
	@see getValue
*/
class FloatOverflowError : public BadNumericFormatError
{
	public:
	FloatOverflowError( const STRING &intString ) 
	: BadNumericFormatError( intString, "Float Overflow" ) 
	{
	}
};

/**
	@brief Exception thrown in case of a float underflow
	@see getValue
*/
class FloatUnderflowError : public BadNumericFormatError
{
	public:
	FloatUnderflowError( const STRING &intString ) 
	: BadNumericFormatError( intString, "Float Underflow" ) 
	{
	}
};

/// @brief Exception thrown in case of some invalid conversions
class InvalidConversionError : public LibraryException
{
	public:
	InvalidConversionError() : LibraryException( "Invalid Conversion" ) 
	{
	}
};

/// @brief Exception thrown if ellapsed time exceeded the specified timeout
class TimeoutException : public LibraryException
{
	public:
	TimeoutException() : LibraryException( "Timeout" ) 
	{
	}
};

namespace Internal
{
	/// @brief Exception thrown if method not yet implemented
	class xNotImplementedException : public LibraryException
	{
		public:
		xNotImplementedException(const char *fileName, size_t line );
	};
}

#define NotImplementedException() Internal::xNotImplementedException(__FILE__,  __LINE__)

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

#endif	// GAK_EXCEPTIONS_H

