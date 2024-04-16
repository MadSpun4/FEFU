#include <SFML/Graphics.hpp>
#include <cmath>
//#include <ctime>
//#include <cstdlib>
//#include <iostream>
#include <Windows.h>

class Tile : public sf::RectangleShape, public sf::Transformable
{
public:
    void setTileSize(sf::Vector2f rect, int text)
    {
        rectShape.setSize(rect);
        numberText.setCharacterSize(text);
    }

    void setTileFont(const sf::Font& font)
    {
        numberText.setFont(font);
    }

    void setTileOrigin(sf::Vector2f origin)
    {
        rectShape.setOrigin(origin);
        numberText.setOrigin(origin);
    }

    void setTileFillColor(sf::Color rectCol, sf::Color textCol)
    {
        rectShape.setFillColor(rectCol);
        numberText.setFillColor(textCol);
    }
    sf::Color getTileRectFillColor() { return rectShape.getFillColor(); }
    sf::Color getTileNumberFillColor() { return numberText.getFillColor(); }

    void setTileOutlineThickness(int rect, int num)
    {
        rectShape.setOutlineThickness(rect);
        numberText.setOutlineThickness(num);
    }

    void setTileOutlineColor(sf::Color rect, sf::Color num)
    {
        rectShape.setOutlineColor(rect);
        numberText.setOutlineColor(num);
    }
    sf::Color getTileRectOutlineColor() { return rectShape.getOutlineColor(); }
    sf::Color getTileNumberOutlineColor() { return numberText.getOutlineColor(); }

    void setTilePosition(sf::Vector2f pos)
    {
        rectShape.setPosition(pos);
        if (numberInt < 10) numberText.setPosition(sf::Vector2f(pos.x + 23, pos.y));
        else numberText.setPosition(sf::Vector2f(pos.x + 3, pos.y));
        position = pos;
    }
    sf::Vector2f getTilePosition() { return position; }

    void setNum(int num)
    {
        numberText.setString(std::to_string(num));
        numberInt = num;
    }
    int getNum() { return numberInt; }

private:
    sf::RectangleShape rectShape;
    sf::Text numberText;
    int numberInt = 0;
    sf::Vector2f position;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(rectShape, states);
        target.draw(numberText, states);
    }
};

class Button : public sf::RectangleShape, public sf::Transformable
{
public: 
    Button() { }

    Button(sf::Vector2f size, sf::Color btnCol)
    {
        btnRect.setSize(size);
        btnRect.setFillColor(btnCol);
    }

    Button(sf::Vector2f size, sf::Color btnCol, std::string text, sf::Font& font, int charSize, sf::Color textCol)
    {
        btnRect.setSize(size);
        btnRect.setFillColor(btnCol);

        btnText.setString(text);
        btnText.setFont(font);
        btnText.setCharacterSize(charSize);
        btnText.setFillColor(textCol);
    }

    /*void setTextString(std::string string)
    {
        btnText.setString(string);

        float textPosX = btnRect.getPosition().x;
        float textPosY = btnRect.getPosition().y;

        float textPosXWidth = btnText.getGlobalBounds().width;
        //float textPosYHeight = btnRect.getLocalBounds().height;
        
        btnText.setPosition(textPosX + textPosXWidth / 11, textPosY);
        //btnText.setPosition((textPosXWidth - textPosX) / 2, (textPosYHeight - textPosY) / 2);
    }*/

    void setRectColor(sf::Color color)
    {
        btnRect.setFillColor(color);
    }
    void setTextColor(sf::Color color)
    {
        btnText.setFillColor(color);
    }

    void setRectOutlineThickness(float thickness)
    {
        btnRect.setOutlineThickness(thickness);
    }
    void setTextOutlineThickness(float thickness)
    {
        btnText.setOutlineThickness(thickness);
    }

    void setRectOutlineColor(sf::Color color)
    {
        btnRect.setOutlineColor(color);
    }
    void setTextOutlineColor(sf::Color color)
    {
        btnText.setOutlineColor(color);
    }

