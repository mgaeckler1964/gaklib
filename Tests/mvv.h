/*
		Project:		GAKLIB
		Module:			mvv.h
		Description:	
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

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

#ifndef MVV_H
#define MVV_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/geoGraph.h>

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

static	const short SBAHN_LAYER = 0;
static	const short UBAHN_LAYER = 1;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

enum CostType
{
	ctBY_DISTANCE, ctBY_SPEED
};

enum Bahnhoefe
{
	// Stammstrecke OST
	BergAmLaim, LeuchtenbergRing, 
	// Stammstrecke Tunnel
	Ostbahnhof_S, Ostbahnhof_U, Marienplatz_S, Marienplatz_U, Stachus_S, Stachus_U, Hauptbahnhof_S, Hauptbahnhof_U1, Hauptbahnhof_U4, 
	// Stammstrecke WEST
	DonnersbergerBruecke, Laim, Pasing,
	// S1 West
	Freising, Flughafen, Besucherpark, Neufarn, Felmoching_S, Felmoching_U, Moosach_S, Moosach_U,
	// S2 West
	Petershausen, Altomuenster, Dachau,
	// S3 West
	Mammendorf,
	// S4 West
	Geltendorf,
	// S8/S6 West
	Hersching, Tutzing, Westkreuz, 
	// S7 West
	Wolfrathshausen, Siemenswerke, Harras_S, Harras_U, Heimeranplatz_S, Heimeranplatz_U,
	// S3 Ost
	Holzkirchen, Giesing_S,  Giesing_U,
	// S7 Ost
	Kreuzstrasse, Neuperlach_Sued,
	// S4 Ost
	Ebersberg, Trudering_S, Trudering_U,
	// S2 Ost
	Erding,

	// U1
	OEZ_U1, SendlingerTor_U1, Kolumbusplatz, Mangfallplatz,
	// U2
	Scheidplatz, InnsbruckerRing, MessestadtOst,
	// U3
	FuerstenriedWest, Obersendling, Implerstrasse, SendlingerTor_U3, Odeonsplatz_U3, MuenchnerFreiheit, OEZ_U3,
	// U4
	Westendstrasse, Odeonsplatz_U4, MaxWeberPlatz, Arabellapark,
	// U5
	LaimerPlatz,
	// U6
	KlinikumGrosshadern, GarchingForschungszentrum,
	// U7
	Westfriedhof, NeuperlachZentrum,
	// U8
	Olympiazentrum,

	// Impossible
	IngolstadtNord
};

struct Station
{
	typedef Bahnhoefe key_type;

	const char					*name;
	math::GpsPosition<double>	position;

	Station()
	{
	}
	Station( const char *name, const math::GpsPosition<double> &position ) : name( name ), position(position)
	{
	}
	const math::GeoPosition<double> &getPosition() const
	{
		return position;
	}
	math::tileid_t getTileID() const
	{
		return position.getTileID();
	}
};

struct Connection
{
	typedef long key_type;

	double	length,
			time;

	Connection()
	{
	}
	Connection(double length, double speed) : length( length ), time( length/speed )
	{
	}
	Connection( const Station &from, const Station &to, double speed )
	: length( getDistance(from.position, to.position) ), time(length/speed)
	{
	}
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class MvvGraph : public GeoGraph<Station, Connection, GraphTree, GeoBTree>
{
	public:
	double		minLon, maxLon, minLat, maxLat;

	private:
	void makeConnection( int id, Bahnhoefe bhf1, Bahnhoefe bhf2, double speed )
	{
		const Station &start = getNode( bhf1 );
		minLon = math::min( minLon, start.getPosition().longitude );
		maxLon = math::max( maxLon, start.getPosition().longitude );

		minLat = math::min( minLat, start.getPosition().latitude );
		maxLat = math::max( maxLat, start.getPosition().latitude );

		const Station &end = getNode( bhf2 );
		minLon = math::min( minLon, end.getPosition().longitude );
		maxLon = math::max( maxLon, end.getPosition().longitude );

		minLat = math::min( minLat, end.getPosition().latitude );
		maxLat = math::max( maxLat, end.getPosition().latitude );

		addLink(
			id,
			Connection(
				getNode( bhf1 ),
				getNode( bhf2 ),
				speed
			),
			bhf1,
			bhf2
		);
		addLink(
			-id,
			Connection(
				getNode( bhf2 ),
				getNode( bhf1 ),
				speed
			),
			bhf2,
			bhf1
		);
	}
	void buildMVV()
	{
		const double CHANGE_DISTANCE = 100;
		const double CHANGE_SPEED = 1./3.6;
		const double S_BAHN_SPEED = 120/3.6;
		const double U_BAHN_SPEED = 80/3.6;
		int linkID = 0;

		const Connection CHANGE_CONNECTION( CHANGE_DISTANCE, CHANGE_SPEED );

		addNode( 
			SBAHN_LAYER, Freising, Station( 
				"Freising", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 44.668 ),
					math::ConvertDegree( 48, 23.705 )
				) 
			) 
		);
		addNode( 
			SBAHN_LAYER, Flughafen, Station( 
				"Flughafen", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 47.219 ),
					math::ConvertDegree( 48, 21.230 )
				) 
			) 
		);
		addNode( 
			SBAHN_LAYER, Besucherpark, Station( 
				"Besucherpark", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 44.512 ),
					math::ConvertDegree( 48, 21.017 )
				) 
			) 
		);
		
		addNode( 
			SBAHN_LAYER, Neufarn, Station( 
				"Neufarn", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 39.643 ),
					math::ConvertDegree( 48, 19.280 )
				) 
			) 
		);
		makeConnection( ++linkID, Freising, Neufarn, S_BAHN_SPEED );
		makeConnection( ++linkID, Flughafen, Besucherpark, S_BAHN_SPEED );
		makeConnection( ++linkID, Besucherpark, Neufarn, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Felmoching_S, Station( 
				"Felmoching S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 32.487 ),
					math::ConvertDegree( 48, 12.832 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Felmoching_U, Station( 
				"Felmoching U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 32.487 ),
					math::ConvertDegree( 48, 12.832 )
				) 
			) 
		);
		makeConnection( ++linkID, Neufarn, Felmoching_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Moosach_S, Station( 
				"Moosach S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 30.379 ),
					math::ConvertDegree( 48, 10.817 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Moosach_U, Station( 
				"Moosach U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 30.379 ),
					math::ConvertDegree( 48, 10.817 )
				) 
			) 
		);
		makeConnection( ++linkID, Felmoching_S, Moosach_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Laim, Station( 
				"Laim", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 30.186 ),
					math::ConvertDegree( 48, 8.665 )
				) 
			) 
		);
		makeConnection( ++linkID, Moosach_S, Laim, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, DonnersbergerBruecke, Station( 
				"Donnersberger Brücke", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 32.089 ),
					math::ConvertDegree( 48, 8.552 )
				) 
			) 
		);
		makeConnection( ++linkID, Laim, DonnersbergerBruecke, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Hauptbahnhof_S, Station( 
				"Hauptbahnhof S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 33.651 ),
					math::ConvertDegree( 48, 8.457 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Hauptbahnhof_U1, Station( 
				"Hauptbahnhof U1/2", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 33.651 ),
					math::ConvertDegree( 48, 8.457 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Hauptbahnhof_U4, Station( 
				"Hauptbahnhof U4/5", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 33.651 ),
					math::ConvertDegree( 48, 8.457 )
				) 
			) 
		);
		makeConnection( ++linkID, DonnersbergerBruecke, Hauptbahnhof_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Stachus_S, Station( 
				"Stachus S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 33.929 ),
					math::ConvertDegree( 48, 8.364 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Stachus_U, Station( 
				"Stachus U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 33.929 ),
					math::ConvertDegree( 48, 8.364 )
				) 
			) 
		);
		makeConnection( ++linkID, Hauptbahnhof_S, Stachus_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Marienplatz_S, Station( 
				"Marienplatz S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 34.529 ),
					math::ConvertDegree( 48, 8.235 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Marienplatz_U, Station( 
				"Marienplatz U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 34.529 ),
					math::ConvertDegree( 48, 8.235 )
				) 
			) 
		);
		makeConnection( ++linkID, Stachus_S, Marienplatz_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Ostbahnhof_S, Station( 
				"Ostbahnhof S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 36.288 ),
					math::ConvertDegree( 48, 7.649 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Ostbahnhof_U, Station( 
				"Ostbahnhof U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 36.288 ),
					math::ConvertDegree( 48, 7.649 )
				) 
			) 
		);
		makeConnection( ++linkID, Marienplatz_S, Ostbahnhof_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Petershausen, Station( 
				"Petershausen", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 28.171 ),
					math::ConvertDegree( 48, 24.797 )
				) 
			) 
		);
		addNode( 
			SBAHN_LAYER, Altomuenster, Station( 
				"Altomünster", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 15.271 ),
					math::ConvertDegree( 48, 23.128 )
				) 
			) 
		);
		addNode( 
			SBAHN_LAYER, Dachau, Station( 
				"Dachau", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 26.651 ),
					math::ConvertDegree( 48, 15.296 )
				) 
			) 
		);
		makeConnection( ++linkID, Petershausen, Dachau, S_BAHN_SPEED );
		makeConnection( ++linkID, Altomuenster, Dachau, S_BAHN_SPEED );
		makeConnection( ++linkID, Dachau, Laim, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, LeuchtenbergRing, Station( 
				"Leuchtenberg Ring", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 36.949 ),
					math::ConvertDegree( 48, 8.070 )
				) 
			) 
		);
		makeConnection( ++linkID, Ostbahnhof_S, LeuchtenbergRing, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, BergAmLaim, Station( 
				"Berg Am Laim", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 37.994 ),
					math::ConvertDegree( 48, 8.113 )
				) 
			) 
		);
		makeConnection( ++linkID, LeuchtenbergRing, BergAmLaim, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Erding, Station( 
				"Erding", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 54.695 ),
					math::ConvertDegree( 48, 18.203 )
				) 
			) 
		);
		makeConnection( ++linkID, BergAmLaim, Erding, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Mammendorf, Station( 
				"Mammendorf", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 10.466 ),
					math::ConvertDegree( 48, 13.033 )
				) 
			) 
		);
		addNode( 
			SBAHN_LAYER, Pasing, Station( 
				"Pasing", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 27.660 ),
					math::ConvertDegree( 48, 9.014 )
				) 
			) 
		);
		makeConnection( ++linkID, Mammendorf, Pasing, S_BAHN_SPEED );
		makeConnection( ++linkID, Pasing, Laim, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Giesing_S, Station( 
				"Giesing S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 35.775 ),
					math::ConvertDegree( 48, 6.659 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Giesing_U, Station( 
				"Giesing U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 35.775 ),
					math::ConvertDegree( 48, 6.659 )
				) 
			) 
		);
		makeConnection( ++linkID, Ostbahnhof_S, Giesing_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Holzkirchen, Station( 
				"Holzkirchen", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 41.802 ),
					math::ConvertDegree( 47, 53.094 )
				) 
			) 
		);
		makeConnection( ++linkID, Giesing_S, Holzkirchen, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Geltendorf, Station( 
				"Geltendorf", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 2.243 ),
					math::ConvertDegree( 48, 6.380 )
				) 
			) 
		);
		makeConnection( ++linkID, Geltendorf, Pasing, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Trudering_S, Station( 
				"Trudering S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 39.811 ),
					math::ConvertDegree( 48, 7.570 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Trudering_U, Station( 
				"Trudering U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 39.811 ),
					math::ConvertDegree( 48, 7.570 )
				) 
			) 
		);
		makeConnection( ++linkID, BergAmLaim, Trudering_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Ebersberg, Station( 
				"Ebersberg", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 58.172 ),
					math::ConvertDegree( 48, 4.514 )
				) 
			) 
		);
		makeConnection( ++linkID, Trudering_S, Ebersberg, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Hersching, Station( 
				"Hersching", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 10.234 ),
					math::ConvertDegree( 47, 59.930 )
				) 
			) 
		);
		addNode( 
			SBAHN_LAYER, Westkreuz, Station( 
				"Westkreuz", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 26.631 ),
					math::ConvertDegree( 48, 8.939 )
				) 
			) 
		);
		makeConnection( ++linkID, Hersching, Westkreuz, S_BAHN_SPEED );
		makeConnection( ++linkID, Westkreuz, Pasing, S_BAHN_SPEED );
		makeConnection( ++linkID, LeuchtenbergRing, Besucherpark, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Tutzing, Station( 
				"Tutzing", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 16.377 ),
					math::ConvertDegree( 47, 54.447 )
				) 
			) 
		);
		makeConnection( ++linkID, Tutzing, Westkreuz, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Wolfrathshausen, Station( 
				"Wolfrathshausen", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 25.596 ),
					math::ConvertDegree( 47, 54.855 )
				) 
			) 
		);
		addNode( 
			SBAHN_LAYER, Siemenswerke, Station( 
				"Siemenswerke", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 31.976 ),
					math::ConvertDegree( 48, 5.725 )
				) 
			) 
		);
		makeConnection( ++linkID, Wolfrathshausen, Siemenswerke, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Harras_S, Station( 
				"Harras S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 32.186 ),
					math::ConvertDegree( 48, 7.029 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Harras_U, Station( 
				"Harras U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 32.186 ),
					math::ConvertDegree( 48, 7.029 )
				) 
			) 
		);
		makeConnection( ++linkID, Siemenswerke, Harras_S, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Heimeranplatz_S, Station( 
				"Heimeranplatz S", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 31.901 ),
					math::ConvertDegree( 48, 7.969 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Heimeranplatz_U, Station( 
				"Heimeranplatz U", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 31.901 ),
					math::ConvertDegree( 48, 7.969 )
				) 
			) 
		);
		makeConnection( ++linkID, Harras_S, Heimeranplatz_S, S_BAHN_SPEED );
		makeConnection( ++linkID, Heimeranplatz_S, DonnersbergerBruecke, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Neuperlach_Sued, Station( 
				"Neuperlach Süd", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 38.708 ),
					math::ConvertDegree( 48, 5.334 )
				) 
			) 
		);
		makeConnection( ++linkID, Giesing_S, Neuperlach_Sued, S_BAHN_SPEED );

		addNode( 
			SBAHN_LAYER, Kreuzstrasse, Station( 
				"Kreuzstrasse", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 45.562 ),
					math::ConvertDegree( 47, 55.028 )
				) 
			) 
		);
		makeConnection( ++linkID, Neuperlach_Sued, Kreuzstrasse, S_BAHN_SPEED );

		addNode( 
			UBAHN_LAYER, OEZ_U1, Station( 
				"OEZ U1", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 31.778 ),
					math::ConvertDegree( 48, 10.921 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, OEZ_U3, Station( 
				"OEZ U3", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 31.778 ),
					math::ConvertDegree( 48, 10.921 )
				) 
			) 
		);
		makeConnection( ++linkID, OEZ_U1, Hauptbahnhof_U1, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Hauptbahnhof_S, Hauptbahnhof_U1 );
		addLink(  -linkID, CHANGE_CONNECTION, Hauptbahnhof_U1, Hauptbahnhof_S );

		addNode( 
			UBAHN_LAYER, SendlingerTor_U1, Station( 
				"SendlingerTor U1/2", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 33.987 ),
					math::ConvertDegree( 48, 7.986 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, SendlingerTor_U3, Station( 
				"SendlingerTor U3/6", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 33.987 ),
					math::ConvertDegree( 48, 7.986 )
				) 
			) 
		);
		makeConnection( ++linkID, Hauptbahnhof_U1, SendlingerTor_U1, U_BAHN_SPEED );

		addNode( 
			UBAHN_LAYER, Kolumbusplatz, Station( 
				"Kolumbusplatz", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 34.555 ),
					math::ConvertDegree( 48, 7.154 )
				) 
			) 
		);
		makeConnection( ++linkID, SendlingerTor_U1, Kolumbusplatz, U_BAHN_SPEED );

		addNode( 
			UBAHN_LAYER, Mangfallplatz, Station( 
				"Mangfallplatz", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 34.868 ),
					math::ConvertDegree( 48, 5.864 )
				) 
			) 
		);
		makeConnection( ++linkID, Kolumbusplatz, Mangfallplatz, U_BAHN_SPEED );

		addLink( ++linkID, CHANGE_CONNECTION, Felmoching_S, Felmoching_U );
		addLink(  -linkID, CHANGE_CONNECTION, Felmoching_U, Felmoching_S );

		addNode( 
			UBAHN_LAYER, Scheidplatz, Station( 
				"Scheidplatz", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 34.388 ),
					math::ConvertDegree( 48, 10.347 )
				) 
			) 
		);
		makeConnection( ++linkID, Felmoching_U, Scheidplatz, U_BAHN_SPEED );
		makeConnection( ++linkID, Scheidplatz, Hauptbahnhof_U1, U_BAHN_SPEED );
		makeConnection( ++linkID, Kolumbusplatz, Giesing_U, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Giesing_S, Giesing_U );
		addLink(  -linkID, CHANGE_CONNECTION, Giesing_U, Giesing_S );


		addNode( 
			UBAHN_LAYER, InnsbruckerRing, Station( 
				"Innsbrucker Ring", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 37.222 ),
					math::ConvertDegree( 48, 7.241 )
				) 
			) 
		);
		makeConnection( ++linkID, Giesing_U, InnsbruckerRing, U_BAHN_SPEED );
		makeConnection( ++linkID, InnsbruckerRing, Trudering_U, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Trudering_S, Trudering_U );
		addLink(  -linkID, CHANGE_CONNECTION, Trudering_U, Trudering_S );

		addNode( 
			UBAHN_LAYER, MessestadtOst, Station( 
				"Messestadt Ost", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 42.286 ),
					math::ConvertDegree( 48, 8.014 )
				) 
			) 
		);
		makeConnection( ++linkID, Trudering_U, MessestadtOst, U_BAHN_SPEED );

		addLink( ++linkID, CHANGE_CONNECTION, Moosach_S, Moosach_U );
		addLink(  -linkID, CHANGE_CONNECTION, Moosach_U, Moosach_S );
		makeConnection( ++linkID, Moosach_U, OEZ_U3, U_BAHN_SPEED );

		addLink( ++linkID, CHANGE_CONNECTION, OEZ_U1, OEZ_U3 );
		addLink(  -linkID, CHANGE_CONNECTION, OEZ_U1, OEZ_U1 );

		addNode( 
			UBAHN_LAYER, Olympiazentrum, Station( 
				"Olympiazentrum", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 33.336 ),
					math::ConvertDegree( 48, 10.808 )
				) 
			) 
		);
		makeConnection( ++linkID, OEZ_U3, Olympiazentrum, U_BAHN_SPEED );
		makeConnection( ++linkID, Olympiazentrum, Scheidplatz, U_BAHN_SPEED );

		addNode( 
			UBAHN_LAYER, MuenchnerFreiheit, Station( 
				"Münchner Freiheit", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 35.187 ),
					math::ConvertDegree( 48, 9.727 )
				) 
			) 
		);
		makeConnection( ++linkID, Scheidplatz, MuenchnerFreiheit, U_BAHN_SPEED );

		addNode( 
			UBAHN_LAYER, Odeonsplatz_U3, Station( 
				"Odeonsplatz U3/6", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 34.666 ),
					math::ConvertDegree( 48, 8.569 )
				) 
			) 
		);
		addNode( 
			UBAHN_LAYER, Odeonsplatz_U4, Station( 
				"Odeonsplatz U4/5", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 34.666 ),
					math::ConvertDegree( 48, 8.569 )
				) 
			) 
		);
		makeConnection( ++linkID, MuenchnerFreiheit, Odeonsplatz_U3, U_BAHN_SPEED );
		makeConnection( ++linkID, Odeonsplatz_U3, Marienplatz_U, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Marienplatz_S, Marienplatz_U );
		addLink(  -linkID, CHANGE_CONNECTION, Marienplatz_U, Marienplatz_S );
		makeConnection( ++linkID, Marienplatz_U, SendlingerTor_U3, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, SendlingerTor_U3, SendlingerTor_U1 );
		addLink(  -linkID, CHANGE_CONNECTION, SendlingerTor_U1, SendlingerTor_U3 );

		addNode( 
			UBAHN_LAYER, Implerstrasse, Station( 
				"Implerstrasse", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 32.898 ),
					math::ConvertDegree( 48, 7.185 )
				) 
			) 
		);
		makeConnection( ++linkID, SendlingerTor_U3, Implerstrasse, U_BAHN_SPEED );

		addNode( 
			UBAHN_LAYER, Obersendling, Station( 
				"Obersendling", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 32.121 ),
					math::ConvertDegree( 48, 5.902 )
				) 
			) 
		);
		makeConnection( ++linkID, Implerstrasse, Obersendling, U_BAHN_SPEED );
		addLink( ++linkID, Connection( 0.4, CHANGE_SPEED ), Siemenswerke, Obersendling );
		addLink(  -linkID, Connection( 0.4, CHANGE_SPEED ), Obersendling, Siemenswerke );

		addNode( 
			UBAHN_LAYER, FuerstenriedWest, Station( 
				"Fürstenried West", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 28.789 ),
					math::ConvertDegree( 48, 5.308 )
				) 
			) 
		);
		makeConnection( ++linkID, Obersendling, FuerstenriedWest, U_BAHN_SPEED );

		addNode(
			UBAHN_LAYER, Westendstrasse, Station( 
				"Westendstrasse", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 31.379 ),
					math::ConvertDegree( 48, 8.081 )
				) 
			) 
		);
		makeConnection( ++linkID, Westendstrasse, Heimeranplatz_U, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Heimeranplatz_S, Heimeranplatz_U );
		addLink(  -linkID, CHANGE_CONNECTION, Heimeranplatz_U, Heimeranplatz_S );
		makeConnection( ++linkID, Heimeranplatz_U, Hauptbahnhof_U4, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Hauptbahnhof_S, Hauptbahnhof_U4 );
		addLink(  -linkID, CHANGE_CONNECTION, Hauptbahnhof_U4, Hauptbahnhof_S );
		addLink( ++linkID, CHANGE_CONNECTION, Hauptbahnhof_U1, Hauptbahnhof_U4 );
		addLink(  -linkID, CHANGE_CONNECTION, Hauptbahnhof_U4, Hauptbahnhof_U1 );
		makeConnection( ++linkID, Hauptbahnhof_U4, Stachus_U, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Stachus_S, Stachus_U );
		addLink(  -linkID, CHANGE_CONNECTION, Stachus_U, Stachus_S );
		makeConnection( ++linkID, Stachus_U, Odeonsplatz_U4, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Odeonsplatz_U4, Odeonsplatz_U3 );
		addLink(  -linkID, CHANGE_CONNECTION, Odeonsplatz_U3, Odeonsplatz_U4 );

		addNode(
			UBAHN_LAYER, MaxWeberPlatz, Station( 
				"Max-Weber-Platz", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 35.872 ),
					math::ConvertDegree( 48, 8.126 )
				) 
			) 
		);
		makeConnection( ++linkID, Odeonsplatz_U4, MaxWeberPlatz, U_BAHN_SPEED );

		addNode(
			UBAHN_LAYER, Arabellapark, Station( 
				"Arabellapark", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 37.249 ),
					math::ConvertDegree( 48, 9.226 )
				) 
			) 
		);
		makeConnection( ++linkID, MaxWeberPlatz, Arabellapark, U_BAHN_SPEED );

		addNode(
			UBAHN_LAYER, LaimerPlatz, Station( 
				"Laimer Platz", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 30.017 ),
					math::ConvertDegree( 48, 8.133 )
				) 
			) 
		);
		makeConnection( ++linkID, LaimerPlatz, Westendstrasse, U_BAHN_SPEED );
		makeConnection( ++linkID, MaxWeberPlatz, Ostbahnhof_U, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Ostbahnhof_S, Ostbahnhof_U );
		addLink(  -linkID, CHANGE_CONNECTION, Ostbahnhof_U, Ostbahnhof_S );
		makeConnection( ++linkID, Ostbahnhof_U, InnsbruckerRing, U_BAHN_SPEED );

		addNode(
			UBAHN_LAYER, NeuperlachZentrum, Station( 
				"Neuperlach Zentrum", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 38.761 ),
					math::ConvertDegree( 48, 6.065 )
				) 
			) 
		);
		makeConnection( ++linkID, InnsbruckerRing, NeuperlachZentrum, U_BAHN_SPEED );
		makeConnection( ++linkID, NeuperlachZentrum, Neuperlach_Sued, U_BAHN_SPEED );

		addNode(
			UBAHN_LAYER, KlinikumGrosshadern, Station( 
				"Klinikum Großhadern", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 28.531 ),
					math::ConvertDegree( 48, 6.518 )
				) 
			) 
		);
		makeConnection( ++linkID, KlinikumGrosshadern, Harras_U, U_BAHN_SPEED );
		addLink( ++linkID, CHANGE_CONNECTION, Harras_S, Harras_U );
		addLink(  -linkID, CHANGE_CONNECTION, Harras_U, Harras_S );
		makeConnection( ++linkID, Harras_U, Implerstrasse, U_BAHN_SPEED );

		addNode(
			UBAHN_LAYER, GarchingForschungszentrum, Station( 
				"Garching Forschungszentrum", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 40.275 ),
					math::ConvertDegree( 48, 15.903 )
				) 
			) 
		);
		makeConnection( ++linkID, MuenchnerFreiheit, GarchingForschungszentrum, U_BAHN_SPEED );

		// Impossible
		addNode(
			UBAHN_LAYER, IngolstadtNord, Station( 
				"Ingolstadt Nord", math::GpsPosition<double>( 
					math::ConvertDegree( 11, 25.982 ),
					math::ConvertDegree( 48, 46.363 )
				) 
			) 
		);
		maxLat = math::ConvertDegree( 48, 46.363 );
	}

	public:
	MvvGraph()
	{
		minLon = minLat = std::numeric_limits<double>::max();
		maxLon = maxLat = -std::numeric_limits<double>::max();

		buildMVV();
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

template <>
inline int compare( const Bahnhoefe &elem1, const Bahnhoefe &elem2 )
{
	return compare( int( elem1), int(elem2) );
}

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

#endif	//  MVV_H