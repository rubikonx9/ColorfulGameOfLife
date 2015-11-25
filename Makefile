include_dirs = -I../gl3w/include/ -I../boost/ -I../glfw/include/
libs_dirs    = -L../gl3w/lib/ -L../glfw/lib-mingw/
libs         = -lglfw -lgl3w -lopengl32

DEFAULT: life

life: main.o init.o GL/program.o GL/shader.o
	g++ main.o init.o GL/program.o GL/shader.o ${libs_dirs} ${libs} -o life

main.o: main.cpp
	g++ -c main.cpp ${include_dirs} -o main.o

init.o: init.cpp
	g++ -c init.cpp ${include_dirs} -o init.o

GL/program.o: GL/Program.cpp
	g++ -c GL/Program.cpp ${include_dirs} -o GL/program.o

GL/shader.o: GL/Shader.cpp
	g++ -c GL/Shader.cpp ${include_dirs} -o GL/shader.o

clean:
	rm -f main.o init.o GL/program.o GL/shader.o life

.PHONY:
	clean
