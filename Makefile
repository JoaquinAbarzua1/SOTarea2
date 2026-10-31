torneo: torneo.cpp
	g++ -Wall -Wextra -std=c++17 -lpthread -o torneo torneo.cpp

clean:
	rm -f torneo