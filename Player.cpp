#include "Player.h"

//*******NOTE************
//Make sure to move the following two funcs to a seperate physics class at some point.

//check if a ray cast will collide with given rectangle
bool rayVsRecttt(sf::Vector2f& p, sf::Vector2f& dir, sf::RectangleShape& target,
    sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& hitNear) {

    //t_near
    sf::Vector2f tN = sf::Vector2f(
        (target.getPosition().x - p.x) / dir.x,
        (target.getPosition().y - p.y) / dir.y);

    //t_far
    sf::Vector2f tF = sf::Vector2f(
        (target.getPosition().x + target.getSize().x - p.x) / dir.x,
        (target.getPosition().y + target.getSize().y - p.y) / dir.y);

    //account for nAn
    if (std::isnan(tN.x)) tN.x = 0;
    if (std::isnan(tN.y)) tN.y = 0;
    if (std::isnan(tF.x)) tF.x = 0;
    if (std::isnan(tF.y)) tF.y = 0;

    //sort if swapped
    if (tN.x > tF.x) std::swap(tN.x, tF.x);
    if (tN.y > tF.y) std::swap(tN.y, tF.y);

    if (tN.x > tF.y || tN.y > tF.x) return false;

    float tHitNear = std::max(tN.x, tN.y);
    float tHitFar = std::min(tF.x, tF.y);

    if (tHitFar <= 0 || std::isnan(tHitFar)) return false;

    hitNear = tHitNear;

    contactPoint = sf::Vector2f(p.x + hitNear * dir.x, p.y + hitNear * dir.y);

    //find normal of intersecting point
    if (tN.x > tN.y)
        if (dir.x < 0)
            contactNormal = { 1, 0 };
        else
            contactNormal = { -1, 0 };
    else if (tN.x < tN.y)
        if (dir.y < 0)
            contactNormal = { 0, 1 };
        else
            contactNormal = { 0, -1 };

    return true;
}

//check if dynamic rectangle will intersect with a static rect
bool DynamicRectVsRect(sf::RectangleShape& dRect, sf::RectangleShape& sRect, sf::Vector2f vel, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& fTime, sf::Time& deltaTime) {
    if (vel.x < 1 && vel.x > -1 && vel.y < 1 && vel.y > -1)
        return false;

    sf::RectangleShape expandedShape;
    expandedShape.setPosition(
        sRect.getPosition().x - dRect.getSize().x / 2,
        sRect.getPosition().y - dRect.getSize().y / 2);

    expandedShape.setSize(sf::Vector2f(
        sRect.getSize().x + dRect.getSize().x,
        sRect.getSize().y + dRect.getSize().y));

    sf::Vector2f centerOfD = sf::Vector2f(dRect.getPosition().x + dRect.getSize().x / 2, dRect.getPosition().y + dRect.getSize().y / 2);
    sf::Vector2f timedVelocity = sf::Vector2f(vel.x * deltaTime.asSeconds(), vel.y * deltaTime.asSeconds());

    if (rayVsRecttt(centerOfD, timedVelocity, expandedShape, contactPoint, contactNormal, fTime)) {
        if (fTime < 1.f)
            return true;
    }
    return false;
}

Player::Player(sf::RenderWindow &mWindow, World &w): world(w){
    
    //graphics
    shape.setSize(sf::Vector2f(16, 16));
    shape.setFillColor(sf::Color::Magenta);
    shape.setPosition(50, 50);
}

void Player::update(sf::Time deltaTime) {
    //simple static movement
    pVelocity = sf::Vector2f(0, 0);
    if (movingUp)
        pVelocity.y -= movementSpeed;
    if (movingDown)
        pVelocity.y += movementSpeed;
    if (movingLeft)
        pVelocity.x -= movementSpeed;
    if (movingRight)
        pVelocity.x += movementSpeed;

    //accounting for small velocities 
    if (pVelocity.x > -10 && pVelocity.x < 10) pVelocity.x = 0;
    if (pVelocity.y > -10 && pVelocity.y < 10) pVelocity.y = 0;              

    handleCollisions(deltaTime);

    shape.move(pVelocity * deltaTime.asSeconds());
}

void Player::handleCollisions(sf::Time& deltaTime) {       
    float t;
    sf::Vector2f normal, contactPoint;    
    sf::RectangleShape tile;
    //std::vector<int> w = world.getWorld();
    std::vector<layer> collisionArrays = world.getColliders();

    //limit collision checks to closest tiles
    sf::Vector2i truncatedPos = sf::Vector2i((int)shape.getPosition().x / world.getTileSize(), (int)shape.getPosition().y / world.getTileSize());
    
    std::vector<std::vector<std::pair<int, float>>> collisionTiles;
    collisionTiles.resize(collisionArrays.size());

    //loop through to find all tiles with possible collision
    for (int i = 0; i < (int)collisionArrays.size();i++) {
        layer collisionLayer = collisionArrays[i];

        for (int y = -2; y < 3; y++) {
            for (int x = -2; x < 3; x++) {
                int arrayPos = (y + truncatedPos.y) * world.getWidth() + x + truncatedPos.x;

                if (arrayPos < 0 || arrayPos >= (int)collisionLayer.collisionArray.size())continue;

                if (collisionLayer.collisionArray[arrayPos] == 1) {

                    tile.setSize(sf::Vector2f((float)world.getTileSize(), (float)world.getTileSize()));
                    tile.setPosition((x + truncatedPos.x) * (float)world.getTileSize(), (y + truncatedPos.y) * (float)world.getTileSize());

                    //collision detection / resolution
                    if (DynamicRectVsRect(shape, tile, pVelocity, contactPoint, normal, t, deltaTime)) {
                        collisionTiles[i].push_back({arrayPos, t});
                    }
                }
            }
        }
    }

    //sort tiles by distance
   /* std::sort(collisionTiles.begin(), collisionTiles.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
        return a.second < b.second;
    });*/

    //collision response through sorted array
    for (auto cTiles : collisionTiles) {
        for (auto tt : cTiles) {
            int x = (tt.first % world.getWidth());
            int y = (tt.first / world.getWidth());

            tile.setSize(sf::Vector2f((float)world.getTileSize(), (float)world.getTileSize()));
            tile.setPosition((x) * (float)world.getTileSize(), (y) * (float)world.getTileSize());

            if (DynamicRectVsRect(shape, tile, pVelocity, contactPoint, normal, t, deltaTime)) {
                pVelocity.x += std::abs(pVelocity.x) * (1.f - t) * normal.x;
                pVelocity.y += std::abs(pVelocity.y) * (1.f - t) * normal.y;
            }
        }
    }
}

void Player::render(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::processInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::W)
        movingUp = isPressed;
    else if (key == sf::Keyboard::A)
        movingLeft = isPressed;
    else if (key == sf::Keyboard::D)
        movingRight = isPressed;
    else if (key == sf::Keyboard::S)
        movingDown = isPressed;
}
