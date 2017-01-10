# ══════════════════════════════════════════════════════════════════════════════
# ──────────────────────────────────────────────────────────────────────────────
#				CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
# ══════════════════════════════════════════════════════════════════════════════
#
#
## MISC
# ══════════════════════════════════════════════════════════════════════════════
# Development Mode
# ──────────────────────────────────────────────────────────────────────────────
# toggles utility flags, make targets
DEVELOPMENT_MODE = T

# path to MySQL library dependency libmysqlclient.{dylib,so,lib} (leave empty if unsure where located)
LIBMYSQLCLIENT	:= 

# Phony Targets
# ──────────────────────────────────────────────────────────────────────────────
.PHONY: all clean


# String Utils
# ──────────────────────────────────────────────────────────────────────────────
EMPTY	       :=
SPACE	       := $(EMPTY) $(EMPTY)
TAB	       := $(EMPTY)	$(EMPTY)
TRIM		= $(subst $(SPACE),$(EMPTY),$1)
JOIN	        = $(subst $(SPACE),$2,$(strip $1))
define NEWLINE :=


endef

# SYSTEM CONFIG
# ══════════════════════════════════════════════════════════════════════════════
# extended from user 'Trevor Robinson''s response to stackoverflow question:
#
# 	http://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS),Windows_NT)
        SYSTEM_WINDOWS 	:= T
        SYS_ENV_FLAGS 	:= -DWIN32

        ifeq      (AMD64,$(PROCESSOR_ARCHITECTURE))
                ARCH_X86_64	:= T
                SYS_ARCH_FLAG	:= -arch x86_64
                SYS_ENV_FLAGS	+= -DAMD_64

        else ifeq (x86,$(PROCESSOR_ARCHITECTURE))
                ARCH_I386	:= T
                SYS_ARCH_FLAG	:= -arch i386
                SYS_ENV_FLAGS	+= -DIA_32
        endif
else
        UNAME_S := $(shell uname -s)

        ifeq      ($(UNAME_S),Linux)
                SYSTEM_LINUX 	:= T
                SYS_ENV_FLAGS 	:= -DLINUX

        else ifeq ($(UNAME_S),Darwin)
                SYSTEM_OSX 	:= T
                SYS_ENV_FLAGS 	:= -DOSX
        endif

        UNAME_M := $(shell uname -m)

        ifeq      (x86_64,$(UNAME_M))
                ARCH_X86_64	:= T
                SYS_ARCH_FLAG	:= -arch x86_64
                SYS_ENV_FLAGS	+= -DAMD_64

        else ifneq (,$(filter %86,$(UNAME_M)))
                ARCH_I386	:= T
                SYS_ARCH_FLAG	:= -arch i386
                SYS_ENV_FLAGS 	+= -DIA_32

        else ifneq (,$(filter arm%,$(UNAME_M)))

                ifneq (,$(filter %32%,$(UNAME_M)))
                        ARCH_ARM_32	:= T
                        SYS_ARCH_FLAG	:= -arch AArch32
                        SYS_ENV_FLAGS 	+= -DARM_32
                else
                        ARCH_ARM_64	:= T
                        SYS_ARCH_FLAG	:= -arch AArch64
                        SYS_ENV_FLAGS 	+= -DARM_64
                endif
        endif
endif


# Path delimiter, chain commands token, executable file extension
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
PATH_DELIM	:= \\
CMD_DELIM 	:= &
BIN_EXT		:= .exe
else
PATH_DELIM	:= /
CMD_DELIM 	:= ;
BIN_EXT		:= $(EMPTY)
endif



# DIRECTORY CONFIG
# ══════════════════════════════════════════════════════════════════════════════
PATH_JOIN		 = $(call JOIN,$1,$(PATH_DELIM))
PROJECT_ROOT		:= ..
SOURCE_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) src)
HEADER_DIR		:= $(SOURCE_DIR)
OBJECT_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) obj)
BINARY_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) bin)

# BOARD CONFIG
# ══════════════════════════════════════════════════════════════════════════════
BOARD_FILENAME	:= board.txt
BOARD_PATH	:= $(call TRIM,"\"$(realpath $(call PATH_JOIN,$(PROJECT_ROOT) $(BOARD_FILENAME)))\"")

# PATH CONFIG
# ══════════════════════════════════════════════════════════════════════════════
FILE_PATH		= $(call PATH_JOIN,$1 $(call TRIM,$2))
SOURCE_FILE_PATH	= $(call FILE_PATH,$1,$2.c)
HEADER_FILE_PATH	= $(call FILE_PATH,$1,$2.h)
OBJECT_FILE_PATH	= $(call FILE_PATH,$1,$2.o)
BINARY_FILE_PATH	= $(call FILE_PATH,$1,$2$(BIN_EXT))

# Project
# ─────────────── source ───────────────────────────────────────────────────────
SOURCE_PATH		= $(call SOURCE_FILE_PATH,$(SOURCE_DIR),$(call PATH_JOIN,$1))# implementation
HEADER_PATH		= $(call HEADER_FILE_PATH,$(HEADER_DIR),$(call PATH_JOIN,$1))# interface
# ─────────────── build ────────────────────────────────────────────────────────
OBJECT_PATH   		= $(call OBJECT_FILE_PATH,$(OBJECT_DIR),$(call JOIN,$1,_))# object files
BINARY_PATH 		= $(call BINARY_FILE_PATH,$(BINARY_DIR),$(call JOIN,$1,_))# binary executable output



