
BINS := hello_world

all: $(BINS)


hello_world:
	g++ -g src/shader.cpp src/object.cpp hello_world.cpp ../external/glad/src/glad.c  -Iinclude -I../external/glad/include -o $@ -lglfw -ldl

clean:
	rm -rf $(BINS)