#include "Player.hpp"

#include <algorithm>

// Construct the player's circle with a radius of 25 pixels
Player::Player()
	: shape_{ 25.f }
{
	// Set the player's current placeholder colour
	shape_.setFillColor(sf::Color::Green);
}

// Handle events that should happen once when a key is pressed
void Player::handleEvent(const sf::Event& event)
{
	// Check whether the supplied event is a KeyPressed event
	const auto* keyPressed{
		event.getIf<sf::Event::KeyPressed>()
	};

	// If this is not a KeyPressed event, there is nothing else to handle
	if (keyPressed == nullptr)
		return;

	// Check whether either of the jump keys was pressed
	const bool jumpPressed{
		keyPressed->code == sf::Keyboard::Key::Space ||
		keyPressed->code == sf::Keyboard::Key::Up
	};

	// Allow jumping only while the player is standing on something
	if (jumpPressed && isGrounded_)
	{
		// Negative vertical velocity moves the player upwards
		verticalVelocity_ = -jumpSpeed_;

		// The player is no longer grounded after beginning a jump
		isGrounded_ = false;
	}
}

// Update the player's movement, gravity, and collisions
void Player::update(
	float deltaTime,
	const std::vector<sf::RectangleShape>& platforms,
	sf::Vector2u windowSize
)
{
	// Handle continuous left movement while the key is held
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		shape_.move({
			-movementSpeed_ * deltaTime,
			0.f
		});
	}

	// Handle continuous right movement while the key is held
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		shape_.move({
			movementSpeed_ * deltaTime,
			0.f
		});
	}

	// Calculate the player's full width and height
	const float diameter{
		shape_.getRadius() * 2.f
	};

	// Record the player's bottom before applying vertical movement
	const float previousBottom{
		shape_.getPosition().y + diameter
	};

	// Gravity increases the player's downward velocity over time
	verticalVelocity_ += gravity_ * deltaTime;

	// Move the player vertically using its current velocity
	shape_.move({
		0.f,
		verticalVelocity_ * deltaTime
	});

	// Get a copy of the player's new position after movement
	auto position{ shape_.getPosition() };

	// Calculate the furthest valid horizontal position
	const float maximumX{
		static_cast<float>(windowSize.x) - diameter
	};

	// Calculate the floor position for the player
	const float maximumY{
		static_cast<float>(windowSize.y) - diameter
	};

	// Prevent the player from leaving the left or right side of the window
	position.x = std::clamp(position.x, 0.f, maximumX);

	// Calculate the player's right edge after movement
	const float playerRight{
		position.x + diameter
	};

	// Calculate the player's bottom edge after movement
	const float playerBottom{
		position.y + diameter
	};

	// Assume the player is in the air until a collision proves otherwise
	isGrounded_ = false;

	// Check the player against every platform in the level
	for (const auto& platform : platforms)
	{
		// Get the current platform's position and size
		const auto platformPosition{ platform.getPosition() };
		const auto platformSize{ platform.getSize() };

		// Calculate the right edge of the platform
		const float platformRight{
			platformPosition.x + platformSize.x
		};

		// Check whether the player overlaps the platform horizontally
		const bool overlapsHorizontally{
			playerRight > platformPosition.x &&
			position.x < platformRight
		};

		// Check whether the player's bottom crossed the platform's top
		// during this frame
		const bool crossedPlatformTop{
			previousBottom <= platformPosition.y &&
			playerBottom >= platformPosition.y
		};

		// Land only when falling, horizontally overlapping the platform,
		// and crossing the platform's top from above
		if (verticalVelocity_ >= 0.f &&
			overlapsHorizontally &&
			crossedPlatformTop)
		{
			// Place the player's bottom directly on the platform's top
			position.y = platformPosition.y - diameter;

			// Stop downward movement
			verticalVelocity_ = 0.f;

			// Allow the player to jump again
			isGrounded_ = true;

			// Stop checking after finding the platform we landed on
			break;
		}
	}

	// Handle collision with the bottom of the window
	if (position.y >= maximumY)
	{
		// Place the player directly on the floor
		position.y = maximumY;

		// Stop downward movement
		verticalVelocity_ = 0.f;

		// Allow the player to jump again
		isGrounded_ = true;
	}

	// Apply the corrected position after resolving collisions
	shape_.setPosition(position);
}

// Draw the player's shape in the supplied window
void Player::draw(sf::RenderWindow& window) const
{
	window.draw(shape_);
}