//#include "AfterMatch.h"
//
//void getHighScore()
//{
//    std::ifstream file;
//    file.open("HighScore.txt");
//    file >> HighScore;
//    file.close();
//}
//bool newHighScore(number& score)
//{
//    if (score.result() > HighScore) return true;
//    return false;
//}
//
//
//int afterMatch(number& score)
//{
//    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 0);
//    SDL_RenderClear(g_screen);
//    if (newHighScore(score))
//    {
//        drawNewHighscore(score);
//        std::ofstream file;
//        file.open("HighScore.txt");
//        file << score.result();
//        file.close();
//    }
//    else
//    {
//        drawNewscore(score);
//        drawGameOver();
//    }
//
//    Text Point;
//    int status = 1;
//    SDL_Rect point[2] = {
//        {g_Width / 2 - BlockSize_Output * 6, g_Height / 2 + 3.5 * BlockSize_Output,  BlockSize_Output * 3, BlockSize_Output * 3}, // quit == 0
//        {g_Width / 2 - BlockSize_Output * 8, g_Height / 2 - BlockSize_Output / 2 ,  BlockSize_Output * 3, BlockSize_Output * 3} // Try again == 1
//
//    };
//    {
//        if (!Point.LoadText("->", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Point' Text failed";
//        Point.setColor(255, 255, 255);
//        Point.Render(point[status]);
//    }
//
//    Text TryAgain;
//    drawLine1(TryAgain, "Try Again?");
//    Text Quit;
//    drawQuit(Quit);
//
//    location mouse;
//    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
//    while (true)
//    {
//        while (SDL_PollEvent(&g_event)) {
//            if (g_event.type == SDL_QUIT) return 0;
//            else if (g_event.type == SDL_KEYDOWN)
//            {
//                switch (g_event.key.keysym.sym)
//                {
//                case SDLK_SPACE:
//                    return status;
//                case SDLK_ESCAPE:
//                    return 0;
//                case SDLK_DOWN:
//                    status--;
//                    if (status < 0) status = 1;
//                    Point.move(point[status]);
//                    break;
//                case SDLK_UP:
//                    status++;
//                    if (status > 1) status = 0;
//                    Point.move(point[status]);
//                    break;
//                }
//            }
//            else if (g_event.type == SDL_MOUSEMOTION)
//            {
//                SDL_GetMouseState(&mouse.x, &mouse.y);
//                //getMouseMotion_wider(mouse, Play, Quit, Point);
//
//                if (mouse.y > TryAgain.top() && mouse.y < TryAgain.dep())
//                {
//                    status = 1;
//                    Point.move(point[status]);
//                }
//                else if (mouse.y > Quit.top() && mouse.y < Quit.dep())
//                {
//                    status = 0;
//                    Point.move(point[status]);
//                }
//            }
//            else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
//                SDL_GetMouseState(&mouse.x, &mouse.y);
//                int temp = getMouseClick(mouse, TryAgain, Quit);
//                if (temp != -1) return temp;
//            }
//        }
//        SDL_Delay(t_Delay);
//    }
//}
//
//void drawGameOver()
//{
//    Text GameOver;
//    GameOver.setColor(255, 20, 25);
//    if (!GameOver.LoadText("Game Over", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'GameOver' Text failed";
//    SDL_Rect rect;
//    rect.w = BlockSize_Output * 18;
//    rect.h = BlockSize_Output * 8;
//    rect.x = (g_Width - rect.w) / 2;
//    rect.y = g_Height / 2 - rect.h - 3 * BlockSize_Output; \
//
//        GameOver.Render(rect);
//}
//
//void drawNewHighscore(number &score)
//{
//    Text highscore;
//    if (!highscore.LoadText("New HighScore", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'New Highscore' Text failed";
//    {
//        SDL_Rect rect;
//        rect.w = BlockSize_Output * 24;
//        rect.h = BlockSize_Output * 6;
//        rect.x = (g_Width - rect.w) / 2;
//        rect.y = g_Height / 2 - rect.h - 4 * BlockSize_Output;
//
//        highscore.setColor(255, 255, 255);
//        highscore.Render(rect);
//    }
//
//
//    Text Text_Score;
//    Text_Score.LoadText(score.value, "Font//3X5_____.ttf", g_screen);
//    {
//        SDL_Rect rect;
//        rect.w = BlockSize_Output * score.value.size() * 2;
//        rect.h = BlockSize_Output * 3;
//        rect.x = (g_Width - rect.w) / 2;
//        rect.y = g_Height / 2 - 5 * BlockSize_Output;
//
//        Text_Score.setColor(255, 255, 255);
//        Text_Score.Render(rect);
//    }
//}
//
//void drawNewscore(number& score)
//{
//    Text Text_Score;
//    Text_Score.LoadText(score.value, "Font//3X5_____.ttf", g_screen);
//    {
//        SDL_Rect rect;
//        rect.w = BlockSize_Output * score.value.size() * 2;
//        rect.h = BlockSize_Output * 3;
//        rect.x = (g_Width - rect.w) / 2;
//        rect.y = g_Height / 2 - 5 * BlockSize_Output;
//
//        Text_Score.setColor(255, 255, 255);
//        Text_Score.Render(rect);
//    }
//}
