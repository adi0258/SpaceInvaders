#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

using namespace std;

class SpaceInvadersMockup {
public:
    int run() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            cout << SDL_GetError() << endl;
            return -1;
        }

        SDL_Window* win;
        SDL_Renderer* ren;

        if (!SDL_CreateWindowAndRenderer("Space Invaders Mockup", 800, 600, 0, &win, &ren)) {
            cout << SDL_GetError() << endl;
            return -1;
        }

        SDL_Texture* rocketTex = IMG_LoadTexture(ren, "res/rocket.png");
        SDL_Texture* alienTex = IMG_LoadTexture(ren, "res/alien.png");

        if (!rocketTex || !alienTex) {
            cout << "Failed to load textures: " << SDL_GetError() << endl;
        }

        if (rocketTex) SDL_SetTextureScaleMode(rocketTex, SDL_SCALEMODE_NEAREST);
        if (alienTex) SDL_SetTextureScaleMode(alienTex, SDL_SCALEMODE_NEAREST);

        SDL_FRect player = { 400 - 16, 500, 32, 48 };
        float player_dir_x = 3.0f;

        SDL_FRect aliens[15];
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 5; ++col) {
                aliens[row * 5 + col] = { 100.f + col * 70.f, 50.f + row * 60.f, 48.f, 48.f };
            }
        }
        float alien_dir_x = 1.0f;
        int alien_steps = 0;

        SDL_FRect bullet = { player.x + 14, player.y, 4, 12 };
        bool bullet_active = true;

        bool quit = false;
        while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)) {
                    quit = true;
                }
            }

            player.x += player_dir_x;
            if (player.x < 50 || player.x > 750 - player.w) player_dir_x *= -1;

            if (bullet_active) {
                bullet.y -= 7.0f;
                if (bullet.y < 0) {
                    bullet.y = player.y;
                    bullet.x = player.x + 14;
                }
            }

            alien_steps++;
            bool move_down = false;
            if (alien_steps > 150) {
                alien_dir_x *= -1;
                alien_steps = 0;
                move_down = true;
            }

            for (int i = 0; i < 15; ++i) {
                aliens[i].x += alien_dir_x;
                if (move_down) aliens[i].y += 10.0f;
            }

            SDL_SetRenderDrawColor(ren, 10, 10, 20, 255); // רקע חלל
            SDL_RenderClear(ren);

            if (rocketTex) {
                SDL_RenderTexture(ren, rocketTex, NULL, &player);
            } else {
                SDL_SetRenderDrawColor(ren, 50, 200, 255, 255);
                SDL_RenderFillRect(ren, &player);
            }

            SDL_SetRenderDrawColor(ren, 255, 50, 50, 255);
            SDL_RenderFillRect(ren, &bullet);

            if (alienTex) {
                for (int i = 0; i < 15; ++i) {
                    SDL_RenderTexture(ren, alienTex, NULL, &aliens[i]);
                }
            } else {
                SDL_SetRenderDrawColor(ren, 50, 255, 50, 255);
                for (int i = 0; i < 15; ++i) {
                    SDL_RenderFillRect(ren, &aliens[i]);
                }
            }

            SDL_RenderPresent(ren);
            SDL_Delay(16);
        }

        if (rocketTex) SDL_DestroyTexture(rocketTex);
        if (alienTex) SDL_DestroyTexture(alienTex);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 0;
    }
};

int main() {
    SpaceInvadersMockup mockup;
    return mockup.run();
}