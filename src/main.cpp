#include <optional>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Player.hpp"

// Create and return the collectibles for a fresh level
std::vector<sf::CircleShape> createCollectibles()
{
	// Store the collectibles created by this function
	std::vector<sf::CircleShape> collectibles;

	// Create a collectible on the first platform
	collectibles.emplace_back(10.f);
	collectibles.back().setPosition({ 150.f, 310.f });
	collectibles.back().setFillColor(sf::Color::Yellow);

	// Create a collectible on the second platform
	collectibles.emplace_back(10.f);
	collectibles.back().setPosition({ 430.f, 400.f });
	collectibles.back().setFillColor(sf::Color::Yellow);

	// Create a collectible on the third platform
	collectibles.emplace_back(10.f);
	collectibles.back().setPosition({ 180.f, 490.f });
	collectibles.back().setFillColor(sf::Color::Yellow);

	// Give the completed vector to the caller
	return collectibles;
}

// Describe the possible states of the current level
enum class GameState
{
	Playing,
	Completed,
	Dead
};

int main()
{
	// Start the level in its normal playing state
	GameState gameState{ GameState::Playing };

	// Create a window with the specified size and title
	sf::RenderWindow window(
		sf::VideoMode({ 800, 600 }),
		"Beetle Pops a Manu!"
	);

	// Prevent held keys from generating repeated KeyPressed events
	window.setKeyRepeatEnabled(false);

	// Create the player object
	Player player;

	// Create the collectibles for the initial level
	std::vector<sf::CircleShape> collectibles{
		createCollectibles()
	};

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

	// Create a dangerous area on the floor
	sf::RectangleShape hazard{{ 120.f, 20.f }};
	hazard.setPosition({ 350.f, 580.f });
	hazard.setFillColor(sf::Color::Red);

	// Create a pool on the right side of the level
	sf::RectangleShape water{ { 200.f, 80.f } };
	water.setPosition({ 570.f, 520.f });
	water.setFillColor(sf::Color{ 30, 120, 220 });

	// Create a white ring to represent a temporary splash
	sf::CircleShape splash{ 35.f };
	splash.setFillColor(sf::Color::Transparent);
	splash.setOutlineColor(sf::Color::White);
	splash.setOutlineThickness(3.f);

	// Position the ring using its centre instead of its top-left corner
	splash.setOrigin({ 35.f, 35.f });

	// Remember whether the player overlapped the water last frame
	bool wasInWater{ false };

	// Track how long the splash should remain visible
	float splashTimeRemaining{ 0.f };

	// Specify how long each splash lasts, in seconds
	constexpr float splashDuration{ 0.4f };

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

			// Check whether this event represents a key being pressed
			if (const auto* keyPressed =
				event->getIf<sf::Event::KeyPressed>())
			{
				// Restart the level when R is pressed
				if (keyPressed->code == sf::Keyboard::Key::R)
				{
					// Reset the player's position and physics
					player.reset();

					// Replace the remaining collectibles with a fresh set
					collectibles = createCollectibles();

					// Return the game to its playing state
					gameState = GameState::Playing;

					// Clear water-entry tracking for the new attempt
					wasInWater = false;

					// Remove any splash left over from the previous attempt
					splashTimeRemaining = 0.f;

					// Restore the original window title
					window.setTitle("Beetle Pops a Manu!");
				}
			}

			// Allow player input only while the level is still running
			if (gameState == GameState::Playing)
			{
				player.handleEvent(*event);
			}
		}

		// Update gameplay only while the player is still playing
		if (gameState == GameState::Playing)
		{
			// Update the player's movement, physics, and collisions
			player.update(
				deltaTime,
				platforms,
				window.getSize()
			);

			// Count down the remaining splash duration
			if (splashTimeRemaining > 0.f)
			{
				splashTimeRemaining -= deltaTime;
			}

			// Check whether the player currently overlaps the water
			const bool isInWater{
				player.getBounds().findIntersection(
					water.getGlobalBounds()
				).has_value()
			};

			// Trigger a splash only when entering the water
			if (isInWater && !wasInWater)
			{
				// Start the splash timer
				splashTimeRemaining = splashDuration;

				// Get the player's current rectangular bounds
				const auto playerBounds{ player.getBounds() };

				// Find the player's horizontal centre
				const float playerCentreX{
					playerBounds.position.x + playerBounds.size.x / 2.f
				};

				// Place the splash at the water surface beneath the player
				splash.setPosition({
					playerCentreX,
					water.getPosition().y
					});
			}

			// Remember this frame's water state for the next frame
			wasInWater = isInWater;

			// Check whether the player is touching the hazard
			const auto hazardIntersection{
				player.getBounds().findIntersection(
					hazard.getGlobalBounds()
				)
			};
			
			// End the attempt when the player touches the hazard
			if (hazardIntersection.has_value())
			{
				gameState = GameState::Dead;
				window.setTitle("You died! Press R to restart");
			}

			// Check collectibles only if the player survived this frame
			if (gameState == GameState::Playing)
			{
				// Start at the first remaining collectible
				auto iterator{ collectibles.begin() };

				// Check every remaining collectible
				while (iterator != collectibles.end())
				{
					// Check whether the player overlaps this collectible
					const auto intersection{
						player.getBounds().findIntersection(
							iterator->getGlobalBounds()
						)
					};

					if (intersection.has_value())
					{
						// Remove the item and receive the next valid iterator
						iterator = collectibles.erase(iterator);
					}
					else
					{
						// Move forward when nothing was removed
						++iterator;
					}
				}

				// Complete the level after collecting every item
				if (collectibles.empty())
				{
					gameState = GameState::Completed;
					window.setTitle("Level complete!");
				}
			}
		}

		// Change the background colour after completing the level
		if (gameState == GameState::Completed)
		{
			window.clear(sf::Color{ 30, 80, 50 });
		}
		else if (gameState == GameState::Dead)
		{
			window.clear(sf::Color{ 80, 20, 20 });
		}
		else
		{
			window.clear(sf::Color::Black);
		}

		// Draw the pool behind the other level objects
		window.draw(water);

		// Draw every platform in the level
		for (const auto& platform : platforms)
		{
			window.draw(platform);
		}

		// Draw every collectible that has not been collected
		for (const auto& collectible : collectibles)
		{
			window.draw(collectible);
		}

		// Draw the dangerous area on the floor
		window.draw(hazard);

		// Draw the splash while its timer is active
		if (splashTimeRemaining > 0.f)
		{
			window.draw(splash);
		}

		// Draw the player after drawing the platforms
		player.draw(window);

		// Display the completed frame
		window.display();
	}
}

