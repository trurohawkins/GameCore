TARGET = game

LIBDIR = lib/
INCDIR = include/
SRCDIR = src/

RENDERER = TUI
RENDERDIR = ../$(RENDERER)/
RENDERLIB = $(RENDERDIR)lib/
RENDERINC = $(RENDERDIR)include/

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

CFLAGS = -MMD -MP -I$(HELPERINC) -I$(MOLTNINC) -I$(OIBINC) -I$(RENDERINC) -I$(INCDIR) -I$(SRCDIR)
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
$(TARGET): $(RENDERLIB)lib$(RENDERER).a $(RENDERINC)$(RENDERER).h $(INCDIR)GameCore.h $(LIBDIR)libGameCore.a $(OIBINC)OIB.h $(OIBLIB)libOIB.a $(MOLTNLIB)libMoltnCore.a $(MOLTNINC)MoltnCore.h $(HELPERLIB)libHelper.a  $(HELPERINC)helper.h main.o  
	gcc main.o -o $@ $(LDFLAGS) $(LIBDIR)libGameCore.a -L$(RENDERLIB) -l$(RENDERER) -L$(OIBLIB) -lOIB -L$(MOLTNLIB) -lMoltnCore -L$(HELPERLIB) -lHelper -lm

$(RENDERLIB)lib$(RENDERER).a:
	$(MAKE) -C $(RENDERDIR)

$(HELPERLIB)libHelper.a:
	$(MAKE) -C $(HELPERDIR)

$(MOLTNLIB)libMoltnCore.a:
	$(MAKE) -C $(MOLTNDIR)

$(OIBLIB)libOIB.a:
	$(MAKE) -C $(OIBDIR)

# Static lib
$(LIBDIR)libGameCore.a: game.o timeWizard.o player.o inputMap.o actor.o menu.o | $(LIBDIR)
	ar rs $@ $^

# Compiling
main.o: main.c menuControls.c
	gcc $(CFLAGS) -c main.c -o $@

#GAME
game.o: $(SRCDIR)game.c $(INCDIR)game.h
	gcc $(CFLAGS) -c $< -o $@

timeWizard.o: $(SRCDIR)timeWizard.c $(INCDIR)timeWizard.h
	gcc $(CFLAGS) -c $< -o $@

player.o: $(SRCDIR)player.c $(INCDIR)player.h $(SRCDIR)playerManager.c $(INCDIR)playerManager.h
	gcc $(CFLAGS) -c $< -o $@

inputMap.o: $(SRCDIR)inputMap.c $(INCDIR)inputMap.h
	gcc $(CFLAGS) -c $< -o $@

actor.o: $(SRCDIR)actor.c $(INCDIR)actor.h $(SRCDIR)actorList.c $(INCDIR)actorList.h
	gcc $(CFLAGS) -c $< -o $@

menu.o: $(SRCDIR)menu.c $(INCDIR)menu.h
	gcc $(CFLAGS) -c $< -o $@

$(LIBDIR):
	mkdir -p $(LIBDIR)

# tools
clean:
	rm -f *.o *.d

fclean:
	rm -f $(TARGET) *.o *.d $(LIBDIR)libGameCore.a 

# merges .d files into dependency graph
-include *.d
