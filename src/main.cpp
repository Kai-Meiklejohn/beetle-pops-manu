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

		window.clear();
		window.draw( shape );
		window.display();
	}
}
