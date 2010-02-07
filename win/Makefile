#
# DISCLAIMER
#

CC=mingw32-gcc
CFLAGS=-Wall -DUNICODE -D_UNICODE
OUT_DIR=bin
SRC_DIR=src
LIB=$(MINGW_PATH)\lib\libpsapi.a $(MINGW_PATH)\lib\libgdi32.a
TARGET=${OUT_DIR}\nocnnic.exe

_OBJ = main.o
OBJ = $(patsubst %,$(OUT_DIR)/%,$(_OBJ))

$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -Wl,-subsystem,windows -o $@ $< $(LIB)

.PHONY: clean

clean:
	rm $(TARGET)
	rm $(OUT_DIR)/*.o

all: $(TARGET)

# END OF FILE
