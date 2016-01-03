CC=g++-4.7
SRC=Editor/Editor.cc Editor/Layer.cc Editor/Map.cc Editor/MapData.cc Editor/Project.cc Editor/Settings.cc Editor/Tileset.cc Game.cc Misc/Logger.cc Misc/SDLException.cc Misc/SDLImage.cc Misc/Timer.cc Overworld/Camera.cc Overworld/Controller.cc Overworld/MapRenderer.cc Overworld/Moving.cc Overworld/OWSprite.cc Overworld/OWSpritePool.cc Overworld/OWSpriteRenderer.cc Overworld/PlayerSprite.cc Scene.cc Screen.cc Surface/AsyncIn.cc Surface/Button.cc Surface/ButtonRadio.cc Surface/ButtonToggle.cc Surface/FontManager.cc Surface/Geometry.cc Surface/HScrollBar.cc Surface/Image.cc Surface/ScrollBar.cc Surface/Scrollable.cc Surface/StatusBar.cc Surface/SubImage.cc Surface/Surface.cc Surface/VScrollBar.cc Surface/WindowHandler.cc Terminal.cc main.cc 
OBJ=Editor/Editor.cc.o Editor/Layer.cc.o Editor/Map.cc.o Editor/MapData.cc.o Editor/Project.cc.o Editor/Settings.cc.o Editor/Tileset.cc.o Game.cc.o Misc/Logger.cc.o Misc/SDLException.cc.o Misc/SDLImage.cc.o Misc/Timer.cc.o Overworld/Camera.cc.o Overworld/Controller.cc.o Overworld/MapRenderer.cc.o Overworld/Moving.cc.o Overworld/OWSprite.cc.o Overworld/OWSpritePool.cc.o Overworld/OWSpriteRenderer.cc.o Overworld/PlayerSprite.cc.o Scene.cc.o Screen.cc.o Surface/AsyncIn.cc.o Surface/Button.cc.o Surface/ButtonRadio.cc.o Surface/ButtonToggle.cc.o Surface/FontManager.cc.o Surface/Geometry.cc.o Surface/HScrollBar.cc.o Surface/Image.cc.o Surface/ScrollBar.cc.o Surface/Scrollable.cc.o Surface/StatusBar.cc.o Surface/SubImage.cc.o Surface/Surface.cc.o Surface/VScrollBar.cc.o Surface/WindowHandler.cc.o Terminal.cc.o main.cc.o 
INCDIRS=-I/home/dave/include
LIBDIRS=-L/home/dave/include/lib
TARGET=pkmn
SDLCFLAGS=$(shell sdl2-config --cflags)
SDLLIBS=$(shell sdl2-config --libs)
DAVLIBS=-ldav_NBT -ldav_gzstream
LIBS=$(DAVLIBS) $(SDLLIBS) -lSDL2_image -lSDL2_ttf -lpthread -lz
CFLAGS=-std=gnu++11 -Wall -O0 -ggdb -DDEBUG $(SDLCFLAGS)

.PHONY: logs clean

all: logs $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBDIRS) -Wl,--start-group $(LIBS) -Wl,--end-group

Editor/Editor.cc.o: Editor/Editor.cc Editor/Editor.h Editor/Layer.h Editor/Map.h Editor/MapData.h Editor/Project.h Editor/Settings.h Editor/Tileset.h Editor/auto_ptr.h Misc/Logger.h Misc/Timer.h Selector.h Surface/Application.h Surface/AsyncIn.h Surface/Button.h Surface/ButtonRadio.h Surface/ButtonToggle.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/HScrollBar.h Surface/Image.h Surface/ScrollBar.h Surface/Scrollable.h Surface/StatusBar.h Surface/SubImage.h Surface/Surface.h Surface/VScrollBar.h Surface/Window.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Editor/Editor.cc -o Editor/Editor.cc.o

Editor/Layer.cc.o: Editor/Layer.cc Editor/Layer.h Editor/MapData.h Editor/Settings.h Editor/auto_ptr.h Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/SubImage.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Editor/Layer.cc -o Editor/Layer.cc.o

Editor/Map.cc.o: Editor/Map.cc Editor/Layer.h Editor/Map.h Editor/MapData.h Editor/Settings.h Editor/auto_ptr.h Misc/Logger.h Selector.h Surface/Button.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/HScrollBar.h Surface/Image.h Surface/ScrollBar.h Surface/Scrollable.h Surface/SubImage.h Surface/Surface.h Surface/VScrollBar.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Editor/Map.cc -o Editor/Map.cc.o

Editor/MapData.cc.o: Editor/MapData.cc Editor/MapData.h Editor/Settings.h Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Editor/MapData.cc -o Editor/MapData.cc.o

