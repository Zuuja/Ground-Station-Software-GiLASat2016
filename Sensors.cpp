#include "Sensors.hpp"

Sensors::Sensors ( Log * LogPointer, Receiver * ReceiverPointer ) {

	this->LogPointer = LogPointer;
	this->ReceiverPointer = ReceiverPointer;
	
	SetGroundPressure( 1000.00 ); }

void Sensors::SetGroundPressure ( REAL64 Value ) {

	GroundPressure = Value;
	
	LogPointer->GroundPressure( Log::LogType::Begin, (REAL64) 0 );
	LogPointer->GroundPressure( Log::LogType::Write, (UINT64) 1 );
	LogPointer->GroundPressure( Log::LogType::Write, (REAL64) GroundPressure );
	LogPointer->GroundPressure( Log::LogType::End, (REAL64) 0 ); }

REAL64 Sensors::GetGroundPressure ( ) {

	return GroundPressure; }

size_t Sensors::GetMeasurementsCount ( MeasurementType Type ) {

	switch ( Type ) {

		case MeasurementType::Temperature:

			return TemperatureMeasurements.size(); break;

		case MeasurementType::Pressure:

			return PressureMeasurements.size(); break;

		case MeasurementType::Height:

			return HeightMeasurements.size(); break;

		case MeasurementType::Voltage:

			return VoltageMeasurements.size(); break; };

	return 0; }

Sensors::Measurement Sensors::GetMeasurement ( MeasurementType Type, size_t Index ) {

	switch ( Type ) {

		case MeasurementType::Temperature:

			return TemperatureMeasurements[ Index ]; break;

		case MeasurementType::Pressure:

			return PressureMeasurements[ Index ]; break;

		case MeasurementType::Height:

			return HeightMeasurements[ Index ]; break;

		case MeasurementType::Voltage:

			return VoltageMeasurements[ Index ]; break; };

	return Measurement(); }

bool Sensors::Update ( ) {

	UINT64 Time;
	UINT16 Temperature, Pressure, Voltage;

	Time = time( NULL );

	if ( ReceiverPointer->GetData( Receiver::Temperature, Temperature ) && ReceiverPointer->GetData( Receiver::Pressure, Pressure ) && ReceiverPointer->GetData( Receiver::Voltage, Voltage ) ) {

		Measurement TemperatureMeasurement, PressureMeasurement, HeightMeasurement, VoltageMeasurement, DispersionMeasurement;

		TemperatureMeasurement.Time = Time;
		PressureMeasurement.Time = Time;
		HeightMeasurement.Time = Time;
		VoltageMeasurement.Time = Time;
		DispersionMeasurement.Time = Time;

		TemperatureMeasurement.Type = MeasurementType::Temperature;
		PressureMeasurement.Type = MeasurementType::Pressure;
		HeightMeasurement.Type = MeasurementType::Height;
		VoltageMeasurement.Type = MeasurementType::Voltage;

		TemperatureMeasurement.Data = Temperature;
		PressureMeasurement.Data = Pressure;
		HeightMeasurement.Data = 0;
		VoltageMeasurement.Data = Voltage;
		DispersionMeasurement.Data = 0;

		TemperatureMeasurement.Value = CalculateTemperature( Temperature );
		PressureMeasurement.Value = CalculatePressure( Pressure );
		HeightMeasurement.Value = CalculateHeight( Temperature, Pressure );
		VoltageMeasurement.Value = CalculateVoltage( Voltage );
		
		TemperatureMeasurements.push_back( TemperatureMeasurement );
		PressureMeasurements.push_back( PressureMeasurement );
		HeightMeasurements.push_back( HeightMeasurement );
		VoltageMeasurements.push_back( VoltageMeasurement );

		LogPointer->Temperature( Log::LogType::Begin, (UINT64) 0 );
		LogPointer->Temperature( Log::LogType::Write, (UINT64) 0 );
		LogPointer->Temperature( Log::LogType::Write, (UINT64) Temperature );
		LogPointer->Temperature( Log::LogType::End, (UINT64) 0 );

		LogPointer->Pressure( Log::LogType::Begin, (UINT64) 0 );
		LogPointer->Pressure( Log::LogType::Write, (UINT64) 0 );
		LogPointer->Pressure( Log::LogType::Write, (UINT64) Pressure );
		LogPointer->Pressure( Log::LogType::End, (UINT64) 0 );

		LogPointer->Voltage( Log::LogType::Begin, (UINT64) 0 );
		LogPointer->Voltage( Log::LogType::Write, (UINT64) 0 );
		LogPointer->Voltage( Log::LogType::Write, (UINT64) Voltage );
		LogPointer->Voltage( Log::LogType::End, (UINT64) 0 );

		LogPointer->Temperature( Log::LogType::Begin, (REAL64) 0 );
		LogPointer->Temperature( Log::LogType::Write, (UINT64) 1 );
		LogPointer->Temperature( Log::LogType::Write, (REAL64) TemperatureMeasurement.Value );
		LogPointer->Temperature( Log::LogType::End, (REAL64) 0 );

		LogPointer->Pressure( Log::LogType::Begin, (REAL64) 0 );
		LogPointer->Pressure( Log::LogType::Write, (UINT64) 1 );
		LogPointer->Pressure( Log::LogType::Write, (REAL64) PressureMeasurement.Value );
		LogPointer->Pressure( Log::LogType::End, (REAL64) 0 );

		LogPointer->Height( Log::LogType::Begin, (REAL64) 0 );
		LogPointer->Height( Log::LogType::Write, (UINT64) 1 );
		LogPointer->Height( Log::LogType::Write, (REAL64) HeightMeasurement.Value );
		LogPointer->Height( Log::LogType::End, (REAL64) 0 );

		LogPointer->Voltage( Log::LogType::Begin, (REAL64) 0 );
		LogPointer->Voltage( Log::LogType::Write, (UINT64) 1 );
		LogPointer->Voltage( Log::LogType::Write, (REAL64) VoltageMeasurement.Value );
		LogPointer->Voltage( Log::LogType::End, (REAL64) 0 );

		return true; }

	return false; }

REAL64 Sensors::CalculateTemperature ( UINT16 Temperature ) {

	return ( (REAL64) Temperature * 48.80 ); }

REAL64 Sensors::CalculatePressure ( UINT16 Pressure ) {

	return ( ( (REAL64) Pressure ) / 1023.00 * 46.00 ); }

REAL64 Sensors::CalculateVoltage ( UINT16 Voltage ) {

	return ( ( ( REAL64) Voltage ) / 1023.00 * 5.00 ); }

REAL64 Sensors::CalculateHeight ( UINT16 Temperature, UINT16 Pressure ) {
	
	return ( ( - 1.00 ) * ( ( 8.3144598 * CalculateTemperature( Temperature ) ) / ( 9.80665 * 0.0289644 ) ) * std::log( CalculatePressure( Pressure ) / GroundPressure ) ); }