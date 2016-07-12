#include "Engine.hpp"

Engine::Engine ( Receiver * ReceiverPointer, Sensors * SensorsPointer, Tracker * TrackerPointer ) {

	this->ReceiverPointer = ReceiverPointer;
	this->SensorsPointer = SensorsPointer;
	this->TrackerPointer = TrackerPointer;

	Window = NULL;
	Enabled = false;

	Mode = RenderMode::Terrain;
	Focus = FocusMode::Satellite;

	Camera = RealPoint( 10.00, 5025.00, 10.00 );
	Satellite = RealPoint( 0.00, 5000, 0.00 );
	Station = RealPoint( 0.00, 0.00, 0.00 );

	Keep = false; }

Engine::~Engine ( ) {

	Disable(); }

void Engine::Enable ( ) {

	if ( Window ) {

		delete Window; }

	sf::ContextSettings ContextSettings;
    ContextSettings.depthBits = 32;

	Window = new sf::RenderWindow ( sf::VideoMode( 800, 600 ), "CanSat Ground Station", sf::Style::Close, ContextSettings );

	if ( Window->isOpen() ) {

		glEnable( GL_DEPTH_TEST );
		glDepthMask( GL_TRUE );
		glDisable( GL_LIGHTING );

		Mode = RenderMode::Terrain;
		Focus = FocusMode::Satellite;

		Font.loadFromFile( "Graphics/Font.ttf" );

		IconTexture[0].loadFromFile( "Graphics/MapIcon.png" );
		IconTexture[0].setSmooth( true );

		IconTexture[1].loadFromFile( "Graphics/TerrainIcon.png" );
		IconTexture[1].setSmooth( true );

		IconTexture[2].loadFromFile( "Graphics/GraphsIcon.png" );
		IconTexture[2].setSmooth( true );

		IconTexture[3].loadFromFile( "Graphics/TargetIcon.png" );
		IconTexture[3].setSmooth( false );

		IconTexture[4].loadFromFile( "Graphics/PressureIcon.png" );
		IconTexture[4].setSmooth( true );
		
		IconTexture[5].loadFromFile( "Graphics/SatelliteIcon.png" );
		IconTexture[5].setSmooth( true );

		IconTexture[6].loadFromFile( "Graphics/StationIcon.png" );
		IconTexture[6].setSmooth( true );

		IconTexture[7].loadFromFile( "Graphics/CrashIcon.png" );
		IconTexture[7].setSmooth( true );

		IconTexture[8].loadFromFile( "Graphics/ZeroIcon.png" );
		IconTexture[8].setSmooth( true );

		MapTexture.loadFromFile( "Graphics/Map.png" );
		MapTexture.setSmooth( true );

        sf::Image TerrainImage;
		TerrainImage.loadFromFile( "Graphics/Terrain.jpg" );

		TerrainTexture = 0;
        glGenTextures( 1, &TerrainTexture );
        glBindTexture( GL_TEXTURE_2D, TerrainTexture );
        gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, TerrainImage.getSize().x, TerrainImage.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, TerrainImage.getPixelsPtr() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

		Camera = RealPoint( 10.00, 5025.00, 10.00 );
		Satellite = RealPoint( 0.00, 5000.00, 0.00 );
		Station = RealPoint( 0.00, 0.00, -0.00 );

		Keep = true;
		Enabled = true; }

	else {

		Enabled = false; } }

void Engine::Disable ( ) {

	if ( Enabled ) {

		Window->close();

		Enabled = false; } }

bool Engine::IsEnabled ( ) {

	return Enabled; }

void Engine::Process ( ) {

	while ( ReceiverPointer->Update() );
	while ( SensorsPointer->Update() );
	while ( TrackerPointer->Update() );

	if ( SensorsPointer->GetMeasurementsCount( Sensors::Height ) > 0 ) {
		
		Satellite.y = fmax( SensorsPointer->GetMeasurement( Sensors::Height, SensorsPointer->GetMeasurementsCount( Sensors::Height ) - 1 ).Value, 0.00 ); }

	if ( TrackerPointer->GetPositionsCount() > 0 ) {
		
		Satellite.x = TrackerPointer->GetPosition( TrackerPointer->GetPositionsCount() - 1 ).Value.x;
		Satellite.z = TrackerPointer->GetPosition( TrackerPointer->GetPositionsCount() - 1 ).Value.z; }

	if ( Keep ) {

		switch ( Focus ) {

			case FocusMode::Satellite:

				Camera.x = Satellite.x + 10.00;
				Camera.y = Satellite.y + 25.00;
				Camera.z = Satellite.z + 10.00;

				break;
		
			case FocusMode::Crash:

				Camera.x = Satellite.x + 10.00;
				Camera.y = 25.00;
				Camera.z = Satellite.z + 10.00;

				break; } }

	// Camera.y = Satellite.y + 10.00;

	//std::cout << SensorsPointer->GetMeasurement( Sensors::Height, 0 ).Value << std::endl;

	}
		
bool Engine::IsEvent ( sf::Event &Event ) {

	return Window->pollEvent( Event ); }

