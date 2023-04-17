#include "editor.hpp"
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

std::string getnstring(int limit) {
	char temp[limit];
	getnstr(temp, limit);

	return std::string(temp);
}

void loadMemos(void);
int promptOptions(std::vector<std::string>);

int main() {
	initscr();
	cbreak(); 
	keypad(stdscr, TRUE);

	// TESTING EDITOR
	setWriteMode();
	std::vector<std::string> lines = {"Hello, World!", "I am Mahdi Rezaei", "Let's edit some text"};
	editor::edit(lines);
	getch();
	endwin();
	return 0;
	// TESTING EDITOR

	static std::vector<Memo> memos;
	loadMemos();

	std::vector<std::string> options;
	for(auto memo : memos) 
		options.push_back(memo.title);
	options.push_back("Create New Memo");
	options.push_back("Exit");

	int selectedOption;
	while(true) {
		if(options.size() < memos.size()+2) 
			options.insert(options.begin(), memos.back().title);

		setSelectMode();
		selectedOption = promptOptions(options);	
		if(selectedOption < memos.size()) { 
			// start editing selected memo
		}
		else if(selectedOption == memos.size()) {
			clear();
			printw("Enter new memo's title (max %d characters):\n", Memo::maxTitleLen);

			setWriteMode();
			std::string title = getnstring(Memo::maxTitleLen);
			title = "[ " + title + " ]";

			std::string path = "memo" + std::to_string(memos.size()) + ".txt";
			memos.push_back(Memo(path, title));
		}
		else 
			break;
	}

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



