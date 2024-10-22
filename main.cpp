#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "World.h"
#include "Inventory.h"


class Game
{
public:
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update(sf::Time);
    void render();
    void handleGameStates(sf::Keyboard::Key);    

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

    enum GameState {
        PLAYING,
        PAUSED        
    };

    //Game
    GameState currentState;  
    Player player;
    World world;
    Inventory inventory;

    //resources
    sf::Font pixelFont;
    sf::Texture tileMap;
    sf::Texture inventoryTileMap;

    //UI

    //possible items
    Item* i_sword;
    Item* i_hoe;
    Item* i_pickaxe;
    Item* i_axe;
    Item* i_potatoSeeds;
    Item* i_tomatoSeeds;
    Item* i_carrotSeeds;
    Item* i_wheatSeeds;
    Item* i_handGun;
    Item* i_machineGun;

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
    currentState = PLAYING;

    //load resources 
    if (!pixelFont.loadFromFile("resources/fonts/pixel_font.ttf")) 
        std::cout << "Could not load pixel_font.ttf\n";    
    else std::cout << "Loaded pixel_font.ttf\n";

    if (!tileMap.loadFromFile("resources/tilemaps/spritesheet.png")) 
        std::cout << "Could not load tilemap (spritesheet.png)\n";    
    else std::cout << "Loaded tilemap (spritesheet.png)\n"; 
    
    if (!inventoryTileMap.loadFromFile("resources/inventory/spritesheet.png"))
        std::cout << "Could not load inventory tilemap (spritesheet.png)\n";
    else std::cout << "Loaded inventory tilemap (spritesheet.png)\n";   
 
    //UI

    view = sf::View(sf::FloatRect(0, 0, 192 * 3, 108 * 3));
    mWindow.setView(view);    

    world = World("resources/maps/map.json", tileMap);
    inventory = Inventory(inventoryTileMap, mWindow, pixelFont);

    //initialise Items
    sf::Sprite swordSprite(inventoryTileMap);
    swordSprite.setTextureRect(sf::IntRect(49, 134, 16, 16));
    i_sword = new Item(1, "Sword", "A basic melee weapon.", swordSprite);
    i_sword->setStackable(false);

    sf::Sprite hoeSprite(inventoryTileMap);
    hoeSprite.setTextureRect(sf::IntRect(1, 134, 16, 16));
    i_hoe = new Item(2, "Hoe", "A tool used for tilling soil.", hoeSprite);
    i_hoe->setStackable(false);

    sf::Sprite pickaxeSprite(inventoryTileMap);
    pickaxeSprite.setTextureRect(sf::IntRect(17, 134, 16, 16));
    i_pickaxe = new Item(3, "Pickaxe", "A tool used for breaking rocks.", pickaxeSprite);
    i_pickaxe->setStackable(false);

    sf::Sprite axeSprite(inventoryTileMap);
    axeSprite.setTextureRect(sf::IntRect(33, 134, 16, 16));
    i_axe = new Item(4, "Axe", "A tool used for cutting wood.", axeSprite);
    i_axe->setStackable(false);

    sf::Sprite potatoSeedSprite(inventoryTileMap);
    potatoSeedSprite.setTextureRect(sf::IntRect(65, 134, 16, 16));
    i_potatoSeeds = new Item(5, "Potato Seeds", "Plant these to grow potatoes.", potatoSeedSprite);

    sf::Sprite tomatoSeedSprite(inventoryTileMap);
    tomatoSeedSprite.setTextureRect(sf::IntRect(81, 134, 16, 16));
    i_tomatoSeeds = new Item(6, "Tomato Seeds", "Plant these to grow tomatoes.", tomatoSeedSprite);

    sf::Sprite carrotSeedSprite(inventoryTileMap);
    carrotSeedSprite.setTextureRect(sf::IntRect(97, 134, 16, 16));
    i_carrotSeeds = new Item(7, "Carrot Seeds", "Plant these to grow carrots.", carrotSeedSprite);

    sf::Sprite wheatSeedSprite(inventoryTileMap);
    wheatSeedSprite.setTextureRect(sf::IntRect(113, 134, 16, 16));
    i_wheatSeeds = new Item(8, "Wheat Seeds", "Plant these to grow wheat.", wheatSeedSprite);

    sf::Sprite handGunSprite(inventoryTileMap);
    handGunSprite.setTextureRect(sf::IntRect(129, 134, 16, 16));
    i_handGun = new Item(9, "Handgun", "Just for fun xx", handGunSprite);
    i_handGun->setStackable(false);

    sf::Sprite machineGunSprite(inventoryTileMap);
    machineGunSprite.setTextureRect(sf::IntRect(145, 134, 16, 16));
    i_machineGun = new Item(10, "Machine Gun", "For even more fun xx", machineGunSprite);
    i_machineGun->setStackable(false);

    //testing
    inventory.addItem(i_axe);
    inventory.addItem(i_hoe);
    inventory.addItem(i_pickaxe);
    inventory.addItem(i_sword);   
    for (int i = 0; i < 110; i++) {
        inventory.addItem(i_potatoSeeds);
    }
    inventory.addItem(i_carrotSeeds);
    inventory.addItem(i_tomatoSeeds);
    inventory.addItem(i_wheatSeeds);
    inventory.addItem(i_handGun);
    inventory.addItem(i_machineGun);

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
    sf::Mouse mouse;
    sf::Vector2i mousePosWindow = mouse.getPosition(mWindow);
    
    inventory.update(mousePosWindow);

    while (mWindow.pollEvent(event))
    {
        switch (event.type) {
        case sf::Event::KeyPressed:
            player.processInput(event.key.code, true);
            inventory.handleKeyPress(event);
            break;
        case sf::Event::KeyReleased:
            player.processInput(event.key.code, false);
            handleGameStates(event.key.code);
            break;
        case sf::Event::MouseButtonPressed:
            inventory.handleClick(event);
            break;
        case sf::Event::MouseButtonReleased:
            inventory.handleClickRelease(event);
            break;
        case sf::Event::MouseWheelScrolled:
            inventory.handleScroll(event);
            break;
        case sf::Event::Closed:
            mWindow.close();
            break;
        }        
    }    
}

void Game::update(sf::Time deltaTime)
{
    if (currentState == PLAYING) {
        player.update(deltaTime);
    }
    else if (currentState = PAUSED) {
        // check if inventory was closed from inside
        if (inventory.getOpen() == false)
            currentState = PLAYING;
    }
}

void Game::render()
{
    mWindow.clear(sf::Color::White);    
    
    mWindow.setView(view);
    world.render(mWindow);
    player.render(mWindow);
    
    mWindow.setView(mWindow.getDefaultView());
    inventory.Render(mWindow);
    mWindow.display();
}

void Game::handleGameStates(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::I) {
        //open inventory        
        if (currentState == PAUSED && inventory.getOpen()) {
            currentState = PLAYING;
            inventory.Close();
        }
        else if (currentState == PLAYING && !inventory.getOpen()) {
            currentState = PAUSED;
            inventory.Open();
        }
    }
    //unpause with ESC
    if (key == sf::Keyboard::Escape && currentState == PAUSED) {
        currentState = PLAYING;
        if (inventory.getOpen()) inventory.Close();
    }

}

//deconstructor
Game::~Game() {
    delete(i_sword);
    delete(i_hoe);
    delete(i_pickaxe);
    delete(i_axe);
    delete(i_potatoSeeds);
    delete(i_carrotSeeds);
    delete(i_tomatoSeeds);
    delete(i_wheatSeeds);
    delete(i_handGun);
    delete(i_machineGun);
}