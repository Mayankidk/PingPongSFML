#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>

inline float radian(float& angle)
{
    return angle * 3.14159 / 180;
}
float findAngle(sf::Vector2f& ballvelo)
{
    float angle = atan(ballvelo.x / ballvelo.y);
    angle = angle * 180 / 3.14159;
    //std::cout << angle;
    return angle;
}
float findAngle(sf::Vector2f& ballpos, sf::Vector2f sidepos)
{
    sf::Vector2f displace;
    displace = { ballpos.x - sidepos.x,ballpos.y - sidepos.y };
    float angle = atan(displace.x / displace.y);
    angle = angle * 180 / 3.14159;
    //std::cout << angle;
    return angle;
}
sf::Vector2f velocity(float angle, float speed)
{
    sf::Vector2f velocity;
    velocity.x = sin(radian(angle)) * speed;
    velocity.y = cos(radian(angle)) * speed;
    return velocity;
}
sf::Vector2f findAngle(float& angle, float& ballspeed)
{
    angle = 180 - angle;
    angle = angle + 30 * (90 - angle) / 90;
    return velocity(angle, ballspeed);
}
void slabLimit(float& posy, float height, float& winy)
{
    if (posy - 5 <= 0)
        posy = 5;
    else if (posy + height + 5 >= winy)
        posy = winy - (height + 5);
}
float distance(sf::Vector2f& ballpos, sf::Vector2f sidepos)
{
    float x, y;
    x = pow(ballpos.x - sidepos.x, 2);
    y = pow(ballpos.y - sidepos.y, 2);
    return sqrt(x + y);
}

class primal
{
public:
    sf::CircleShape top;
    sf::CircleShape bottom;
    sf::RectangleShape middle;
    sf::Vector2f size;
    sf::Vector2f actsize;

