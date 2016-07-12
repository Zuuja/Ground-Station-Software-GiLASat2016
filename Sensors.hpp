#ifndef CANSAT_SENSORS
#define CANSAT_SENSORS

#include "Config.hpp"
#include "Log.hpp"
#include "Receiver.hpp"

class Sensors {

	public:

		enum MeasurementType {

			Temperature,
			Pressure,
			Height,
			Voltage

			};

		struct Measurement {

			UINT64 Time;
			MeasurementType Type;
			UINT64 Data;
			REAL64 Value;

			};

	public:

		Sensors ( Log * LogPointer, Receiver * ReceiverPointer );

		void SetGroundPressure ( REAL64 Value );
		REAL64 GetGroundPressure ( );

		size_t GetMeasurementsCount ( MeasurementType Type );
		Measurement GetMeasurement ( MeasurementType Type, size_t Index );

		bool Update ( );

	private:
		
		REAL64 CalculateTemperature ( UINT16 Temperature );
		REAL64 CalculatePressure ( UINT16 Pressure );
		REAL64 CalculateVoltage ( UINT16 Voltage );
		REAL64 CalculateHeight ( UINT16 Temperature, UINT16 Pressure );

	private:

		Log * LogPointer;
		Receiver * ReceiverPointer;

		REAL64 GroundPressure;

		std::vector <Measurement> TemperatureMeasurements;
		std::vector <Measurement> PressureMeasurements;
		std::vector <Measurement> HeightMeasurements;
		std::vector <Measurement> VoltageMeasurements;

	};

#endif