#include "SpaceInvaders.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
using namespace std;

#include "bagel.h"
using namespace bagel;


namespace invaders {
    Entity createPlayer(b2WorldId world, float x, float y) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
    	bodyDef.type = b2_dynamicBody;

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
		Transform,
		WeaponComponent>(
		KeysComponent{ SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT },
		IntentComponent{ false, false, false },
		ColliderComponent{ playerBody },
		Drawable{
			{ gs::PLAYER_SPRITE_X, gs::PLAYER_SPRITE_Y, gs::PLAYER_SPRITE_W, gs::PLAYER_SPRITE_H },
			{ gs::PLAYER_DRAW_W, gs::PLAYER_DRAW_H } },
		VelocityComponent{},
		Transform{
			SDL_FPoint{ x + gs::PLAYER_DRAW_HALF_W, y + gs::PLAYER_DRAW_HALF_H },
			0.f },
		WeaponComponent{ 0 });
        return player;
    }


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

	Entity createAlien(b2WorldId world, float x, float y) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody; // גוף קינמטי שלא מושפע מכבידה
    bodyDef.position = { x / gs::BOX_SCALE, y / gs::BOX_SCALE };
    b2BodyId body = b2CreateBody(world, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2Polygon poly = b2MakeBox((gs::ALIEN_SPRITE_W * 2.f) / gs::BOX_SCALE, (gs::ALIEN_SPRITE_H * 2.f) / gs::BOX_SCALE);
    b2CreatePolygonShape(body, &shapeDef, &poly);

    Entity alien = Entity::create();
    alien.addAll<ColliderComponent, Drawable, VelocityComponent, Transform, AlienAIComponent>(
        ColliderComponent{ body },
        Drawable{
            { gs::ALIEN_SPRITE_X, gs::ALIEN_SPRITE_Y, gs::ALIEN_SPRITE_W, gs::ALIEN_SPRITE_H },
            { gs::ALIEN_SPRITE_W * 4.f, gs::ALIEN_SPRITE_H * 4.f } // הגדלנו פי 4 לציור
        },
        VelocityComponent{ gs::ALIEN_VELOCITY_X, gs::ALIEN_VELOCITY_Y },
        Transform{ SDL_FPoint{x, y}, 0.f },
        AlienAIComponent{ gs::ALIEN_AI_TIME_TO_MOVE, gs::ALIEN_AI_INITIAL_DIRECTION }
    );

    // נותנים לחייזר מהירות התחלתית
    b2Body_SetLinearVelocity(body, { gs::ALIEN_VELOCITY_X * 10.f, gs::ALIEN_VELOCITY_Y });
    return alien;
}

Entity createBullet(b2WorldId world, float x, float y, float dy) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    bodyDef.position = { x / gs::BOX_SCALE, y / gs::BOX_SCALE };
    b2BodyId body = b2CreateBody(world, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = true; // הקליע הוא סנסור, הוא לא ידחוף פיזית שחקנים/חייזרים
    b2Polygon poly = b2MakeBox((gs::BULLET_SPRITE_W) / gs::BOX_SCALE, (gs::BULLET_SPRITE_H) / gs::BOX_SCALE);
    b2CreatePolygonShape(body, &shapeDef, &poly);

    Entity bullet = Entity::create();
    bullet.addAll<ColliderComponent, Drawable, Transform>(
        ColliderComponent{ body },
        Drawable{
            { gs::BULLET_SPRITE_X, gs::BULLET_SPRITE_Y, gs::BULLET_SPRITE_W, gs::BULLET_SPRITE_H },
            { gs::BULLET_SPRITE_W * 2.f, gs::BULLET_SPRITE_H * 2.f }
        },
        Transform{ SDL_FPoint{x, y}, 0.f }
    );

    // קובעים את מהירות הקליע כלפי מעלה
    b2Body_SetLinearVelocity(body, { 0.f, dy });
    return bullet;
}

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

	void SpaceInvaders::shooting_system() {
    	static const Mask mask = MaskBuilder()
			.set<IntentComponent>()
			.set<Transform>()
			.set<WeaponComponent>()
			.build();

    	for (Entity e = Entity::first(); !e.eof(); e.next()) {
    		if (e.test(mask)) {
    			const auto& intent = e.get<IntentComponent>();
    			const auto& t = e.get<Transform>();
    			auto& weapon = e.get<WeaponComponent>();

    			if (weapon.cooldown > 0) {
    				weapon.cooldown--;
    			}
    			else if (intent.isShooting) {
    				// יוצרים קליע בדיוק מעל השחקן
    				createBullet(box, t.p.x, t.p.y - 20.f, -40.f);
    				weapon.cooldown = 20; // מגדירים Cooldown לכ-1/3 שנייה (ב-60 FPS)
    			}
    		}
    	}
    }

	void SpaceInvaders::alien_ai_system() {
    	static const Mask mask = MaskBuilder()
			.set<Transform>()
			.set<ColliderComponent>()
			.set<AlienAIComponent>()
			.build();

    	for (Entity e = Entity::first(); !e.eof(); e.next()) {
    		if (e.test(mask)) {
    			auto& ai = e.get<AlienAIComponent>();
    			const auto& t = e.get<Transform>();
    			const auto& c = e.get<ColliderComponent>();

    			// היפוך כיוון פשוט כשהחייזר מגיע לקצוות המסך
    			if (t.p.x < 50.f) ai.direction = 1;
    			if (t.p.x > WIN_W - 50.f) ai.direction = -1;

    			b2Body_SetLinearVelocity(c.body, { ai.direction * 15.f, 0.f });
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
    		shooting_system();
    		alien_ai_system();
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

    	createAlien(box, WIN_W_MID, 100.f);

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