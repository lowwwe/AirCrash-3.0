/// <summary>
/// author Pete Lowe Feb 2026
/// you need to change the above line or lose marks
/// </summary>


#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the texts
/// load and setup the images
/// load and setup the sounds
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{WIDTH, HEIGHT}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{false} //when true game will exit
{
	
	setupSprites(); // load texture
	
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) // close window message 
		{
			m_DELETEexitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (newEvent->is<sf::Event::MouseButtonPressed>())
		{
			processMouseDown(newEvent);
		}
		if (newEvent->is<sf::Event::MouseButtonReleased>())
		{
			processMouseUp(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_DELETEexitGame = true; 
	}
	if (sf::Keyboard::Key::F1 == newKeypress->code)
	{
		m_debugging = !m_debugging;
	}
	if (sf::Keyboard::Key::B == newKeypress->code)
	{
		if (m_boundingBoxes)
		{
			m_boundingBoxes = false;
			m_window.setTitle("Distance collision");
		}
		else
		{
			m_boundingBoxes = true;
			m_window.setTitle("Bounding Boxes");
		}

	}
}

void Game::processMouseDown(const std::optional<sf::Event> t_event)
{
	const sf::Event::MouseButtonPressed* newMousePress = t_event->getIf<sf::Event::MouseButtonPressed>();
	m_mouseDownLocation.x =  static_cast<float>(newMousePress->position.x);
	m_mouseDownLocation.y = static_cast<float>(newMousePress->position.y);
}

void Game::processMouseUp(const std::optional<sf::Event> t_event)
{
	sf::Vector2f mouseUpLocation;
	sf::Vector2f displacement;
	float headingRadians;
	const sf::Event::MouseButtonReleased* newMouseRelease = t_event->getIf<sf::Event::MouseButtonReleased>();

	mouseUpLocation = static_cast<sf::Vector2f>(newMouseRelease->position);
	displacement = mouseUpLocation - m_mouseDownLocation;
	headingRadians = std::atan2(displacement.y, displacement.x);
	headingRadians += 1.57; // add 90 deg cause drawing
	if (sf::Mouse::Button::Left == newMouseRelease->button)
	{
		m_bigPlaneHeading = sf::radians(headingRadians);
		m_bigPlaneVelocity = displacement / 100.0f;
		m_bigPlaneSprite.setRotation(m_bigPlaneHeading);
	}
	if (sf::Mouse::Button::Right == newMouseRelease->button)
	{
		m_smallPlaneHeading = sf::radians(headingRadians);
		m_smallPlaneSptire.setRotation(m_smallPlaneHeading);
		m_smallPlaneVelocity = displacement / 50.0f;
	}


}

/// <summary>
/// Check if any keys are currently pressed
/// </summary>
void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_DELETEexitGame = true; 
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	movePlanes();
	keepOnScreen(m_bigPlaneLocation);
	keepOnScreen(m_smallPalneLocation);
	if (m_boundingBoxes)
	{
		if (collisionBounding(m_bigPlaneSprite, m_smallPlaneSptire))
		{
			//m_bigPlaneVelocity = sf::Vector2f{ 0.0f,0.0f };
			//m_smallPlaneVelocity = sf::Vector2f{ 0.0f,0.0f };
		}
	}
	else
	{
		if (collisionDistance(m_bigPlaneLocation, m_bigPlaneRadius, m_smallPalneLocation, m_smallPlaneRadius))
		{
			//m_bigPlaneVelocity = sf::Vector2f{ 0.0f,0.0f };
			//m_smallPlaneVelocity = sf::Vector2f{ 0.0f,0.0f };
		}
	}
	if (m_exploding)
	{
		animateExplosion();
	}
	if (m_DELETEexitGame)
	{
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(ULTRAMARINE);

	m_window.draw(m_skySprite);

	if (m_debugging)
	{
		drawPlane(m_smallPlaneSptire);
		drawPlane(m_bigPlaneSprite);
	}
	else
	{
		m_window.draw(m_bigPlaneSprite);
		m_window.draw(m_smallPlaneSptire);
	}
	if (m_exploding)
	{
		m_window.draw(m_explosionSprite);
	}
	m_window.display();
}

void Game::drawPlane(sf::Sprite& t_plane)
{
	sf::CircleShape dot{ 4.0f };
	sf::CircleShape ring;
	sf::RectangleShape globalBounds;
	sf::RectangleShape localBounds;
	float ringRadius = 0.0f;

	m_window.draw(t_plane);
	
	localBounds.setFillColor(sf::Color::Transparent);
	localBounds.setOutlineColor(sf::Color::Black);
	localBounds.setOutlineThickness(2.0f);
	localBounds.setPosition(t_plane.getPosition());
	localBounds.setRotation(t_plane.getRotation());
	localBounds.setSize(t_plane.getLocalBounds().size);
	localBounds.setOrigin(t_plane.getOrigin());


	globalBounds.setFillColor(sf::Color::Transparent);
	globalBounds.setOutlineColor(sf::Color::Green);
	globalBounds.setOutlineThickness(2.0f);
	
	globalBounds.setSize(t_plane.getGlobalBounds().size);		

	globalBounds.setOrigin(sf::Vector2f{ 
		t_plane.getGlobalBounds().size.x / 2.0f
		, t_plane.getGlobalBounds().size.y / 2.0f });
	globalBounds.setPosition(t_plane.getPosition());

	dot.setFillColor(sf::Color::Yellow);
	dot.setOrigin(sf::Vector2f{ 4.0f,4.0f });
	dot.setPosition(t_plane.getPosition());

	ring.setFillColor(sf::Color::Transparent);
	ring.setOutlineColor(sf::Color::Red);
	ring.setOutlineThickness(2.0f);

	if (t_plane.getLocalBounds().size.x > t_plane.getLocalBounds().size.y)
	{
		ringRadius = t_plane.getLocalBounds().size.x / 2.0f;
	}
	else
	{
		ringRadius = t_plane.getLocalBounds().size.y / 2.0f;
	}
	ring.setRadius(ringRadius);
	ring.setOrigin(sf::Vector2f{ ringRadius,ringRadius });
	ring.setPosition(t_plane.getPosition());

	m_window.draw(dot);
	m_window.draw(ring);
	m_window.draw(globalBounds);
	m_window.draw(localBounds);
}

bool Game::collisionDistance(sf::Vector2f t_location1, float t_radius1, sf::Vector2f t_location2, float t_radius2)
{
	bool result = false;
	sf::Vector2f displacement;
	float distance;
	float minmumSafeDistance;

	minmumSafeDistance = t_radius2 + t_radius1;
	displacement = t_location1 - t_location2;
	distance = displacement.length();
	if (distance < minmumSafeDistance)
	{
		m_exploding = true;
		m_explosionSprite.setPosition((t_location1 + t_location2) / 2.0f);
		result = true;
	}


	return result;
}

bool Game::collisionBounding(sf::Sprite& t_plane1, sf::Sprite& t_plane2)
{
	bool result = false;
	std::optional<sf::FloatRect> overlap = t_plane1.getGlobalBounds().findIntersection(t_plane2.getGlobalBounds());
	if (overlap.has_value())
	{
		result = true;
		m_exploding = true;
		m_explosionSprite.setPosition(overlap->position + overlap->size / 2.0f);
	}
	return result;
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>


/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprites()
{
	setupSky();
	setupBigPlane();
	setupSamllPlane();
	setupExplosion();
}

/// <summary>
/// load texure and set repeated for sky
/// </summary>
void Game::setupSky()
{
	if (!m_skyTexture.loadFromFile("ASSETS/IMAGES/sky.jpg"))
	{
		std::cout << "the is falling down";
	}
	m_skyTexture.setRepeated(true);
	m_skySprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,0}, sf::Vector2i{WIDTH,HEIGHT} });
	m_skySprite.setPosition(sf::Vector2f{ 0.0f,0.0f });
}

