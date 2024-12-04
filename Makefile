
# ***************************** CONFIGURATION ******************************** #

#	Executable

NAME 			:=	wordle


#	Directories

SRC_DIR			:=	source
INC_DIR			:=	source
BUILD_DIR		:=	build
OBJ_DIR			:=	$(BUILD_DIR)/_obj
DEP_DIR			:=	$(BUILD_DIR)/_dep


#	Dependencies

LIBRARIES		:=	readline ncursesw
BUILDFILES		:=	Makefile


#	Flags

CC 				?=	cc
CC_VERSION		:=	$(shell $(CC) --version | head -1)
CFLAGS_STD		:=	-Wall -Wextra -Werror
CFLAGS_DBG		:=	-ggdb3
CFLAGS_SAN		:=	-fsanitize=address,undefined,bounds,float-divide-by-zero
CFLAGS_OPT		:=	-O3
CFLAGS 			?=	$(CFLAGS_STD) $(CFLAGS_DBG)
CPPFLAGS 		:=	$(addprefix -I,$(INC_DIR))
DEPFLAGS		=	-M -MP -MF $@ -MT "$(OBJ_DIR)/$*.o $@"
LDLIBS			:=	$(addprefix -l,$(patsubst lib%,%,$(LIBRARIES)))
MAKEFLAGS		+=	-j -s


#	Valgrind

VALGRIND		:=	$(shell which valgrind)

VALGRINDFLAGS	=	--errors-for-leak-kinds=all \
					--leak-check=full \
					--read-var-info=yes \
					--show-error-list=yes \
					--show-leak-kinds=all \
					--trace-children=yes \
					--track-origins=yes

VALGRINDFDFLAGS	:=	--track-fds=all


#	Terminal

TERMINAL		:=	$(shell which gnome-terminal 2>/dev/null)

ifeq (val, $(filter val,$(MAKECMDGOALS)))
TERMINALTITLE	:=	valgrind $(NAME)
else ifeq (valfd, $(filter valfd,$(MAKECMDGOALS)))
TERMINALTITLE	:=	valgrind-fd $(NAME)
else
TERMINALTITLE	:=	$(NAME)
endif

TERMINALFLAGS	:=	--title="$(TERMINALTITLE)" --


#	Files

SRC_EXTENSION	:=	.c
SRC				:=	$(shell find $(SRC_DIR) -type f -name "*$(SRC_EXTENSION)" -printf "%P\n")
OBJ				:=	$(SRC:%$(SRC_EXTENSION)=$(OBJ_DIR)/%.o)
DEP				:=	$(SRC:%$(SRC_EXTENSION)=$(DEP_DIR)/%.d)


#	Subdirectories

OBJ_SUBDIRS		:=	$(sort $(dir $(OBJ)))
DEP_SUBDIRS		:=	$(sort $(dir $(DEP)))


# *************************** BUILD PREPARATION ****************************** #

export				CC CFLAGS MAKECMDGOALS MAKEFLAGS

PHONY_TARGETS	:=	all eval run opt san val valfd term clear modes re clean fclean ffclean
ENV_VARIABLES	:=	MODE ARGS
HELP_TARGETS	:=	help help-print \
					$(addprefix help-,$(PHONY_TARGETS) $(ENV_VARIABLES)) \
					$(addsuffix -help,$(PHONY_TARGETS) $(ENV_VARIABLES))
PHONY_TARGETS	+=	$(HELP_TARGETS)
export .PHONY	:	$(PHONY_TARGETS)

REBUILD_TARGETS	:=	opt san re
CLEAN_TARGETS	:=	clean fclean ffclean

.DEFAULT		:
					$(MAKE) help

.DEFAULT_GOAL	:=	all


# ********************************* MODES ************************************ #

ENV				:=

ifeq (eval, $(filter eval,$(MAKECMDGOALS) $(MODE)))
RUN				:=	true
NEW_TERM		:=	true
ARGS			+=	--eval
endif

ifeq (run, $(filter run,$(MAKECMDGOALS) $(MODE)))
RUN				:=	true
endif

ifeq (opt, $(filter opt,$(MAKECMDGOALS) $(MODE)))
CFLAGS			:=	$(CFLAGS_STD) $(CFLAGS_OPT)
RECOMPILE		:=	true
endif

