#include "stdafx.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "Character.h"
#include "ImpTimer.h"
#include "text.h"
#include"PlayerMoney.h"

BaseObject g_background;
BaseObject g_start_menu;
BaseObject g_game_over;

bool InitData() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    // Set render scale quality
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Create window
    g_window = SDL_CreateWindow("TRAVERLER RUN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    // Create renderer
    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_image
    int img_flags = IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_image could not initialize: %s", IMG_GetError());
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf could not initialize: %s", TTF_GetError());
        return false;
    }

    // Load font
    font_1 = TTF_OpenFont("font//dlxfont_.ttf", 20);
	font_2 = TTF_OpenFont("font//VCENTBI.TTF", 40);
    if (font_1 == NULL || font_2 == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font: %s", TTF_GetError());
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer could not initialize: %s", Mix_GetError());
        return false;
    }

    // Load music
    bg_music = Mix_LoadMUS("music/bgmusic.mp3");
    if (bg_music == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load bgmusic: %s", Mix_GetError());
        return false;
    }

     //Load sound effects (if needed)
     money = Mix_LoadWAV("music///money.wav");
	 die = Mix_LoadWAV("music//die.wav");
	 explosion = Mix_LoadWAV("music/explosion.wav");
	 win = Mix_LoadWAV("music/win.wav");
	 if (money == NULL || die == NULL || explosion == NULL || win == NULL) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect: %s", Mix_GetError());
         return false;
     }
    return true;
}

bool LoadBackground(){
	bool ret1 = g_background.LoadImg("img//background.png", g_screen);
	bool ret2 = g_start_menu.LoadImg("img//menu.png", g_screen);
    bool ret3 = g_game_over.LoadImg("img//game_over.png", g_screen);
	if (ret1 == false || ret2 == false || ret3 == false)
	{
        return false;
	}
    return true;
}

void close() {
    // Free resources
    g_background.Free();
	g_start_menu.Free();
    g_game_over.Free();

    Mix_FreeChunk(money);
    Mix_FreeChunk(explosion);
    Mix_FreeChunk(die);
	Mix_FreeMusic(bg_music);
    TTF_CloseFont(font_1);

    // Destroy window and renderer
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);

    // Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void GameOver()
{
	Mix_HaltMusic();
    SDL_RenderClear(g_screen);
    g_game_over.Render(g_screen, NULL);
    SDL_RenderPresent(g_screen);
    Mix_PlayChannel(-1, die, 0);
    SDL_Delay(3000);
    close();
}

