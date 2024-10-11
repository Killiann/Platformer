#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "World.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time);
    void render();

private:
    //Engine
    sf::RenderWindow mWindow;    
    sf::Keyboard keyboard;
    sf::View view;

    //time keeping
    sf::Clock clock;
    sf::Time timeSinceLastUpdate{ sf::Time::Zero };
    sf::Time TimePerFrame;
    const float updateFrameRate{ 60 };

    Player player;
    World world;

    //resources
    sf::Font pixelFont;
    sf::Texture tileMap;

    //UI

    //FPS
    float lastTime{ 0 };
    sf::Text frameCounter;
};

int main()
{
    Game game;
    game.run();

    return 0;
}

Game::Game() :
    mWindow(sf::VideoMode(1920, 1080), "Platformer"), 
    player(mWindow, world)
{

    //initialisation    
    TimePerFrame = sf::seconds(1.f / updateFrameRate);    

    //resources
    if (!pixelFont.loadFromFile("resources/fonts/pixel_font.ttf")) {
        std::cout << "Could not load pixel_font.ttf\n";
    }
    else
        std::cout << "Loaded pixel_font.ttf\n";

    if (!tileMap.loadFromFile("resources/tilemaps/spritesheet.png")) {
        std::cout << "Could not load tilemap (spritesheet.png)\n";
    }
    else std::cout << "Loaded tilemap (spritesheet.png)\n"; 

    world = World("resources/maps/map.json", tileMap);

    //UI

    view = sf::View(sf::FloatRect(0, 0, 192 * 3, 108 * 3));
    mWindow.setView(view);

    frameCounter.setFont(pixelFont);
    frameCounter.setString("000");
    frameCounter.setPosition(20, 20);
    frameCounter.setFillColor(sf::Color::Black);
    frameCounter.setCharacterSize(40);
}

void Game::run()
{   
    while (mWindow.isOpen())
    {
        processEvents();

        //clock + fps handling
        sf::Time restartedFrame = clock.restart();        

        //broken FPS handling
        /*float currentTime = restartedFrame.asSeconds();
        int fps = 1 / (currentTime - lastTime);
        frameCounter.setString(std::to_string(fps));
        lastTime = currentTime;*/

        timeSinceLastUpdate += restartedFrame;
        
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;            
            processEvents();

            update(TimePerFrame);
        }
    
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        switch (event.type) {
        case sf::Event::KeyPressed:
            player.processInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            player.processInput(event.key.code, false);
            break;
        case sf::Event::Closed:
            mWindow.close();
            break;
        }        
    }
}

void Game::update(sf::Time deltaTime)
{
    //pVelocity = sf::Vector2f(0.f, 0.f);
    player.update(deltaTime);
}

void Game::render()
{
    mWindow.clear(sf::Color::White);    
    world.render(mWindow);
    player.render(mWindow);
    //UI
    //mWindow.draw(frameCounter);
    mWindow.display();
}