void Engine::Update ( sf::Event Event ) {

	if ( Event.type == sf::Event::MouseMoved ) {

		Mouse.x = sf::Mouse::getPosition( *Window ).x;
		Mouse.y = sf::Mouse::getPosition( *Window ).y; }
	
	if ( Mode == RenderMode::Map ) {

		if ( Event.type == sf::Event::MouseButtonReleased ) {

			if ( Event.mouseButton.button == sf::Mouse::Button::Left ) {

				Mouse.z = 1.00f; }

			else if ( Event.mouseButton.button == sf::Mouse::Button::Right ) {

				Mouse.z = - 1.00f; } } }

	if ( Mode == RenderMode::Terrain ) {

		if ( Event.type == sf::Event::MouseButtonReleased ) {

			if ( Event.mouseButton.button == sf::Mouse::Button::Left ) {

				Mouse.z = 1.00f; }

			else if ( Event.mouseButton.button == sf::Mouse::Button::Right ) {

				Mouse.z = -1.00f; } }

		else if ( Event.type == sf::Event::MouseWheelMoved ) {

			if ( Event.mouseWheel.delta > 0 && Camera.y > 25.00 ) {

				Camera.y = Camera.y - 10.00; }

			else if ( Event.mouseWheel.delta < 0 && Camera.y < 10000.00 ) {

				Camera.y = Camera.y + 10.00; } }

		else if ( Event.type == sf::Event::KeyPressed ) {

			if ( Event.key.code == sf::Keyboard::Add && Camera.y > 25.00 ) {

				Camera.y = Camera.y - 10; }

			else if ( Event.key.code == sf::Keyboard::Subtract && Camera.y < 10000.00 ) {

				Camera.y = Camera.y + 10; }

			else if ( Event.key.code == sf::Keyboard::Left ) {

				Camera.x = Camera.x - 10; }

			else if ( Event.key.code == sf::Keyboard::Right ) {

				Camera.x = Camera.x + 10; }

			else if ( Event.key.code == sf::Keyboard::Up ) {

				Camera.z = Camera.z - 10; }

			else if ( Event.key.code == sf::Keyboard::Down ) {

				Camera.z = Camera.z + 10; } }

		else if ( Event.type == sf::Event::KeyReleased ) {

			if ( Event.key.code == sf::Keyboard::Space ) {

				Keep = !Keep; }

			else if ( Event.key.code == sf::Keyboard::Num1 ) {

				Camera.x = Satellite.x + 10.00;
				Camera.y = Satellite.y + 25.00;
				Camera.z = Satellite.z + 10.00;

				Focus = FocusMode::Satellite; }

			else if ( Event.key.code == sf::Keyboard::Num2 ) {
				
				Camera.x = Station.x + 10.00;
				Camera.y = Station.y + 25.00;
				Camera.z = Station.z + 10.00;

				Focus = FocusMode::Station; }

			else if ( Event.key.code == sf::Keyboard::Num3 ) {

				Camera.x = Satellite.x + 10.00;
				Camera.y = 25.00;
				Camera.z = Satellite.z + 10.00;

				Focus = FocusMode::Crash; }

			else if ( Event.key.code == sf::Keyboard::Num4 ) {

				Camera.x = 10.00;
				Camera.y = 25.00;
				Camera.z = 10.00;

				Focus = FocusMode::Zero; }

			else if ( Event.key.code == sf::Keyboard::Num0 ) {

				Focus = FocusMode::Satellite; }

			else if ( Event.key.code == sf::Keyboard::Num9 ) {

				Focus = FocusMode::Station; }

			else if ( Event.key.code == sf::Keyboard::Num8 ) {

				Focus = FocusMode::Crash; }

			else if ( Event.key.code == sf::Keyboard::Num7 ) {

				Focus = FocusMode::Zero; } } }

	if ( Mode == RenderMode::Graphs ) {

		if ( Event.type == sf::Event::MouseButtonReleased ) {

			if ( Event.mouseButton.button == sf::Mouse::Button::Left ) {

				Mouse.z = 1.00f; }

			else if ( Event.mouseButton.button == sf::Mouse::Button::Right ) {

				Mouse.z = - 1.00f; } } }

	if ( Event.type == sf::Event::Closed ) {

		Disable(); } }

