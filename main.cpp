#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;

void resetGame(vector<sf::RectangleShape>& snake, sf::Vector2f& direction, int& score, sf::Text& scoreText, sf::Sprite& appleSprite, sf::RectangleShape& border) {
    snake.clear();
    sf::RectangleShape head(sf::Vector2f(20, 20));
    head.setFillColor(sf::Color::Black);
    head.setPosition(600, 450);
    snake.push_back(head);

    direction = {20, 0};
    score = 0;
    scoreText.setString("Score: 0");
    appleSprite.setPosition(
        100 + rand() % (int)(border.getSize().x - 20),
        120 + rand() % (int)(border.getSize().y - 20)
    );
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Snake Game");

    sf::RectangleShape border(sf::Vector2f(800 - 4, 800 - 4));
    border.setPosition(100, 120);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(4);
    border.setOutlineColor(sf::Color::Black);

    vector<sf::RectangleShape> snake;
    int score = 0;

    sf::Font font;
    if (!font.loadFromFile("Arima-VariableFont_wght.ttf")) {
        cout << "Failed to load font!" << endl;
        return -1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(200, 80);

    sf::Text title;
    title.setFont(font);
    title.setString("Snake Game");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Black);
    title.setStyle(sf::Text::Bold);
    title.setPosition(380, 50);

    sf::Text collisionText;
    collisionText.setFont(font);
    collisionText.setString("Game Over\nPress any key to restart");
    collisionText.setCharacterSize(40);
    collisionText.setStyle(sf::Text::Bold);
    collisionText.setFillColor(sf::Color::Black);
    collisionText.setPosition(300, 450);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("image.jpg")) {
        cout << "Failed to load background!" << endl;
        return -1;
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Vector2u texSize = backgroundTexture.getSize();
    sf::Vector2u winSize = window.getSize();
    backgroundSprite.setScale((float)winSize.x / texSize.x, (float)winSize.y / texSize.y);

    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile("apple.png")) {
        cout << "Failed to load apple!" << endl;
        return -1;
    }

    sf::Sprite appleSprite;
    appleSprite.setTexture(appleTexture);
    appleSprite.setScale(0.1f, 0.1f);

    sf::Vector2f direction;
    resetGame(snake, direction, score, scoreText, appleSprite, border);

    sf::Clock clock;
    float moveDelay = 0.1f, timer = 0;
    bool gameOver = false;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        timer += deltaTime;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (!gameOver) {
                    if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) && direction.y == 0)
                        direction = {0, -20};
                    if ((event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) && direction.y == 0)
                        direction = {0, 20};
                    if ((event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) && direction.x == 0)
                        direction = {-20, 0};
                    if ((event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) && direction.x == 0)
                        direction = {20, 0};
                } else {
                    gameOver = false;
                    resetGame(snake, direction, score, scoreText, appleSprite, border);
                }
            }
        }

        if (!gameOver && timer >= moveDelay) {
            timer = 0;
            sf::Vector2f nextPos = snake[0].getPosition() + direction;

            if (nextPos.x < border.getPosition().x || nextPos.x + 20 > border.getPosition().x + border.getSize().x ||
                nextPos.y < border.getPosition().y || nextPos.y + 20 > border.getPosition().y + border.getSize().y) {
                gameOver = true;
            }

            for (size_t i = 1; i < snake.size(); ++i) {
                if (snake[i].getPosition() == nextPos) {
                    gameOver = true;
                    break;
                }
            }

            if (!gameOver) {
                for (int i = snake.size() - 1; i > 0; --i)
                    snake[i].setPosition(snake[i - 1].getPosition());

                snake[0].setPosition(nextPos);

                if (snake[0].getGlobalBounds().intersects(appleSprite.getGlobalBounds())) {
                    score++;
                    scoreText.setString("Score: " + to_string(score));
                    appleSprite.setPosition(
                        100 + rand() % (int)(border.getSize().x - 20),
                        120 + rand() % (int)(border.getSize().y - 20)
                    );

                    sf::RectangleShape newPart(sf::Vector2f(20, 20));
                    newPart.setFillColor(sf::Color::Black);
                    newPart.setPosition(snake.back().getPosition());
                    snake.push_back(newPart);
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(backgroundSprite);
        window.draw(border);
        window.draw(appleSprite);
        for (const auto& s : snake)
            window.draw(s);
        window.draw(scoreText);
        window.draw(title);
        if (gameOver)
            window.draw(collisionText);
        window.display();
    }

    return 0;
}