int main(int argc, char* argv[]){
	// Initialize data
	if (!InitData()) {
		std::cerr << "Failed to initialize data!" << std::endl;
		return 1;
	}

	// Load background and play background music
	if (!LoadBackground()) {
		std::cerr << "Failed to load background!" << std::endl;
		return 1;
	}
	
	// Load game map and tiles
	GameMap game_map;
	game_map.LoadMap("map//map.dat");
	game_map.LoadTiles(g_screen);

	// Load character image and set clips
	Character character;
	character.LoadImg("img//walk_right.png", g_screen);
	character.set_clips();

	// Initialize player money
	PlayerMoney player_money;
	player_money.Init(g_screen);

	// Set up time and money text
	Text time_game;
	Text money_game;
	time_game.SetColor(Text::RED_TEXT);
	money_game.SetColor(Text::BLACK_TEXT);
	
	Text text_menu[NUM_OF_MENU_OPTIONS];
	bool is_selected[NUM_OF_MENU_OPTIONS] = { 0 };

	bool is_playing = false;
    bool is_quit = false;
	bool is_quit_menu = false;

	// Set up menu options
	const char* menu_option_text[NUM_OF_MENU_OPTIONS] = {"PLAY GAME", "EXIT", "HOW TO PLAY"};
	for (int i = 0; i < NUM_OF_MENU_OPTIONS; i++)
	{
		text_menu[i].SetColor(Text::WHITE_TEXT);
	}
	

	 // Menu game
    while (!is_quit_menu)
    {
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen); //clear lại màn

        g_start_menu.Render(g_screen, NULL); //fill ảnh vào màn

		for (int i = 0; i < NUM_OF_MENU_OPTIONS; i++)
		{
			text_menu[i].SetText(menu_option_text[i]);
			text_menu[i].LoadFromRenderText(font_2, g_screen);
			text_menu[i].SetRect(200, 350 + i*100);
			text_menu[i].RenderText(g_screen, text_menu[i].GetRectFrame().x, text_menu[i].GetRectFrame().y);
		}

		bool is_showing_msbox = false;
		 while (SDL_PollEvent(&g_event) != 0){
            switch (g_event.type){
            case SDL_QUIT:
            {
                is_quit_menu = true;
                is_quit = true;
            }
            break;
            case SDL_MOUSEMOTION:
            {
                int xm = g_event.motion.x;
                int ym = g_event.motion.y;
				for (int i = 0; i < NUM_OF_MENU_OPTIONS; i++)
                {
					if (CommonFunc::CheckFocusWithRect(xm, ym, text_menu[i].GetRectFrame()))
                    {
                        if (is_selected[i] == false)
                        {
                            is_selected[i] = true;
                            text_menu[i].SetColor(Text::RED_TEXT);
                        }
                    }
                    else
					{
                        if (is_selected[i] == true)
                        {
                            is_selected[i] = false;
                            text_menu[i].SetColor(Text::WHITE_TEXT);
                        }
                    }
                }
            }
            break;

		
            case SDL_MOUSEBUTTONDOWN:
            {
                int xm = g_event.button.x;
                int ym = g_event.button.y;
				
				for (int i = 0; i < NUM_OF_MENU_OPTIONS; i++)
                {
					if (CommonFunc::CheckFocusWithRect(xm, ym, text_menu[i].GetRectFrame()))
                    {
                        if (i == 1)
							{
							is_quit = true;
							is_quit_menu = true;
							}

						else if (i==0) 
						{
							is_quit_menu = true;
						}
							
						else{
							// Set up message box with game instructions
							const char* messageTitle = "HOW TO PLAY";
							const char* messageText = "Use the left and right arrow keys to move your character, and the up arrow key or SPACE to jump.\n\nEvery time you touch a coin, you get +1 point. If you touch a bomb, you lose 10 points.\n\nYou lose when you fall into a pit, time's up, or your score is less than 0.\n\nOnce you finish the map and collect the treasure, you win and the game ends.";
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, messageTitle, messageText, NULL);
							is_showing_msbox = true;
						}
                    }
                }
            }
            break;
			  case SDL_KEYDOWN:
                if (g_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    is_quit_menu = true;
                    is_quit = true;
                }
            break;

            default:
				break;
			}
        }
        SDL_RenderPresent(g_screen);
    }
	
	Mix_PlayMusic(bg_music, -1);

	// Set up game loop
	ImpTimer fps_timer;
	Uint32 menu_time = SDL_GetTicks() / 1000;

	while (!is_quit) {
	  fps_timer.start();

	  // Handle events
	  while (SDL_PollEvent(&g_event) != 0) {
		if (g_event.type == SDL_QUIT) {
		  is_quit = true;
		}

		character.HandleInputAction(g_event, g_screen);
	  }

	  // Clear screen
	  SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
	  SDL_RenderClear(g_screen);

	  // Render background
	  g_background.Render(g_screen, NULL);

	  // Render game map
	  Map map_data = game_map.getMap();

	  character.SetMapXY(map_data.start_x_, map_data.start_y_);
	  character.DoPlayer(map_data);
	  character.Show(g_screen);

	  game_map.SetMap(map_data);
	  game_map.DrawMap(g_screen);

	  // Render player money
	  player_money.Show(g_screen);

	  // Render game time
	  std::string str_time = "Time: ";
	  Uint32 time_val = SDL_GetTicks() / 1000;  // Game time
	  Uint32 val_time = 150 - time_val + menu_time; // Remaining time

	  if (val_time <= 0 || character.getY() > map_data.max_y_ || character.GetMoneyCount() < 0) {
		GameOver();
	  } else {
		std::string str_val = std::to_string(val_time);
		str_time += str_val;

		time_game.SetText(str_time);
		time_game.LoadFromRenderText(font_1, g_screen);
		time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 30);
	  }

	  // Render player score
	  int money_count = character.GetMoneyCount();
	  std::string money_str = std::to_string(money_count);

	  money_game.SetText(money_str);
	  money_game.LoadFromRenderText(font_1, g_screen);
	  money_game.RenderText(g_screen, 70, 30);

	  // Update screen
	  SDL_RenderPresent(g_screen);

	  // Delay to maintain constant FPS
	  int real_imp_time = fps_timer.get_ticks();
	  int time_one_frame = 1000 / FRAME_PER_SECOND; // ms

	  if (real_imp_time < time_one_frame) {
		int delay_time = time_one_frame - real_imp_time;
		SDL_Delay(delay_time);
	  }
	}

	// Clean up and exit
	close();
	return 0;
}