void Engine::Render ( ) {

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if ( Mode == RenderMode::Map ) {

		Window->pushGLStates();

		sf::Sprite Map;
		sf::Sprite TargetIcon;
		sf::Sprite PressureIcon;
		sf::RectangleShape PressureBar;
		sf::RectangleShape PressureBarBackground;
		sf::Text PressureText;

		Map.setTexture( MapTexture );
		Map.setPosition( sf::Vector2f( ( Window->getSize().x - MapTexture.getSize().x * ( ( Window->getSize().y - 200.00f ) / MapTexture.getSize().y ) ) / 2.00f, 100.00f ) );
		Map.setScale( ( Window->getSize().y - 200.00f ) / MapTexture.getSize().y, ( Window->getSize().y - 200.00f ) / MapTexture.getSize().y );

		if ( Mouse.x >= Map.getPosition().x && Mouse.y >= Map.getPosition().y && Mouse.x < ( Map.getPosition().x + Map.getGlobalBounds().width ) && Mouse.y < ( Map.getPosition().y + Map.getGlobalBounds().height ) ) {

			Window->setMouseCursorVisible( false );

			TargetIcon.setTexture( IconTexture[3] );
			TargetIcon.setOrigin( sf::Vector2f( IconTexture[3].getSize().x / 2.00, IconTexture[3].getSize().y / 2.00 ) );
			TargetIcon.setPosition( Mouse.x, Mouse.y );
			TargetIcon.setScale( 50.00f / IconTexture[3].getSize().x, 50.00f / IconTexture[4].getSize().y );
			TargetIcon.setColor( sf::Color( 255, 255, 255, 255 ) );

			if ( Mouse.z > 0.50 ) {

				Station.x = - 1.00 * ( ( ( Map.getPosition().x + Map.getGlobalBounds().width ) - Mouse.x ) / Map.getGlobalBounds().width * 10000.00 - 5000.00 );
				Station.z = - 1.00 * ( ( ( Map.getPosition().y + Map.getGlobalBounds().height ) - Mouse.y ) / Map.getGlobalBounds().height * 10000.00 - 5000.00 ); } }

		else {

			Window->setMouseCursorVisible( true );

			TargetIcon.setTexture( IconTexture[3] );
			TargetIcon.setOrigin( sf::Vector2f( IconTexture[3].getSize().x / 2.00, IconTexture[3].getSize().y / 2.00 ) );
			TargetIcon.setPosition( sf::Vector2f( Map.getPosition().x + Map.getGlobalBounds().width * ( ( Station.x + 5000.00f ) / 10000.00f ), Map.getPosition().y + Map.getGlobalBounds().height * ( ( Station.z + 5000.00f ) / 10000.00f ) ) );
			TargetIcon.setScale( 50.00f / IconTexture[3].getSize().x, 50.00f / IconTexture[4].getSize().y );
			TargetIcon.setColor( sf::Color( 255, 255, 255, 255 ) ); }

		PressureIcon.setTexture( IconTexture[4] );
		PressureIcon.setPosition( sf::Vector2f( 25.00f, Window->getSize().y - 75.00f ) );
		PressureIcon.setScale( 50.00f / IconTexture[4].getSize().x, 50.00f / IconTexture[4].getSize().y );
		PressureIcon.setColor( sf::Color( 0, 127, 255, 255 ) );

		if ( Mouse.x >= 100.00f && Mouse.y >= ( Window->getSize().y - 75.00f ) && Mouse.x < ( Window->getSize().x - 25.00f ) && Mouse.y < ( Window->getSize().y - 25.00f ) ) {

			std::string Text;
			std::stringstream Buffer;

			Buffer.precision( 2 );

			Buffer << std::fixed << ( 950.00 + 100.00 * ( ( Mouse.x - 100.00f ) / ( Window->getSize().x - 125.00f ) ) );
			Buffer >> Text;

			PressureBar.setSize( sf::Vector2f( ( Window->getSize().x - 125.00f ) * ( ( Mouse.x - 100.00f ) / ( Window->getSize().x - 125.00f ) ), 50.00f ) );
			PressureBar.setPosition( sf::Vector2f( 100.00f, Window->getSize().y - 75.00f ) );
			PressureBar.setFillColor( sf::Color( 0, 127, 255, 255 ) );

			PressureText.setFont( Font );
			PressureText.setCharacterSize( 25 );
			PressureText.setString( Text + " hPa" );
			PressureText.setPosition( sf::Vector2f( Window->getSize().x - PressureText.getGlobalBounds().width - 35.00f, Window->getSize().y - 68.00f ) );
			PressureText.setColor( sf::Color( 255, 255, 255, 255 ) );

			if ( Mouse.z > 0.50 ) {

				SensorsPointer->SetGroundPressure( 950.00 + 100.00 * ( ( Mouse.x - 100.00f ) / ( Window->getSize().x - 125.00f ) ) ); } }

		else {

			std::string Text;
			std::stringstream Buffer;

			Buffer.precision( 2 );

			Buffer << std::fixed << SensorsPointer->GetGroundPressure();
			Buffer >> Text;

			PressureText.setFont( Font );
			PressureText.setCharacterSize( 25 );
			PressureText.setString( Text + " hPa" );
			PressureText.setPosition( sf::Vector2f( Window->getSize().x - PressureText.getGlobalBounds().width - 35.00f, Window->getSize().y - 68.00f ) );
			PressureText.setColor( sf::Color( 255, 255, 255, 255 ) );

			PressureBar.setSize( sf::Vector2f( ( Window->getSize().x - 125.00f ) * ( ( SensorsPointer->GetGroundPressure() - 950.00 ) / 100.00 ), 50.00f ) );
			PressureBar.setPosition( sf::Vector2f( 100.00f, Window->getSize().y - 75.00f ) );
			PressureBar.setFillColor( sf::Color( 0, 127, 255, 255 ) ); }

		PressureBarBackground.setSize( sf::Vector2f( Window->getSize().x - 125.00f, 50.00f ) );
		PressureBarBackground.setPosition( sf::Vector2f( 100.00f, Window->getSize().y - 75.00f ) );
		PressureBarBackground.setFillColor( sf::Color( 0, 127, 255, 150 ) );

		Window->draw( Map );
		Window->draw( TargetIcon );
		Window->draw( PressureIcon );
		Window->draw( PressureBar );
		Window->draw( PressureBarBackground );
		Window->draw( PressureText );

		sf::Sprite MapIcon;
		sf::Sprite TerrainIcon;
		sf::Sprite GraphsIcon;

		MapIcon.setTexture( IconTexture[0] );
		MapIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 - 100.00f, 25.00f ) );
		MapIcon.setScale( 50.00f / IconTexture[0].getSize().x, 50.00f / IconTexture[0].getSize().y );
		MapIcon.setColor( sf::Color( 0, 127, 255, 255 ) );
		
		TerrainIcon.setTexture( IconTexture[1] );
		TerrainIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 - 25.00f, 25.00f ) );
		TerrainIcon.setScale( 50.00f / IconTexture[1].getSize().x, 50.00f / IconTexture[1].getSize().y );
		TerrainIcon.setColor( sf::Color( 0, 127, 255, 255 ) );

		GraphsIcon.setTexture( IconTexture[2] );
		GraphsIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 + 50.00f, 25.00f ) );
		GraphsIcon.setScale( 50.00f / IconTexture[2].getSize().x, 50.00f / IconTexture[2].getSize().y );
		GraphsIcon.setColor( sf::Color( 0, 127, 255, 255 ) );
		
		if ( Mouse.x >= MapIcon.getPosition().x && Mouse.y >= MapIcon.getPosition().y && Mouse.x < ( MapIcon.getPosition().x + 50.00f ) && Mouse.y < ( MapIcon.getPosition().y + 50.00f ) ) {
			
			MapIcon.setColor( sf::Color( 0, 127, 255, 150 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Map; } }

		if ( Mouse.x >= TerrainIcon.getPosition().x && Mouse.y >= TerrainIcon.getPosition().y && Mouse.x < ( TerrainIcon.getPosition().x + 50.00f ) && Mouse.y < ( TerrainIcon.getPosition().y + 50.00f ) ) {

			TerrainIcon.setColor( sf::Color( 0, 127, 255, 150 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Terrain; } }

		if ( Mouse.x >= GraphsIcon.getPosition().x && Mouse.y >= GraphsIcon.getPosition().y && Mouse.x < ( GraphsIcon.getPosition().x + 50.00f ) && Mouse.y < ( GraphsIcon.getPosition().y + 50.00f ) ) {

			GraphsIcon.setColor( sf::Color( 0, 127, 255, 150 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Graphs; } }

		Window->draw( MapIcon );
		Window->draw( TerrainIcon );
		Window->draw( GraphsIcon );

		Window->popGLStates(); }

	else if ( Mode == RenderMode::Terrain ) {

		glClearColor( 0.0, 0.5, 1.0, 1.0 );
		glClear( GL_COLOR_BUFFER_BIT );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		gluPerspective( 45.00, (REAL64) Window->getSize().x / (REAL64) Window->getSize().y, 0.25, 20000.00 );
		
		switch ( Focus ) {

			case FocusMode::Satellite:

				gluLookAt( Camera.x, Camera.y, Camera.z, Satellite.x, Satellite.y, Satellite.z, 0, 1, 0 );

				break;

			case FocusMode::Station:

				gluLookAt( Camera.x, Camera.y, Camera.z, Station.x, Station.y, Station.z, 0, 1, 0 );

				break;

			case FocusMode::Crash:

				gluLookAt( Camera.x, Camera.y, Camera.z, Satellite.x, 0.00, Satellite.z, 0, 1, 0 );

				break;

			case FocusMode::Zero:

				gluLookAt( Camera.x, Camera.y, Camera.z, 0.00, 0.00, 0.00, 0, 1, 0 );

				break;
			
			default:
				
				gluLookAt( Camera.x, Camera.y, Camera.z, Satellite.x, Satellite.y, Satellite.z, 0, 1, 0 ); }

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		RenderTerrain();
		RenderHorizont();
		RenderSatellite();
		RenderStation();
		RenderGrid();
		RenderTrack();

		Window->pushGLStates();

		sf::Sprite SatelliteIcon;
		sf::Sprite StationIcon;
		sf::Sprite CrashIcon;
		sf::Sprite ZeroIcon;

		SatelliteIcon.setTexture( IconTexture[5] );
		SatelliteIcon.setPosition( sf::Vector2f( 25.00f, Window->getSize().y - 75.00f ) );
		SatelliteIcon.setScale( 50.00f / IconTexture[5].getSize().x, 50.00f / IconTexture[5].getSize().y );
		SatelliteIcon.setColor( sf::Color( 255, 255, 255, 100 ) );

		StationIcon.setTexture( IconTexture[6] );
		StationIcon.setPosition( sf::Vector2f( 100.00f, Window->getSize().y - 75.00f ) );
		StationIcon.setScale( 50.00f / IconTexture[5].getSize().x, 50.00f / IconTexture[5].getSize().y );
		StationIcon.setColor( sf::Color( 255, 255, 255, 100 ) );

		CrashIcon.setTexture( IconTexture[7] );
		CrashIcon.setPosition( sf::Vector2f( 175.00f, Window->getSize().y - 75.00f ) );
		CrashIcon.setScale( 50.00f / IconTexture[5].getSize().x, 50.00f / IconTexture[5].getSize().y );
		CrashIcon.setColor( sf::Color( 255, 255, 255, 100 ) );

		ZeroIcon.setTexture( IconTexture[8] );
		ZeroIcon.setPosition( sf::Vector2f( 250.00f, Window->getSize().y - 75.00f ) );
		ZeroIcon.setScale( 50.00f / IconTexture[5].getSize().x, 50.00f / IconTexture[5].getSize().y );
		ZeroIcon.setColor( sf::Color( 255, 255, 255, 100 ) );

		if ( Mouse.x >= SatelliteIcon.getPosition().x && Mouse.y >= SatelliteIcon.getPosition().y && Mouse.x < ( SatelliteIcon.getPosition().x + 50.00f ) && Mouse.y < ( SatelliteIcon.getPosition().y + 50.00f ) ) {

			SatelliteIcon.setColor( sf::Color( 255, 255, 255, 200 ) );
			
			if ( Mouse.z > 0.50f ) {

				Camera.x = Satellite.x + 10.00;
				Camera.y = Satellite.y + 25.00;
				Camera.z = Satellite.z + 10.00;

				Focus = FocusMode::Satellite; }
			
			else if ( Mouse.z < -0.50f ) {

				Focus = FocusMode::Satellite; } }

		if ( Mouse.x >= StationIcon.getPosition().x && Mouse.y >= StationIcon.getPosition().y && Mouse.x < ( StationIcon.getPosition().x + 50.00f ) && Mouse.y < ( StationIcon.getPosition().y + 50.00f ) ) {

			StationIcon.setColor( sf::Color( 255, 255, 255, 200 ) );
			
			if ( Mouse.z > 0.50f ) {

				Camera.x = Station.x + 10.00;
				Camera.y = Station.y + 25.00;
				Camera.z = Station.z + 10.00;

				Focus = FocusMode::Station; }

			else if ( Mouse.z < -0.50f ) {

				Focus = FocusMode::Station; } }

		if ( Mouse.x >= CrashIcon.getPosition().x && Mouse.y >= CrashIcon.getPosition().y && Mouse.x < ( CrashIcon.getPosition().x + 50.00f ) && Mouse.y < ( CrashIcon.getPosition().y + 50.00f ) ) {

			CrashIcon.setColor( sf::Color( 255, 255, 255, 200 ) );
			
			if ( Mouse.z > 0.50f ) {

				Camera.x = Satellite.x + 10.00;
				Camera.y = 25.00;
				Camera.z = Satellite.z + 10.00;

				Focus = FocusMode::Crash; }

			else if ( Mouse.z < -0.50f ) {

				Focus = FocusMode::Crash; } }

		if ( Mouse.x >= ZeroIcon.getPosition().x && Mouse.y >= ZeroIcon.getPosition().y && Mouse.x < ( ZeroIcon.getPosition().x + 50.00f ) && Mouse.y < ( ZeroIcon.getPosition().y + 50.00f ) ) {

			ZeroIcon.setColor( sf::Color( 255, 255, 255, 200 ) );
			
			if ( Mouse.z >= 0.50f ) {

				Camera.x = 10.00;
				Camera.y = 25.00;
				Camera.z = 10.00;

				Focus = FocusMode::Zero; }

			else if ( Mouse.z < -0.50f ) {

				Focus = FocusMode::Zero; } }

		Window->draw( SatelliteIcon );
		Window->draw( StationIcon );
		Window->draw( CrashIcon );
		Window->draw( ZeroIcon );

		sf::Text HeightText;
		sf::Text DistanceText;
		sf::Text SpeedText;

		std::string HeightString;
		std::string DistanceString;
		std::string SpeedString;

		std::stringstream HeightBuffer;
		std::stringstream DistanceBuffer;
		std::stringstream SpeedBuffer;

		HeightBuffer.precision( 2 );
		DistanceBuffer.precision( 2 );
		SpeedBuffer.precision( 2 );

		HeightBuffer << std::fixed << Satellite.y;
		HeightBuffer >> HeightString;

		DistanceBuffer << std::fixed << sqrt( ( Satellite.x - Station.x ) * ( Satellite.x - Station.x ) + ( Satellite.y - Station.y ) * ( Satellite.y - Station.y ) + ( Satellite.z - Station.z ) * ( Satellite.z - Station.z ) );
		DistanceBuffer >> DistanceString;

		if ( SensorsPointer->GetMeasurementsCount( Sensors::Height ) > 1 ) {

			Sensors::Measurement A = SensorsPointer->GetMeasurement( Sensors::Height, SensorsPointer->GetMeasurementsCount( Sensors::Height ) - 1 );
			Sensors::Measurement B = SensorsPointer->GetMeasurement( Sensors::Height, SensorsPointer->GetMeasurementsCount( Sensors::Height ) - 2 );

			SpeedBuffer << std::fixed << ( B.Value - A.Value ) / fmax( A.Time - B.Time, 1.00 );
			SpeedBuffer >> SpeedString; }

		else {

			SpeedBuffer << std::fixed << 0;
			SpeedBuffer >> SpeedString; }

		HeightText.setFont( Font );
		HeightText.setCharacterSize( 10 );
		HeightText.setString( sf::String( HeightString ) + L" metrów nad ziemi¹" );
		HeightText.setPosition( sf::Vector2f( 25.00f, Window->getSize().y - 150.00f ) );
		HeightText.setColor( sf::Color( 255, 255, 255, 150 ) );

		DistanceText.setFont( Font );
		DistanceText.setCharacterSize( 10 );
		DistanceText.setString( sf::String( DistanceString ) + L" metrów od stacji" );
		DistanceText.setPosition( sf::Vector2f( 25.00f, Window->getSize().y - 130.00f ) );
		DistanceText.setColor( sf::Color( 255, 255, 255, 150 ) );

		SpeedText.setFont( Font );
		SpeedText.setCharacterSize( 10 );
		SpeedText.setString( sf::String( SpeedString ) + L" metrów na sekundê" );
		SpeedText.setPosition( sf::Vector2f( 25.00f, Window->getSize().y - 110.00f ) );
		SpeedText.setColor( sf::Color( 255, 255, 255, 150 ) );

		Window->draw( HeightText );
		Window->draw( DistanceText );
		Window->draw( SpeedText );

		sf::RectangleShape MiniMap;
		sf::RectangleShape MiniMapCamera;
		sf::RectangleShape MiniMapSatellite;
		
		MiniMap.setSize( sf::Vector2f( 110.00f, 110.00f ) );
		MiniMap.setPosition( sf::Vector2f( Window->getSize().x - 125.00f, Window->getSize().y - 125.00f ) );
		MiniMap.setFillColor( sf::Color( 255, 255, 255, 150 ) );

		MiniMapCamera.setSize( sf::Vector2f( 10.00f, 10.00f ) );
		MiniMapCamera.setOrigin( sf::Vector2f( 5.00f, 5.00f ) );
		MiniMapCamera.setPosition( sf::Vector2f( MiniMap.getPosition().x + 55.00f + ( Camera.x / 100.00 ), MiniMap.getPosition().y + 55.00f + ( Camera.z / 100.00 ) ) );
		MiniMapCamera.setFillColor( sf::Color( 0, 255, 0, 150 ) );

		MiniMapSatellite.setSize( sf::Vector2f( 10.00f, 10.00f ) );
		MiniMapSatellite.setOrigin( sf::Vector2f( 5.00f, 5.00f ) );
		MiniMapSatellite.setPosition( sf::Vector2f( MiniMap.getPosition().x + 55.00f + ( Satellite.x / 100.00 ), MiniMap.getPosition().y + 55.00f + ( Satellite.z / 100.00 ) ) );
		MiniMapSatellite.setFillColor( sf::Color( 255, 0, 0, 150 ) );

		Window->draw( MiniMap );
		Window->draw( MiniMapCamera );
		Window->draw( MiniMapSatellite );
		
		sf::RectangleShape CameraHeight;
		sf::RectangleShape SatelliteHeight;

		CameraHeight.setSize( sf::Vector2f( 20.00f, fmax( Camera.y * 0.011f, 0.00f ) ) );
		CameraHeight.setPosition( sf::Vector2f( Window->getSize().x - 195.00f, Window->getSize().y - 15.00f - fmax( Camera.y * 0.011f, 0.00f ) ) );
		CameraHeight.setFillColor( sf::Color( 0, 255, 0, 150 ) );

		SatelliteHeight.setSize( sf::Vector2f( 20.00f, fmax( Satellite.y * 0.011f, 0.00 ) ) );
		SatelliteHeight.setPosition( sf::Vector2f( Window->getSize().x - 160.00f, Window->getSize().y - 15.00f - fmax( Satellite.y * 0.011f, 0.00f ) ) );
		SatelliteHeight.setFillColor( sf::Color( 255, 0, 0, 150 ) );

		Window->draw( CameraHeight );
		Window->draw( SatelliteHeight );

		sf::Sprite MapIcon;
		sf::Sprite TerrainIcon;
		sf::Sprite GraphsIcon;

		MapIcon.setTexture( IconTexture[0] );
		MapIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 - 100.00f, 25.00f ) );
		MapIcon.setScale( 50.00f / IconTexture[0].getSize().x, 50.00f / IconTexture[0].getSize().y );
		MapIcon.setColor( sf::Color( 255, 255, 255, 100 ) );
		
		TerrainIcon.setTexture( IconTexture[1] );
		TerrainIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 - 25.00f, 25.00f ) );
		TerrainIcon.setScale( 50.00f / IconTexture[1].getSize().x, 50.00f / IconTexture[1].getSize().y );
		TerrainIcon.setColor( sf::Color( 255, 255, 255, 100 ) );

		GraphsIcon.setTexture( IconTexture[2] );
		GraphsIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 + 50.00f, 25.00f ) );
		GraphsIcon.setScale( 50.00f / IconTexture[2].getSize().x, 50.00f / IconTexture[2].getSize().y );
		GraphsIcon.setColor( sf::Color( 255, 255, 255, 100 ) );
		
		if ( Mouse.x >= MapIcon.getPosition().x && Mouse.y >= MapIcon.getPosition().y && Mouse.x < ( MapIcon.getPosition().x + 50.00f ) && Mouse.y < ( MapIcon.getPosition().y + 50.00f ) ) {
			
			MapIcon.setColor( sf::Color( 255, 255, 255, 200 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Map; } }

		if ( Mouse.x >= TerrainIcon.getPosition().x && Mouse.y >= TerrainIcon.getPosition().y && Mouse.x < ( TerrainIcon.getPosition().x + 50.00f ) && Mouse.y < ( TerrainIcon.getPosition().y + 50.00f ) ) {

			TerrainIcon.setColor( sf::Color( 255, 255, 255, 200 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Terrain; } }

		if ( Mouse.x >= GraphsIcon.getPosition().x && Mouse.y >= GraphsIcon.getPosition().y && Mouse.x < ( GraphsIcon.getPosition().x + 50.00f ) && Mouse.y < ( GraphsIcon.getPosition().y + 50.00f ) ) {

			GraphsIcon.setColor( sf::Color( 255, 255, 255, 200 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Graphs; } }

		Window->draw( MapIcon );
		Window->draw( TerrainIcon );
		Window->draw( GraphsIcon );

		Window->popGLStates(); }

	else if ( Mode == RenderMode::Graphs ) {

		Window->pushGLStates();

		size_t GraphWidth = ( Window->getSize().x - 75 ) / 2;
		size_t GraphHeight = ( Window->getSize().y - 200 ) / 2;

		UINT64 GraphTime = 0;
		UINT64 GraphBegin = time( NULL ) - GraphWidth;
		UINT64 GraphEnd = time( NULL );

		REAL64 * TemperatureGraph = new REAL64 [ GraphWidth ];
		REAL64 * PressureGraph = new REAL64 [ GraphWidth ];
		REAL64 * VoltageGraph = new REAL64 [ GraphWidth ];
		REAL64 * HeightGraph = new REAL64 [ GraphWidth ];

		for ( size_t i = 0; i < GraphWidth; i++ ) {
			
			TemperatureGraph[i] = 0.00;
			PressureGraph[i] = 400.00;
			VoltageGraph[i] = 0.00;
			HeightGraph[i] = 10.00; }
		
		for ( size_t i = ( SensorsPointer->GetMeasurementsCount( Sensors::MeasurementType::Temperature ) - GraphWidth ); i < SensorsPointer->GetMeasurementsCount( Sensors::MeasurementType::Temperature ); i++ ) {

			TemperatureGraph[i] = SensorsPointer->GetMeasurement( Sensors::MeasurementType::Temperature, i ).Value; }

		for ( size_t i = ( SensorsPointer->GetMeasurementsCount( Sensors::MeasurementType::Pressure ) - GraphWidth ); i < SensorsPointer->GetMeasurementsCount( Sensors::MeasurementType::Pressure ); i++ ) {

			PressureGraph[i] = SensorsPointer->GetMeasurement( Sensors::MeasurementType::Pressure, i ).Value / 100.00; }

		for ( size_t i = ( SensorsPointer->GetMeasurementsCount( Sensors::MeasurementType::Voltage ) - GraphWidth ); i < SensorsPointer->GetMeasurementsCount( Sensors::MeasurementType::Voltage ); i++ ) {

			VoltageGraph[i] = SensorsPointer->GetMeasurement( Sensors::MeasurementType::Voltage, i ).Value; }

		for ( size_t i = ( SensorsPointer->GetMeasurementsCount( Sensors::MeasurementType::Height ) - GraphWidth ); i < SensorsPointer->GetMeasurementsCount( Sensors::MeasurementType::Height ); i++ ) {

			HeightGraph[i] = SensorsPointer->GetMeasurement( Sensors::MeasurementType::Height, i ).Value / 1000.00; }
			
		RenderGraph( Window, TemperatureGraph, - 50.00, 50.00, 10.00, 25.00f, 125.00f, GraphWidth, GraphHeight );
		RenderGraph( Window, PressureGraph,	400.00, 1200.00, 100.00, 50.00f + GraphWidth, 125.00f, GraphWidth, GraphHeight );
		RenderGraph( Window, VoltageGraph, - 10.00, 10.00, 2, 25.00f, 175.00f + GraphHeight, GraphWidth, GraphHeight );
		RenderGraph( Window, HeightGraph, 0.00, 10.00, 1.00, 50.00f + GraphWidth, 175.00f + GraphHeight, GraphWidth, GraphHeight );

		delete TemperatureGraph;
		delete PressureGraph;
		delete VoltageGraph;
		delete HeightGraph;

		sf::Text TemperatureGraphText;
		sf::Text PressureGraphText;
		sf::Text VoltageGraphText;
		sf::Text HeightGraphText;

		TemperatureGraphText.setFont( Font );
		TemperatureGraphText.setCharacterSize( 15 );
		TemperatureGraphText.setString( L"Temperatura [ °C ]" );
		TemperatureGraphText.setPosition( sf::Vector2f( 25.00f, 100.00f ) );
		TemperatureGraphText.setColor( sf::Color( 0, 127, 255, 255 ) );

		PressureGraphText.setFont( Font );
		PressureGraphText.setCharacterSize( 15 );
		PressureGraphText.setString( L"Ciœnienie [ hPa ]" );
		PressureGraphText.setPosition( sf::Vector2f( 50.00f + GraphWidth, 100.00f ) );
		PressureGraphText.setColor( sf::Color( 0, 127, 255, 255 ) );

		VoltageGraphText.setFont( Font );
		VoltageGraphText.setCharacterSize( 15 );
		VoltageGraphText.setString( L"Napiêcie [ V ]" );
		VoltageGraphText.setPosition( sf::Vector2f( 25.00f, 150.00f + GraphHeight ) );
		VoltageGraphText.setColor( sf::Color( 0, 127, 255, 255 ) );

		HeightGraphText.setFont( Font );
		HeightGraphText.setCharacterSize( 15 );
		HeightGraphText.setString( L"Wysokoœæ [ km ]" );
		HeightGraphText.setPosition( sf::Vector2f( 50.00f + GraphWidth, 150.00f + GraphHeight ) );
		HeightGraphText.setColor( sf::Color( 0, 127, 255, 255 ) );

		Window->draw( TemperatureGraphText );
		Window->draw( PressureGraphText );
		Window->draw( VoltageGraphText );
		Window->draw( HeightGraphText );

		sf::Sprite MapIcon;
		sf::Sprite TerrainIcon;
		sf::Sprite GraphsIcon;

		MapIcon.setTexture( IconTexture[0] );
		MapIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 - 100.00f, 25.00f ) );
		MapIcon.setScale( 50.00f / IconTexture[0].getSize().x, 50.00f / IconTexture[0].getSize().y );
		MapIcon.setColor( sf::Color( 0, 127, 255, 255 ) );
		
		TerrainIcon.setTexture( IconTexture[1] );
		TerrainIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 - 25.00f, 25.00f ) );
		TerrainIcon.setScale( 50.00f / IconTexture[1].getSize().x, 50.00f / IconTexture[1].getSize().y );
		TerrainIcon.setColor( sf::Color( 0, 127, 255, 255 ) );

		GraphsIcon.setTexture( IconTexture[2] );
		GraphsIcon.setPosition( sf::Vector2f( Window->getSize().x / 2 + 50.00f, 25.00f ) );
		GraphsIcon.setScale( 50.00f / IconTexture[2].getSize().x, 50.00f / IconTexture[2].getSize().y );
		GraphsIcon.setColor( sf::Color( 0, 127, 255, 255 ) );
		
		if ( Mouse.x >= MapIcon.getPosition().x && Mouse.y >= MapIcon.getPosition().y && Mouse.x < ( MapIcon.getPosition().x + 50.00f ) && Mouse.y < ( MapIcon.getPosition().y + 50.00f ) ) {
			
			MapIcon.setColor( sf::Color( 0, 127, 255, 150 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Map; } }

		if ( Mouse.x >= TerrainIcon.getPosition().x && Mouse.y >= TerrainIcon.getPosition().y && Mouse.x < ( TerrainIcon.getPosition().x + 50.00f ) && Mouse.y < ( TerrainIcon.getPosition().y + 50.00f ) ) {

			TerrainIcon.setColor( sf::Color( 0, 127, 255, 150 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Terrain; } }

		if ( Mouse.x >= GraphsIcon.getPosition().x && Mouse.y >= GraphsIcon.getPosition().y && Mouse.x < ( GraphsIcon.getPosition().x + 50.00f ) && Mouse.y < ( GraphsIcon.getPosition().y + 50.00f ) ) {

			GraphsIcon.setColor( sf::Color( 0, 127, 255, 150 ) );
			
			if ( Mouse.z > 0.50f ) {

				Mode = RenderMode::Graphs; } }

		Window->draw( MapIcon );
		Window->draw( TerrainIcon );
		Window->draw( GraphsIcon );

		Window->popGLStates(); }

	Mouse.z = 0.00f;

	Window->display(); }

