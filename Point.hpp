#ifndef CANSAT_POINT
#define CANSAT_POINT

#include "Config.hpp"

class IntegerPoint {

	public:
		
		IntegerPoint ( );
		IntegerPoint ( UINT64 x, UINT64 y, UINT64 z );

	public:

		UINT64 x;
		UINT64 y;
		UINT64 z;

	};

class RealPoint {

	public:

		RealPoint ( );
		RealPoint ( REAL64 x, REAL64 y, REAL64 z );

	public:

		REAL64 x;
		REAL64 y;
		REAL64 z;

	};

#endif