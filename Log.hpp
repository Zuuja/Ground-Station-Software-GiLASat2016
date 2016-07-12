#ifndef CANSAT_LOG
#define CANSAT_LOG

#include "Config.hpp"

class Log {

	public:

		enum class LogType {

			Begin,
			Write,
			End

			};

	public:

		Log ( ) :

			DataFile ( "Data/Data.txt", std::ios::app ),
			TemperatureFile ( "Data/Temperature.txt", std::ios::app ),
			GroundPressureFile ( "Data/Ground Pressure.txt", std::ios::app ),
			PressureFile ( "Data/Pressure.txt", std::ios::app ),
			VoltageFile ( "Data/Voltage.txt", std::ios::app ),
			HeightFile ( "Data/Height.txt", std::ios::app ),
			PositionFile ( "Data/Position.txt", std::ios::app )

			{ }

		~ Log ( ) {

			DataFile.close();
			TemperatureFile.close();
			GroundPressureFile.close();
			PressureFile.close();
			VoltageFile.close();
			HeightFile.close();
			PositionFile.close(); }

		void Data ( LogType Type, UINT64 Value );
		void Data ( LogType Type, REAL64 Value );

		void Temperature ( LogType Type, UINT64 Value );
		void Temperature ( LogType Type, REAL64 Value );

		void GroundPressure ( LogType Type, UINT64 Value );
		void GroundPressure ( LogType Type, REAL64 Value );

		void Pressure ( LogType Type, UINT64 Value );
		void Pressure ( LogType Type, REAL64 Value );

		void Voltage ( LogType Type, UINT64 Value );
		void Voltage ( LogType Type, REAL64 Value );

		void Height ( LogType Type, UINT64 Value );
		void Height ( LogType Type, REAL64 Value );

		void Position ( LogType Type, UINT64 Value );
		void Position ( LogType Type, REAL64 Value );

	private:

		std::fstream DataFile;
		std::fstream TemperatureFile;
		std::fstream GroundPressureFile;
		std::fstream PressureFile;
		std::fstream VoltageFile;
		std::fstream HeightFile;
		std::fstream PositionFile;

	};

#endif