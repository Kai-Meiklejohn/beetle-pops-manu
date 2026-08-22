#include <algorithm>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(
		sf::VideoMode({ 800, 600 }),
		"Beetle Pops a Manu!"
	);

	sf::CircleShape shape{ 25.f };
	shape.setFillColor( sf::Color::Green );

	sf::Clock clock;
	constexpr float movementSpeed{ 200.f };

	float verticalVelocity{ 0.f };

	constexpr float gravity{ 1200.f };

	while ( window.isOpen() )
	{
		const float deltaTime{ clock.restart().asSeconds() };

		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
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

		if (position.y > maximumY)
		{
			position.y = maximumY;
			verticalVelocity = 0.f;
		}

		shape.setPosition(position);

		// Render the shape
		window.clear();
		window.draw( shape );
		window.display();
	}
}
