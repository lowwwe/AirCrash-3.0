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
	m_window.draw(m_bigPlaneSprite);
	m_window.draw(m_smallPlaneSptire);
	
	m_window.display();
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
}

void Game::setupSamllPlane()
{
	m_smallPlaneSptire.setTextureRect(sf::IntRect{ sf::Vector2i{362,115}, sf::Vector2i{87,69} });
	m_smallPlaneSptire.setOrigin(sf::Vector2f{ 43.5f,34.5f });
	m_smallPlaneSptire.setPosition(m_smallPalneLocation);
	m_smallPlaneSptire.setRotation(m_smallPlaneHeading);
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