    primal(float x, float y)
    {
        size = { x,y };
        actsize = { x,y + 2 * x };
        top.setOrigin(x, x);
        bottom.setOrigin(x, x);
        top.setRadius(x);
        middle.setSize(sf::Vector2f(x, y));
        bottom.setRadius(x);
    }
    void setFillColour(int r, int g, int b)
    {
        top.setFillColor(sf::Color(r, g - 20, b - 20));
        middle.setFillColor(sf::Color(r, g, b));
        bottom.setFillColor(sf::Color(r, g - 20, b - 20));
    }
    void setPosition(sf::Vector2f& coord)
    {
        top.setPosition(coord.x + size.x, coord.y + size.x);
        middle.setPosition(coord.x, coord.y + size.x);
        bottom.setPosition(coord.x + size.x, coord.y + size.y + size.x);
    }
    inline sf::Vector2f getSize()
    {
        return actsize;
    }
};
int main()
{
    bool spawnball = 0, playermoving = 0, hitable = 1;
    float playerx = 5, playery = 5, speed = 40, ballspeed = 25, botspeed = 35;
    int temprand;
    sf::Vector2f ballvelo, ballpos = { 800,400 }, windowsize = { 1600,900 };
    sf::Vector2f botpos = { 1575,5 }, botvelo = { 0,0 };
    float reactime = 0, adrenaline = 0, angle, ballangle;

    sf::RenderWindow window(sf::VideoMode(windowsize.x, windowsize.y), "PONG PRIME");
    window.setFramerateLimit(30);

    //player initial property
    sf::RectangleShape player(sf::Vector2f(20, 150));
    player.setFillColor(sf::Color::White);
    player.setPosition(playerx, playery);

    //backgrond
    sf::RectangleShape back(windowsize);
    back.setFillColor(sf::Color(15, 15, 40));
    back.setPosition(0, 0);

    //ball initial property
    sf::CircleShape ball(30);
    ball.setFillColor(sf::Color::Red);
    ball.setPosition(ballpos);
    ball.setOrigin(30, 30);

    //bot initial property
    primal bot(25, 110);
    bot.setFillColour(0, 255, 255);
    bot.setPosition(botpos);

    srand(time(0));
    rand();
    rand();
    rand();
    rand();

    ballvelo = velocity(rand() % 360, ballspeed);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            playery -= speed;
            playermoving = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            playery += speed;
            playermoving = 1;
        }

        //border logic for player
        slabLimit(playery, player.getSize().y, windowsize.y);

        //ball logic when at player
        //reflect if ball alive
        if (!spawnball)
        {
            if (ball.getGlobalBounds().intersects(player.getGlobalBounds()))
            {
                hitable = 0;
                //std::cout << "Intersection area: " << intersection.left << ", " << intersection.top << ", " << intersection.width << ", " << intersection.height << std::endl;
                //player top reflection logic
                if (ball.getPosition().y < player.getPosition().y + 25)
                {
                    //intersect = intersection.top + intersection.height / 2;
                    angle = ballpos.y - player.getPosition().y;
                    if (angle < 0)
                        angle = 0;
                    angle = angle * 18 / 5;
                    ballvelo = findAngle(angle, ballspeed);
                    //angle = 180 - angle;
                    //angle = angle + 30 * (90 - angle) / 90;
                    //ballvelo = velocity(angle, ballspeed);
                }
                //player bottom reflection logic
                else if (ball.getPosition().y > player.getPosition().y + 125)
                {
                    //intersect = intersection.top + intersection.height / 2;
                    angle = ballpos.y - player.getPosition().y;
                    if (angle > 150)
                        angle = 150;
                    angle = angle - 125;
                    angle = angle * 18 / 5;
                    angle = angle + 90;
                    ballvelo = findAngle(angle, ballspeed);
                    //angle = 180 - angle;
                    //angle = angle + 30 * (90 - angle) / 90;
                    //ballvelo = velocity(angle, ballspeed);
                }
                //player mid reflection logic
                else
                {
                    if (ballvelo.x > -12.5)
                    {
                        if (ballvelo.y > 0)
                            ballvelo = velocity(30, ballspeed);
                        else
                            ballvelo = velocity(150, ballspeed);
                    }
                    else
                    {
                        ballvelo.x = std::abs(ballvelo.x);
                        ballangle = findAngle(ballvelo);
                        if (ballangle < 0)
                            ballangle += 180;
                        ballvelo = velocity(ballangle + rand() % 5, ballspeed);
                    }
                }
            }
        }
        //ball respawn
        else
        {
            if (!playermoving)
            {
                if (ballpos.y <= playery)
                    ballvelo.y = -ballvelo.y;
                else if (ballpos.y >= playery + 150)
                    ballvelo.y = -ballvelo.y;
            }
            else
            {
                ballpos.y = playery + 75;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                angle = ballpos.y - playery;
                angle = angle * 6 / 5;
                ballvelo = findAngle(angle, ballspeed);
                spawnball = 0;
            }
        }

        //world border logic for ball
        if (ball.getPosition().x - 15 <= 0)
        {
            //respawn ball at player
            spawnball = 1;
            ballpos.x = 25 + 30;
            ballpos.y = playery + 75;
            ballvelo = velocity(0, 5);
        }
        else if (ball.getPosition().x + 15 >= windowsize.x)
        {
            spawnball = 1;
            ballpos.x = 25 + 30;
            ballpos.y = playery + 75;
            ballvelo = velocity(0, 5);
        }
        if (ball.getPosition().y - 30 <= 0)
            ballvelo.y = std::abs(ballvelo.y);
        else if (ball.getPosition().y + 30 >= windowsize.y)
            ballvelo.y = -std::abs(ballvelo.y);

        //bot logic
        reactime++;
        if (reactime >= 4)
        {
            temprand = rand();
            reactime = 0 + adrenaline;
            if (ballpos.x > 550 + (temprand % 300) && ballvelo.x > 0)
            {
                adrenaline += 0.20;
                if (ballvelo.y > 0)
                {
                    if (ballpos.y < botpos.y - 10)
                        botvelo.y = -botspeed;
                    else if (ballpos.y > botpos.y + 145)
                        botvelo.y = botspeed;
                    else if (botvelo.y < 0)
                        botvelo = {};
                    else if (temprand % 90 > std::abs(findAngle(ballvelo)))
                        botvelo.y = botspeed;
                }
                else
                {
                    if (ballpos.y < botpos.y + 15)
                        botvelo.y = -botspeed;
                    else if (ballpos.y > botpos.y + 170)
                        botvelo.y = botspeed;
                    else if (botvelo.y > 0)
                        botvelo = {};
                    else if (temprand % 90 > std::abs(findAngle(ballvelo)))
                        botvelo.y = -botspeed;
                    //else if (std::abs(findAngle(ballvelo)) < 75)
                        //botvelo.y = -botspeed;
                }
                /*
                else if (temprand % 100 < std::abs(findAngle(ballvelo)))
                    botvelo = {};
                */
            }
            else
            {
                adrenaline = 0;
                temprand %= 9;
                if (temprand == 0 && botpos.y < 10)
                    botvelo.y = botspeed;
                else if (temprand == 1 && botpos.y > 620)
                    botvelo.y = -botspeed;
                else if (temprand == 2)
                    botvelo.y = botspeed;
                else if (temprand == 3)
                    botvelo.y = -botspeed;
                else if (temprand > 6)
                    botvelo = {};
            }
        }

        /*reactime++;
        if (reactime >= 4)
        {
            temprand = rand();
            reactime = 0 + adrenaline;
            if (ballpos.x > 500 + (temprand % 300) && ballvelo.x > 0)
            {
                adrenaline += 0.20;
                if (ballpos.y < botpos.y + 20)
                    botvelo.y = -botspeed;
                else if (ballpos.y > botpos.y + 120)
                    botvelo.y = botspeed;
                else
                {
                    if (temprand % 5 < 2)
                        botvelo = {};
                }
            }
            else
            {
                adrenaline = 0;
                temprand %= 10;
                if (temprand == 0 && botpos.y < 10)
                    botvelo.y = std::abs(botvelo.y);
                else if (temprand == 1 && botpos.y > 620)
                    botvelo.y = -std::abs(botvelo.y);
                else if (temprand == 2)
                    botvelo.y = botspeed;
                else if (temprand == 7)
                    botvelo.y = -botspeed;
                else if (temprand > 7)
                    botvelo = {};
            }
        }*/
        //bot logic

        /*
        if (ballpos.x > 800 && ballvelo.x > 0)
        {
            if (ballpos.y < botpos.y + 20)
                botpos.y -= speed;
            else if (ballpos.y > botpos.y + 140)
                botpos.y += speed;
        }
        */
        if (ballpos.x > 100 && ballpos.x < 1500)
            hitable = 1;

        if (ballpos.x > 1540 && hitable)
        {
            //if (ball.getLocalBounds().intersects(bot.middle.getLocalBounds()))
            if (bot.middle.getGlobalBounds().contains(ballpos.x + ball.getRadius(), ballpos.y))
            {
                hitable = 0;
                //ballangle = findAngle(ballvelo);
                //std::cout << "hit mid\n";
                ballvelo.x = -std::abs(ballvelo.x);
            }
            else if (distance(ballpos, bot.top.getPosition()) < ball.getRadius() + bot.size.x)
            {
                hitable = 0;
                ballangle = findAngle(ballvelo);
                if (ballangle < 0)
                    ballangle += 180;
                angle = findAngle(ballpos, bot.top.getPosition());
                ballangle = 2 * angle - ballangle;
                ballangle = ballangle + 30 * (90 - ballangle) / 90;
                ballvelo = velocity(180 + ballangle, ballspeed);
                //std::cout << "top hit\n";
            }
            else if (distance(ballpos, bot.bottom.getPosition()) < ball.getRadius() + bot.size.x)
            {
                hitable = 0;
                ballangle = findAngle(ballvelo);
                if (ballangle > 0)
                    ballangle -= 180;
                angle = findAngle(ballpos, bot.bottom.getPosition());
                ballangle = 2 * angle - ballangle;
                ballangle = ballangle - 30 * (90 + ballangle) / 90;
                ballvelo = velocity(360 + ballangle, ballspeed);
                //std::cout << "bottom hit\n";
            }
        }

        botpos.y += botvelo.y;
        ballpos += ballvelo;
        slabLimit(botpos.y, bot.getSize().y, windowsize.y);
        ball.setPosition(ballpos);
        player.setPosition(playerx, playery);
        bot.setPosition(botpos);
        playermoving = 0;

        window.clear();

        window.draw(back);
        window.draw(player);
        window.draw(ball);
        window.draw(bot.top);
        window.draw(bot.bottom);
        window.draw(bot.middle);

        window.display();
    }
    return 0;
}