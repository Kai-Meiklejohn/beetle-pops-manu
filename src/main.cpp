#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window( sf::VideoMode( { 200, 200 } ), "Beetle Pops a Manu!" );
	sf::CircleShape shape( 100.f );
	shape.setFillColor( sf::Color::Green );

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		// Handle keyboard input to move the shape
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			shape.move({ -1.f, 0.f });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			shape.move({ 1.f, 0.f });
		}

		window.clear();
		window.draw( shape );
		window.display();
	}
}
