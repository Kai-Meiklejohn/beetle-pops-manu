#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

// The Player class owns the player's shape, movement state,
// and the behaviour required to update and draw the player.
class Player
{
public:
	// Construct the player and configure its appearance
	Player();

	// Handle individual events, such as pressing the jump key
	void handleEvent(const sf::Event& event);

	// Update movement, gravity, and collision every frame
	void update(
		float deltaTime,
		const std::vector<sf::RectangleShape>& platforms,
		sf::Vector2u windowSize
	);

	// Draw the player without changing it
	void draw(sf::RenderWindow& window) const;

private:
	// The SFML shape currently used to represent the player
	sf::CircleShape shape_;

	// The player's current vertical speed
	float verticalVelocity_{ 0.f };

	// Records whether the player is standing on the floor or a platform
	bool isGrounded_{ false };

	// Player movement and physics settings
	static constexpr float movementSpeed_{ 200.f };
	static constexpr float gravity_{ 1200.f };
	static constexpr float jumpSpeed_{ 500.f };
};