#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

//constants for window size and snake block size
const int width = 800;
const int height = 600;
const int block_size = 20;

// Struct to represent snake 
struct snake
{
    vector<Vector2f> body; //stores positions of all snake segments
    Vector2f direction; //current movement direction of the snake
};

// Struct to represent food
struct food
{
    Vector2f position;  //position of food on the grid

};

class snake_game
{
    public:
    //generates food at random position not ovelapping with the snake
    void generatefood(food & food , const snake & snake)
    {
        bool validPosition;
        //random position alligned to the grid
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
    
     // Updates the snake's position, checks collisions and food eating
    void updateSnake(snake & snake, food & food, int & score, Sound & eatSound, RenderWindow & window)
    {
        //calculate new head position based on current direction
        Vector2f newHead = snake.body[0] + snake.direction;
        snake.body.insert(snake.body.begin() , newHead);
        
        if(snake.body[0] == food.position)
        {
            generatefood(food, snake); //generate new food
            score++; // increment score
            eatSound.play(); //play eating sound
        }
        else
        {
            snake.body.pop_back();
        }

        if(snake.body[0].x < 0 || snake.body[0].x >= width || 
        snake.body[0].y < 0 || snake.body[0].y >= height)
        {
            cout<<"Game over! Score: "<<score<<endl;
            window.close(); //closes the game window 
        }
        
        for ( size_t i=1; i<snake.body.size(); ++i)
        {
            if(snake.body[0] == snake.body[i])
            {
                cout<<"Game over! Score: "<<score<<endl;
                window.close(); //closes game window 
            }
        }
    }

    // Draws the snake and food on the game window
    void drawGame(RenderWindow & window , const snake & snake , const food & food)
    {
        for(const auto & segment : snake.body)
        {
            RectangleShape rect(Vector2f(block_size, block_size));
            rect.setPosition(segment);
            rect.setFillColor(Color::Green);
            window.draw(rect);
        }
        // Draw the food as a red block
        RectangleShape foodRect(Vector2f(block_size , block_size));
        foodRect.setPosition(food.position);
        foodRect.setFillColor(Color::Red);
        window.draw(foodRect);

    }

     // Handles keyboard input to change snake direction
    void handleinput(Event &event, snake &snake)
    {
        if( event.type == Event::KeyPressed)
        {
             // Prevent the snake from reversing direction
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
     // Create the game window with specified width, height and title
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

