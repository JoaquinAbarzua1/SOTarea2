all: torneo

torneo:
	# ir agrgando archivos nuevos para compilar
	g++ -std=c++17 -Wall -Wextra -pthread codigos/main.cpp -o torneo

clean:
	rm -f torneo