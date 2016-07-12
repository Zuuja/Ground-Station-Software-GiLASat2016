#ifndef CANSAT_TRACKER
#define CANSAT_TRACKER

#include "Config.hpp"
#include "Point.hpp"
#include "Receiver.hpp"

class Tracker {

	public:

		struct Position {

			UINT64 Time;
			IntegerPoint Data;
			RealPoint Value;

			};

	public:

		Tracker ( Log * LogPointer, Receiver * ReceiverPointer );

		size_t GetPositionsCount ( );
		Position GetPosition ( size_t Index );

		bool Update ( );

	private:

		Log * LogPointer;
		Receiver * ReceiverPointer;

		std::vector <Position> Positions;

	};

#endif