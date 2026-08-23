all:
	gcc -o Joc-Sah src/main.c src/board.c src/graphics.c src/piece_graphics.c src/mouse_interaction.c src/moves.c src/piece.c src/game.c src/ai.c src/promovare.c src/special.c src/mod_special.c $(shell sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lm

clean:
	rm -f Joc-Sah