void Engine::RenderTerrain ( ) {

	glEnable( GL_TEXTURE_2D );

	glBegin( GL_QUADS );
	glColor3d( 1.0, 1.0, 1.0 );

	if ( Camera.y < 1000 ) {
		
		glBindTexture( GL_TEXTURE_2D, TerrainTexture );
		
		glTexCoord2d ( 0.0, 0.0 );
		glVertex3d( -5000.00, -1.00, -5000.00 );
		glTexCoord2d ( 0.0, 1.0 );
		glVertex3d( -5000.00, -1.00, 5000.00 );
		glTexCoord2d ( 1.0, 1.0 );
		glVertex3d( 5000.00, -1.00, 5000.00 );
		glTexCoord2d ( 1.0, 0.0 );
		glVertex3d( 5000.00, -1.00, -5000.00 ); }

	else if ( Camera.y < 5000 ) {

		glBindTexture( GL_TEXTURE_2D, TerrainTexture );

		glBegin( GL_QUADS );
		glColor3d( 1.0, 1.0, 1.0 );
			
		glTexCoord2d ( 0.0, 0.0 );
		glVertex3d( -5000.00, -25.00, -5000.00 );
		glTexCoord2d ( 0.0, 1.0 );
		glVertex3d( -5000.00, -25.00, 5000.00 );
		glTexCoord2d ( 1.0, 1.0 );
		glVertex3d( 5000.00, -25.00, 5000.00 );
		glTexCoord2d ( 1.0, 0.0 );
		glVertex3d( 5000.00, -25.00, -5000.00 ); }

	else {

		glBindTexture( GL_TEXTURE_2D, TerrainTexture );

		glBegin( GL_QUADS );
		glColor3d( 1.0, 1.0, 1.0 );
					
		glTexCoord2d ( 0.0, 0.0 );
		glVertex3d( -5000.00, -50.00, -5000.00 );
		glTexCoord2d ( 0.0, 1.0 );
		glVertex3d( -5000.00, -50.00, 5000.00 );
		glTexCoord2d ( 1.0, 1.0 );
		glVertex3d( 5000.00, -50.00, 5000.00 );
		glTexCoord2d ( 1.0, 0.0 );
		glVertex3d( 5000.00, -50.00, -5000.00 ); }
	
	glEnd();
	glFlush();

	glDisable( GL_TEXTURE_2D ); }

