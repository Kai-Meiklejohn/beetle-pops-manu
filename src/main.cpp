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

	// Track elapsed time for frame-rate-independent movement
	sf::Clock clock;;

	float verticalVelocity{ 0.f };
	bool isGrounded{ false };

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

		// Apply gravity to the vertical velocity
		verticalVelocity += gravity * deltaTime;

		shape.move({
			0.f,
			verticalVelocity * deltaTime
		});

		// Clamp the shape's position to stay within the window bounds
		auto position = shape.getPosition();
		
		const float circleDiameter{ shape.getRadius() * 2.f };

		const float maximumX{
			static_cast<float>(window.getSize().x) - circleDiameter
		};

		const float maximumY{
			static_cast<float>(window.getSize().y) - circleDiameter
		};

		position.x = std::clamp(position.x, 0.f, maximumX);

		isGrounded = false;

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
		window.draw( shape );
		window.display();
	}
}
