//#include "Menu.h"
//
//int OpenMenu()
//{
//    drawTETRIS();
//
//    Text Play;
//    drawLine1(Play, "PLAY");
//
//    /*Text Guide;
//    drawGuide();*/
//
//    Text Quit;
//    drawQuit(Quit);
//
//    Text Point;
//    int status = 1;
//    SDL_Rect point[2] = {
//        {g_Width / 2 - BlockSize_Output * 6, g_Height / 2 + 3.5 * BlockSize_Output,  BlockSize_Output * 3, BlockSize_Output * 3}, // quit == 0
//        {g_Width / 2 - BlockSize_Output * 6, g_Height / 2 - BlockSize_Output / 2 ,  BlockSize_Output * 3, BlockSize_Output * 3} // Play == 1
//
//    };
//    {
//        if (!Point.LoadText("->", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Point' Text failed";
//        Point.setColor(255, 255, 255);
//        Point.Render(point[status]);
//    }
//
//
//    SDL_RenderPresent(g_screen);
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
//                if (mouse.y > Play.top() && mouse.y < Play.dep())
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
//                int temp = getMouseClick(mouse, Play, Quit);
//                if (temp != -1) return temp;
//            }
//        }
//        SDL_Delay(t_Delay);
//    }
//}
//
//void drawTETRIS()
//{
//    Text Tetris;
//    Tetris.LoadMultipleText("Font//ARCADE.ttf", g_screen);
//    SDL_Rect rect{ g_Width / 2 - BlockSize_Output * 9, g_Height / 2 - BlockSize_Output * 10, BlockSize_Output * 3, BlockSize_Output * 12 };
//    Tetris.setText_Color("T", rect, 230, 14, 18);
//    rect.x += 3 * BlockSize_Output;
//    Tetris.setText_Color("E", rect, 246, 119, 27);
//    rect.x += 3 * BlockSize_Output;
//    Tetris.setText_Color("T", rect, 253, 241, 44);
//    rect.x += 3 * BlockSize_Output;
//    Tetris.setText_Color("R", rect, 108, 238, 75);
//    rect.x += 3 * BlockSize_Output;
//    Tetris.setText_Color("I", rect, 3, 254, 253);
//    rect.x += 3 * BlockSize_Output;
//    Tetris.setText_Color("S", rect, 255, 50, 254);
//}
//
//void drawLine1(Text& Play, std::string content)
//{
//    if (!Play.LoadText(content, "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Play' Text failed";
//    SDL_Rect rect;
//    rect.w = BlockSize_Output * content.size();
//    rect.h = BlockSize_Output * 2;
//    rect.x = (g_Width - rect.w) / 2;
//    rect.y = g_Height / 2;
//
//    Play.setColor(255, 255, 255);
//    Play.Render(rect);
//}
//
//void drawQuit(Text& Quit)
//{
//    if (!Quit.LoadText("QUIT", "Font//ARCADE.ttf", g_screen)) std::cout << "Load 'Quit' Text failed";
//    SDL_Rect rect{ g_Width / 2 - BlockSize_Output * 2, g_Height / 2 + 4 * BlockSize_Output,  BlockSize_Output * 4, BlockSize_Output * 2 };
//    Quit.setColor(255, 255, 255);
//    Quit.Render(rect);
//}
//
//void drawGuide()
//{
//
//}
//
//void getMouseMotion_wider(location& mouse, Text& Play, Text& Quit)
//{
//    if (mouse.x > Play.left() && mouse.x < Play.right() && mouse.y > Play.top() && mouse.y < Play.dep())
//    {
//        Play.wider();
//    }
//    else Play.unWider();
//
//    if (mouse.x > Quit.left() && mouse.x < Quit.right() && mouse.y > Quit.top() && mouse.y < Quit.dep())
//    {
//        Quit.wider();
//    }
//    else Quit.unWider();
//}
//
//int getMouseClick(location& mouse, Text& Play, Text& Quit)
//{
//    if (mouse.x > Play.left() && mouse.x < Play.right() && mouse.y > Play.top() && mouse.y < Play.dep())
//    {
//        return 1;
//    }
//    else if (mouse.x > Quit.left() && mouse.x < Quit.right() && mouse.y > Quit.top() && mouse.y < Quit.dep())
//    {
//        return 0;
//    }
//    else return -1;
//}