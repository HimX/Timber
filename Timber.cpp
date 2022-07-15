#include <sstream>
#include <SFML/Graphics.hpp>

// Make code easier to type with "using namespace"
using namespace sf;

// This is where our game starts from
int main() {
    // Create a video mode object
    VideoMode vm(1920, 1080);

    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    // Create a texture to hold a graphic on the GPU
    Texture textureBackground;

    // Load a graphic into the texture
    textureBackground.loadFromFile("../graphics/background.png");

    // Create a sprite
    Sprite spriteBackground;

    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);

    // Set the spriteBackground to cover the screen
    spriteBackground.setPosition(0, 0);

    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("../graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("../graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    // Is the bee currently moving?
    bool beeActive = false;

    // How fast can the bee fly
    float beeSpeed = 0.0f;

    // make 3 cloud sprites from 1 texture
    Texture textureCloud;

    // Load 1 new texture
    textureCloud.loadFromFile("../graphics/cloud.png");

    // 3 New sprites with the same texture
    Sprite cloudSprites[3];
    bool cloudsActive[3];
    float cloudsSpeed[3];
    int cloudsAmount = sizeof(cloudSprites) / sizeof(Sprite);

    for (int i = 0; i < cloudsAmount; i++) {
        cloudSprites[i].setTexture(textureCloud);
        // Position the clouds on the left of the screen
        // at different heights
        cloudSprites[i].setPosition(0, (i * 250.0f));
        // Are the clouds currently on the screen?
        cloudsActive[i] = false;
        // How fast is each cloud?
        cloudsSpeed[i] = 0.0f;
    }

    // Variables to control time itself
    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // Track whether the game is running
    bool paused = true;

    // Draw some text
    int score = 0;

    Text messageText;
    Text scoreText;

    // We need to choose a font
    Font font;
    font.loadFromFile("../fonts/KOMIKAP_.ttf");

    // Set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);

    // Assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    // Make it huge
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    // Position the text
    FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin(
            textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f
    );

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20, 20);

    while (window.isOpen()) {
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        // Start the game
        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;

            // Reset the time and the score
            score = 0;
            timeRemaining = 6;
        }

        /*
        ****************************************
        Update the scene
        ****************************************
        */

        if (!paused) {
            // Measure time
            Time dt = clock.restart();

            // Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // Size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                // Pause the game
                paused = true;

                // Change the message shown to the player
                messageText.setString("Out of time!!");

                // Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(
                        textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f
                );
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            // Setup the bee
            if (!beeActive) {
                // How fast is the bee
                srand((int) time(0));
                beeSpeed = (rand() % 200) + 200;

                // How high is the bee
                srand((int) time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            } else {
                //Move the bee
                spriteBee.setPosition(
                        spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                        spriteBee.getPosition().y
                );

                // Has the bee reached the left-hand edge of the screen?
                if (spriteBee.getPosition().x < -100) {
                    // Set it up ready to be a whole new bee next frame
                    beeActive = false;
                }
            }

            // Manage clouds
            for (int i = 0; i < cloudsAmount; ++i) {
                if (!cloudsActive[i]) {
                    // How fast is the cloud
                    srand((int) time(0) * (i + 10));
                    cloudsSpeed[i] = (rand() % 200);

                    // How high is the cloud
                    srand((int) time(0) * (i + 10));
                    float height = (rand() % 150);
                    cloudSprites[i].setPosition(-200, height);
                    cloudsActive[i] = true;
                } else {
                    cloudSprites[i].setPosition(
                            cloudSprites[i].getPosition().x + (cloudsSpeed[i] * dt.asSeconds()),
                            cloudSprites[i].getPosition().y
                    );

                    // Has the cloud reached the right-hand edge of the screen?
                    if (cloudSprites[i].getPosition().x > 1920) {
                        cloudsActive[i] = false;
                    }
                }
            }

            // Update the score text
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());
        } // End if (!paused)

        /*
        ****************************************
        Draw the scene
        ****************************************
        */

        // Clear everything from the last frame
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);

        // Draw the clouds
        for (int i = 0; i < cloudsAmount; ++i) {
            window.draw(cloudSprites[i]);
        }

        // Draw the tree
        window.draw(spriteTree);

        // Draw the insect
        window.draw(spriteBee);

        // Draw the score
        window.draw(scoreText);

        // Draw the time bar
        window.draw(timeBar);

        if (paused) {
            // Draw our message
            window.draw(messageText);
        }

        // Show everything we just drew
        window.display();
    }

    return 0;
}
