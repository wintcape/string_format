TARGET := lib
APP := test

CC ?= gcc
CFLAGS := -g -O2 -W -Wall -Wshadow -Wextra -Werror -Werror=vla -Wno-unused-parameter
DEPENDENCIES := m
LIB_INCLUDE := src
APP_INCLUDE := test/src

################################################################################

LIB_OBJFILES := math.o memory.o logger.o string_utils.o string.o string_format.o array_utils.o array.o filesystem.o platform.o
APP_OBJFILES := test_main.o

################################################################################

# Lib objects.
obj/array.o:                            src/container/array.c
obj/string.o:                           src/container/string.c
obj/string_format.o:                    src/container/string/format.c
obj/array_utils.o:                      src/core/array.c
obj/logger.o:                           src/core/logger.c
obj/string_utils.o:                     src/core/string.c
obj/math.o:                             src/math/math.c
obj/filesystem.o:                       src/platform/filesystem.c
obj/memory.o:                           src/platform/memory.c

# App objects.
obj/test_main.o:                        test/src/main.c

################################################################################

# Lib objects.
obj\array.o:                            src\container\array.c
obj\string.o:                           src\container\string.c
obj\string_format.o:                    src\container\string\format.c
obj\array_utils.o:                      src\core\array.c
obj\logger.o:                           src\core\logger.c
obj\string_utils.o:                     src\core\string.c
obj\math.o:                             src\math\math.c
obj\filesystem.o:                       src\platform\filesystem.c
obj\memory.o:                           src\platform\memory.c

# App objects.
obj\test_main.o:                        test\src\main.c