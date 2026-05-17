#pragma once
#include <algorithm>
#include <string>

#include "bagel.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <box2d/box2d.h>

namespace invaders {
    using namespace bagel;

    /**
     * @brief Game Settings and Constants.
     *
     * Contains all the global configurations for the game, such as
     * sprite coordinates, physics scaling, player attributes, and HUD layouts.
     */
    namespace gs
    {
        inline constexpr const char* SPRITE_SHEET_PATH = "res/alternate_space_invaders_sprite_sheet.png";
        constexpr float TEX_SCALE = 0.25f;
        constexpr int SPRITESHEET_TEXTURE_ID = -1;

        constexpr float BOX_SCALE = 10.f;

        constexpr float PLAYER_SPRITE_X = 1.f;
        constexpr float PLAYER_SPRITE_Y = 20.f;
        constexpr float PLAYER_SPRITE_W = 13.f;
        constexpr float PLAYER_SPRITE_H = 8.f;
        constexpr float PLAYER_SPRITE_HALF_W = PLAYER_SPRITE_W * 0.5f;
        constexpr float PLAYER_SPRITE_HALF_H = PLAYER_SPRITE_H * 0.5f;

        constexpr float PLAYER_REF_WIN_W = 800.f;
        constexpr float PLAYER_WIDTH_TO_SCREEN = 0.10f;
        constexpr float PLAYER_DRAW_W = PLAYER_REF_WIN_W * PLAYER_WIDTH_TO_SCREEN;
        constexpr float PLAYER_DRAW_H = PLAYER_DRAW_W * (PLAYER_SPRITE_H / PLAYER_SPRITE_W);
        constexpr float PLAYER_DRAW_HALF_W = PLAYER_DRAW_W * 0.5f;
        constexpr float PLAYER_DRAW_HALF_H = PLAYER_DRAW_H * 0.5f;
        constexpr int PLAYER_INITIAL_HP = 3;
        constexpr float PLAYER_BODY_DENSITY = 1.f;
        constexpr int PLAYER_INVULNERABLE_FRAMES = 120;

        constexpr float PLAYER_DESTRUCTION_1_SPRITE_X = 17.f;
        constexpr float PLAYER_DESTRUCTION_1_SPRITE_Y = 22.f;
        constexpr float PLAYER_DESTRUCTION_1_SPRITE_W = 13.f;
        constexpr float PLAYER_DESTRUCTION_1_SPRITE_H = 6.f;

        constexpr float PLAYER_DESTRUCTION_2_SPRITE_X = 33.f;
        constexpr float PLAYER_DESTRUCTION_2_SPRITE_Y = 20.f;
        constexpr float PLAYER_DESTRUCTION_2_SPRITE_W = 13.f;
        constexpr float PLAYER_DESTRUCTION_2_SPRITE_H = 8.f;

        constexpr float PLAYER_DESTRUCTION_3_SPRITE_X = 49.f;
        constexpr float PLAYER_DESTRUCTION_3_SPRITE_Y = 20.f;
        constexpr float PLAYER_DESTRUCTION_3_SPRITE_W = 13.f;
        constexpr float PLAYER_DESTRUCTION_3_SPRITE_H = 8.f;
        constexpr int PLAYER_DESTRUCTION_FRAMES_TO_NEXT_STAGE = 40;

        constexpr float ALIEN_SPRITE_X = 4.f;
        constexpr float ALIEN_SPRITE_Y = 52.f;
        constexpr float ALIEN_SPRITE_W = 8.f;
        constexpr float ALIEN_SPRITE_H = 8.f;

        constexpr float ALIEN_DESTRUCTION_SPRITE_X = 129.f;
        constexpr float ALIEN_DESTRUCTION_SPRITE_Y = 52.f;
        constexpr float ALIEN_DESTRUCTION_SPRITE_W = 14.f;
        constexpr float ALIEN_DESTRUCTION_SPRITE_H = 8.f;
        constexpr int ALIEN_DESTRUCTION_FRAMES_TO_NEXT_STAGE = 15;