ifeq (san, $(filter san,$(MAKECMDGOALS) $(MODE)))
CFLAGS			+=	$(CFLAGS_STD) $(CFLAGS_DBG) $(CFLAGS_SAN)
RECOMPILE		:=	true
endif

ifeq (val, $(filter val,$(MAKECMDGOALS) $(MODE)))
ENV				+=	$(VALGRIND) $(VALGRINDFLAGS)
PATH			:=	/bin:/usr/bin:/usr/sbin:$(PATH)
RUN				:=	true
endif

ifeq (valfd, $(filter valfd,$(MAKECMDGOALS) $(MODE)))
ENV				+=	$(VALGRIND) $(VALGRINDFLAGS) $(VALGRINDFDFLAGS)
PATH			:=	/bin:/usr/bin:/usr/sbin:$(PATH)
RUN				:=	true
endif

ifeq (term, $(filter term,$(MAKECMDGOALS) $(MODE)))
NEW_TERM		:=	true
RUN				:=	true
endif

ifeq (clear, $(filter clear,$(MAKECMDGOALS) $(MODE)))
CLEAR			:=	true
endif

ifdef ARGS
RUN				:=	true
endif


# ***************************** BUILD TARGETS ******************************** #

all				:
					if $(MAKE) --question $(NAME); then \
						echo -n $(MSG_NO_CHANGE); \
						echo -n $(MSG_HELP); \
					else \
						echo -n $(MSG_MODE); \
						echo -n " "$(MSG_INFO); \
						echo -n " "$(MSG_HELP); \
						echo -n $(MSG_START); \
						if $(MAKE) $(NAME); then \
							echo; \
							echo -n $(MSG_SUCCESS); \
						else \
							echo; \
							echo -n $(MSG_FAILURE); \
							exit 42; \
						fi; \
					fi; \
					echo -n $(MSG_USAGE)


eval run opt san val valfd term clear: modes

modes			:
					if [ "$(RECOMPILE)" = "true" ]; then \
						MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) MSG_SUCCESS="" MSG_USAGE="" $(MAKE) re; \
						MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) $(MAKE) clean; \
					else \
						MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) MSG_USAGE="" $(MAKE) all; \
					fi
					if [ "$(CLEAR)" = "true" ]; then \
						clear; \
					fi
					if [ "$(NEW_TERM)" = "true" ] && [ -n "$(TERMINAL)" ]; then \
						$(TERMINAL) $(TERMINALFLAGS) bash -c \
							"trap '' SIGINT; \
							$(ENV) ./$(NAME) $(ARGS)"; \
					elif [ "$(RUN)" = "true" ]; then \
						$(ENV) "./$(NAME)" $(ARGS); \
					else \
						echo -n $(MSG_USAGE); \
					fi

re				:
					$(MAKE) fclean
					MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) $(MAKE) all


# ***************************** BUILD PROCESS ******************************** #

#	Dependency files inclusion

ifeq (, $(filter $(HELP_TARGETS) $(REBUILD_TARGETS) $(CLEAN_TARGETS),$(MAKECMDGOALS)))
    ifneq (, $(wildcard $(OBJ_DIR)))
        -include	$(DEP)
    endif
endif


#	Executable linkage

$(NAME)			:	$(OBJ)
					$(CC) $(CFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)


#	Source file compilation

$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%$(SRC_EXTENSION) $(BUILDFILES) | $(OBJ_SUBDIRS)
					$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ \
						&& echo -n $(MSG_PROGRESS)


#	Pre-processing and dependency file creation

$(DEP_DIR)/%.d	:	$(SRC_DIR)/%$(SRC_EXTENSION) $(BUILDFILES) | $(DEP_SUBDIRS)
					$(CC) $(CPPFLAGS) $(DEPFLAGS) $(CFLAGS) $<


#	Directory structure mirroring of source files for build artifacts

$(OBJ_SUBDIRS) \
$(DEP_SUBDIRS)	:
					mkdir -p $@


# ***************************** CLEAN TARGETS ******************************** #

