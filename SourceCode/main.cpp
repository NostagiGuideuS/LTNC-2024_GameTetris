#include "CommonFunc.h"
#include "BaseObject.h"
#include "Shape.h"
#include "BoardBase.h"
#include "Text.h"



Shape Layer[15]; // use 1->7 as true Layer, 8->14 as Shadow
Shape *Shadow = Layer + 7;
BoardBase base;
Shape* currentLayer;
Shape* currentShadow;
Shape* Bench;
number score;


std::ostream& operator<< (std::ostream& os, const BoardBase& base)
{
    for (int i = 0; i < base.height; i++)
    {
        for (int j = 0; j < base.width; j++)
        {
            os << base.block[j][i] << ' ';
        }
        os << '\n';
    }
    return os;
}


// Construction
bool InitData();
bool LoadBackGround(); // Load image or set back ground color;
void Close();

// Work with image
void createLayer();
void createShadow();
void createBase();


// Mechanical Issues
int Control();
void littleFix(); // if Layer near the wall, something must change
void add2Base(Shape& Layer); // When a block has to been fixed to a position on base
bool stopLayer_whenDown(Shape& Layer); // Check if we need to 'ass2Base'
bool stopLayer_whenRight(Shape& Layer);
bool stopLayer_whenLeft(Shape& Layer);
void localReDraw(Shape& Layer); // When image moving, erase its foot print
void get1Line(); // happen when 'add2Base success
void resetBench();
void SwitchBench();
bool CannotChange();
void foreShadow();
void hardcoreCounting(int k);



// InGame design
void drawScoreBoard();


// Main menu
void drawTETRIS();
void drawLine1(Text& Play, std::string content); // draw Play text
void drawMidLine(Text& Mode, std::string content, int color);
void drawLine2(Text& Quit, std::string content);
void drawGuide();
void getMouseMotion_wider(location& mouse, Text& Play, Text& Quit);
bool getMouseClick(location& mouse, Text& Button);

int OpenMenu();


// after match already done
int afterMatch();

void getHighScore();
bool newHighScore();
void insertScore(int i);

void drawNewHighscore();
void drawGameOver();
void drawNewscore();


int GameOn()
{
    getHighScore();
    srand(time(NULL));
    bool isQuit = false;
    bool Gameover = false;
    int count = 0;


    createLayer();
    createBase();
    createShadow();
    drawScoreBoard();

    Bench = Layer + (rand() % 7 + 1);
    resetBench();

    while (!isQuit)
    {
        SDL_Delay(t_Delay);
        // Drop per lapses
        if (!(count % t_Drop))
        {
            count = 0;
            localReDraw(*currentLayer);
            currentLayer->move('d');
            if (!stopLayer_whenDown(*currentLayer)); //Mix_PlayChannel(2, whenDrop, 0);
        }

        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT) return 2;
            else if (g_event.type == SDL_KEYDOWN)
            {
                if (Control() == 0) return -1;;
            }
            else { ; }
        }

        foreShadow();
        currentShadow->Show();
        currentLayer->Show();

        if (base.GameOver())
        {
            Gameover = true;
            isQuit = true;
        }

        SDL_RenderPresent(g_screen);
        count++;
    }
    return Gameover;
}
// Main
//#undef main
int main(int argc, char* argv[])
{
    int n; // just temperature
    if (InitData() == false) return -1;
    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
    SDL_RenderClear(g_screen);

    MainMenu:
    switch (OpenMenu())
    {
    case 1: // click Play
        Game_on:
        if (LoadBackGround())
        {
            if (GameOn() == 2) break;
            base.clear();
        }
        else
        {
            std::cout << "Load BackGround error!!!";
            break;
        }
        n = afterMatch();
        if (n == 2) goto Game_on;
        else if (n == 1) goto MainMenu;
        
    break;
    default:
        break; // escape a.k.a click Quit
    }

    Close();
    return 0;
}



// Construction
bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (ret < 0) return false;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("TETRIS",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        g_Width, g_Height,
        SDL_WINDOW_SHOWN);

    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor (g_screen, 150, 150, 150, 255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
            {
                success = false;
            }
        }
    }
    
    whenAdd2Base = Mix_LoadWAV("General Sounds\\se_game_landing.wav"); // OK
    whenMove = Mix_LoadWAV("General Sounds\\se_game_move.wav"); // Ok
    whenDrop = Mix_LoadWAV("General Sounds\\se_game_softdrop.wav"); // OK
    //whenGameOver = Mix_LoadWAV("General Sounds\\.wav")

    whenGet1Line[0] = Mix_LoadWAV("General Sounds\\se_game_single.wav"); // OK
    whenGet1Line[1] = Mix_LoadWAV("General Sounds\\se_game_double.wav"); // OK
    whenGet1Line[2] = Mix_LoadWAV("General Sounds\\se_game_triple.wav"); // OK
    whenGet1Line[3] = Mix_LoadWAV("General Sounds\\se_game_kbcomp.wav"); // OK

    whenChangeFrame = Mix_LoadWAV("General Sounds\\se_game_rotate.wav"); // OK

    whenClickOption = Mix_LoadWAV("General Sounds\\se_sys_select.wav"); // OK

    return success;
}