        constexpr float ALIEN_VELOCITY_X = 1.0f;
        constexpr float ALIEN_VELOCITY_Y = 0.f;
        constexpr float ALIEN_AI_TIME_TO_MOVE = 1.0f;
        constexpr int ALIEN_AI_INITIAL_DIRECTION = 1;

        constexpr int ALIEN_ROWS = 4;
        constexpr int ALIEN_COLS = 8;
        constexpr float ALIEN_GRID_START_X = 100.f;
        constexpr float ALIEN_GRID_START_Y = 60.f;
        constexpr float ALIEN_GRID_SPACING_X = 60.f;
        constexpr float ALIEN_GRID_SPACING_Y = 50.f;

        constexpr float BUNKER_SPRITE_X = 32.f;
        constexpr float BUNKER_SPRITE_Y = 112.f;
        constexpr float BUNKER_SPRITE_W = 16.f;
        constexpr float BUNKER_SPRITE_H = 8.f;
        constexpr int BUNKER_INITIAL_HP = 10;

        constexpr float BULLET_SPRITE_X = 15.f;
        constexpr float BULLET_SPRITE_Y = 80.f;
        constexpr float BULLET_SPRITE_W = 3.f;
        constexpr float BULLET_SPRITE_H = 8.f;
        constexpr int BULLET_DAMAGE = 1;

        constexpr float EXPLOSION_SPRITE_X = 33.f;
        constexpr float EXPLOSION_SPRITE_Y = 20.f;
        constexpr float EXPLOSION_SPRITE_W = 13.f;
        constexpr float EXPLOSION_SPRITE_H = 8.f;
        constexpr float EXPLOSION_LIFETIME_SEC = 0.5f;

        inline constexpr const char* HUD_SPRITE_SHEET_PATH = "res/hud_sprite_sheet.png";
        inline constexpr const char* YOU_WIN_PATH = "res/you win sheet.jpg";
        inline constexpr int HUD_SHEET_W = 2196;
        inline constexpr int HUD_SHEET_H = 1952;

        inline constexpr SDL_Rect HUD_SRC_LIVES{ 26, 367, 365, 133 };
        inline constexpr SDL_Rect HUD_SRC_GAME_OVER{ 28, 1053, 1132, 129 };
        inline constexpr SDL_Rect HUD_SRC_START_GAME{ 29, 1310, 2011, 108 };
        inline constexpr SDL_Rect HUD_SRC_YOU_WIN{ 0, 0, 1000, 200 };
        inline constexpr SDL_Rect HUD_SRC_HEARTS_3{ 7, 450 + 200 * 10 / 100, 562, 200 - 200 * 10 / 100 };
        inline constexpr SDL_Rect HUD_SRC_HEARTS_2{ 10, 712, 362, 168 };
        inline constexpr SDL_Rect HUD_SRC_HEARTS_1{ 626, 712, 174, 168 };
        inline constexpr SDL_Rect HUD_SRC_HEARTS_0{ 384, 712, 175, 168 };

        constexpr int GAME_OVER_TTL_FRAMES = 200;
        constexpr float HUD_CORNER_PADDING = 16.f;
        constexpr float HUD_HEARTS_MAX_DRAW_W = 120.f;
        constexpr float HUD_TITLE_MAX_DRAW_W_FRAC = 0.92f;
    }

    /** @brief Represents the position and rotation of an entity in the world. */
    struct Transform { SDL_FPoint p; float a; };

    /** @brief Represents the linear movement speed of an entity. */
    struct VelocityComponent { float dx{ 0 }, dy{ 0 }; };

    /** @brief Holds the texture source rectangle and destination drawing size. */
    struct Drawable { SDL_FRect part; SDL_FPoint size; };