Editor/Project.cc.o: Editor/Project.cc Editor/MapData.h Editor/Project.h Editor/Settings.h Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Editor/Project.cc -o Editor/Project.cc.o

Editor/Settings.cc.o: Editor/Settings.cc Editor/Settings.h Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Editor/Settings.cc -o Editor/Settings.cc.o

Editor/Tileset.cc.o: Editor/Tileset.cc Editor/Settings.h Editor/Tileset.h Misc/Logger.h Surface/Button.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/HScrollBar.h Surface/Image.h Surface/ScrollBar.h Surface/Scrollable.h Surface/SubImage.h Surface/Surface.h Surface/VScrollBar.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Editor/Tileset.cc -o Editor/Tileset.cc.o

Game.cc.o: Game.cc Display.h Game.h Misc/Logger.h Misc/SDLException.h Misc/Timer.h Overworld/Camera.h Overworld/Controller.h Overworld/Map.h Overworld/Moving.h Overworld/OWSprite.h Overworld/PlayerSprite.h Scene.h Screen.h Terminal.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Game.cc -o Game.cc.o

Misc/Logger.cc.o: Misc/Logger.cc Misc/Logger.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Misc/Logger.cc -o Misc/Logger.cc.o

Misc/SDLException.cc.o: Misc/SDLException.cc Misc/SDLException.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Misc/SDLException.cc -o Misc/SDLException.cc.o

Misc/SDLImage.cc.o: Misc/SDLImage.cc Misc/SDLException.h Misc/SDLImage.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Misc/SDLImage.cc -o Misc/SDLImage.cc.o

Misc/Timer.cc.o: Misc/Timer.cc Misc/Timer.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Misc/Timer.cc -o Misc/Timer.cc.o

Overworld/Camera.cc.o: Overworld/Camera.cc Overworld/Camera.h Overworld/Moving.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Overworld/Camera.cc -o Overworld/Camera.cc.o

Overworld/Controller.cc.o: Overworld/Controller.cc Display.h Misc/Logger.h Misc/SDLException.h Misc/SDLImage.h Overworld/Camera.h Overworld/Controller.h Overworld/Map.h Overworld/MapRenderer.h Overworld/Moving.h Overworld/OWSprite.h Overworld/OWSpriteRenderer.h Overworld/PlayerSprite.h Pool.h Screen.h Singleton.h Terminal.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Overworld/Controller.cc -o Overworld/Controller.cc.o

Overworld/MapRenderer.cc.o: Overworld/MapRenderer.cc Display.h Misc/Logger.h Misc/SDLException.h Misc/SDLImage.h Misc/Timer.h Overworld/Map.h Overworld/MapRenderer.h Pool.h Screen.h Singleton.h Terminal.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Overworld/MapRenderer.cc -o Overworld/MapRenderer.cc.o

Overworld/Moving.cc.o: Overworld/Moving.cc Overworld/Moving.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Overworld/Moving.cc -o Overworld/Moving.cc.o

Overworld/OWSprite.cc.o: Overworld/OWSprite.cc Overworld/Moving.h Overworld/OWSprite.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Overworld/OWSprite.cc -o Overworld/OWSprite.cc.o

Overworld/OWSpritePool.cc.o: Overworld/OWSpritePool.cc Display.h Misc/SDLException.h Misc/SDLImage.h Overworld/OWSpritePool.h Pool.h Screen.h Singleton.h Terminal.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Overworld/OWSpritePool.cc -o Overworld/OWSpritePool.cc.o

Overworld/OWSpriteRenderer.cc.o: Overworld/OWSpriteRenderer.cc Display.h Misc/Logger.h Misc/SDLException.h Misc/SDLImage.h Overworld/Moving.h Overworld/OWSprite.h Overworld/OWSpritePool.h Overworld/OWSpriteRenderer.h Pool.h Screen.h Singleton.h Terminal.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Overworld/OWSpriteRenderer.cc -o Overworld/OWSpriteRenderer.cc.o

Overworld/PlayerSprite.cc.o: Overworld/PlayerSprite.cc Overworld/Moving.h Overworld/OWSprite.h Overworld/PlayerSprite.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Overworld/PlayerSprite.cc -o Overworld/PlayerSprite.cc.o

Scene.cc.o: Scene.cc Display.h Misc/Timer.h Scene.h Terminal.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Scene.cc -o Scene.cc.o

Screen.cc.o: Screen.cc Display.h Misc/SDLException.h Screen.h Terminal.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Screen.cc -o Screen.cc.o

Surface/AsyncIn.cc.o: Surface/AsyncIn.cc Surface/AsyncIn.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/AsyncIn.cc -o Surface/AsyncIn.cc.o

