/*
		Project:		GAKLIB
		Module:			physic.h
		Description:	Some physical constants and methods
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

#ifndef GAK_PHYSIC_H
#define GAK_PHYSIC_H

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
namespace physic
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //
	
const double GRAVITATION_CONST	= 6.67430e-11;
const double MASS_MOON			= 7.34581e22;
const double RADIUS_MOON		= 1737400;	

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	the gravity acceleration from one small object to a large object (planet)
*/
/**
	calculate the gravity from one mass to another large object
	@param distance the distance from center to center
	@param mass the planet's mass [kg]
*/
inline double gravityAccelerationFromDistance( double distance, double mass )
{
	return GRAVITATION_CONST * mass / (distance*distance);
}

/**
	calculate the gravity acceleration from one mass to another large object
	@param height the height from the small object above the ground of the planet [m]
	@param radius the planet's radius [m]
	@param mass the planet's mass [kg]
*/
inline double gravityAccelerationFromHeight( double height, double radius, double mass )
{
	const double distance = radius + height;
	return gravityAccelerationFromDistance( distance, mass );
}

/// calculate the medium gravity acceleration on the moon for a specific height
inline double moonAcceleration( double height )
{
	return gravityAccelerationFromHeight( height, RADIUS_MOON, MASS_MOON );
}

/*
	the gravity between two objects
*/
/**
	calculate the gravity acceleration between two mass objects
	@param dist the distance from center to center
	@param mass1 the first planet's mass [kg]
	@param mass2 the second planet's mass [kg]
*/
inline double gravityAccelerationFromDistance( double distance, double mass1, double mass2 )
{
	return gravityAccelerationFromDistance(distance, mass1) + gravityAccelerationFromDistance(distance, mass2);
}

/**
	calculate the gravity force between two mass objects
	@param dist the distance from center to center
	@param mass1 the first planet's mass [kg]
	@param mass2 the second planet's mass [kg]
*/
inline double gravityForce( double distance, double mass1, double mass2 )
{
	return GRAVITATION_CONST * mass1 * mass2 / (distance*distance);
}

/*
	movement
*/
/// calculate the distance for a linear movement
inline double distance( double speed, double time )
{
	return speed * time;
}

/// calculate the distance for a accelerated movement
inline double distance( double startSpeed, double accel, double time )
{
	return distance(startSpeed, time) + (0.5 * accel * time * time);
}

/// calculate the accelerated end speed
inline double speed( double startSpeed, double accel, double time )
{
	return startSpeed + accel * time;
}

/// calculate the acceleration from mass and force
inline double acceleration( double mass, double force )
{
	return force/mass;
}

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

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

}	// namespace physic
}	//namespace gak

#endif	// GAK_PHYSIC_H
