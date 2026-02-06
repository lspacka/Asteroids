build:
	gcc asteroids.c -o asteroids -lraylib -lopengl32 -lgdi32 -lwinmm

run:
	./asteroids

clean:
	rm asteroids

chk:
	make build run