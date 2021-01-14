/*
		Project:		GAKLIB
		Module:			optional.h
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

#ifndef GAK_OPTIONAL_H
#define GAK_OPTIONAL_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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

/**
	@brief An object of this class can optionaly contain an item of any type
	@tparam OBJ The type of the item this object can contain
*/
template <typename OBJ>
class Optional
{
	private:
	char	m_buffer[sizeof(OBJ)];
	bool	m_isSet;

	void clear()
	{
		if( m_isSet )
		{
			reinterpret_cast<OBJ*>(m_buffer)->~OBJ();
			m_isSet = false;
		}
	}
	void set( const OBJ &src )
	{
		if( !m_isSet )
		{
			new (m_buffer) OBJ( src );
			m_isSet = true;
		}
		else
		{
			*reinterpret_cast<OBJ*>(m_buffer) = src;
		}
	}
	void set( const Optional<OBJ> &src )
	{
		if( !m_isSet && !src.m_isSet )
		{
			// do noting
		}
		else if( !m_isSet && src.m_isSet )
		{
			new (m_buffer) OBJ( *reinterpret_cast<const OBJ*>(src.m_buffer) );
			m_isSet = true;
		}
		else if( m_isSet && !src.m_isSet )
		{
			reinterpret_cast<OBJ*>(m_buffer)->~OBJ();
			m_isSet = false;
		}
		else
		{
			*reinterpret_cast<OBJ*>(m_buffer) = *reinterpret_cast<const OBJ*>(src.m_buffer);
		}
	}
	public:
	/// Creates an empty Optional item
	Optional()
	{
		m_isSet = false;
	}
	/**
		@brief Creates a filled Optional item
		@param [in] src The source of the item this object will contain
	*/
	Optional( const OBJ &src )
	{
		m_isSet = false;
		set( src );
	}
	/// A copy constructor
	Optional( const Optional<OBJ> &src )
	{
		m_isSet = false;
		set( src );
	}
	/**
		@brief Assigns a new item to the Optional object
		@param [in] src The source of the new item
		@return The Optional itself
	*/
	const Optional<OBJ> &operator = ( const OBJ &src )
	{
		set( src );

		return *this;
	}
	/**
		@brief Assigns a new item to the Optional object
		@param [in] src The optional source of the new item
		@return The Optional itself
	*/
	const Optional<OBJ> &operator = ( const Optional<OBJ> &src )
	{
		set( src );

		return *this;
	}
	/// Deletes this item
	~Optional()
	{
		clear();
	}

	/**
		@brief Sets this optional to the Default value of the item type
	*/
	Optional<OBJ> &setDefault ( void )
	{
		set( OBJ() );

		return *this;
	}
	/// Returns true if this object contains an item
	bool isPresent( void ) const
	{
		return m_isSet;
	}
	/// Returns true if this object does not contain an item
	bool operator !( void ) const
	{
		return !m_isSet;
	}
	/// Returns true if this object contains an item
	operator bool ( void ) const
	{
		return m_isSet;
	}
	/**
		@brief Returns the item this optional contains or a default
		@param [in] src The default value returned if the optional is empty
		@return The value this optional contains or the default
	*/
	const OBJ &orElse( const OBJ &src ) const
	{
		return m_isSet ? *reinterpret_cast<const OBJ*>(m_buffer) : src;
	}

	/**
		@brief Returns the item this optional contains
		@return The value this optional contains
		@throws OptionalError if this optional is empty
	*/
	const OBJ &get( void ) const
	{
		if( !m_isSet )
		{
			throw OptionalError();
		}

		return *reinterpret_cast<const OBJ*>(m_buffer);
	}
	/**
		@brief Returns the item this optional contains
		@return The value this optional contains
		@throws OptionalError if this optional is empty
	*/
	OBJ &get( void )
	{
		if( !m_isSet )
		{
			throw OptionalError();
		}

		return *reinterpret_cast<OBJ*>(m_buffer);
	}

	/**
		@brief Creates a filled Optional item
		@param [in] src The source of the item this object will contain
		@return the new Optional
	*/
	static Optional<OBJ> of( const OBJ &src )
	{
		return Optional<OBJ>( src );
	}
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

} // namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_OPTIONAL_H