    /** @brief Links the entity to its Box2D physics body and collision shape. */
    struct ColliderComponent { b2BodyId body{}; b2ShapeId shape{}; };

    /** @brief Maps SDL Scancodes for player input controls. */
    struct KeysComponent { SDL_Scancode left, right; };

    /** @brief Stores the current intended actions (movement/shooting) based on input. */
    struct IntentComponent { bool left, right, isShooting; };

    /** @brief Manages weapon cooldown to limit the firing rate. */
    struct WeaponComponent { int cooldown{ 0 }; };

    /** @brief Tag component identifying a player's bullet. */
    struct BulletComponent {};

    /** @brief Tag component identifying an alien's bullet. */
    struct AlienBulletComponent {};

    /** @brief Tracks the remaining lives of the player. */
    struct LivesComponent { int lives{ 3 }; };

    /** @brief Handles multi-stage destruction animations for entities. */
    struct DestructionComponent { int currentDestructionStage{ 0 }, totalDestructionStages{ 3 }, framesToNextStage{ gs::PLAYER_DESTRUCTION_FRAMES_TO_NEXT_STAGE }; };

    /** @brief Grants temporary invulnerability frames to an entity after being hit. */
    struct InvulnerableComponent { int ttl; };

    /**
     * @brief Manages the global state of the game UI/HUD.
     *
     * States: 0 = Start Screen, 1 = Playing, 2 = Game Over, 3 = Victory.
     */
    struct GameStateComponent {
        int state{ 0 };
        int ttl{ 0 };
        int gameOverTtl{ gs::GAME_OVER_TTL_FRAMES };
    };

    /** @brief Controls the swarm movement logic and direction for alien entities. */
    struct AlienAIComponent {
        float timeToMove{ 1.0f };
        int direction{ 1 };
    };

    /**
     * @brief Creates the player entity and registers it to the ECS.
     *
     * @param world The Box2D physics world identifier.
     * @param x The starting X position of the player.
     * @param y The starting Y position of the player.
     * @return Entity The newly created player entity.
     */
    Entity createPlayer(b2WorldId world, float x, float y);

    /**
     * @brief Creates an alien enemy entity.
     *
     * @param world The Box2D physics world.
     * @param x The starting X position.
     * @param y The starting Y position.
     * @return Entity The constructed alien entity.
     */
    Entity createAlien(b2WorldId world, float x, float y);

    /**
     * @brief Creates a projectile fired by the player.
     *
     * @param world The Box2D physics world.
     * @param x The X position where the bullet spawns.
     * @param y The Y position where the bullet spawns.
     * @param dy The vertical velocity of the bullet.
     * @return Entity The constructed player bullet entity.
     */
    Entity createBullet(b2WorldId world, float x, float y, float dy);

    /**
     * @brief Creates a projectile fired by an alien enemy.
     *
     * @param world The Box2D physics world.
     * @param x The X position where the bullet spawns.
     * @param y The Y position where the bullet spawns.
     * @param dy The vertical velocity of the bullet.
     * @return Entity The constructed alien bullet entity.
     */
    Entity createAlienBullet(b2WorldId world, float x, float y, float dy);

    /**
     * @brief Core class managing the Space Invaders game loop and resources.
     */
    class SpaceInvaders
    {
    public:
        /**
         * @brief Initializes the game engine, physics, and assets.
         */
        SpaceInvaders();

        /**
         * @brief Safely cleans up all resources upon game destruction.
         */
        ~SpaceInvaders();

        /**
         * @brief Main game loop executing input, logic, and rendering systems.
         * @return void
         */
        void run();

        /**
         * @brief Checks if the Box2D physics world was successfully created.
         * @return bool True if valid, false otherwise.
         */
        bool valid() const { return b2World_IsValid(box); }

    private:
        static constexpr int    WIN_W = 800;
        static constexpr int    WIN_H = 600;

        static constexpr int    FPS = 60;
        static constexpr Uint64 GAME_FRAME = 1000/FPS;
        static constexpr float  RAD_TO_DEG = 57.2958f;

