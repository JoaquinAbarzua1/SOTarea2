all:
	g++ -std=c++17 -Wall -Wextra -pthread codigos/main.cpp codigos/parseo.cpp codigos/tablero.cpp codigos/jugadores.cpp codigos/perro.cpp codigos/puma.cpp codigos/mesa.cpp -o torneo

clean:
	rm -f torneo *.log