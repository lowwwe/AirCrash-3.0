/// <summary>
/// author Pete Lowe May 2025
/// you need to change the above line or lose marks
/// Also don't have any member properties called Delete...
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )
// ignore this warning
// C:\SFML - 3.0.0\include\SFML\System\Exception.hpp(41, 47) : 
// warning C4275 : non dll - interface class 'std::runtime_error' used as base for dll - interface class 'sf::Exception'

/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the #endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const unsigned WIDTH = 1000; // witdh of window
const unsigned HEIGHT = 800; // height of window

const sf::Color ULTRAMARINE{ 5, 55,242,255 }; // const colour

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void processMouseDown(const std::optional<sf::Event> t_event);
	void processMouseUp(const std::optional<sf::Event> t_event);

	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();
	void drawPlane(sf::Sprite& t_plane);
	bool collisionDistance(sf::Vector2f t_location1, float t_radius1, sf::Vector2f t_location2, float t_radius2);
	bool collisionBounding(sf::Sprite& t_plane1, sf::Sprite& t_plane2);

	void setupSprites();
	void setupSky();
	void setupBigPlane();
	void setupSamllPlane();
	void setupExplosion();

	void movePlanes();
	void keepOnScreen(sf::Vector2f& t_location);
	void animateExplosion();
	


	sf::Vector2f m_mouseDownLocation; // mouse down position

	sf::Texture m_planesTexture;// texture for planes
	sf::Sprite m_bigPlaneSprite{m_planesTexture};// sprite for big plane
	sf::Vector2f m_bigPlaneLocation{ 200.0f,200.0f };// location of big plane
	sf::Vector2f m_bigPlaneVelocity{ 1.0f,1.0f };// velocity of big plane
	sf::Angle m_bigPlaneHeading{ sf::degrees(135.0f) };// heading for big plane
	float m_bigPlaneRadius; // radikus of big plane

	sf::Sprite m_smallPlaneSptire{ m_planesTexture}; // smalll plane sprite
	sf::Vector2f m_smallPalneLocation{ 500.0f, 50.0f };// small palne location
	sf::Vector2f m_smallPlaneVelocity{ -0.6f,0.6f };// small plane velocity
	sf::Angle m_smallPlaneHeading{sf::degrees(225.0f)};// small plane heading
	float m_smallPlaneRadius;// radius of bound circle for small plane


	sf::Texture m_explosionTexture;// texure
	sf::Sprite m_explosionSprite{m_explosionTexture};// srite
	bool m_exploding{ false };// are we exploding
	int m_explosionFrame{ 0 };// current frame
	float m_explosionFrameCounter{ 0.0f };// frame counter
	float m_frameIncrement{ 0.6f };// frame increment
	
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_jerseyFont;// font used by message
	
	sf::Texture m_skyTexture;  //texture for sky
	sf::Sprite m_skySprite{m_skyTexture};// sprite for sky
	bool m_DELETEexitGame; // control exiting game

	bool m_debugging{ false }; // are we showing the boxes
	bool m_boundingBoxes{ false };
	bool m_stop{ false }; // stop on crash

};

#pragma warning( pop ) 
#endif // !GAME_HPP

