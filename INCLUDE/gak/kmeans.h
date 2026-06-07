/*
		Project:		gaklib
		Module:			kmeans.h
		Description:	K-Means is used to cluster any amount of any data
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

#ifndef K_MEANS_H
#define K_MEANS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/arrayBase.h>
#include <gak/map.h>
#include <gak/math.h>
#include <gak/logfile.h>

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

template<class OBJ>
PairMap< OBJ, Array<const OBJ *> > kMeans( const ArrayBase<OBJ> &src, size_t numCluster )
{
	typedef Array< Array<const OBJ *> >			MyCluster;	// indexed for current work
	typedef PairMap< OBJ, Array<const OBJ *> >	MapCluster;	// maped for the result
	typedef typename math::DistanceType<OBJ>::ResultType	DistType;	// the distance

#ifndef NDEBUG
	std::cout << "Container:"; printContainer(std::cout, src, ',') << '\n';
#endif
	Array<OBJ>	curMeans, newMeans;
	MyCluster	allCluster;

	if( numCluster > 0 && src.size() >= numCluster )
	{
/*
	this is a simple method to select n cluster 
	it can produce poor results if the cluster center are too close

		std::size_t clusterSize = src.size()/numCluster;
		std::size_t cur = clusterSize/2;
		while( cur < src.size() )
		{
			curMeans.push_back(src[cur]);
			allCluster.createElement().empty();
			cur += clusterSize;
		}
*/
		curMeans.push_back( *src.cbegin() );
		while( curMeans.size() < numCluster )
		{
			// we are searching for the second farest entry
			bool		nextCenter1OK = false, nextCenter2OK = false;
			OBJ			nextCenter1, nextCenter2;
			DistType	maxDistance1 = std::numeric_limits<DistType>::min();
			DistType	maxDistance2 = std::numeric_limits<DistType>::min();
			for(
				typename ArrayBase<OBJ>::const_iterator it1 = src.cbegin(), endIT1 = src.cend();
				it1 != endIT1;
				++it1
			)
			{
				// determine the distance to the closest center
				DistType minDistance = std::numeric_limits<DistType>::max();
				bool skipped = false;
				for(
					typename ArrayBase<OBJ>::const_iterator it2 = curMeans.cbegin(), endIT2 = curMeans.cend();
					it2 != endIT2 && (skipped = *it1 == *it2) == false;		// do not use that point if it is already a center
					++it2
				)
				{
					DistType	dist = math::distance( *it2, *it1 );
					if( dist < minDistance )
					{
						minDistance = dist;
						// If current point is closer than the current second 
						// it cannot become neither second nor first
						if( dist < maxDistance2 && nextCenter2OK )
						{
							skipped = true;
							break;
						}
					}
				}
				if( skipped ) 
/*^*/				continue;
				if( minDistance > maxDistance1 )	// current point beats the first
				{
					if( nextCenter1OK )				// last first is now the second
					{
						maxDistance2 = maxDistance1;
						nextCenter2 = nextCenter1;
						nextCenter2OK = true;
					}

					maxDistance1 = minDistance;
					nextCenter1 = *it1;
					nextCenter1OK = true;
				}
				else if( minDistance > maxDistance2 )	// current point beats the second
				{
					maxDistance2 = minDistance;
					nextCenter2 = *it1;
					nextCenter2OK = true;				
				}
			}
			if( nextCenter2OK )
				curMeans.push_back( nextCenter2 );
			else if( nextCenter1OK )
				curMeans.push_back( nextCenter1 );
			else
			{
				// if the data does not allow the required number of cluster
				numCluster = curMeans.size();
				break;
			}
		}

		bool inProgress = true;
		while( true )
		{
			// distribute all nodes to the cluster
			for(
				typename ArrayBase<OBJ>::const_iterator it = src.cbegin(), endIT = src.cend();
				it != endIT;
				++it
			)
			{
				size_t	nearest = 0;
				if(numCluster>1)
				{
					// search for the closest cluster center
					math::MinMax<DistType>	minDistance(math::distance( curMeans[0], *it ));
#ifndef NDEBUG
					std::cout << "Distance " << minDistance.getMin() << ' ' << *it << '-' << curMeans[0] << '\n';
#endif
					for( size_t i=1; i<numCluster; ++i )
					{
						DistType	dist = math::distance( curMeans[i], *it );
#ifndef NDEBUG
						std::cout << "Distance " << minDistance.getMin() << ' ' << *it << '-' << curMeans[i] << '\n';
#endif
						minDistance.test(dist);
						if( minDistance.getMin() == dist )
						{
							nearest = i;
						}
					}
#ifndef NDEBUG
					std::cout << nearest << ' ' << minDistance.getMin() << ' ' << *it << '-' << curMeans[nearest] << '\n';
#endif
				}

				allCluster[nearest].push_back( it );
			}

#ifndef NDEBUG
			std::cout << "\nZentren: "; printContainer( std::cout, curMeans, ',' ) << std::endl;
#endif

			// calculate the mean of each cluster
			size_t nIdx=0;
			newMeans.empty();
			for(
				typename MyCluster::const_iterator it1 = allCluster.cbegin(), endIT1 = allCluster.cend();
				it1 != endIT1;
				++it1
			)
			{
#ifndef NDEBUG
				std::cout << "Cluster: "; printContainer( std::cout, *it1, ',' ) << std::endl;
#endif
				math::Mean<OBJ>	mean;
				for(
					typename Array<const OBJ*>::const_iterator it2 = it1->cbegin(), endIT2 = it1->cend();
					it2 != endIT2;
					++it2
				)
				{
					mean.add(**it2);
				}
				if( mean.getCount() )
				{
#ifndef NDEBUG
					std::cout << "Mittel: " << mean.getMean() << '\n';
#endif
					newMeans.push_back(mean.getMean());
				}
				else
				{
					newMeans[nIdx] = curMeans[nIdx];
					nIdx++;
				}
			}

			// check whether a mean has changed
#ifndef NDEBUG
			std::cout << "Neue Mittelwerte: "; printContainer( std::cout, newMeans, ',' ) << std::endl;
#endif
			inProgress = false;
			for( size_t i=0; i<numCluster; ++i )
			{
				if( curMeans[i] != newMeans[i] )
				{
					inProgress = true;	// yes, there is a new center
					curMeans[i] = newMeans[i];
				}
			}
			if( !inProgress )	// no new center -> break
/*v*/			break;

			// clear all center before continuing
			for( size_t i=0; i<numCluster; ++i )
			{
				allCluster[i].empty();
			}
		}
	}

	MapCluster	result;
	for( size_t i=0; i<numCluster; ++i )
	{
		result[curMeans[i]].moveFrom( allCluster[i] );
	}
	return result;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// K_MEANS_H
