TARGET = GameCore

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP
LDFLAGS =

dev: CFLAGS += $(DEV_CFLAGS)
dev: LDFLAGS += $(DEV_LDFLAGS)
dev: $(TARGET)

tsan: CFLAGS += $(TSAN_CFLAGS)
tsan: LDFLAGS += $(TSAN_LDFLAGS)
tsan: $(TARGET)

prod: CFLAGS += $(PROD_CFLAGS)
prod: LDFLAGS += $(PROD_LDFLAGS)
prod: $(TARGET)

RENDERDIR = ../OIB/

# Linking
$(TARGET): GameCore.h libGame.a OIB.h libOIB.a libMoltnCore.a libHelper.a  main.o  
	gcc main.o -o $@ $(LDFLAGS) libGame.a libOIB.a libMoltnCore.a libHelper.a -lm

libHelper.a:
	$(MAKE) -C ../FormNetwork/
	cp ../FormNetwork/libHelper.a .

libOIB.a:
	$(MAKE) -C $(RENDERDIR)
	cp $(RENDERDIR)libOIB.a .

OIB.h:
	$(MAKE) -C $(RENDERDIR)
	cp $(RENDERDIR)OIB.h .

libMoltnCore.a:
	$(MAKE) -C $(RENDERDIR)
	cp $(RENDERDIR)libMoltnCore.a .

GameCore.h: OIB.h 
	@echo "Generating core game header"
	@echo "#pragma once" > GameCore.h
	@cat OIB.h actor.h actorList.h inputMap.h player.h playerManager.h timeWizard.h game.h >> GameCore.h

# Static lib
libGame.a: game.o timeWizard.o player.o inputMap.o actor.o
	ar rs $@ $^

# Compiling
main.o: main.c
	gcc $(CFLAGS) -c main.c -o $@

#GAME
game.o: game.c game.h OIB.h
	gcc $(CFLAGS) -c game.c -o $@

timeWizard.o: timeWizard.c timeWizard.h
	gcc $(CFLAGS) -c timeWizard.c -o $@

player.o: player.c player.h playerManager.c playerManager.h
	gcc $(CFLAGS) -c player.c -o $@

inputMap.o: inputMap.c inputMap.h
	gcc $(CFLAGS) -c inputMap.c -o $@

actor.o: actor.c actor.h actorList.c actorList.h
	gcc $(CFLAGS) -c actor.c -o $@

# tools
clean:
	rm -f *.o *.a *.d

fclean:
	rm -f $(TARGET) *.o *.a *.d GameCore.h OIB.h

fixTerminal:
	stty sane

# merges .d files into dependency graph
-include *.d