void Engine::RenderHorizont ( ) {

	if ( Camera.y < 1000 ) {

		REAL64 H = std::max( 5.00 * Camera.y, 1000.00 );

		glBegin( GL_QUADS );
		glColor3d( 0.0, 0.5, 1.0 );

		glVertex3d( Camera.x, -1500.00, Camera.z + H );
		glVertex3d( Camera.x + H, -1500.00, Camera.z );
		glVertex3d( Camera.x + H, 1500.00, Camera.z );
		glVertex3d( Camera.x, 1500.0, Camera.z + H );

		glVertex3d( Camera.x + H, -1500.00, Camera.z );
		glVertex3d( Camera.x, -1500.00, Camera.z - H );
		glVertex3d( Camera.x, 1500.00, Camera.z - H );
		glVertex3d( Camera.x + H, 1500.0, Camera.z );

		glVertex3d( Camera.x, -1500.00, Camera.z - H );
		glVertex3d( Camera.x - H, -1500.00, Camera.z );
		glVertex3d( Camera.x - H, 1500.00, Camera.z );
		glVertex3d( Camera.x, 1500.0, Camera.z - H );

		glVertex3d( Camera.x - H, -1500.00, Camera.z );
		glVertex3d( Camera.x, -1500.00, Camera.z + H );
		glVertex3d( Camera.x, 1500.00, Camera.z + H );
		glVertex3d( Camera.x - H, 1500.0, Camera.z );
			
		glEnd(); }

	glFlush(); }

