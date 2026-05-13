#include "SpaceInvaders.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
using namespace std;

#include "bagel.h"
using namespace bagel;


namespace invaders {
    Entity createPlayer(b2WorldId world, float x, float y) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_kinematicBody;
        bodyDef.position = {
            (x + gs::PLAYER_DRAW_HALF_W) / gs::BOX_SCALE,
            (y + gs::PLAYER_DRAW_HALF_H) / gs::BOX_SCALE };
        b2BodyId playerBody = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = gs::PLAYER_BODY_DENSITY;
        b2Polygon poly = b2MakeBox(
            gs::PLAYER_DRAW_HALF_W / gs::BOX_SCALE,
            gs::PLAYER_DRAW_HALF_H / gs::BOX_SCALE);
        b2CreatePolygonShape(playerBody, &shapeDef, &poly);

        Entity player = Entity::create();
        player.addAll<
            KeysComponent,
            IntentComponent,
            ColliderComponent,
            Drawable,
            VelocityComponent,
            Transform>(
            KeysComponent{ SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT },
            IntentComponent{ false, false, false },
            ColliderComponent{ playerBody },
            Drawable{
                { gs::PLAYER_SPRITE_X, gs::PLAYER_SPRITE_Y, gs::PLAYER_SPRITE_W, gs::PLAYER_SPRITE_H },
                { gs::PLAYER_DRAW_W, gs::PLAYER_DRAW_H } },
            VelocityComponent{},
            Transform{
                SDL_FPoint{ x + gs::PLAYER_DRAW_HALF_W, y + gs::PLAYER_DRAW_HALF_H },
                0.f });
        return player;
    }

    // ent_type createAlien(float x, float y) {
    //     ent_type e = World::createEntity();
    //     //World::addComponent(e, PositionComponent{ x, y });
    //     World::addComponent(e, VelocityComponent{ gs::ALIEN_VELOCITY_X, gs::ALIEN_VELOCITY_Y });
    //     World::addComponent(e, RenderComponent{
    //         { gs::ALIEN_SPRITE_X, gs::ALIEN_SPRITE_Y, gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H },
    //         { x, y, gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H },
    //         gs::SPRITESHEET_TEXTURE_ID });
    //     //World::addComponent(e, ColliderComponent{ gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H });
    //     World::addComponent(e, AlienAIComponent{ gs::ALIEN_AI_TIME_TO_MOVE, gs::ALIEN_AI_INITIAL_DIRECTION });
    //     return e;
    // }

    // ent_type createBunker(float x, float y) {
    //     ent_type e = World::createEntity();
    //     //World::addComponent(e, PositionComponent{ x, y });
    //     World::addComponent(e, RenderComponent{
    //         { gs::BUNKER_SPRITE_X, gs::BUNKER_SPRITE_Y, gs::BUNKER_SPRITE_W, gs::BUNKER_SPRITE_H },
    //         { x, y, gs::BUNKER_SPRITE_W, gs::BUNKER_SPRITE_H },
    //         gs::SPRITESHEET_TEXTURE_ID });
    //     //World::addComponent(e, ColliderComponent{ gs::BUNKER_SPRITE_W, gs::BUNKER_SPRITE_H });
    //     //World::addComponent(e, HealthComponent{ gs::BUNKER_INITIAL_HP });
    //     return e;
    // }

    // ent_type createBullet(float x, float y, float dy) {
    //     ent_type e = World::createEntity();
    //     //World::addComponent(e, PositionComponent{ x, y });
    //     World::addComponent(e, VelocityComponent{ 0, dy });
    //     World::addComponent(e, RenderComponent{
    //         { gs::BULLET_SPRITE_X, gs::BULLET_SPRITE_Y, gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H },
    //         { x, y, gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H },
    //         gs::SPRITESHEET_TEXTURE_ID });
    //     //World::addComponent(e, ColliderComponent{ gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H });
    //     //World::addComponent(e, DamageComponent{ gs::BULLET_DAMAGE });
    //     return e;
    // }

    // ent_type createExplosion(float x, float y) {
    //     ent_type e = World::createEntity();
    //     //World::addComponent(e, PositionComponent{ x, y });
    //     World::addComponent(e, RenderComponent{
    //         { gs::EXPLOSION_SPRITE_X, gs::EXPLOSION_SPRITE_Y, gs::EXPLOSION_SPRITE_W, gs::EXPLOSION_SPRITE_H },
    //         { x, y, gs::EXPLOSION_SPRITE_W, gs::EXPLOSION_SPRITE_H },
    //         gs::SPRITESHEET_TEXTURE_ID });
    //     //World::addComponent(e, LifetimeComponent{ gs::EXPLOSION_LIFETIME_SEC });
    //     return e;
    // }

    void SpaceInvaders::draw_system()
	{
		static const Mask mask = MaskBuilder()
			.set<Transform>()
			.set<Drawable>()
			.build();

		for (Entity e = Entity::first(); !e.eof(); e.next()) {
			if (e.test(mask)) {
				const auto& t = e.get<Transform>();
				const auto& d = e.get<Drawable>();

				SDL_FRect dest = {
					t.p.x-d.size.x/2,
					t.p.y-d.size.y/2,
					d.size.x, d.size.y };

				SDL_RenderTextureRotated(
					ren, tex, &d.part, &dest, t.a,
					nullptr, SDL_FLIP_NONE);
			}
		}
	}

    void SpaceInvaders::input_system()
	{
		static const Mask mask = MaskBuilder()
			.set<KeysComponent>()
			.set<IntentComponent>()
			.build();

		SDL_PumpEvents();
		const bool* keys = SDL_GetKeyboardState(nullptr);

		for (Entity e = Entity::first(); !e.eof(); e.next()) {
			if (e.test(mask)) {
				const auto& k = e.get<KeysComponent>();
				auto& i = e.get<IntentComponent>();

				i.left = keys[k.left];
				i.right = keys[k.right];
				i.isShooting = keys[SDL_SCANCODE_SPACE];
			}
		}
	}

    void SpaceInvaders::movement_system()
	{
		static const Mask mask = MaskBuilder()
			.set<IntentComponent>()
			.set<ColliderComponent>()
			.build();

		for (Entity e = Entity::first(); !e.eof(); e.next()) {
			if (e.test(mask)) {
				const auto& i = e.get<IntentComponent>();
				const auto& c = e.get<ColliderComponent>();

				const float f = i.left ? -30 : i.right ? 30 : 0;
				b2Body_SetLinearVelocity(c.body, {f,0});
			}
		}
	}

    void SpaceInvaders::box_system()
	{
		static constexpr float BOX_STEP = 1.f/FPS;
		static const Mask mask = MaskBuilder()
			.set<Transform>()
			.set<ColliderComponent>()
			.build();

		b2World_Step(box, BOX_STEP, 4);

		for (Entity e = Entity::first(); !e.eof(); e.next()) {
			if (e.test(mask)) {
				const auto t = b2Body_GetTransform(e.get<ColliderComponent>().body);
				auto& tr = e.get<Transform>();
				tr.p.x = t.p.x * gs::BOX_SCALE;
				tr.p.y = t.p.y * gs::BOX_SCALE;
				tr.a = RAD_TO_DEG * b2Rot_GetAngle(t.q);
			}
		}
	}

    void SpaceInvaders::run()
	{
		auto start = SDL_GetTicks();
		bool quit = false;

		while (!quit) {
			// update all systems (except draw)
			input_system();
			movement_system();
			box_system();

			SDL_RenderClear(ren);
			draw_system();
			SDL_RenderPresent(ren);

			const auto end = SDL_GetTicks();
			if (end-start < GAME_FRAME) {
				SDL_Delay(GAME_FRAME - (end-start));
			}
			start += GAME_FRAME;

			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				if ((e.type==SDL_EVENT_QUIT) ||
					((e.type == SDL_EVENT_KEY_DOWN) && (e.key.scancode == SDL_SCANCODE_ESCAPE))) {
					quit = true;
				}
			}
		}
	}

    SpaceInvaders::SpaceInvaders()
    {
        constexpr float WIN_H_MID = WIN_H/2.f;
        constexpr float WIN_W_MID = WIN_W/2.f;

        // initialize SDL for video (drawing to screen)
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            cout << SDL_GetError() << endl;
            return;
        }

        // create game window
        if (!SDL_CreateWindowAndRenderer(
            "Space Invaders!", WIN_W, WIN_H, 0, &win, &ren)) {
            cout << SDL_GetError() << endl;
            return;
            }

        // load spritesheet image
        SDL_Surface *surf = IMG_Load(gs::SPRITE_SHEET_PATH);
        if (surf == nullptr) {
            cout << SDL_GetError() << endl;
            return;
        }

        // create texture from spritesheet and destroy orig image
        tex = SDL_CreateTextureFromSurface(ren, surf);
        if (tex == nullptr) {
            cout << SDL_GetError() << endl;
            return;
        }
        SDL_DestroySurface(surf);

        // seed randomizer and set default clear color
        SDL_srand(time(nullptr));
        SDL_SetRenderDrawColor(ren, 0,0,0,255);

        // create Box2D world
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = {0,0};
        box = b2CreateWorld(&worldDef);
        if (!b2World_IsValid(box)) {
            cout << "Failed creating Box2D world" << endl;
            return;
        }

        // create left & right walls (no entities - they just block movement)
        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.type = b2_staticBody;
        wallDef.position = {-1, WIN_H_MID/gs::BOX_SCALE};
        // left wall
        b2ShapeDef wallShapeDef = b2DefaultShapeDef();
        wallShapeDef.density = 1;
        b2Polygon wallPoly = b2MakeBox(1, WIN_H_MID/gs::BOX_SCALE);
        b2BodyId wallBody = b2CreateBody(box, &wallDef);
        b2CreatePolygonShape(wallBody, &wallShapeDef, &wallPoly);
        // right wall
        wallDef.position.x = WIN_W/gs::BOX_SCALE + 1;
        wallBody = b2CreateBody(box, &wallDef);
        b2CreatePolygonShape(wallBody, &wallShapeDef, &wallPoly);

        createPlayer(box,
            WIN_W_MID - gs::PLAYER_DRAW_HALF_W,
            WIN_H - gs::PLAYER_DRAW_H);

    }

    SpaceInvaders::~SpaceInvaders()
    {
        if (b2World_IsValid(box))
            b2DestroyWorld(box);
        if (tex != nullptr)
            SDL_DestroyTexture(tex);
        if (ren != nullptr)
            SDL_DestroyRenderer(ren);
        if (win != nullptr)
            SDL_DestroyWindow(win);

        SDL_Quit();
    }

}