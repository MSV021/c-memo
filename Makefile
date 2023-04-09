cc = g++
sdir = src
out = bin/cmemo.out

$(out): $(sdir)/*.cpp 
	@$(cc) -o $(out) $(sdir)/*.cpp -lncurses -ltinfo 

.PHONY: run clean
run: $(out) 
	@./$(out)
clean: 
	@rm $(out)
