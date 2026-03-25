# Universal makefile for single C++ program
#
# Use gcc flag -MMD (user) or -MD (user/system) to generate dependencies among source files.
# Use make default rules for commonly used file-name suffixes and make variables names.
#
# % make [ a.out ]

########## Variables ##########
CXX = g++-11					# compiler
CXXFLAGS = -std=c++20 -g -Wall -Werror=vla -MMD			# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

SRC_DIR = implementation
OBJ_DIR = compiled
DEP_DIR = dependencies

SOURCES = $(wildcard $(SRC_DIR)/*.cc) # source files (*.cc)
OBJECTS = $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SOURCES))	# object files forming executable
DEPENDS = $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.d,$(OBJECTS)) # substitute ".o" with ".d"
EXEC = watan # executable name

all: ${EXEC}
# Ensure object and dependency directories exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

########## Targets ##########

.PHONY : clean					# not file names

${EXEC} : ${OBJECTS}				# link step
	${CXX} ${CXXFLAGS} $^ -o $@		# additional object files before $^

# Pattern rule to compile .cc to .o and generate .d files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR) $(DEP_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MF $(DEP_DIR)/$*.d -c $< -o $@

# make implicitly generates rules to compile C++ files that generate .o files

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}
