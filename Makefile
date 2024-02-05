# Minimal makefile
# FOR SFML PROJECTS

PROGRAM = prog
FILES.cpp = main.cpp Controlling.cpp
FILES.h = 
FILES.o = $(FILES.cpp:.cpp=.o)

CC 		= g++
LDFLAGS	= -lsfml-graphics -lsfml-window -lsfml-system -L
LDLIBS		= /home/mmark/.local/lib/SFML-2.5.1/lib
INCLUDE	= /home/mmark/.local/lib/SFML-2.5.1/include 
INCFLAGS	= -I

all: $(PROGRAM)

$(PROGRAM): $(FILES.o)
	$(CC) $(FILES.o) -o $@ $(LDFLAGS) $(LDLIBS)

.cpp.o:
	$(CC) -c $< -o $@ $(INCFLAGS) $(INCLUDE)

clean:
	rm -f $(PROGRAM)
	rm -f $(FILES.o)