    void setPosition(sf::Vector2f pos)
    {
        btnRect.setPosition(pos);

        float textPosX = (pos.x + btnRect.getGlobalBounds().width / 2) - (btnText.getGlobalBounds().width / 2);
        float textPosY = (pos.y + btnRect.getGlobalBounds().height / 2) - (btnText.getGlobalBounds().height) + 4;

        btnText.setPosition(textPosX, textPosY);
    }

    bool isMouseOver(sf::RenderWindow& window)
    {
        float mouseX = sf::Mouse::getPosition(window).x;
        float mouseY = sf::Mouse::getPosition(window).y;

        float btnPosX = btnRect.getPosition().x;
        float btnPosY = btnRect.getPosition().y;

        float btnPosXWidth = btnRect.getPosition().x + btnRect.getLocalBounds().width;
        float btnPosYHeight = btnRect.getPosition().y + btnRect.getLocalBounds().height;

        if (mouseX > btnPosX && mouseX < btnPosXWidth && mouseY > btnPosY && mouseY < btnPosYHeight)
        {
            return true;
        }

        return false;
    }
private: 
    sf::RectangleShape btnRect;
    sf::Text btnText;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(btnRect, states);
        target.draw(btnText, states);
    }
};

void swapParameters(Tile& tile1, Tile& tile2)
{
    Tile helpTile;

    helpTile.setTileFillColor(tile1.getTileRectFillColor(), tile1.getTileNumberFillColor());
    helpTile.setTileOutlineColor(tile1.getTileRectOutlineColor(), tile1.getTileNumberOutlineColor());
    helpTile.setNum(tile1.getNum());

    tile1.setTileFillColor(tile2.getTileRectFillColor(), tile2.getTileNumberFillColor());
    tile1.setTileOutlineColor(tile2.getTileRectOutlineColor(), tile2.getTileNumberOutlineColor());
    tile1.setNum(tile2.getNum());

    tile2.setTileFillColor(helpTile.getTileRectFillColor(), helpTile.getTileNumberFillColor());
    tile2.setTileOutlineColor(helpTile.getTileRectOutlineColor(), helpTile.getTileNumberOutlineColor());
    tile2.setNum(helpTile.getNum());

    //fixes number positions
    tile1.setTilePosition(tile1.getTilePosition());
    tile2.setTilePosition(tile2.getTilePosition());
}


const float gameWidth = 800, gameHeight = 600;

