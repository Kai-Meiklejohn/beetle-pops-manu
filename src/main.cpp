#include <algorithm>
#include <SFML/Graphics.hpp>

int main()
{
	// Create a window with the specified size and title
	sf::RenderWindow window(
		sf::VideoMode({ 800, 600 }),
		"Beetle Pops a Manu!"
	);

	// Prevent held keys from generating repeated KeyPressed events
	window.setKeyRepeatEnabled(false);

	// Create a circle shape with a radius of 25 pixels and set its fill color to green
	sf::CircleShape shape{ 25.f };
	shape.setFillColor( sf::Color::Green );

	// Create a platform with the specified size and set its fill color to blue
	sf::RectangleShape platform{ { 200.f, 20.f } };
	platform.setPosition({ 300.f, 510.f });
	platform.setFillColor(sf::Color::Blue);

	// Track elapsed time for frame-rate-independent movement
	sf::Clock clock;

	float verticalVelocity{ 0.f };
	bool isGrounded{ false };

	const float circleDiameter{ shape.getRadius() * 2.f };
	constexpr float movementSpeed{ 200.f };
	constexpr float gravity{ 1200.f };
	constexpr float jumpSpeed{ 500.f };

	while ( window.isOpen() )
	{
		const float deltaTime{ clock.restart().asSeconds() };

		// Handle window events
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();

			if (const auto* keyPressed =
				event->getIf<sf::Event::KeyPressed>()) // Check if the event is a KeyPressed event
			{
				if ((keyPressed->code == sf::Keyboard::Key::Space ||
					keyPressed->code == sf::Keyboard::Key::Up) &&
					isGrounded)
				{
					verticalVelocity = -jumpSpeed;
					isGrounded = false;
				}
			}
		}

		// Handle keyboard input to move the shape
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			shape.move({ -movementSpeed * deltaTime, 0.f });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			shape.move({ movementSpeed * deltaTime, 0.f });
		}

		// This records where the player’s bottom was before vertical movement.
		const float previousBottom{
			shape.getPosition().y + circleDiameter
		};

		// Apply gravity to the vertical velocity
		verticalVelocity += gravity * deltaTime;

		shape.move({
			0.f,
			verticalVelocity * deltaTime
		});

		// Clamp the shape's position to stay within the window bounds
		auto position = shape.getPosition();
		
		const float maximumX{
			static_cast<float>(window.getSize().x) - circleDiameter
		};

		const float maximumY{
			static_cast<float>(window.getSize().y) - circleDiameter
		};

		position.x = std::clamp(position.x, 0.f, maximumX);


		// Check for collision with the platform
		const auto platformPosition = platform.getPosition();
		const auto platformSize = platform.getSize();

		const float playerRight{
			position.x + circleDiameter
		};

		const float playerBottom{
			position.y + circleDiameter
		};

		const float platformRight{
			platformPosition.x + platformSize.x
		};

		// Checks whether the player is above some portion of the platform.
		const bool overlapsHorizontally{
			playerRight > platformPosition.x &&
			position.x < platformRight
		};

		// Checks whether the player moved from above the platform’s top to below it during this frame.
		const bool crossedPlatformTop{
			previousBottom <= platformPosition.y &&
			playerBottom >= platformPosition.y
		};

		isGrounded = false;

		// Platform collision detection and response
		if (verticalVelocity >= 0.f &&
			overlapsHorizontally &&
			crossedPlatformTop)
		{
			position.y = platformPosition.y - circleDiameter;
			verticalVelocity = 0.f;
			isGrounded = true;
		}

		// Clamp the shape's vertical position and handle ground collision
		if (position.y >= maximumY)
		{
			position.y = maximumY;
			verticalVelocity = 0.f;
			isGrounded = true;
		}

		shape.setPosition(position);

		// Render the shape
		window.clear();
		window.draw( platform );
		window.draw( shape );
		window.display();
	}
}