void Engine::RenderSatellite ( ) {

	glBegin( GL_TRIANGLES );
	
	glColor3d( 1.0, 1.0, 1.0 );
	glVertex3d( Satellite.x + 2.00, Satellite.y, Satellite.z );
	glVertex3d( Satellite.x, Satellite.y, Satellite.z + 2.00 );
	glVertex3d( Satellite.x, Satellite.y + 2.00, Satellite.z );
	
	glColor3d( 0.0, 0.0, 0.0 );
	glVertex3d( Satellite.x, Satellite.y, Satellite.z + 2.00 );
	glVertex3d( Satellite.x - 2.00, Satellite.y, Satellite.z );
	glVertex3d( Satellite.x, Satellite.y + 2.00, Satellite.z );

	glColor3d( 1.0, 1.0, 1.0 );
	glVertex3d( Satellite.x - 2.00, Satellite.y, Satellite.z );
	glVertex3d( Satellite.x, Satellite.y, Satellite.z - 2.00 );
	glVertex3d( Satellite.x, Satellite.y + 2.00, Satellite.z );

	glColor3d( 0.0, 0.0, 0.0 );
	glVertex3d( Satellite.x, Satellite.y, Satellite.z - 2.00 );
	glVertex3d( Satellite.x + 2.00, Satellite.y, Satellite.z );
	glVertex3d( Satellite.x, Satellite.y + 2.00, Satellite.z );

	glColor3d( 0.0, 0.0, 0.0 );
	glVertex3d( Satellite.x + 2.00, Satellite.y, Satellite.z );
	glVertex3d( Satellite.x, Satellite.y, Satellite.z + 2.00 );
	glVertex3d( Satellite.x, Satellite.y - 2.00, Satellite.z );

	glColor3d( 1.0, 1.0, 1.0 );
	glVertex3d( Satellite.x, Satellite.y, Satellite.z + 2.00 );
	glVertex3d( Satellite.x - 2.00, Satellite.y, Satellite.z );
	glVertex3d( Satellite.x, Satellite.y - 2.00, Satellite.z );

	glColor3d( 0.0, 0.0, 0.0 );
	glVertex3d( Satellite.x - 2.00, Satellite.y, Satellite.z );
	glVertex3d( Satellite.x, Satellite.y, Satellite.z - 2.00 );
	glVertex3d( Satellite.x, Satellite.y - 2.00, Satellite.z );

	glColor3d( 1.0, 1.0, 1.0 );
	glVertex3d( Satellite.x, Satellite.y, Satellite.z - 2.00 );
	glVertex3d( Satellite.x + 2.00, Satellite.y, Satellite.z );
	glVertex3d( Satellite.x, Satellite.y - 2.00, Satellite.z );

	glEnd();
	glFlush(); }