int main()
{
    HWND consoleWindow;
    AllocConsole();
    consoleWindow = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(consoleWindow, 0);

    const float tagWidth = 80, tagHeight = 80;
    bool menuLayer = true;
    bool winLayer = false;
    bool playLayer = false;
    bool rulesLayer = false;
    int emptyX = 3; int emptyY = 3;
    Tile helpTile;
    int moveCounter = 0;
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "The 15 Puzzle Game");

    sf::Clock clock; sf::Clock delayClock;
    sf::Time updateTime = sf::seconds(0.01f); sf::Time delay = sf::seconds(4.f);
    int hintTextAlpha = 255;

    //Loading font
    sf::Font fnt;
    std::string fontDir = "resources/tuffy.ttf";
    if (!fnt.loadFromFile(fontDir));
        //std::cout << "Your font is not loaded bruh...";
    //else
        //std::cout << "font in '" << fontDir << "' is loaded succesfully!" << std::endl;


    //Play layer initialization-----------------
    sf::RectangleShape bgRect;
    bgRect.setSize(sf::Vector2f(400, 400));
    bgRect.setOrigin(200, 200);
    bgRect.setPosition(sf::Vector2f(gameWidth / 2, gameHeight / 2));
    bgRect.setFillColor(sf::Color(64, 64, 64));
    bgRect.setOutlineThickness(5);
    bgRect.setOutlineColor(sf::Color::Black);

    sf::Text moveCounterText;
    moveCounterText.setCharacterSize(50);
    moveCounterText.setFont(fnt);
    moveCounterText.setOutlineColor({ 0, 0, 0 });
    moveCounterText.setOutlineThickness(4);

    sf::Text hintText;
    hintText.setCharacterSize(45);
    hintText.setFont(fnt);
    hintText.setString("Press 'R' to open rules.");

    Tile tile[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tile[j][i].setTileFont(fnt);
            tile[j][i].setNum((j + i * 4) + 1);
            tile[j][i].setTileSize(sf::Vector2f(tagWidth, tagHeight), 60);
            tile[j][i].setTileOrigin(sf::Vector2f(tagWidth / 2, tagHeight / 2));
            tile[j][i].setTileFillColor(sf::Color(255, 255 - (i * 4 + j) * 15, 255 - (i * 4 + j) * 15), sf::Color(0, 0, 0));
            tile[j][i].setTileOutlineThickness(5, 0);
            tile[j][i].setTileOutlineColor(sf::Color::Black, sf::Color::Transparent);
            tile[j][i].setTilePosition(sf::Vector2f((gameWidth / 2) - 150 + j * 100, (gameHeight / 2) - 150 + i * 100));
        }
    }

    //Last transparent tag
    tile[3][3].setTileFillColor(sf::Color::Transparent, sf::Color::Transparent);
    tile[3][3].setTileOutlineColor(sf::Color::Transparent, sf::Color::Transparent);
    //------------------------------------------

    //Main menu objects initialization----------
    sf::Text menuMsg;
    menuMsg.setFont(fnt);
    menuMsg.setString("The 15 puzzle game");
    menuMsg.setOutlineColor(sf::Color::Black);
    menuMsg.setOutlineThickness(5);
    menuMsg.setCharacterSize(64);
    menuMsg.setOrigin(sf::Vector2f(220, 50));
    menuMsg.setPosition(sf::Vector2f(gameWidth / 2 - 50, gameHeight / 4));

    Button startBtn(sf::Vector2f(250, 50), sf::Color::White, "Start Game", fnt, 38, sf::Color::Black);
    startBtn.setPosition(sf::Vector2f(gameWidth / 2 - 125, gameHeight / 3 + 50));
    startBtn.setRectOutlineColor(sf::Color::Black);
    startBtn.setRectOutlineThickness(5);

    Button rulesBtn(sf::Vector2f(250, 50), sf::Color::White, "Rules", fnt, 38, sf::Color::Black);
    rulesBtn.setPosition(sf::Vector2f(gameWidth / 2 - 125, gameHeight / 3 + 125));
    rulesBtn.setRectOutlineColor(sf::Color::Black);
    rulesBtn.setRectOutlineThickness(5);

    Button exitBtn(sf::Vector2f(250, 50), sf::Color::White, "Exit", fnt, 38, sf::Color::Black);
    exitBtn.setPosition(sf::Vector2f(gameWidth / 2 - 125, gameHeight / 3 + 200));
    exitBtn.setRectOutlineColor(sf::Color::Black);
    exitBtn.setRectOutlineThickness(5);
    //------------------------------------------

    //Rules layer objects initialization--------
    sf::Text rulesMsg;
    rulesMsg.setFont(fnt);
    rulesMsg.setString("- The game consists of 15 tiles on a 4x4 board.\n- The tiles have numbers from 1 to 15\n- You can move tiles using the arrow keys on your\n keyboard.\n- To complete the game, you need to decompose all \nthe tiles in ascending numbers on them.");
    rulesMsg.setCharacterSize(32);
    rulesMsg.setFillColor(sf::Color::White);
    rulesMsg.setPosition(sf::Vector2f(25, gameHeight / 5));

    Button restartBtn(sf::Vector2f(250, 50), sf::Color::White, "Restart Game", fnt, 38, sf::Color::Black);
    restartBtn.setPosition(sf::Vector2f(gameWidth / 2 - 125, gameHeight / 3 + 50));
    restartBtn.setRectOutlineColor(sf::Color::Black);
    restartBtn.setRectOutlineThickness(5);
    //------------------------------------------

    //Win layer objects initialization----------
    sf::Text winMsg;
    winMsg.setFont(fnt);
    winMsg.setPosition(sf::Vector2f(180, 100));
    winMsg.setCharacterSize(100);
    winMsg.setFillColor(sf::Color::White);
    winMsg.setOutlineColor(sf::Color::Black);
    winMsg.setOutlineThickness(5);
    winMsg.setString("YOU WIN!!!");

    Button backBtn(sf::Vector2f(150, 50), sf::Color::White, "Back", fnt, 38, sf::Color::Black);
    backBtn.setPosition(sf::Vector2f(25, 25));
    backBtn.setRectOutlineColor(sf::Color::Black);
    backBtn.setRectOutlineThickness(5);
    //------------------------------------------

    //Main cycle
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved)
            {
                if (startBtn.isMouseOver(window)) startBtn.setRectColor({ 175, 175, 175 });
                else startBtn.setRectColor(sf::Color::White);

                if (rulesBtn.isMouseOver(window)) rulesBtn.setRectColor({ 175, 175, 175 });
                else rulesBtn.setRectColor(sf::Color::White);

                if (exitBtn.isMouseOver(window)) exitBtn.setRectColor({ 175, 175, 175 });
                else exitBtn.setRectColor(sf::Color::White);

                if (backBtn.isMouseOver(window)) backBtn.setRectColor({ 175, 175, 175 });
                else backBtn.setRectColor(sf::Color::White);

                if (restartBtn.isMouseOver(window)) restartBtn.setRectColor({ 175, 175, 175 });
                else restartBtn.setRectColor(sf::Color::White);
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if ((startBtn.isMouseOver(window) || restartBtn.isMouseOver(window)) && (menuLayer || winLayer))
                {
                    //Shuffle
                    srand(time(0));
                    int direction;
                    menuLayer = false; winLayer = false;
                    moveCounter = 0;
                    delayClock.restart();
                    hintTextAlpha = 255;
                    hintText.setFillColor(sf::Color(255, 255, 255, hintTextAlpha));
                    for (int i = 0; i < 10; i++)
                    {
                        direction = rand() % 4;
                        switch (direction)
                        {
                        case 0:
                        {
                            if (emptyX > 0)
                            {
                                swapParameters(tile[emptyX][emptyY], tile[emptyX - 1][emptyY]);
                                emptyX = emptyX - 1;
                                break;
                            }
                        }
                        case 1:
                        {
                            if (emptyX < 3)
                            {
                                swapParameters(tile[emptyX][emptyY], tile[emptyX + 1][emptyY]);
                                emptyX = emptyX + 1;
                                break;
                            }
                        }
                        case 2:
                        {
                            if (emptyY < 3)
                            {
                                swapParameters(tile[emptyX][emptyY], tile[emptyX][emptyY + 1]);
                                emptyY = emptyY + 1;
                                break;
                            }
                        }
                        case 3:
                        {
                            if (emptyY > 0)
                            {
                                swapParameters(tile[emptyX][emptyY], tile[emptyX][emptyY - 1]);
                                emptyY = emptyY - 1;
                                break;
                            }
                        }

                        default:
                            break;
                        }
                    }
                    while (emptyX < 3)
                    {
                        swapParameters(tile[emptyX][emptyY], tile[emptyX + 1][emptyY]);
                        emptyX = emptyX + 1;
                    }

                    while (emptyY < 3)
                    {
                        swapParameters(tile[emptyX][emptyY], tile[emptyX][emptyY + 1]);
                        emptyY = emptyY + 1;
                    }
                    playLayer = true;
                }
                if (rulesBtn.isMouseOver(window) && (menuLayer || winLayer)) rulesLayer = true;
                if (exitBtn.isMouseOver(window) && (menuLayer || winLayer)) window.close();

                if (backBtn.isMouseOver(window) && rulesLayer) rulesLayer = false;
            }

            if (playLayer && !winLayer)
            {
                //Controls (left, right, down, up, rules)
                if ((event.type == sf::Event::KeyPressed) && 
                    (event.key.code == sf::Keyboard::Left) && 
                    emptyX > 0 && (playLayer && !rulesLayer)) //Left
                {
                    swapParameters(tile[emptyX][emptyY], tile[emptyX - 1][emptyY]);
                    moveCounter++;
                    emptyX = emptyX - 1;
                }

                if ((event.type == sf::Event::KeyPressed) && 
                    (event.key.code == sf::Keyboard::Right) && 
                    emptyX < 3 && (playLayer && !rulesLayer)) //Right
                {
                    swapParameters(tile[emptyX][emptyY], tile[emptyX + 1][emptyY]);
                    moveCounter++;
                    emptyX = emptyX + 1;

                    winLayer = true;
                    playLayer = false;
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (tile[j][i].getNum() != (j + i * 4) + 1)
                            {
                                winLayer = false;
                                playLayer = true;
                            }
                        }
                    }
                }

                if ((event.type == sf::Event::KeyPressed) && 
                    (event.key.code == sf::Keyboard::Down) && 
                    emptyY < 3 && (playLayer && !rulesLayer)) //Down
                {
                    swapParameters(tile[emptyX][emptyY], tile[emptyX][emptyY + 1]);
                    moveCounter++;
                    emptyY = emptyY + 1;

                    winLayer = true;
                    playLayer = false;
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (tile[j][i].getNum() != (j + i * 4) + 1)
                            {
                                winLayer = false;
                                playLayer = true;
                            }
                        }
                    }
                }

                if ((event.type == sf::Event::KeyPressed) && 
                    (event.key.code == sf::Keyboard::Up) && 
                    emptyY > 0 && (playLayer && !rulesLayer)) //Up
                {
                    swapParameters(tile[emptyX][emptyY], tile[emptyX][emptyY - 1]);
                    moveCounter++;
                    emptyY = emptyY - 1;
                }

                if ((event.type == sf::Event::KeyPressed) && 
                    (event.key.code == sf::Keyboard::R) && 
                    (playLayer && !rulesLayer)) //Rules
                {
                    rulesLayer = true;
                }
            }
        }

        window.clear(sf::Color(128, 128, 128));
        if (rulesLayer) //Rules layer draw
        {
            window.draw(rulesMsg);
            window.draw(backBtn);
        }
        else if (menuLayer) //Main menu draw
        {
            window.draw(menuMsg);
            window.draw(startBtn);
            window.draw(rulesBtn);
            window.draw(exitBtn);
        }
        else if (winLayer) //Win layer draw
        {
            window.draw(winMsg);
            window.draw(restartBtn);
            window.draw(rulesBtn);
            window.draw(exitBtn);

            moveCounterText.setString("Total moves: " + std::to_string(moveCounter));
            sf::FloatRect textRect = moveCounterText.getGlobalBounds();
            moveCounterText.setOrigin(textRect.width / 2, textRect.height / 2);
            moveCounterText.setPosition(sf::Vector2f(gameWidth / 2, gameHeight - 65));
            window.draw(moveCounterText);
        }
        else if (playLayer) //Play layer draw
        {
            window.draw(bgRect);
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    window.draw(tile[j][i]);
                }
            }
            moveCounterText.setString("Moves: " + std::to_string(moveCounter));
            sf::FloatRect moveTextRect = moveCounterText.getGlobalBounds();
            moveCounterText.setOrigin(moveTextRect.width / 2, moveTextRect.height / 2);
            moveCounterText.setPosition(sf::Vector2f(gameWidth / 2, gameHeight - 65));
            window.draw(moveCounterText);

            sf::FloatRect hintTextRect = hintText.getGlobalBounds();
            hintText.setOrigin(hintTextRect.width / 2, hintTextRect.height / 2);
            hintText.setPosition(sf::Vector2f(gameWidth / 2, 50));
            if (clock.getElapsedTime() > updateTime && delayClock.getElapsedTime() > delay && hintTextAlpha != 0)
            {
                clock.restart();
                hintTextAlpha = hintTextAlpha - 5;
                hintText.setFillColor(sf::Color(255, 255, 255, hintTextAlpha));
            }
            window.draw(hintText);
        }

        window.display();
    }

    return 0;
}