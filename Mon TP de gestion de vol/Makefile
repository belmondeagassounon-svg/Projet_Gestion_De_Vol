CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
SRC      = main.cpp Agence.cpp Passager.cpp Vol.cpp Reservation.cpp Paiement.cpp
OBJ      = $(SRC:.cpp=.o)

agence: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) agence