void Engine::RenderStation ( ) {

	glBegin( GL_TRIANGLES );
	
	glColor3d( 1.0, 1.0, 0.0 );
	glVertex3d( Station.x + 2.00, Station.y, Station.z );
	glVertex3d( Station.x, Station.y, Station.z + 2.00 );
	glVertex3d( Station.x, Station.y + 2.00, Station.z );
	
	glColor3d( 0.0, 0.0, 0.0 );
	glVertex3d( Station.x, Station.y, Station.z + 2.00 );
	glVertex3d( Station.x - 2.00, Station.y, Station.z );
	glVertex3d( Station.x, Station.y + 2.00, Station.z );

	glColor3d( 1.0, 1.0, 0.0 );
	glVertex3d( Station.x - 2.00, Station.y, Station.z );
	glVertex3d( Station.x, Station.y, Station.z - 2.00 );
	glVertex3d( Station.x, Station.y + 2.00, Station.z );

	glColor3d( 0.0, 0.0, 0.0 );
	glVertex3d( Station.x, Station.y, Station.z - 2.00 );
	glVertex3d( Station.x + 2.00, Station.y, Station.z );
	glVertex3d( Station.x, Station.y + 2.00, Station.z );

	glEnd();
	glFlush(); }

void Engine::RenderGrid ( ) {

	glBegin( GL_LINES );

	glColor3d( 1.0, 1.0, 1.0 );
	glVertex3d( Satellite.x, 0.00, -5000.00 );
	glVertex3d( Satellite.x, 0.00, 5000.00 );
	glVertex3d( -5000.00, 0.00, Satellite.z );
	glVertex3d( 5000.00, 0.00, Satellite.z );

	glColor3d( 1.0, 1.0, 0.0 );
	glVertex3d( Station.x, 0.00, Station.z );
	glVertex3d( Satellite.x, 0.00, Satellite.z );

	if ( Camera.y < 1000 ) {

		UINT8 Hectometer = 9;
		UINT8 Kilometer = 9;

		for ( size_t i = 0; i < 1001; i++ ) {

			Hectometer++;

			if ( Hectometer == 10 ) {

				Kilometer++;

				if ( Kilometer == 10 ) {

					glColor3d( 1.0, 0.0, 0.0 );

					Kilometer = 0; }

				else {

					glColor3d( 0.0, 0.0, 1.0 ); }

				Hectometer = 0; }

			else {

				glColor3d( 0.0, 1.0, 0.0 ); }

			REAL64 Shift = ( (REAL64) i - 500 ) * 10.00;

			glVertex3d( Shift, 0.00, -5000.0 );
			glVertex3d( Shift, 0.00, 5000.0 );

			glVertex3d( -5000.0, 0.00, Shift );
			glVertex3d( 5000.0, 0.00, Shift ); } }

	else if ( Camera.y < 5000 ) {

		UINT8 Kilometer = 9;

		for ( size_t i = 0; i < 101; i++ ) {

			Kilometer++;

			if ( Kilometer == 10 ) {

				glColor3d( 1.0, 0.0, 0.0 );

				Kilometer = 0; }

			else {

				glColor3d( 0.0, 0.0, 1.0 ); }

			REAL64 Shift = ( (REAL64) i - 50 ) * 100.00;

			glVertex3d( Shift, 0.00, -5000.0 );
			glVertex3d( Shift, 0.00, 5000.0 );

			glVertex3d( -5000.0, 0.00, Shift );
			glVertex3d( 5000.0, 0.00, Shift ); } }

	else {

		glColor3d( 1.0, 0.0, 0.0 );

		for ( size_t i = 0; i < 11; i++ ) {

			REAL64 Shift = ( (REAL64) i - 5 ) * 1000.00;

			glVertex3d( Shift, 0.00, -5000.0 );
			glVertex3d( Shift, 0.00, 5000.0 );

			glVertex3d( -5000.0, 0.00, Shift );
			glVertex3d( 5000.0, 0.00, Shift ); } }

	glEnd();
	glFlush(); }