clean			:
					echo -n $(MSG_CLEAN)
					rm -f $(OBJ) $(DEP)
                    ifneq (, $(wildcard $(OBJ_DIR)))
						-find $(OBJ_DIR) -type d -empty -delete
                    endif
                    ifneq (, $(wildcard $(DEP_DIR)))
						-find $(DEP_DIR) -type d -empty -delete
                    endif
					echo -n $(MSG_SUCCESS)

fclean			:
					echo -n $(MSG_FCLEAN)
					$(MAKE) clean
					rm -f $(NAME)
					echo -n $(MSG_SUCCESS)

ffclean			:
					echo -n $(MSG_FFCLEAN)
					$(MAKE) fclean
					rm -rf $(OBJ_DIR) $(DEP_DIR)
					echo -n $(MSG_SUCCESS)


# ****************************** HELP TARGETS ******************************** #

help			:
					echo "Targets:"
					echo "  all              Build the project (default target)"
					echo "  eval			 Build the project and run the project in eval mode"
					echo "  run              Build and run the project"
					echo "  opt              Rebuild the project with optimizations"
					echo "  san              Rebuild the project with sanitizers"
					echo "  val              Build and run the project with valgrind"
					echo "  valfd            Build and run the project with valgrind and file descriptor tracking"
					echo "  term             Build and run the project in a new terminal window"
					echo "  clear            Build the project and clear the terminal"
					echo "  re               Rebuild the project"
					echo "  clean            Remove build artifacts"
					echo "  fclean           Remove build artifacts and executable"
					echo "  ffclean          Remove build artifacts and executable without checking for unknown files"
					echo "  print-%          Print the value of a Makefile variable (replace % with variable name)"
					echo "  help             Display this message"
					echo "  help-% | %-help  Display more information for a specific target (replace % with target name)"
					echo
					echo "Environment Variables:"
					echo "  MODE             Build mode to combine multiple targets"
					echo "  ARGS             If specified, the program will run with those arguments after compilation."
					echo
					echo "Usage: make [\\$(STY_UND)target\\$(STY_RES)] [MODE=\"<\\$(STY_UND)mode1\\$(STY_RES)> [\\$(STY_UND)mode2\\$(STY_RES)] [...]\"] [ARGS=\"'<\\$(STY_UND)arg1\\$(STY_RES)>' '[\\$(STY_UND)arg2\\$(STY_RES)]' '[...]'\"]"

help-all		:
					echo "Build the project."
					echo "This is the default target when no target is specified."

help-eval		:
					echo "Build the project and run the project in eval mode."
					echo "This mode always shows the word to guess so it's easier to test the program."
					echo
					echo "This is equivalent to running './$(NAME) --eval'"

help-run		:
					echo "Build the project and run the executable."
					echo "Arguments to the program can be passed via the ARGS variable."
					echo
					echo "Usage: make run ARGS=\"'<\\$(STY_UND)arg1\\$(STY_RES)>' '[\\$(STY_UND)arg2\\$(STY_RES)]' '[...]'\"" #Maybe omit the usage

help-opt		:
					echo "Rebuild the project with the following compiler optimization flags:"
					echo "  $(CFLAGS_OPT)"

help-san		:
					echo "Rebuild the project with the following sanitizer flags:"
					echo "  $(CFLAGS_SAN)"

help-val		:
					echo "Build the project and run the executable with valgrind."
					echo "Arguments to the program can be passed via the ARGS variable."
					echo
					echo "The following valgrind flags are used:"
					echo "$(VALGRINDFLAGS)" | tr ' ' '\n' | sed 's/^/  /'
					echo
					echo "Usage: make val ARGS=\"'<\\$(STY_UND)arg1\\$(STY_RES)>' '[\\$(STY_UND)arg2\\$(STY_RES)]' '[...]'\""

help-valfd		:
					echo "Build the project and run the executable with valgrind and file descriptor tracking."
					echo "A new terminal window is opened to avoid inheriting open file descriptors."
					echo "Arguments to the program can be passed via the ARGS variable."
					echo
					echo "The following valgrind flags are used:"
					echo "$(VALGRINDFLAGS)" | tr ' ' '\n' | sed 's/^/  /'
					echo "File descriptor specific flags:"
					echo "$(VALGRINDFDFLAGS)" | tr ' ' '\n' | sed 's/^/  /'
					echo
					echo "Usage: make valfd ARGS=\"'<\\$(STY_UND)arg1\\$(STY_RES)>' '[\\$(STY_UND)arg2\\$(STY_RES)]' '[...]'\""