void Game::setupBigPlane()
{

	if (!m_planesTexture.loadFromFile("assets/images/planes.png"))
	{
		std::cout << "no planes";
	}
	m_bigPlaneSprite.setTextureRect(sf::IntRect{ sf::Vector2i{3,11}, sf::Vector2i{104,93} });
	m_bigPlaneSprite.setOrigin(sf::Vector2f{ 52.0f,46.5f });
	m_bigPlaneSprite.setPosition(m_bigPlaneLocation);
	m_bigPlaneSprite.setRotation(m_bigPlaneHeading);
	m_bigPlaneRadius = 52.0f;
}

void Game::setupSamllPlane()
{
	m_smallPlaneSptire.setTextureRect(sf::IntRect{ sf::Vector2i{362,115}, sf::Vector2i{87,69} });
	m_smallPlaneSptire.setOrigin(sf::Vector2f{ 43.5f,34.5f });
	m_smallPlaneSptire.setPosition(m_smallPalneLocation);
	m_smallPlaneSptire.setRotation(m_smallPlaneHeading);
	m_smallPlaneRadius = 43.5f;
}

void Game::setupExplosion()
{
	if (!m_explosionTexture.loadFromFile("assets/images/explosion.png"))
	{
		std::cout << "where my bang gone";
	}
	m_explosionSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,0}, sf::Vector2i{100,100} });
	m_explosionSprite.setOrigin(sf::Vector2f{ 50.0f,50.0f });

}

void Game::movePlanes()
{
	m_bigPlaneLocation += m_bigPlaneVelocity;
	m_bigPlaneSprite.setPosition(m_bigPlaneLocation);
	m_smallPalneLocation += m_smallPlaneVelocity;
	m_smallPlaneSptire.setPosition(m_smallPalneLocation);
}

void Game::keepOnScreen(sf::Vector2f& t_location)
{
	float screenWidth = static_cast<float>(WIDTH);
	float screenHeight = static_cast<float>(HEIGHT);
	if (t_location.x < 0.0f)
	{
		t_location.x = 0.0f;
	}
	if (t_location.x > screenWidth)
	{
		t_location.x = screenWidth;
	}
	if (t_location.y < 0.0f)
	{
		t_location.y = 0.0f;
	}
	if (t_location.y > screenHeight)
	{
		t_location.y = screenHeight;
	}

}

void Game::animateExplosion()
{
	int frame;
	const int ROWS = 6;
	const int COLS = 8;
	const int SIZE = 100;
	int col;
	int row;

	m_explosionFrameCounter += m_frameIncrement;
	frame = static_cast<int>(m_explosionFrameCounter);
	if (frame > 47)
	{
		m_exploding = false;
		frame = 0;
		m_explosionFrameCounter = 0.0f;
	}
	if (frame != m_explosionFrame)
	{
		col = frame % COLS;
		row = frame / COLS;
		m_explosionSprite.setTextureRect(sf::IntRect{ sf::Vector2i{col * SIZE, row * SIZE}, sf::Vector2i{SIZE,SIZE} });
	}
}


