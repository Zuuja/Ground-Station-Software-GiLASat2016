#include "Main.hpp"

int main ( ) {

	srand( (UINT32) time( NULL ) );

	Log * LogPointer = new Log ( );
	Receiver * ReceiverPointer = new Receiver ( LogPointer );
	Sensors * SensorsPointer = new Sensors ( LogPointer, ReceiverPointer );
	Tracker * TrackerPointer = new Tracker( LogPointer, ReceiverPointer );
	Engine * EnginePointer = new Engine ( ReceiverPointer, SensorsPointer, TrackerPointer );

	EnginePointer->Enable();

	while ( EnginePointer->IsEnabled() ) {

		sf::Event Event;

		while ( EnginePointer->IsEvent( Event ) ) {

			EnginePointer->Update( Event ); }

		EnginePointer->Process();
		EnginePointer->Render(); }

	delete EnginePointer;
	delete TrackerPointer;
	delete SensorsPointer;
	delete ReceiverPointer;

	return 0; }