help-term		:
					echo "Build the project and run the executable in a new terminal window."
					echo "Arguments to the program can be passed via the ARGS variable."
					echo
					echo "Usage: make term ARGS=\"'<\\$(STY_UND)arg1\\$(STY_RES)>' '[\\$(STY_UND)arg2\\$(STY_RES)]' '[...]'\""

help-clear		:
					echo "Build the project and clear the terminal."

help-re			:
					echo "Rebuild the project."

help-clean		:
					echo "Remove build artifacts."

help-fclean		:
					echo "Remove build artifacts and the executable."

help-ffclean	:
					echo "Remove build artifacts and the executable without checking for unknown files."

help-print		:
					echo "Print the value of a Makefile variable by appending the variable name to print-..."
					echo "Useful for Makefile debugging."
					echo
					echo "Usage: make print-<\\$(STY_UND)variable name\\$(STY_RES)>"

help-help		:
					echo "Display more information for a specific target by appending or prepending help."
					echo
					echo "Usage: make help-<\\$(STY_UND)target\\$(STY_RES)> | make <\\$(STY_UND)target\\$(STY_RES)>-help"

help-MODE MODE-help:
					echo "Build mode to combine with other targets."
					echo "Multiple modes can be combined by separating them with a space."
					echo
					echo "Usage: make <\\$(STY_UND)target\\$(STY_RES)> MODE=\"<\\$(STY_UND)mode1\\$(STY_RES)> [\\$(STY_UND)mode2\\$(STY_RES)] [...]\""

help-ARGS ARGS-help:
					echo "If specified, the program will run with those arguments after compilation."
					echo
					echo "Usage: make <\\$(STY_UND)target\\$(STY_RES)> ARGS=\"'<\\$(STY_UND)arg1\\$(STY_RES)>' '[\\$(STY_UND)arg2\\$(STY_RES)]' '[...]'\""

%-help:
					$(MAKE) help-$(subst -help,,$@)


# ********************************* COLORS *********************************** #

STY_RES			:=	"\e[0m"
STY_BOL			:=	"\e[1m"
STY_ITA			:=	"\e[3m"
STY_UND			:=	"\e[4m"
STY_RED			:=	"\e[31m"
STY_GRE			:=	"\e[32m"
STY_YEL			:=	"\e[33m"
STY_BLU			:=	"\e[34m"
STY_MAG			:=	"\e[35m"
STY_CYA			:=	"\e[36m"
STY_WHI			:=	"\e[37m"
STY_GRA			:=	"\e[90m"
STY_WHI_BRI		:=	"\e[97m"
STY_BLA_BG		:=	"\e[40m"
STY_RED_BG		:=	"\e[41m"
STY_GRE_BG		:=	"\e[42m"
STY_YEL_BG		:=	"\e[43m"
STY_BLU_BG		:=	"\e[44m"
STY_MAG_BG		:=	"\e[45m"
STY_CYA_BG		:=	"\e[46m"
STY_WHI_BG		:=	"\e[47m"
STY_GRA_BG		:=	"\e[100m"
STY_WHI_BRI_BG	:=	"\e[107m"


# **************************** CUSTOM MESSAGES ******************************* #

MSG_PROGRESS	:=	"🔡"


########################## Top-level only messages #############################
ifeq (0, $(MAKELEVEL))

#	Make status messages

MSG_INFO		:=	$(STY_ITA)$(STY_WHI)"Make version: $(MAKE_VERSION)\n\
					Compiler version: $(CC_VERSION)"$(STY_RES)"\n"

MSG_HELP		:=	$(STY_ITA)$(STY_WHI)"Run 'make help' to see all available Makefile targets."$(STY_RES)"\n"

ifneq (, $(filter $(REBUILD_TARGETS),$(MAKECMDGOALS) $(MODE)))
MSG_START		:=	$(STY_ITA)"Rebuilding $(NAME) ... "$(STY_RES)
else
MSG_START		:=	$(STY_ITA)"Building $(NAME) ... "$(STY_RES)
endif

