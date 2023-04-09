#include "Memo.hpp"
#include <vector> 
#include <string> 

#include <ncurses.h> 

void setSelectMode() {
	noecho();
	curs_set(0);
}

void setWriteMode() {
	echo();
	curs_set(1);
}

void loadMemos(void);
int promptOptions(std::vector<std::string>);

int main() {
	initscr();
	cbreak(); 
	keypad(stdscr, TRUE);

	static std::vector<Memo> memos;
	loadMemos();

	std::vector<std::string> options;
	for(auto memo : memos) 
		options.push_back(memo.title);
	options.push_back("Create New Memo");
	options.push_back("Exit");

	int selectedOption = promptOptions(options);	

	endwin();
	return 0;
}

int promptOptions(std::vector<std::string> options) {
	setSelectMode();

	int currentOption = 0;
	while(true) {
		clear();
		for(int i = 0; i < options.size(); i++) {
			if(i == currentOption)
				attrset(A_REVERSE); 
			
			addstr(options[i].c_str());
			addch('\n');

			attrset(A_NORMAL);
		}

		int key;
		while((key = getch()) != KEY_UP && key != KEY_DOWN && key != '\n') 
			;

		switch(key) {
			case KEY_UP: 
				if(currentOption != 0) 
					currentOption--;
				else 
					currentOption = options.size()-1;
				break;
			case KEY_DOWN: 
				if(currentOption != options.size()-1) 
					currentOption++;
				else 
					currentOption = 0;
				break;
			case '\n': 
				clear();
				return currentOption;
		}
	}	
}

void loadMemos() {} 