        /**
         * @brief Spawns the initial layout of the player and alien grid.
         * @return void
         */
        void setup_entities_for_new_game();

        /**
         * @brief Applies horizontal movement to entities based on intent.
         * @return void
         */
        void movement_system();

        /**
         * @brief Renders all in-game entities to the screen.
         * @return void
         */
        void draw_system();

        /**
         * @brief Renders the Heads-Up Display (HUD) elements.
         * @return void
         */
        void draw_hud_system();

        /**
         * @brief Steps the Box2D physics engine and updates visual transforms.
         * @return void
         */
        void box_system();

        /**
         * @brief Processes keyboard input and updates Intent components.
         * @return void
         */
        void input_system();

        /**
         * @brief Manages the grid movement of the alien swarm.
         * @return void
         */
        void alien_ai_system();

        /**
         * @brief Processes collision events between game entities.
         * @return void
         */
        void collision_system();

        /**
         * @brief Handles player shooting logic and bullet spawning.
         * @return void
         */
        void shooting_system();

        /**
         * @brief Handles random alien shooting logic.
         * @return void
         */
        void alien_shooting_system();

        /**
         * @brief Removes entities that have moved out of the screen boundaries.
         * @return void
         */
        void cleanup_system();

        /**
         * @brief Animates and completes the destruction of alien entities.
         * @return void
         */
        void alien_destruction_system();

        /**
         * @brief Verifies if the win condition has been met (no aliens left).
         * @return void
         */
        void check_win_system();

        /**
         * @brief Transitions the game into the Game Over state.
         * @return void
         */
        void enter_game_over();

        /**
         * @brief Transitions the game into the Victory state.
         * @return void
         */
        void enter_victory();

        /**
         * @brief Handles player hit animations, life reduction, and game over.
         * @return void
         */
        void player_destruction_system();

        /**
         * @brief Manages the player's temporary invulnerability frames.
         * @return void
         */
        void invulnerability_system();

        SDL_Texture* tex = nullptr;
        SDL_Texture* hud_tex = nullptr;
        SDL_Texture* win_tex = nullptr;
        SDL_Renderer* ren = nullptr;
        SDL_Window* win = nullptr;
        b2WorldId         box = b2_nullWorldId;

        Entity HudEntity = {{0}};
    };
}

template <> struct bagel::Storage<invaders::Transform> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::Transform>; };
template <> struct bagel::Storage<invaders::VelocityComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::VelocityComponent>; };
template <> struct bagel::Storage<invaders::Drawable> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::Drawable>; };
template <> struct bagel::Storage<invaders::ColliderComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::ColliderComponent>; };
template <> struct bagel::Storage<invaders::KeysComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::KeysComponent>; };
template <> struct bagel::Storage<invaders::IntentComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::IntentComponent>; };
template <> struct bagel::Storage<invaders::AlienAIComponent> final : bagel::NoInstance { using type = bagel::SparseStorage<invaders::AlienAIComponent>; };
template <> struct bagel::Storage<invaders::WeaponComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::WeaponComponent>; };
template <> struct bagel::Storage<invaders::BulletComponent> final : bagel::NoInstance { using type = bagel::TaggedStorage<invaders::BulletComponent>; };
template <> struct bagel::Storage<invaders::AlienBulletComponent> final : bagel::NoInstance { using type = bagel::TaggedStorage<invaders::AlienBulletComponent>; };
template <> struct bagel::Storage<invaders::LivesComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::LivesComponent>; };
template <> struct bagel::Storage<invaders::DestructionComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::DestructionComponent>; };
template <> struct bagel::Storage<invaders::InvulnerableComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::InvulnerableComponent>; };
template <> struct bagel::Storage<invaders::GameStateComponent> final : bagel::NoInstance { using type = bagel::PackedStorage<invaders::GameStateComponent>; };