Surface/Button.cc.o: Surface/Button.cc Misc/Logger.h Surface/Button.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/SubImage.h Surface/Surface.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/Button.cc -o Surface/Button.cc.o

Surface/ButtonRadio.cc.o: Surface/ButtonRadio.cc Misc/Logger.h Surface/Button.h Surface/ButtonRadio.h Surface/ButtonToggle.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/SubImage.h Surface/Surface.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/ButtonRadio.cc -o Surface/ButtonRadio.cc.o

Surface/ButtonToggle.cc.o: Surface/ButtonToggle.cc Misc/Logger.h Surface/Button.h Surface/ButtonToggle.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/SubImage.h Surface/Surface.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/ButtonToggle.cc -o Surface/ButtonToggle.cc.o

Surface/FontManager.cc.o: Surface/FontManager.cc Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/FontManager.cc -o Surface/FontManager.cc.o

Surface/Geometry.cc.o: Surface/Geometry.cc Misc/Logger.h Surface/Exception.h Surface/Geometry.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/Geometry.cc -o Surface/Geometry.cc.o

Surface/HScrollBar.cc.o: Surface/HScrollBar.cc Misc/Logger.h Surface/Button.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/HScrollBar.h Surface/Image.h Surface/ScrollBar.h Surface/SubImage.h Surface/Surface.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/HScrollBar.cc -o Surface/HScrollBar.cc.o

Surface/Image.cc.o: Surface/Image.cc Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/Image.cc -o Surface/Image.cc.o

Surface/ScrollBar.cc.o: Surface/ScrollBar.cc Misc/Logger.h Surface/Button.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/ScrollBar.h Surface/SubImage.h Surface/Surface.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/ScrollBar.cc -o Surface/ScrollBar.cc.o

Surface/Scrollable.cc.o: Surface/Scrollable.cc Misc/Logger.h Surface/Button.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/HScrollBar.h Surface/Image.h Surface/ScrollBar.h Surface/Scrollable.h Surface/SubImage.h Surface/Surface.h Surface/VScrollBar.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/Scrollable.cc -o Surface/Scrollable.cc.o

Surface/StatusBar.cc.o: Surface/StatusBar.cc Misc/Logger.h Surface/Button.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/StatusBar.h Surface/SubImage.h Surface/Surface.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/StatusBar.cc -o Surface/StatusBar.cc.o

Surface/SubImage.cc.o: Surface/SubImage.cc Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/SubImage.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/SubImage.cc -o Surface/SubImage.cc.o

Surface/Surface.cc.o: Surface/Surface.cc Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/SubImage.h Surface/Surface.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/Surface.cc -o Surface/Surface.cc.o

Surface/VScrollBar.cc.o: Surface/VScrollBar.cc Misc/Logger.h Surface/Button.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/ScrollBar.h Surface/SubImage.h Surface/Surface.h Surface/VScrollBar.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/VScrollBar.cc -o Surface/VScrollBar.cc.o

Surface/WindowHandler.cc.o: Surface/WindowHandler.cc Misc/Logger.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/Image.h Surface/SubImage.h Surface/WindowHandler.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Surface/WindowHandler.cc -o Surface/WindowHandler.cc.o

Terminal.cc.o: Terminal.cc Display.h Terminal.h
	$(CC) -c $(CFLAGS) $(INCDIRS) Terminal.cc -o Terminal.cc.o

main.cc.o: main.cc Display.h Editor/Editor.h Editor/Layer.h Editor/Map.h Editor/MapData.h Editor/Project.h Editor/Settings.h Editor/Tileset.h Editor/auto_ptr.h Game.h Misc/Logger.h Misc/SDLException.h Misc/Timer.h Overworld/Camera.h Overworld/Controller.h Overworld/Map.h Overworld/Moving.h Overworld/OWSprite.h Overworld/PlayerSprite.h Scene.h Screen.h Selector.h Surface/Application.h Surface/AsyncIn.h Surface/Button.h Surface/ButtonRadio.h Surface/ButtonToggle.h Surface/Exception.h Surface/FontManager.h Surface/Geometry.h Surface/HScrollBar.h Surface/Image.h Surface/ScrollBar.h Surface/Scrollable.h Surface/StatusBar.h Surface/SubImage.h Surface/Surface.h Surface/VScrollBar.h Surface/Window.h Surface/WindowHandler.h Terminal.h common.h
	$(CC) -c $(CFLAGS) $(INCDIRS) main.cc -o main.cc.o

logs:
	if [ -d "logs" ]; then for LF in *.log; do if [ -f "$$LF" ]; then mv $$LF logs/; fi; done; fi

clean:
	rm -f $(OBJ) $(TARGET)
