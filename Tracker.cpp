#include "Tracker.hpp"

Tracker::Tracker ( Log * LogPointer, Receiver * ReceiverPointer ) {

	this->LogPointer = LogPointer;
	this->ReceiverPointer = ReceiverPointer; }

size_t Tracker::GetPositionsCount ( ) {

	return Positions.size(); }

Tracker::Position Tracker::GetPosition ( size_t Index ) {

	return Positions[ Index ]; }

bool Tracker::Update ( ) {

	UINT64 Time;
	UINT16 PositionX, PositionY, PositionZ;

	Time = time( NULL );

	if ( ReceiverPointer->GetData( Receiver::PositionX, PositionX ) && ReceiverPointer->GetData( Receiver::PositionY, PositionY ) && ReceiverPointer->GetData( Receiver::PositionZ, PositionZ ) ) {

		Position Position;

		Position.Time = Time;
		Position.Data.x = PositionX;
		Position.Data.y = PositionY;
		Position.Data.z = PositionZ;
		Position.Value.x = (REAL64) ( PositionX - 32768 );
		Position.Value.y = (REAL64) ( PositionY - 32768 );
		Position.Value.z = (REAL64) ( PositionZ - 32768 );

		Positions.push_back( Position );

		LogPointer->Position( Log::LogType::Begin, (UINT64) 0 );
		LogPointer->Position( Log::LogType::Write, (UINT64) 0 );
		LogPointer->Position( Log::LogType::Write, (UINT64) Position.Data.x );
		LogPointer->Position( Log::LogType::Write, (UINT64) Position.Data.y );
		LogPointer->Position( Log::LogType::Write, (UINT64) Position.Data.z );
		LogPointer->Position( Log::LogType::End, (UINT64) 0 );

		LogPointer->Position( Log::LogType::Begin, (REAL64) 0 );
		LogPointer->Position( Log::LogType::Write, (UINT64) 1 );
		LogPointer->Position( Log::LogType::Write, (REAL64) Position.Value.x );
		LogPointer->Position( Log::LogType::Write, (REAL64) Position.Value.y );
		LogPointer->Position( Log::LogType::Write, (REAL64) Position.Value.z );
		LogPointer->Position( Log::LogType::End, (REAL64) 0 );

		return true; }

	return false; }