# UTILITY CONFIG
# ══════════════════════════════════════════════════════════════════════════════
# C Compiler
# ──────────────────────────────────────────────────────────────────────────────
CC		:= gcc
CC_ENV_FLAGS	:= -D__USE_FIXED_PROTOTYPES__ -D_GNU_SOURCE $(SYS_ENV_FLAGS)
CC_BASE_FLAGS	:= -std=gnu99 -march=native $(CC_ENV_FLAGS) -I$(HEADER_DIR)

ifeq (T,$(DEVELOPMENT_MODE))
CC_FLAGS	:= -g -Wall $(CC_BASE_FLAGS)
else
CC_FLAGS	:= -O2 -funroll-loops $(CC_BASE_FLAGS)
endif

# Linker
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_OSX))
LD		:= ld
LD_FLAGS	:= -macosx_version_min 10.11 -no_pie $(SYS_ARCH_FLAG)
LD_LIB_FLAGS	:= -lc
else
LD		:= gcc
LD_FLAGS	:= $(EMPTY)
LD_LIB_FLAGS	:= $(EMPTY)
endif

# Cleaner
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
RM		:= cmd \/C del
RM_FLAGS	:= /F /Q
else
RM		:= rm
RM_FLAGS	:= -rf
endif



# COMPILATION MODULES
# ══════════════════════════════════════════════════════════════════════════════
# independent headers
# ──────────────────────────────────────────────────────────────────────────────
UTILS_UTILS_HDR	:= $(call HEADER_PATH,utils utils)


# string/string_utils
# ──────────────────────────────────────────────────────────────────────────────
STRING_STRING_UTILS_SRC 	:= $(call SOURCE_PATH,string string_utils)
STRING_STRING_UTILS_HDR		:= $(call HEADER_PATH,string string_utils)
STRING_STRING_UTILS_OBJ 	:= $(call OBJECT_PATH,string string_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
STRING_STRING_UTILS_OBJ_PREQS	:= $(STRING_STRING_UTILS_SRC) 	\
			   	   $(STRING_STRING_UTILS_HDR)
STRING_STRING_UTILS_OBJ_GROUP	:= $(STRING_STRING_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(STRING_STRING_UTILS_OBJ)



# file/file_utils
# ──────────────────────────────────────────────────────────────────────────────
FILE_FILE_UTILS_SRC 		:= $(call SOURCE_PATH,file file_utils)
FILE_FILE_UTILS_HDR		:= $(call HEADER_PATH,file file_utils)
FILE_FILE_UTILS_OBJ 		:= $(call OBJECT_PATH,file file_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
FILE_FILE_UTILS_OBJ_PREQS	:= $(FILE_FILE_UTILS_SRC) 	\
			   	   $(FILE_FILE_UTILS_HDR)	\
			   	   $(UTILS_UTILS_HDR)
FILE_FILE_UTILS_OBJ_GROUP	:= $(FILE_FILE_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(FILE_FILE_UTILS_OBJ)


# sudoku (entry point)
# ──────────────────────────────────────────────────────────────────────────────
SUDOKU_SRC 		:= $(call SOURCE_PATH,sudoku)
SUDOKU_HDR		:= $(call HEADER_PATH,sudoku)
SUDOKU_OBJ 		:= $(call OBJECT_PATH,sudoku)
SUDOKU_BIN 		:= $(call BINARY_PATH,sudoku)
# ─────────────── target prequisites ───────────────────────────────────────────
SUDOKU_BIN_PREQS	:= $(SUDOKU_OBJ)			\
			   $(FILE_FILE_UTILS_OBJ_GROUP)		\
			   $(STRING_STRING_UTILS_OBJ_GROUP)
SUDOKU_OBJ_PREQS	:= $(SUDOKU_SRC) 			\
			   $(SUDOKU_HDR)			\
			   $(FILE_FILE_UTILS_HDR)		\
			   $(STRING_STRING_UTILS_HDR)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(SUDOKU_OBJ)			\
			   $(SUDOKU_BIN)



# ══════════════════════════════════════════════════════════════════════════════
# ──────────────────────────────────────────────────────────────────────────────
# 				MAKE RULES
# ──────────────────────────────────────────────────────────────────────────────
# ══════════════════════════════════════════════════════════════════════════════
#
#
## COMPOSITE RULES
# ══════════════════════════════════════════════════════════════════════════════
# make all targets
# ──────────────────────────────────────────────────────────────────────────────
all:  $(TARGETS)

# remove all targets
# ──────────────────────────────────────────────────────────────────────────────
clean:
	$(RM) $(RM_FLAGS) $(TARGETS)

## MAKE PROJECT
# ══════════════════════════════════════════════════════════════════════════════
# make sudoku binary
# ──────────────────────────────────────────────────────────────────────────────
$(SUDOKU_BIN): $(SUDOKU_BIN_PREQS)
	$(LD) $(LD_FLAGS) $^ $(LD_LIB_FLAGS) -o $@

# make sudoku object
# ──────────────────────────────────────────────────────────────────────────────
$(SUDOKU_OBJ): $(SUDOKU_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -DBOARD_PATH=$(BOARD_PATH) -c $< -o $@

# make file/file_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(FILE_FILE_UTILS_OBJ): $(FILE_FILE_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make string/string_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(STRING_STRING_UTILS_OBJ): $(STRING_STRING_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@
