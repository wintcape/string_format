include build/Makefile

################################################################################

TARGET := $(TARGET)-macos
obj/platform.o: src/platform/macos.m

################################################################################

LIB_INCFLAGS := $(foreach x,$(LIB_INCLUDE), $(addprefix -I,$(x)))
LIB_OBJFLAGS := $(CFLAGS) -c
LIB_LDFLAGS := $(foreach x,$(DEPENDENCIES), $(addprefix -l,$(x)))
LIB_OBJ := $(foreach x,$(LIB_OBJFILES), $(addprefix obj/,$(x)))

APP_INCFLAGS := $(LIB_INCFLAGS) $(foreach x,$(APP_INCLUDE), $(addprefix -I,$(x)))
APP_OBJFLAGS := $(CFLAGS) -c
APP_LDFLAGS := -Llib -l$(TARGET) $(LIB_LDFLAGS)
APP_OBJ := $(foreach x,$(APP_OBJFILES), $(addprefix obj/,$(x)))

lib/lib$(TARGET).a: $(LIB_OBJ)
	ar rcs $@ $^

bin/$(APP): $(APP_OBJ)
	$(CC) $(APP_INCFLAGS) -o $@ $^ $(CFLAGS) $(APP_LDFLAGS)

$(LIB_OBJ):
	$(CC) $(LIB_INCFLAGS) $(LIB_OBJFLAGS) -o $@ $<
$(APP_OBJ):
	$(CC) $(APP_INCFLAGS) $(APP_OBJFLAGS) -o $@ $<

.PHONY: lib
lib: mkdir lib-clean lib/lib$(TARGET).a

.PHONY: app-only
app-only: mkdir app-clean bin/$(APP) run-app

.PHONY: app
app: lib app-only

.PHONY: all
all: lib app-only

.PHONY: mkdir
mkdir:
	@mkdir -p bin
	@mkdir -p lib
	@mkdir -p obj

LIB_CLEAN := lib/$(TARGET).lib $(LIB_OBJ)
APP_CLEAN := bin/$(APP) $(APP_OBJ)

.PHONY: lib-clean
lib-clean:
	@echo rm -rf $(LIB_CLEAN)
	@rm -rf $(LIB_CLEAN) 2>/dev/null

.PHONY: app-clean
app-clean:
	@echo rm -rf $(APP_CLEAN)
	@rm -rf $(APP_CLEAN) 2>/dev/null

.PHONY: run-app
run-app:
	@bin/$(APP)