void Close()
{
    Mix_FreeChunk (whenMove); // Channel 2
    Mix_FreeChunk (whenDrop); // Channel 2
    Mix_FreeChunk (whenAdd2Base); // Channel 1
    Mix_FreeChunk (whenGameOver); // Channel 3
    for (int i=0; i<4; i++) Mix_FreeChunk (whenGet1Line[i]); // Channel 1
    Mix_FreeChunk (whenChangeFrame); // channel 2
    Mix_FreeChunk(whenClickOption); // channel 2
       
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

bool LoadBackGround()
{
    /*bool ret = g_backGround.LoadImage("img//background.png", g_screen);
    if (!ret)
    {
        std::cout << "Load BackGround image failed\n";
    }*/

    // Ve vien
    SDL_SetRenderDrawColor(g_screen, 50, 55, 60, 255);
    SDL_RenderClear(g_screen); 
    // Ve khuon tro choi
    SDL_SetRenderDrawColor(g_screen, playGroundColor_R, playGroundColor_G, playGroundColor_B, 255);
    SDL_Rect match { BlockSize_Output, BlockSize_Output, g_w * BlockSize_Output, g_h * BlockSize_Output };
    SDL_RenderFillRect(g_screen, &match);
    return true;
}


// Work with Image
void createLayer()
{
    // Layer 1 - Done
    {
        FrameSize sample[4] = {
            {0, 0, 3, 2},
            {3, 0, 2, 3},
            {5, 0, 3, 2},
            {8, 0, 2, 3} };
        location** mt = new location * [4];
        location mt_value[8] = {
                     {0, 1}, {1, 1} ,
                     {0, 0}, {0, 1} ,
                     {1, 0}, {2, 0} ,
                     {1, 1}, {1, 2} };
        for (int i = 0; i < 4; i++)
        {
            mt[i] = mt_value + 2 * i;
        }
        Layer[1].createFrame(1, sample, 4, mt, 2, "img//Layer 1.png", g_screen);

        delete[] mt;
    }
    // Layer 2 - Done
    {
        FrameSize sample[4] = {
            {0, 0, 3, 2},
            {3, 0, 2, 3},
            {5, 0, 3, 2},
            {8, 0, 2, 3} };
        location** mt = new location * [4];
        location mt_value[8] = {
                     {0, 1}, {2, 1} ,
                     {0, 0}, {0, 2} ,
                     {0, 0}, {2, 0} ,
                     {1, 0}, {1, 2} };
        for (int i = 0; i < 4; i++)
        {
            mt[i] = mt_value + 2 * i;
        }
        Layer[2].createFrame(2, sample, 4, mt, 2, "img//Layer 2.png", g_screen);

        delete[] mt;
    }
    // Layer 3 - Done
    {
        FrameSize sample[4] = {
            {0, 0, 3, 2},
            {3, 0, 2, 3},
            {5, 0, 3, 2},
            {8, 0, 2, 3} };
        location** mt = new location * [4];
        location mt_value[8] = {
                     {1, 1}, {2, 1} ,
                     {0, 1}, {0, 2} ,
                     {0, 0}, {1, 0} ,
                     {1, 0}, {1, 1} };
        for (int i = 0; i < 4; i++)
        {
            mt[i] = mt_value + 2 * i;
        }
        Layer[3].createFrame(3, sample, 4, mt, 2, "img//Layer 3.png", g_screen);

        delete[] mt;
    }
    // Layer 4 - Done
    {
        FrameSize sample[2] = {
            {0, 0, 3, 2},
            {3, 0, 2, 3} };
        location** mt = new location * [2];
        location mt_value[4] = {
                     {0, 0}, {2, 1} ,
                     {1, 0}, {0, 2} };
        for (int i = 0; i < 2; i++)
        {
            mt[i] = mt_value + 2 * i;
        }
        Layer[4].createFrame(4, sample, 2, mt, 2, "img//Layer 4.png", g_screen);

        delete[] mt;
    }
    // Layer 5 - Done
    {
        FrameSize sample[2] = {
            {0, 0, 4, 1},
            {4, 0, 1, 4} };

        // This layer has no empty
        Layer[5].createFrame(5, sample, 2, "img//Layer 5.png", g_screen);
    }
    // Layer 6 - Done
    {
        FrameSize sample[1] = {
            {0, 0, 2, 2}, };

        // This layer has no empty
        Layer[6].createFrame(6, sample, 1, "img//Layer 6.png", g_screen);
    }
    // Layer 7 - Done
    {
        FrameSize sample[2] = {
            {0, 0, 3, 2},
            {3, 0, 2, 3} };
        location** mt = new location * [2];
        location mt_value[4] = {
                     {0, 1}, {2, 0} ,
                     {0, 0}, {1, 2} };
        for (int i = 0; i < 2; i++)
        {
            mt[i] = mt_value + 2 * i;
        }
        Layer[7].createFrame(7, sample, 2, mt, 2, "img//Layer 7.png", g_screen);

        delete[] mt;
    }
}

void createBase()
{
    base.createBoardBase("img//Brick.png", g_screen);
}

void localReDraw(Shape& Layer)
{
    /* Mechanical Issues:
        When a Layer move, we don't draw all the background again, because it may erase all base which we already draw, and never want to draw it back again.
        Therefore, re-draw just where it has coming through
        And if we erase a base's element, just re-draw single it
    */

    SDL_SetRenderDrawColor(g_screen, playGroundColor_R, playGroundColor_G, playGroundColor_B, 255);
    SDL_Rect whatLeft{ (Layer.pos.x + 1) * BlockSize_Output, (Layer.pos.y + 1) * BlockSize_Output, Layer.currentWidth() * BlockSize_Output, Layer.currentHeight() * BlockSize_Output };
    SDL_RenderFillRect(g_screen, &whatLeft);

    for (int i = 0; i < Layer.currentWidth(); i++)
    {
        for (int j = 0; j < Layer.currentHeight(); j++)
        {
            location temp = { i, j };
            base.LocalRender(Layer.pos + temp, g_screen, NULL);
        }
    }
}

void createShadow()
{
    Shadow[1].Shadow_need_Host(Layer + 1, "img//Shadow 1.png");
    Shadow[2].Shadow_need_Host(Layer + 2, "img//Shadow 2.png");
    Shadow[3].Shadow_need_Host(Layer + 3, "img//Shadow 3.png");
    Shadow[4].Shadow_need_Host(Layer + 4, "img//Shadow 4.png");
    Shadow[5].Shadow_need_Host(Layer + 5, "img//Shadow 5.png");
    Shadow[6].Shadow_need_Host(Layer + 6, "img//Shadow 6.png");
    Shadow[7].Shadow_need_Host(Layer + 7, "img//Shadow 7.png");
}



// Mechanical Issues
int Control()
{
    localReDraw(*currentLayer);
    switch (g_event.key.keysym.sym)
    {
    case SDLK_a:
    case SDLK_LEFT:
        currentLayer->move('l');
        if (!stopLayer_whenLeft(*currentLayer)) Mix_PlayChannel(2, whenMove, 0);
        break;
    case SDLK_d:
    case SDLK_RIGHT:
        currentLayer->move('r');
        if (!stopLayer_whenRight(*currentLayer)) Mix_PlayChannel(2, whenMove, 0);
        break;
    case SDLK_s:
    case SDLK_DOWN:
        currentLayer->move('d');
        if (!stopLayer_whenDown(*currentLayer)) //Mix_PlayChannel(2, whenDrop, 0);;
            break;
        /*case SDLK_w:
        case SDLK_UP:
            currentLayer->move('u');
            stopLayer_whenDown(*currentLayer);
            break;*/
    case SDLK_SPACE:
        currentLayer->nextFrame();
        if (CannotChange()) currentLayer->prevFrame();
        else Mix_PlayChannel(2, whenChangeFrame, 0);
        break;
    case SDLK_RCTRL:
    case SDLK_LCTRL:
        currentLayer->prevFrame();
        if (CannotChange()) currentLayer->nextFrame();
        else Mix_PlayChannel(2, whenChangeFrame, 0);
        break;
    case SDLK_RALT:
    case SDLK_LSHIFT:
    case SDLK_LALT:
        SwitchBench();
        break;
    case SDLK_F1:
    case SDLK_p:
    {
        bool pause = true;
        while (pause)
        {
            SDL_PollEvent(&g_event);
            if (g_event.type == SDL_QUIT) return 0;
            else if (g_event.type == SDL_KEYDOWN)
            {
                if (g_event.key.keysym.sym == SDLK_p || g_event.key.keysym.sym == SDLK_F1) pause = false;
            }
        }
        break;
    }
    }
    return 1;
}

void littleFix()
{
    
    if (currentLayer->pos.x + currentLayer->currentWidth() > base.width)
    {
        currentLayer->pos.x = base.width - currentLayer->currentWidth();
    }
}

bool CannotChange()
{
    int oldX = currentLayer->pos.x;
    littleFix();
    for (int i = currentLayer->currentWidth() - 1; i >= 0; i--)
    {
        for (int j = currentLayer->currentHeight() - 1; j >= 0; j--)
        {
            if (!currentLayer->isEmpty({ i, j }))
            {
                if (base.block[currentLayer->pos.x + i][currentLayer->pos.y + j] != -1)
                {
                    currentLayer->pos.x = oldX;
                    return true;
                }
            }
        }
    }
    return false;
}

void add2Base(Shape& Layer)
{
    for (int i = 0; i < Layer.currentWidth(); i++)
    {
        for (int j = 0; j < Layer.currentHeight(); j++)
        {
            location temp = { i, j };
            if (!Layer.isEmpty(temp))
            {
                temp.y--;
                base.block[Layer.pos.x + temp.x][Layer.pos.y + temp.y] = Layer.status;
                base.LocalRender(Layer.pos + temp, g_screen, NULL);
            }
        }
    }
    get1Line();
    resetBench();
}

bool stopLayer_whenDown(Shape& Layer)
{
    location check;
    for (int i = 0; i < Layer.currentWidth(); i++)
    {
        check = Layer.aBottom(i);
        if (base.block[check.x][check.y] != -1)
        {
            add2Base(Layer);
            return true;
        }
        else if (check.y >= base.height)
        {
            add2Base(Layer);
            return true;
        }
    }
    return false;
}

bool stopLayer_whenRight(Shape& Layer)
{
    location check{ Layer.currentWidth() - 1, 0 };
    location temp;
    for (; check.y < Layer.currentHeight(); check.y++)
    {
        if (!Layer.isEmpty(check))
        {
            temp = Layer.pos + check;
            if (base.block[temp.x][temp.y] != -1)
            {
                Layer.move('l');
                return true;
            }
        }
    }
    return false;
}
bool stopLayer_whenLeft(Shape& Layer)
{
    location check{ 0, 0 };
    location temp;
    for (; check.y < Layer.currentHeight(); check.y++)
    {
        if (!Layer.isEmpty(check))
        {
            temp = Layer.pos + check;
            if (base.block[temp.x][temp.y] != -1)
            {
                Layer.move('r');
                return true;
            }
        }
    }
    return false;
}

void get1Line()
{
    int line = currentLayer->pos.y-1;
    int end = line + currentLayer->currentHeight();
    bool* mark = new bool[currentLayer->currentHeight()];
    int haveALine = currentLayer->currentHeight();
    for (int i = currentLayer->currentHeight()-1; i >= 0 ; i--) mark[i] = true;
    for (int k = line; k < end; k++)
    {
        for (int i = 0; i < base.width; i++)
        {
            if (base.block[i][k] == -1) // mean it's empty
            {
                mark[k - line] = false;
                haveALine--;
                break;
            }
            // next in line
        }
    }

    // draw aniamation when it get a line
    // ++ score
    if (haveALine != 0)
    {
        Mix_PlayChannel(1, whenGet1Line[haveALine - 1], 0);
        hardcoreCounting(haveALine);
        score.Render();

        base.setupErase();
        for (int x = 0; x < base.width; x++)
        {
            for (int i = 0; i < currentLayer->currentHeight(); i++)
            {
                if (mark[i])
                {
                    base.drawErase(line + i, x);
                }
            }
            SDL_RenderPresent(g_screen);
            SDL_Delay(t_Delay);
        }
        base.setupBackgroundColor();
    }
    // draw the new
    for (int i = 0; i < currentLayer->currentHeight(); i++)
    {
        if (mark[i])
        {
            base.eraseLine(line + i); // it erase then render the new view of base
        }
    }

    delete[] mark;
}

void resetBench()
{
    currentLayer = Bench;
    currentLayer->spawn();
    currentShadow = currentLayer + 7;
    int n = rand() % 7 + 1;
    Bench = Layer + n;

    Bench->showOnBench();
}

void SwitchBench()
{
    Shape* temp = currentLayer;
    currentLayer = Bench;
    Bench = temp;
    localReDraw(*currentShadow);
    currentLayer->spawn();
    currentShadow = currentLayer + 7;
    hardcore += rand() % 2;
    Bench->showOnBench();
}

void foreShadow()
{
    location lowest;
    int dis = base.height;
    for (int i = currentLayer->currentWidth() - 1; i >= 0; i--)
    {
        lowest =  currentLayer->aBottom(i);
        while (base.block[lowest.x][lowest.y] == -1 && lowest.y < base.height)
            lowest.y++;
        if (lowest.y - currentLayer->aBottom(i).y - 1 < dis)
            dis = lowest.y - currentLayer->aBottom(i).y - 1;
    }
    localReDraw(*currentShadow);
    currentShadow->pos.y = currentLayer->pos.y + dis;
    currentShadow->pos.x = currentLayer->pos.x;
    currentShadow->currentFrame = currentLayer->currentFrame;
}


// InGame design
void drawScoreBoard()
{
    Text Next;
    Next.LoadText("NEXT:", "Font//ARCADE.ttf", g_screen);
    {
        SDL_Rect rect{ (g_w + 3) * BlockSize_Output, BlockSize_Output * 2,  BlockSize_Output * 10, BlockSize_Output * 3 };
        Next.setColor(255, 255, 255);
        Next.Render(rect);
    }

    Text Text_Score;
    Text_Score.LoadText("SCORE:", "Font//ARCADE.ttf", g_screen);
    {
        SDL_Rect rect{ (g_w + 3) * BlockSize_Output, g_Height / 2,  BlockSize_Output * 10, BlockSize_Output * 3 };
        Text_Score.setColor(255, 255, 255);
        Text_Score.Render(rect);
    }

    score.LoadText("Font//3X5_____.ttf", g_screen);
    score.Render();
}

void hardcoreCounting(int k)
{
    hardcore += k;
    score += hardcore / 10 + scoreMode * k - 1;
    if (hardcore % 5 == 0 && t_Drop > 1) t_Drop = t_Drop * 2 / 3;
}



// Main menu

int OpenMenu()
{
    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
    SDL_RenderClear(g_screen);

    drawTETRIS();

    Text Play;
    drawLine1(Play, "PLAY");

    Text Mode;
    int mode = 1;
    drawMidLine(Mode, " EASY ", mode);

    Text Quit;
    drawLine2(Quit, "QUIT");

    Text Point;
    Text Keep[2];
    int status = 1;
    SDL_Rect point[4] = {
        {Quit.renderRect.x - BlockSize_Output*3, Quit.renderRect.y + Quit.renderRect.h / 2 - BlockSize_Output * 1.5 ,  BlockSize_Output * 3, BlockSize_Output * 3}, // quit == 0
        {Play.renderRect.x - BlockSize_Output * 3, Play.renderRect.y + Play.renderRect.h / 2 - BlockSize_Output * 1.5 ,  BlockSize_Output * 3, BlockSize_Output * 3}, // Play == 1
        {Mode.renderRect.x - BlockSize_Output * 2, Mode.renderRect.y + Mode.renderRect.h / 2 - BlockSize_Output * 1.5 , BlockSize_Output * 1.5, BlockSize_Output * 3}, // Mode == 2, 3
        {Mode.renderRect.x + Mode.renderRect.w + BlockSize_Output , Mode.renderRect.y + Mode.renderRect.h / 2 - BlockSize_Output * 1.5 , BlockSize_Output * 1.5, BlockSize_Output * 3}
    };
    {
        if (!Point.LoadText("->", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Point' Text failed";
        Point.setColor(255, 255, 255); // first Postion
        Point.Render(point[status]);
        if (!Keep[0].LoadText("<", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Keep' Text failed";
        if (!Keep[1].LoadText(">", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Keep' Text failed";
    }


    SDL_RenderPresent(g_screen);
    location mouse;
    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
    while (true)
    {
        while (SDL_PollEvent(&g_event)) {
            if (g_event.type == SDL_QUIT) return 0;
            else if (g_event.type == SDL_KEYDOWN)
            {
                switch (g_event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    if (status == 3) break;
                    Mix_PlayChannel(2, whenClickOption, 0);
                    SDL_Delay(100);
                    return status;
                case SDLK_ESCAPE:
                    Mix_PlayChannel(2, whenClickOption, 0);
                    return 0;
                case SDLK_DOWN:
                    Mix_PlayChannel(1, whenChangeFrame, 0);
                    if (status == 1) // play currently
                    {
                        status = 3;
                        Point.clear();
                        Keep[0].move(point[2]);
                        Keep[1].move(point[3]);
                    }
                    else if (status == 3)
                    {
                        Keep[0].clear();
                        Keep[1].clear();
                        status = 0;
                        Point.move(point[status]);
                    }
                    else if (status == 0) // Quit
                    {
                        status = 1;
                        Point.move(point[status]);
                    }
                    break;
                case SDLK_UP:
                    Mix_PlayChannel(1, whenChangeFrame, 0);
                    if (status == 0) // quit currently
                    {
                        status = 3;
                        Point.clear();
                        Keep[0].move(point[2]);
                        Keep[1].move(point[3]);
                    }
                    else if (status == 3)
                    {
                        Keep[0].clear();
                        Keep[1].clear();
                        status = 1;
                        Point.move(point[status]);
                    }
                    else if (status == 1) // Play
                    {
                        status = 0;
                        Point.move(point[status]);
                    }
                    break;
                case SDLK_RIGHT:
                    if (status == 3)
                    {
                        Mix_PlayChannel(1, whenChangeFrame, 0);
                        switch (mode)
                        {
                        case 1: mode = 2; drawMidLine(Mode, "MEDIUM", mode); break;
                        case 2: mode = 3; drawMidLine(Mode, " HARD ", mode); break;
                        case 3: mode = 1; drawMidLine(Mode, " EASY ", mode); break;
                        }
                    }
                    break;
                case SDLK_LEFT:
                    if (status == 3)
                    {
                        Mix_PlayChannel(1, whenChangeFrame, 0);
                        switch (mode)
                        {
                        case 1: mode = 3; drawMidLine(Mode, " HARD ", mode); break;
                        case 2: mode = 1; drawMidLine(Mode, " EASY ", mode); break;
                        case 3: mode = 2; drawMidLine(Mode, "MEDIUM", mode); break;
                        }
                    }
                    break;
                }
            }
            else if (g_event.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&mouse.x, &mouse.y);
                //getMouseMotion_wider(mouse, Play, Quit, Point);

                if (mouse.y > Play.top() && mouse.y < Play.dep())
                {
                    status = 1;
                    Point.move(point[status]);
                }
                else if (mouse.y > Quit.top() && mouse.y < Quit.dep())
                {
                    status = 0;
                    Point.move(point[status]);
                }
                else Point.clear();
                if (mouse.y > Mode.top() && mouse.y < Mode.dep())
                {
                    status = 3;
                    Keep[0].move(point[2]);
                    Keep[1].move(point[3]);
                }
                else
                {
                    Keep[0].clear();
                    Keep[1].clear();
                }
            }
            else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (getMouseClick(mouse, Play)) return 1;
                else if (getMouseClick(mouse, Quit)) return 0;
                if (status == 3)
                {
                    if (getMouseClick(mouse, Keep[0]))
                    {
                        Mix_PlayChannel(1, whenChangeFrame, 0);
                        switch (mode)
                        {
                        case 1: mode = 3; drawMidLine(Mode, " HARD ", mode); break;
                        case 2: mode = 1; drawMidLine(Mode, " EASY ", mode); break;
                        case 3: mode = 2; drawMidLine(Mode, "MEDIUM", mode); break;
                        }
                    }
                    else if (getMouseClick(mouse, Keep[1]))
                    {
                        Mix_PlayChannel(1, whenChangeFrame, 0);
                        switch (mode)
                        {
                        case 1: mode = 2; drawMidLine(Mode, "MEDIUM", mode); break;
                        case 2: mode = 3; drawMidLine(Mode, " HARD ", mode); break;
                        case 3: mode = 1; drawMidLine(Mode, " EASY ", mode); break;
                        }
                    }
                }
            }
        }
        SDL_Delay(t_Delay);
    }
}

void drawTETRIS()
{
    Text Tetris;
    Tetris.LoadMultipleText("Font//ARCADE.ttf", g_screen);
    SDL_Rect rect{ g_Width / 2 - BlockSize_Output * 9, g_Height / 2 - BlockSize_Output * 10, BlockSize_Output * 3, BlockSize_Output * 10 };
    Tetris.setText_Color("T", rect, 230, 14, 18);
    rect.x += 3 * BlockSize_Output;
    Tetris.setText_Color("E", rect, 246, 119, 27);
    rect.x += 3 * BlockSize_Output;
    Tetris.setText_Color("T", rect, 253, 241, 44);
    rect.x += 3 * BlockSize_Output;
    Tetris.setText_Color("R", rect, 108, 238, 75);
    rect.x += 3 * BlockSize_Output;
    Tetris.setText_Color("I", rect, 3, 254, 253);
    rect.x += 3 * BlockSize_Output;
    Tetris.setText_Color("S", rect, 255, 50, 254);
}

void drawLine1(Text& Play, std::string content)
{
    Play.setColor(255, 255, 255);

    if (!Play.LoadText(content, "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Play' Text failed";
    SDL_Rect rect;
    rect.w = BlockSize_Output * content.size();
    rect.h = BlockSize_Output * 2;
    rect.x = (g_Width - rect.w) / 2;
    rect.y = g_Height / 2; // -2 * BlockSize_Output;

    Play.Render(rect);
}

void drawMidLine(Text& Mode, std::string content, int color = 0)
{
    switch (color)
    {
        // color 1 for easy, 2 to medium, 3 for hard
    case 1: Mode.setColor(100, 200, 250); t_Drop = 50; scoreMode = 4; break;
    case 2: Mode.setColor(250, 200, 100); t_Drop = 40; scoreMode = 6; break;
    case 3: Mode.setColor(255, 50, 50); t_Drop = 25; scoreMode = 10; break;
    default: Mode.setColor(255, 255, 255);
    }

    if (!Mode.LoadText(content, "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Mode' Text failed";
    SDL_Rect rect;
    rect.w = BlockSize_Output * content.size();
    rect.h = BlockSize_Output * 2;
    rect.x = (g_Width - rect.w) / 2;
    rect.y = g_Height / 2 + 3 * BlockSize_Output;

    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
    SDL_RenderFillRect(g_screen, &rect);

    Mode.Render(rect);
    SDL_RenderPresent(g_screen);
}

void drawLine2(Text& Quit, std::string content)
{
    Quit.setColor(255, 255, 255);

    if (!Quit.LoadText(content, "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Quit' Text failed";

    SDL_Rect rect;
    rect.x = g_Width / 2 - BlockSize_Output * content.size() / 2;
    rect.y = g_Height / 2 + 6 * BlockSize_Output;
    rect.w = BlockSize_Output * content.size();
    rect.h = BlockSize_Output * 2;

    Quit.Render(rect);
}

void drawGuide()
{

}

void getMouseMotion_wider(location& mouse, Text& Play, Text& Quit)
{
    if (mouse.x > Play.left() && mouse.x < Play.right() && mouse.y > Play.top() && mouse.y < Play.dep())
    {
        Play.wider();
    }
    else Play.unWider();

    if (mouse.x > Quit.left() && mouse.x < Quit.right() && mouse.y > Quit.top() && mouse.y < Quit.dep())
    {
        Quit.wider();
    }
    else Quit.unWider();
}

bool getMouseClick(location& mouse, Text& Button)
{
    if (mouse.x > Button.left() && mouse.x < Button.right() && mouse.y > Button.top() && mouse.y < Button.dep())
    {
        Mix_PlayChannel(2, whenClickOption, 0);
        SDL_Delay(100);
        return 1;
    }
    else return 0;
}



// about High_score
void getHighScore()
{
    std::ifstream file;
    file.open("HighScore.txt");
    for (int i = 0; i < 3; i++)
    {
        if (!file.eof())
        {
            file >> HighScore[i];
        }
        else HighScore[i] = 0;
    }
    file.close();
}
bool newHighScore()
{
    for (int i = 0; i < 5; i++)
    {
        if (score.result() > HighScore[i])
        {
            insertScore(i);
            std::ofstream file;
            for (int i = 0; i < 5; i++)
            {
                file << HighScore[i];
            }
            file.close();
            return true;
        }
    }
    return false;
}
void insertScore(int i)
{
    for (int j = i; j < 4; j++)
    {
        HighScore[j + 1] = HighScore[j];
    }
    HighScore[i] = score.result();
}


// After the Match already done. We have
int afterMatch()
{
    /* return 0 for quit
    return 1 for try again
    keep the screen if not return anything*/

    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 0);
    SDL_RenderClear(g_screen);
    if (newHighScore())
    {
        drawNewHighscore();
    }
    else
    {
        drawNewscore();
        drawGameOver();
    }

    Text TryAgain;
    drawLine1(TryAgain, "Try Again?");
    Text Quit;
    drawLine2(Quit, "QUIT");
    Text Menu;
    drawMidLine(Menu, "MAIN MENU");
    
    Text Point;
    int status = 1;
    SDL_Rect point[3] = {
        {Quit.renderRect.x - BlockSize_Output * 3, Quit.renderRect.y + Quit.renderRect.h / 2 - BlockSize_Output * 1.5 ,  BlockSize_Output * 3, BlockSize_Output * 3}, // quit == 0
        {Menu.renderRect.x - BlockSize_Output * 3, Menu.renderRect.y + Menu.renderRect.h / 2 - BlockSize_Output * 1.5, BlockSize_Output * 3, BlockSize_Output * 3 },
        {TryAgain.renderRect.x - BlockSize_Output * 3, TryAgain.renderRect.y + TryAgain.renderRect.h / 2 - BlockSize_Output * 1.5 ,  BlockSize_Output * 3, BlockSize_Output * 3} // Play == 1
    };
    {
        if (!Point.LoadText("->", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Point' Text failed";
        Point.setColor(255, 255, 255);
        Point.Render(point[status]);
    }

   
    location mouse;
    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
    while (true)
    {
        while (SDL_PollEvent(&g_event)) {
            if (g_event.type == SDL_QUIT) return 0;
            else if (g_event.type == SDL_KEYDOWN)
            {
                switch (g_event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    Mix_PlayChannel(2, whenClickOption, 0);
                    SDL_Delay(100);
                    return status;
                case SDLK_ESCAPE:
                    Mix_PlayChannel(2, whenClickOption, 0);
                    return 0;
                case SDLK_DOWN:
                    Mix_PlayChannel(1, whenChangeFrame, 0);
                    status--;
                    if (status < 0) status = 2;
                    Point.move(point[status]);
                    break;
                case SDLK_UP:
                    Mix_PlayChannel(1, whenChangeFrame, 0);
                    status++;
                    if (status > 2) status = 0;
                    Point.move(point[status]);
                    break;
                }
            }
            else if (g_event.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&mouse.x, &mouse.y);
                //getMouseMotion_wider(mouse, Play, Quit, Point);

                if (mouse.y > TryAgain.top() && mouse.y < TryAgain.dep())
                {
                    status = 2;
                    Point.move(point[status]);
                }
                else if (mouse.y > Quit.top() && mouse.y < Quit.dep())
                {
                    status = 0;
                    Point.move(point[status]);
                }
                else if (mouse.y > Menu.top() && mouse.y < Menu.dep())
                {
                    status = 1;
                    Point.move(point[status]);
                }
            }
            else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (getMouseClick(mouse, Quit)) return 0;
                if (getMouseClick(mouse, Menu)) return 1;
                if (getMouseClick(mouse, TryAgain)) return 2;
            }
        }
        SDL_Delay(t_Delay);
    }
}

void drawGameOver()
{
    Text GameOver;
    GameOver.setColor(255, 20, 25);
    if (!GameOver.LoadText("Game Over", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'GameOver' Text failed";
    SDL_Rect rect;
    rect.w = BlockSize_Output * 18;
    rect.h = BlockSize_Output * 8;
    rect.x = (g_Width - rect.w) / 2;
    rect.y = g_Height / 2 - rect.h - 3 * BlockSize_Output;

    GameOver.Render(rect);
}

void drawNewHighscore()
{
    Text highscore;
    if (!highscore.LoadText("New HighScore", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'New Highscore' Text failed";
    {
        SDL_Rect rect;
        rect.w = BlockSize_Output * 24;
        rect.h = BlockSize_Output * 6;
        rect.x = (g_Width - rect.w) / 2;
        rect.y = g_Height / 2 - rect.h - 4 * BlockSize_Output;

        highscore.setColor(255, 255, 255);
        highscore.Render(rect);
    }


    Text Text_Score;
    Text_Score.LoadText(score.value, "Font//3X5_____.ttf", g_screen);
    {
        SDL_Rect rect;
        rect.w = BlockSize_Output * score.value.size() * 2;
        rect.h = BlockSize_Output * 3;
        rect.x = (g_Width - rect.w) / 2;
        rect.y = g_Height / 2 - 5 * BlockSize_Output;

        Text_Score.setColor(255, 255, 255);
        Text_Score.Render(rect);
    }
}

void drawNewscore()
{
    Text Text_Score;
    Text_Score.LoadText(score.value, "Font//3X5_____.ttf", g_screen);
    {
        SDL_Rect rect;
        rect.w = BlockSize_Output * score.value.size() * 2;
        rect.h = BlockSize_Output * 3;
        rect.x = (g_Width - rect.w) / 2;
        rect.y = g_Height / 2 - 5 * BlockSize_Output;

        Text_Score.setColor(255, 255, 255);
        Text_Score.Render(rect);
    }
}

