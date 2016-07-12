#ifndef CANSAT_RECEIVER
#define CANSAT_RECEIVER

#include "Config.hpp"
#include "Log.hpp"
#include "Serial.hpp"

class Receiver {

	public:

		enum DataType {

			Temperature,
			Pressure,
			Voltage,
			PositionX,
			PositionY,
			PositionZ

			};

	public:

		Receiver ( Log * LogPointer );
		~ Receiver ( );

		bool GetData ( DataType Type, UINT16 &Data );

		bool Update ( );

	private:

		struct Packet {
	
			UINT8 Data;
			UINT8 Session;
			UINT8 Offset;

			};

	private:

		void ClearMessage ( );
		bool ReadMessage ( Packet &_Packet );
		void ProcessMessage ( );
		void ExtractMessage ( );
		void SaveMessage ( );

		UINT8 Receiver::Checksum ( UINT8 * Data, UINT8 Size );
		UINT8 Receiver::Checksum ( UINT8 * Data, UINT8 First, UINT8 Last );

	private:
		
		Log * LogPointer;
		Serial * SerialPointer;

		std::deque <UINT16> Temperatures;
		std::deque <UINT16> Pressures;
		std::deque <UINT16> Voltages;
		std::deque <UINT16> PositionsX;
		std::deque <UINT16> PositionsY;
		std::deque <UINT16> PositionsZ;

		UINT8 Message [ 32 ];
		UINT8 MessageBuffer [ 32 ];

		bool Storage [ 32 ];
		bool StorageBuffer [ 32 ];

		UINT8 Session;
		UINT8 SessionBuffer;

		UINT32 Received;
		bool Updated;

	};

#endif