MSG_SUCCESS		?=	$(STY_BOL)$(STY_ITA)$(STY_GRE)"DONE!"$(STY_RES)"\n"

MSG_NO_CHANGE	:=	$(STY_ITA)$(STY_WHI)"Everything up-to-date!"$(STY_RES)"\n"

MSG_FAILURE		:=	$(STY_BOL)$(STY_ITA)$(STY_RED)"BUILD FAILED!"$(STY_RES)"\n"

MSG_USAGE		?=	"\nUsage: ./$(NAME) [--eval]\n"


#	Build modes

MSG_EVAL		:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_GRE)"~~~~~~~~~~~~~~~~~~~~ EVALUATION MODE ~~~~~~~~~~~~~~~~~~~"$(STY_RES)"\n"
ifneq (, $(filter eval,$(MAKECMDGOALS) $(MODE)))
MSG_MODE		:=	$(MSG_MODE)$(MSG_EVAL)
endif

MSG_RUN			:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_YEL)"~~~~~~~~~~~~~~~~~~~~~~~ RUN MODE ~~~~~~~~~~~~~~~~~~~~~~~"$(STY_RES)"\n"
ifneq (, $(filter run,$(MAKECMDGOALS) $(MODE)))
MSG_MODE		:=	$(MSG_MODE)$(MSG_RUN)
endif

MSG_OPT			:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_GRE)"~~~~~~~~~~~~~~~~~~~ OPTIMIZATION MODE ~~~~~~~~~~~~~~~~~~"$(STY_RES)"\n"
ifneq (, $(filter opt,$(MAKECMDGOALS) $(MODE)))
MSG_MODE		:=	$(MSG_MODE)$(MSG_OPT)
endif

MSG_SAN			:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_RED)"~~~~~~~~~~~~~~~~~~~~ SANITIZER MODE ~~~~~~~~~~~~~~~~~~~~"$(STY_RES)"\n"
ifneq (, $(filter san,$(MAKECMDGOALS) $(MODE)))
MSG_MODE		:=	$(MSG_MODE)$(MSG_SAN)
endif

MSG_VAL			:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_BLU)"~~~~~~~~~~~~~~~~~~~~~ VALGRIND MODE ~~~~~~~~~~~~~~~~~~~~"$(STY_RES)"\n"
ifneq (, $(filter val,$(MAKECMDGOALS) $(MODE)))
MSG_MODE		:=	$(MSG_MODE)$(MSG_VAL)
endif

MSG_VALFD		:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_CYA)"~~~~~~~~~~~~~~~~~~~ VALGRIND FD MODE ~~~~~~~~~~~~~~~~~~~"$(STY_RES)"\n"
ifneq (, $(filter valfd,$(MAKECMDGOALS) $(MODE)))
MSG_MODE		:=	$(MSG_MODE)$(MSG_VALFD)
endif

MSG_TERM		:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_YEL)"~~~~~~~~~~~~~~~~~~~ NEW TERMINAL MODE ~~~~~~~~~~~~~~~~~~"$(STY_RES)"\n"
ifneq (, $(filter term,$(MAKECMDGOALS) $(MODE)))
MSG_MODE		:=	$(MSG_MODE)$(MSG_TERM)
endif

MSG_CLEAR		:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_GRA)"~~~~~~~~~~~~~~~~~~~~~ CLEAR MODE ~~~~~~~~~~~~~~~~~~~~"$(STY_RES)"\n"
ifneq (, $(filter clear,$(MAKECMDGOALS) $(MODE)))
MSG_MODE		:=	$(MSG_MODE)$(MSG_CLEAR)
endif


#	Clean messages

MSG_CLEAN		:=	$(STY_ITA)"Cleaning up build artifacts ... "$(STY_RES)"\n"

MSG_FCLEAN		:=	$(STY_ITA)"Cleaning up build artifacts and executable ... "$(STY_RES)"\n"

MSG_FFCLEAN		:=	$(STY_ITA)"Forcefully cleaning up build artifacts directory and executable ... "$(STY_RES)"\n"

endif
################################################################################


# *************************** MAKEFILE DEBUGGING ***************************** #

print-%			:
					echo $* = $($*)


# ********************************* NOTES ************************************ #
