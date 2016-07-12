#include "Log.hpp"

void Log::Data ( LogType Type, UINT64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			DataFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			DataFile << " " << Value;

			break;
		
		case LogType::End:

			DataFile << "\r\n";

			break; }
	
	DataFile.flush(); }

void Log::Data ( LogType Type, REAL64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			DataFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			DataFile.precision( 5 );

			DataFile << " " << Value;

			break;
		
		case LogType::End:

			DataFile << "\r\n";

			break; }
	
	DataFile.flush(); }

void Log::Temperature ( LogType Type, UINT64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			TemperatureFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			TemperatureFile << " " << Value;

			break;
		
		case LogType::End:

			TemperatureFile << "\r\n";

			break; }
	
	TemperatureFile.flush(); }

void Log::Temperature ( LogType Type, REAL64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			TemperatureFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			TemperatureFile.precision( 5 );

			TemperatureFile << " " << Value;

			break;
		
		case LogType::End:

			TemperatureFile << "\r\n";

			break; }
	
	TemperatureFile.flush(); }

void Log::GroundPressure ( LogType Type, UINT64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			GroundPressureFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			GroundPressureFile << " " << Value;

			break;
		
		case LogType::End:

			GroundPressureFile << "\r\n";

			break; }
	
	GroundPressureFile.flush(); }

void Log::GroundPressure ( LogType Type, REAL64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			GroundPressureFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			GroundPressureFile.precision( 5 );

			GroundPressureFile << " " << Value;

			break;
		
		case LogType::End:

			GroundPressureFile << "\r\n";

			break; }
	
	GroundPressureFile.flush(); }

void Log::Pressure ( LogType Type, UINT64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			PressureFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			PressureFile << " " << Value;

			break;
		
		case LogType::End:

			PressureFile << "\r\n";

			break; }
	
	PressureFile.flush(); }

void Log::Pressure ( LogType Type, REAL64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			PressureFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			PressureFile.precision( 5 );

			PressureFile << " " << Value;

			break;
		
		case LogType::End:

			PressureFile << "\r\n";

			break; }
	
	PressureFile.flush(); }

void Log::Voltage ( LogType Type, UINT64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			VoltageFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			VoltageFile << " " << Value;

			break;
		
		case LogType::End:

			VoltageFile << "\r\n";

			break; }
	
	VoltageFile.flush(); }

void Log::Voltage ( LogType Type, REAL64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			VoltageFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			VoltageFile.precision( 5 );

			VoltageFile << " " << Value;

			break;
		
		case LogType::End:

			VoltageFile << "\r\n";

			break; }
	
	VoltageFile.flush(); }

void Log::Height ( LogType Type, UINT64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			HeightFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			HeightFile << " " << Value;

			break;
		
		case LogType::End:

			HeightFile << "\r\n";

			break; }
	
	HeightFile.flush(); }

void Log::Height ( LogType Type, REAL64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			HeightFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			HeightFile.precision( 5 );

			HeightFile << " " << Value;

			break;
		
		case LogType::End:

			HeightFile << "\r\n";

			break; }
	
	HeightFile.flush(); }

void Log::Position ( LogType Type, UINT64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			PositionFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			PositionFile << " " << Value;

			break;
		
		case LogType::End:

			PositionFile << "\r\n";

			break; }
	
	PositionFile.flush(); }

void Log::Position ( LogType Type, REAL64 Value ) {

	switch ( Type ) {

		case LogType::Begin:

			PositionFile << (UINT64) time( NULL );

			break;

		case LogType::Write:

			PositionFile.precision( 5 );

			PositionFile << " " << Value;

			break;
		
		case LogType::End:

			PositionFile << "\r\n";

			break; }
	
	PositionFile.flush(); }