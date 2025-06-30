#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

const int width = 800;
const int height = 600;
const int block_size = 20;

struct snake
{
    vector<Vector2f> body;
    Vector2f direction;
};

struct food
{
    Vector2f position;

};

class snake_game
{
    public:
    void generatefood(food & food , const snake & snake)
    {
        bool validPosition;
        do
        {
            validPosition = true;
            food.position.x = rand() % (width / block_size) * block_size;
            food.position.y = rand() % (height / block_size) * block_size;

            for(const auto & segment : snake.body)
            {
                if(food.position ==segment)
                {
                    validPosition = false;
                    break;
                }
            }
        } while(!validPosition);
    }
     
    void updateSnake(snake & snake, food & food, int & score, Sound & eatSound, RenderWindow & window)
    {
        Vector2f newHead = snake.body[0] + snake.direction;
        snake.body.insert(snake.body.begin() , newHead);
        
        if(snake.body[0] == food.position)
        {
            generatefood(food, snake);
            score++;
            eatSound.play();
        }
        else
        {
            snake.body.pop_back();
        }

        if(snake.body[0].x < 0 || snake.body[0].x >= width || 
        snake.body[0].y < 0 || snake.body[0].y >= height)
        {
            cout<<"Game over! Score: "<<score<<endl;
            window.close();
        }
        
        for ( size_t i=1; i<snake.body.size(); ++i)
        {
            if(snake.body[0] == snake.body[i])
            {
                cout<<"Game over! Score: "<<score<<endl;
                window.close();
            }
        }
    }
    void drawGame(RenderWindow & window , const snake & snake , const food & food)
    {
        for(const auto & segment : snake.body)
        {
            RectangleShape rect(Vector2f(block_size, block_size));
            rect.setPosition(segment);
            rect.setFillColor(Color::Green);
            window.draw(rect);
        }
        RectangleShape foodRect(Vector2f(block_size , block_size));
        foodRect.setPosition(food.position);
        foodRect.setFillColor(Color::Red);
        window.draw(foodRect);

    }
    void handleinput(Event &event, snake &snake)
    {
        if( event.type == Event::KeyPressed)
        {
            if(event.key.code == Keyboard::Up && snake.direction.y == 0)
                snake.direction = Vector2f(0, -block_size); //Move up
            if(event.key.code == Keyboard::Down && snake.direction.y == 0)
                snake.direction = Vector2f(0, block_size);  //Move down
            if (event.key.code == Keyboard::Left && snake.direction.x == 0)
                snake.direction = Vector2f(-block_size, 0); // Move left
            if (event.key.code == Keyboard::Right && snake.direction.x == 0)
                snake.direction = Vector2f(block_size, 0); // Move right
        }
    }

};

int main()
{
    RenderWindow window (VideoMode(width , height) , "Snake Game");
    window.setFramerateLimit(10);

    snake s;
    s.body.push_back(Vector2f(width/2 , height/2));
    s.direction = Vector2f(block_size , 0);

    food f;

    snake_game s_g;

    s_g.generatefood(f,s);
    
    int score=0;

    SoundBuffer eatBuffer;

    if(!eatBuffer.loadFromFile("eat.wav"))
    {
        cerr<<"Failed to load eat sound effect!"<<endl;
        return -1;
    }
    Sound eatSound;
    eatSound.setBuffer(eatBuffer);

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
            {
                window.close();
            }
            s_g.handleinput(event , s);
        }
        s_g.updateSnake(s , f , score , eatSound, window);
        window.clear();
        s_g.drawGame(window , s , f);
        window.display();
    }
    return 0;
}

