#include "Receiver.hpp"

Receiver::Receiver ( Log * LogPointer ) {

	this->LogPointer = LogPointer;
	SerialPointer = new Serial ( "\\\\.\\COM10" );

	SerialPointer->WriteData( "\0x00", 1 );

	Session = 0;
	SessionBuffer = 255;

	Received = 0;
	Updated = false; }

Receiver::~Receiver ( ) {

	delete SerialPointer; }

bool Receiver::GetData ( DataType Type, UINT16 &Data ) {

	switch ( Type ) {

		case DataType::Temperature:

			if ( !Temperatures.empty() ) {

				Data = Temperatures.front();
				Temperatures.pop_front();

				return true; }

			break;

		case DataType::Pressure:

			if ( !Pressures.empty() ) {

				Data = Pressures.front();
				Pressures.pop_front();

				return true; }

			break;

			case DataType::Voltage:

			if ( !Voltages.empty() ) {

				Data = Voltages.front();
				Voltages.pop_front();

				return true; }

			break;

		case DataType::PositionX:

			if ( !PositionsX.empty() ) {

				Data = PositionsX.front();
				PositionsX.pop_front();

				return true; }

			break;

		case DataType::PositionY:

			if ( !PositionsY.empty() ) {

				Data = PositionsY.front();
				PositionsY.pop_front();

				return true; }

			break;

		case DataType::PositionZ:

			if ( !PositionsZ.empty() ) {

				Data = PositionsZ.front();
				PositionsZ.pop_front();

				return true; }

			break; }

	return false; }

bool Receiver::Update ( ) {

	Updated = false;

	if ( SerialPointer->IsConnected() ) {

		for ( size_t i = 0; i < 256; i++ ) {

			Packet _Packet;
		
			if ( ReadMessage( _Packet ) ) {

				if ( _Packet.Session == Session ) {

					Message[ _Packet.Offset ] = _Packet.Data;
					Storage[ _Packet.Offset ] = true; }

				else {

					if ( Received > 4 ) {

						ProcessMessage(); }

					Received = 0;
					ClearMessage();

					Session = _Packet.Session;
					Message[ _Packet.Offset ] = _Packet.Data;
					Storage[ _Packet.Offset ] = true; }
			
				Received++; } } }

	return Updated;	}

void Receiver::ClearMessage ( ) {

	for ( UINT8 i = 0; i < 32; i++ ) {

		Message[i] = 0x00;
		Storage[i] = false; } }

bool Receiver::ReadMessage ( Packet &_Packet ) {

	char _Data [4];
	UINT8 * Data = reinterpret_cast < UINT8* > ( _Data );
	
	Data[0] = 0x00;
	Data[1] = 0x00;
	Data[2] = 0x00;
	Data[3] = 0x00;

	if ( SerialPointer->ReadData( _Data, 1 ) <= 0 ) {

		return false; }

	if ( Data[0] != 0xE3 ) {

		return false; }

	SerialPointer->ReadData( _Data + 1, 1 );
	SerialPointer->ReadData( _Data + 2, 1 );
	SerialPointer->ReadData( _Data + 3, 1 );

	if ( !( Data[3] & 0x0B ) ) {

		return false; }

	Data[3] = Data[3] & 0xF0;
	Data[3] = Data[3] >> 4;
	
	if ( Checksum( Data, 3 ) != Data[3] ) {
		
		return false; }

	_Packet.Data = Data[1];
	_Packet.Session = ( Data[2] & 0xE0 ) >> 5;
	_Packet.Offset = Data[2] & 0x1F;

	return true; }

void Receiver::ProcessMessage ( ) {

	if ( SessionBuffer == 255 ) {

		SessionBuffer = Session; }

	else if ( SessionBuffer != Session ) {

		ExtractMessage();
		SaveMessage();

		for ( UINT8 i = 0; i < 32; i++ ) {

			MessageBuffer[i] = 0x00;
			StorageBuffer[i] = false; }

		for ( UINT8 i = 0; i < 32; i++ ) {

			if ( Storage[i] ) {

				Message[i] = Message[i];
				StorageBuffer[i] = true; } }

		SessionBuffer = Session; }

	else {

		for ( UINT8 i = 0; i < 32; i++ ) {

			if ( Storage[i] ) {

				MessageBuffer[i] = Message[i];
				StorageBuffer[i] = true; } } } }

void Receiver::ExtractMessage ( ) {
	
	UINT16 Temperature;
	UINT16 Pressure;
	UINT16 Voltage;
	UINT32 PositionX;
	UINT32 PositionY;
	UINT32 PositionZ;

	Temperature = Message[1] | Message[2] << 8;
	Pressure = Message[3] | Message[4] << 8;
	Voltage = Message[5] | Message[6] << 8;
	PositionX = Message[7] | Message[8] << 8 | Message[9] << 16 | Message[10] << 24;
	PositionY = Message[11] | Message[12] << 8 | Message[13] << 16 | Message[14] << 24;
	PositionZ = Message[15] | Message[16] << 8 | Message[17] << 16 | Message[18] << 24;

	Temperatures.push_back( Temperature );
	Pressures.push_back( Pressure );
	Voltages.push_back( Voltage );
	PositionsX.push_back( PositionX );
	PositionsY.push_back( PositionY );
	PositionsZ.push_back( PositionZ );

	Updated = true; }

void Receiver::SaveMessage ( ) {

	LogPointer->Data( Log::LogType::Begin, (UINT64) 0 );

	for ( UINT8 i = 0; i < 32; i++ ) {

		if ( StorageBuffer[i] ) {

			LogPointer->Data( Log::LogType::Write, (UINT64) MessageBuffer[i] ); }

		else {

			LogPointer->Data( Log::LogType::Write, (UINT64) 999 ); } }

	LogPointer->Data( Log::LogType::End, (UINT64) 0 ); }

UINT8 Receiver::Checksum ( UINT8 * Data, UINT8 Size ) {

	UINT8 _Checksum = 0x00;

	for ( UINT8 i = 0; i < Size; i++ ) {
		
		_Checksum = ( _Checksum >> 2 ) ^ Data[i]; }

	return ( _Checksum & 0x0F ); }

UINT8 Receiver::Checksum ( UINT8 * Data, UINT8 First, UINT8 Last ) {

	UINT8 _Checksum = 0x00;

	for ( UINT8 i = First; i <= Last; i++ ) {
		
		_Checksum = ( _Checksum + Data[i] ) % 256; }

	return _Checksum; }