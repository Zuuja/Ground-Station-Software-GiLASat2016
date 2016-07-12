#ifndef CANSAT_ENGINE
#define CANSAT_ENGINE

#include "Config.hpp"
#include "Point.hpp"
#include "Receiver.hpp"
#include "Sensors.hpp"
#include "Tracker.hpp"

class Engine {

	public:

		enum class RenderMode {

			Map,
			Terrain,
			Graphs

			};

		enum class FocusMode {

			Satellite,
			Station,
			Crash,
			Zero

			};

	public:

		Engine ( Receiver * ReceiverPointer, Sensors * SensorsPointer, Tracker * TrackerPointer );
		~Engine ( );

		void Enable ( );
		void Disable ( );
		bool IsEnabled ( );

		void Process ( );
		
		bool IsEvent ( sf::Event &Event );
		void Update ( sf::Event Event );
		void Render ( );

	private:

		void RenderTerrain ( );
		void RenderHorizont ( );
		void RenderSatellite ( );
		void RenderStation ( );
		void RenderGrid ( );
		void RenderTrack ( );
		void RenderGraph ( sf::RenderWindow * Window, REAL64 * Data,  REAL64 Minimum, REAL64 Maximum, REAL64 Sample, REAL32 PositionX, REAL32 PositionY, REAL32 Width, REAL32 Height );

	private:

		Receiver * ReceiverPointer;
		Sensors * SensorsPointer;
		Tracker * TrackerPointer;
		
		sf::RenderWindow * Window;
		
		bool Enabled;
		RealPoint Mouse;

		RenderMode Mode;
		FocusMode Focus;

		sf::Font Font;
		sf::Texture IconTexture [9];
		sf::Texture MapTexture;
		GLuint TerrainTexture;

		RealPoint Camera;
		RealPoint Satellite;
		RealPoint Station;

		bool Keep;

	};

#endif