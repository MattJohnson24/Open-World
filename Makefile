all:
	g++ -Isrc/include -Lsrc/lib -o main main.cpp Game.cpp TextureManager.cpp GameObject.cpp Map.cpp Vector2D.cpp Collision.cpp src/ECS/ECS.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image