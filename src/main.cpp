#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <deque>

const Color green = {173, 204, 96, 255};
const Color darkGreen = {43, 51, 24, 255};

const int cellSize = 30;
const int cellCount = 25;
const int offset = 75;

double lastFrame = 0.0;

bool EventTriggered(double interval){
    double currentFrame = GetTime();

    if (currentFrame - lastFrame >= interval){
        lastFrame = currentFrame;
        return true;
    }
    return false;
}

class Snake{
    private:
    std::deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};

    public:
    Vector2 direction = Vector2{1, 0};
    bool addSegment = false;

    void Update(){
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment) 
            addSegment = false;
        else
            body.pop_back();
    }

    void Draw(){
        for (size_t i = 0; i < body.size(); i++){
            float xPos = body[i].x * cellSize;
            float yPos = body[i].y * cellSize;
            Rectangle segment = Rectangle{offset + xPos, offset + yPos, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5f, 6, darkGreen);
        }
    }

    void Reset(){
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        this->direction = Vector2{1, 0};
    }

    inline std::deque<Vector2> GetBody() const{return body;}
};

class Food{
    private:
    Vector2 position;
    Texture2D texture;

    public:
    Food(const std::deque<Vector2>& snakeBody){
        GenerateRandomPosition(position, snakeBody);

        Image image = LoadImage("resources/graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~Food(){
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    void GenerateRandomPosition(Vector2& position, const std::deque<Vector2>& snakeBody){
        bool validPosition = false;
        while (!validPosition){
            position.x = GetRandomValue(0, cellCount - 1);
            position.y = GetRandomValue(0, cellCount - 1);
            
            validPosition = true;
            for (const auto& snakeBodyPos: snakeBody){
                if (Vector2Equals(position, snakeBodyPos)){
                    validPosition = false;
                    break;
                }
            }
        }
        this->position = position;
    }

    inline Vector2 GetPosition() const{return position;}
};

class Game{
    private:
    bool isRunning = true;
    int currentScore = 0;
    int highScore = 0;

    Sound eatSound;
    Sound wallSound;

    std::unique_ptr<Snake> snake = std::make_unique<Snake>();
    std::unique_ptr<Food> food = std::make_unique<Food>(snake->GetBody());

    public:
    Game(){
        InitAudioDevice();
        eatSound = LoadSound("resources/sounds/eat.mp3");
        wallSound = LoadSound("resources/sounds/wall.mp3");
    }

    ~Game(){
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Update(){
        if (isRunning){
            snake->Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckSelfCollision();
        }
    }

    void HandleInput(){
        if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && snake->direction.y != 1){
            snake->direction = {0, -1};
            isRunning = true;
        }
        if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && snake->direction.y != -1){
            snake->direction = {0, 1};
            isRunning = true;
        }
        if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && snake->direction.x != 1){
            snake->direction = {-1, 0};
            isRunning = true;
        }
        if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && snake->direction.x != -1){
            snake->direction = {1, 0};
            isRunning = true;
        }
    }

    void CheckCollisionWithFood(){
        std::deque<Vector2> body = snake->GetBody();
        Vector2 foodPosition = food->GetPosition();

        if (Vector2Equals(body[0], foodPosition)){
            food->GenerateRandomPosition(foodPosition, body);
            snake->addSegment = true;
            highScore = std::max(highScore, ++currentScore);
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges(){
        std::deque<Vector2> body = snake->GetBody();
        if (body[0].x < 0 || body[0].y < 0 || body[0].x >= cellCount || body[0].y >= cellCount){
            GameOver();
        }
    }

    void CheckSelfCollision(){
        std::deque<Vector2> body = snake->GetBody();
        for (int i = 1; i < body.size(); i++){
            if (Vector2Equals(body[0], body[i])){
                GameOver();
            }
        }
    }

    void Draw(){
        food->Draw();
        snake->Draw();
    }

    void GameOver(){
        snake->Reset();
        Vector2 position = food->GetPosition();
        food->GenerateRandomPosition(position, snake->GetBody());

        isRunning = false;
        currentScore = 0;
        PlaySound(wallSound);
    }

    inline int GetScore() const{return currentScore;}
    inline int GetHighScore() const{return highScore;}
};

int main(){
    InitWindow(2*offset + cellCount * cellSize, 2*offset + cellCount * cellSize, "Retro Snake");
    SetTargetFPS(60);

    std::unique_ptr<Game> game = std::make_unique<Game>();

    while (!WindowShouldClose()){
        BeginDrawing();

        game->HandleInput();
        if (EventTriggered(0.2)){
            game->Update();
        }
        
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{offset-5, offset-5, cellSize*cellCount+10, cellSize*cellCount+10}, 5, darkGreen);
        DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
        DrawText(TextFormat("Score: %i", game->GetScore()), offset - 5, offset + cellSize*cellCount + 10, 40, darkGreen);
        DrawText(TextFormat("High Score: %i", game->GetHighScore()), (cellSize - 8)*cellCount, offset + cellSize*cellCount + 10, 40, darkGreen);
        game->Draw();

        EndDrawing();
    }
}