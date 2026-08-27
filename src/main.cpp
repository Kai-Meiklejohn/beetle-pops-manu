#include <optional>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Player.hpp"

int main()
{
	// Create a window with the specified size and title
	sf::RenderWindow window(
		sf::VideoMode({ 800, 600 }),
		"Beetle Pops a Manu!"
	);

	// Prevent held keys from generating repeated KeyPressed events
	window.setKeyRepeatEnabled(false);

	// Create the player object
	Player player;

	// Create a vector containing the platforms in the level
	std::vector<sf::RectangleShape> platforms;

	// Create and configure the first platform
	platforms.emplace_back(sf::Vector2f{ 180.f, 20.f });
	platforms.back().setPosition({ 100.f, 330.f });
	platforms.back().setFillColor(sf::Color::Blue);

	// Create and configure the second platform
	platforms.emplace_back(sf::Vector2f{ 180.f, 20.f });
	platforms.back().setPosition({ 350.f, 420.f });
	platforms.back().setFillColor(sf::Color::Blue);

	// Create and configure the third platform
	platforms.emplace_back(sf::Vector2f{ 200.f, 20.f });
	platforms.back().setPosition({ 100.f, 510.f });
	platforms.back().setFillColor(sf::Color::Blue);

	// Track elapsed time for frame-rate-independent movement
	sf::Clock clock;

	// Continue running until the window is closed
	while (window.isOpen())
	{
		// Measure the number of seconds since the previous frame
		const float deltaTime{
			clock.restart().asSeconds()
		};

		// Process every event that has occurred since the previous frame
		while (const std::optional event = window.pollEvent())
		{
			// Close the window when the user presses its close button
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			// Give the player an opportunity to handle this event
			player.handleEvent(*event);
		}

		// Update the player's movement, physics, and collisions
		player.update(
			deltaTime,
			platforms,
			window.getSize()
		);

		// Clear everything drawn during the previous frame
		window.clear();

		// Draw every platform in the level
		for (const auto& platform : platforms)
		{
			window.draw(platform);
		}

		// Draw the player after drawing the platforms
		player.draw(window);

		// Display the completed frame
		window.display();
	}
}