void Engine::RenderTrack ( ) {

	if ( TrackerPointer->GetPositionsCount() > 1 ) {

		glBegin( GL_LINES );
		glColor3d( 1.0, 1.0, 1.0 );

		for ( size_t i = 0; i < ( TrackerPointer->GetPositionsCount() - 1 ); i++ ) {

			glVertex3d( TrackerPointer->GetPosition( i ).Value.x, fmax( SensorsPointer->GetMeasurement( Sensors::Height, i ).Value, 0.00 ), TrackerPointer->GetPosition( i ).Value.z );
			glVertex3d( TrackerPointer->GetPosition( i + 1 ).Value.x, fmax( SensorsPointer->GetMeasurement( Sensors::Height, i + 1 ).Value, 0.00 ), TrackerPointer->GetPosition( i + 1 ).Value.z ); }

		glEnd();
		glFlush(); } }

void Engine::RenderGraph ( sf::RenderWindow * Window, REAL64 * Data, REAL64 Minimum, REAL64 Maximum, REAL64 Sample, REAL32 PositionX, REAL32 PositionY, REAL32 Width, REAL32 Height ) {

	sf::VertexArray Lines ( sf::PrimitiveType::Lines );
	sf::VertexArray Points ( sf::PrimitiveType::LinesStrip );

	for ( size_t i = 0; i < (size_t) ( ( Maximum - Minimum ) / Sample + 1 ); i++ ) {

		REAL32 Shift = i * ( Height / ( ( Maximum - Minimum ) / Sample ) );

		Lines.append( sf::Vertex( sf::Vector2f( PositionX, PositionY + Shift ), sf::Color( 0, 127, 255, 255 ) ) );
		Lines.append( sf::Vertex( sf::Vector2f( PositionX + Width - 1, PositionY + Shift ), sf::Color( 0, 127, 255, 255 ) ) ); \
		
		sf::Text Text;
		std::string String;
		std::stringstream Buffer;

		Buffer.precision( 0 );
		Buffer << std::fixed << ( Maximum - Sample * i );
		Buffer >> String;

		Text.setFont( Font );
		Text.setCharacterSize( 8 );
		Text.setString( String );
		Text.setPosition( sf::Vector2f( PositionX + Width - Text.getGlobalBounds().width, PositionY + Shift ) );
		Text.setColor( sf::Color( 0, 127, 255, 255 ) );

		Window->draw( Text ); }
	
	for ( size_t i = 0; i < (size_t) Width; i++ ) {

		REAL32 Value = Data[i];

		if ( Value < Minimum ) {

			Value = Minimum; }

		if ( Value > Maximum ) {

			Value = Maximum; }

		Value = ( Value - Minimum ) / ( Maximum - Minimum ) * Height;

		Points.append( sf::Vertex( sf::Vector2f( PositionX + i, PositionY + Height - Value ), sf::Color( 255, 0, 0, 255 ) ) ); }

	Window->draw( Lines );
	Window->draw( Points ); }