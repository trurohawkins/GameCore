TARGET = GameCore

LIBDIR = lib/
INCDIR = include/

HELPERDIR = ../HelperFuncs/
HELPERINC = $(HELPERDIR)include/
HELPERLIB = $(HELPERDIR)lib/

MOLTNDIR = ../MOLTN/
MOLTNINC = $(MOLTNDIR)include/
MOLTNLIB = $(MOLTNDIR)lib/

OIBDIR = ../OIB/
OIBINC = $(OIBDIR)include/
OIBLIB = $(OIBDIR)lib/

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP -I$(HELPERINC) -I$(MOLTNINC) -I$(OIBINC) -I$(INCDIR)
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

# Linking
$(TARGET): $(INCDIR)GameCore.h $(LIBDIR)libGameCore.a $(OIBINC)OIB.h $(OIBLIB)libOIB.a $(MOLTNLIB)libMoltnCore.a $(MOLTNINC)MoltnCore.h $(HELPERLIB)libHelper.a  $(HELPERINC)helper.h main.o  
	gcc main.o -o $@ $(LDFLAGS) $(LIBDIR)libGameCore.a -L$(OIBLIB) -lOIB -L$(MOLTNLIB) -lMoltnCore -L$(HELPERLIB) -lHelper -lm

$(HELPERLIB)libHelper.a:
	$(MAKE) -C $(HELPERDIR)

$(MOLTNLIB)libMoltnCore.a:
	$(MAKE) -C $(MOLTNDIR)

$(OIBLIB)libOIB.a:
	$(MAKE) -C $(OIBDIR)

GameCore.h: OIB.h 
	@echo "Generating core game header"
	@echo "#pragma once" > GameCore.h
	@cat OIB.h actor.h actorList.h inputMap.h player.h playerManager.h timeWizard.h game.h >> GameCore.h

# Static lib
$(LIBDIR)libGameCore.a: game.o timeWizard.o player.o inputMap.o actor.o | $(LIBDIR)
	ar rs $@ $^

# Compiling
main.o: main.c
	gcc $(CFLAGS) -c main.c -o $@

#GAME
game.o: game.c $(INCDIR)game.h
	gcc $(CFLAGS) -c game.c -o $@

timeWizard.o: timeWizard.c $(INCDIR)timeWizard.h
	gcc $(CFLAGS) -c timeWizard.c -o $@

player.o: player.c $(INCDIR)player.h playerManager.c $(INCDIR)playerManager.h
	gcc $(CFLAGS) -c player.c -o $@

inputMap.o: inputMap.c $(INCDIR)inputMap.h
	gcc $(CFLAGS) -c inputMap.c -o $@

actor.o: actor.c $(INCDIR)actor.h actorList.c $(INCDIR)actorList.h
	gcc $(CFLAGS) -c actor.c -o $@

$(LIBDIR):
	mkdir -p $(LIBDIR)

# tools
clean:
	rm -f *.o *.d

fclean:
	rm -f $(TARGET) *.o *.d $(LIBDIR)libGameCore.a 

# merges .d files into dependency graph
-include *.d
