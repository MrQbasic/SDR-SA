FILE_OUT 			:= SDRSA

DIR_BLD				:= ./build
DIR_SRC				:= ./src
DIR_LIB				:= ./lib

GPP					:= g++
GPP_FLAGS			:= -lrtlsdr -lm -lraylib -lfftw3 -g

MKDIR				:= mkdir

#------------------------------------------------------------------------------

FILES_CPP				:= $(shell find $(DIR_SRC) -type f -name '*.cpp')
FILES_O				:= $(patsubst $(DIR_SRC)/%.cpp,$(DIR_BLD)/%.o,$(FILES_CPP))


.all: $(FILE_OUT)

$(FILE_OUT): $(FILES_O)
	$(GPP) $(GPP_FLAGS) -I $(DIR_LIB) -o $@ $^ 

$(FILES_O): $(DIR_BLD)/%.o: $(DIR_SRC)/%.cpp
	$(MKDIR) -p $(dir $@)
	$(GPP) $(GPP_FLAGS) -I $(DIR_LIB) -c $^ -o $@


clean:
	rm -f $(FILES_O)
	rm -f $(FILE_OUT)

run: $(FILE_OUT)
	./$(FILE_OUT)
