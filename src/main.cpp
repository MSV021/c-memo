#include "editor.hpp"
#include "Memo.hpp"
#include <cstdio> 
#include <fstream> 
#include <vector> 
#include <string> 

#include <ncurses.h> 
#define KEY_ESC 27

const std::string SAVEFILE = "save.bin";

std::string getnstring(int);
void setSelectMode(void); 
void setWriteMode(void);

void printOptions(std::vector<std::string>, int);
void printGreeting();
void editMemo(Memo&);
void removeMemo(std::vector<Memo>&, int);
void saveMemos(std::vector<Memo>);
void loadMemos(std::vector<Memo>&);
void createNewMemo(std::vector<Memo>&);

int main() {
	initscr();
	keypad(stdscr, TRUE);
	set_escdelay(100);

	static std::vector<Memo> memos;
	loadMemos(memos);

	std::vector<std::string> options;
	for(auto memo : memos) 
		options.push_back(memo.title);

	int key;
	int selectedOption = 0;
	while(true) {
		clear();
		if(!options.empty()) {
			attrset(A_BOLD);
			printw("YOUR MEMOS\n");
			attrset(A_NORMAL);
			printOptions(options, selectedOption);
		}
		else 
			printGreeting();

		setSelectMode();
		key = getch();
		if(!options.empty()) {
			switch(key) {
				case KEY_UP: 
					selectedOption--;
					if(selectedOption < 0) 
						selectedOption = options.size()-1;
					break;
				case KEY_DOWN: 
					selectedOption++;
					if(selectedOption >= options.size()) 
						selectedOption = 0;
					break;
				case '\n': 
					editMemo(memos[selectedOption]);
					saveMemos(memos);
					break;	
			}
		}

		switch(key) {
			case 'c':
				createNewMemo(memos);
				options.insert(options.end(), memos.back().title);
				break;
			case 'x': 
				removeMemo(memos, selectedOption);
				options.erase(options.begin()+selectedOption);
				if(selectedOption == memos.size()) 
					selectedOption--;
				saveMemos(memos);
				break;
			case KEY_ESC: 
				endwin();
				return 0;
		}
	}
}

std::string getnstring(int limit) {
	char temp[limit];
	getnstr(temp, limit);

	return std::string(temp);
}

void setSelectMode() {
	noecho();
	curs_set(0);
}

void setWriteMode() {
	echo();
	curs_set(1);
}

void printGreeting() {
	printw("You don't have any memos!\n");
	printw("Press [C] to create a new one\n");
}

void printOptions(std::vector<std::string> options, int currentOption) {
	for(int i = 0; i < options.size(); i++) {
		if(i == currentOption)
			attrset(A_REVERSE); 
			
		addstr(options[i].c_str());
		addch('\n');

		attrset(A_NORMAL);
	}
}

void createNewMemo(std::vector<Memo>& memos) {
		clear();
		printw("Enter new memo's title (max %d characters):\n", Memo::maxTitleLen);

		setWriteMode();
		std::string title = getnstring(Memo::maxTitleLen);
		title = "[ " + title + " ]";

		std::string path = "memo" + std::to_string(memos.size()) + ".txt";
		memos.push_back(Memo(path, title));
}

void editMemo(Memo& memo) {
	setWriteMode();
	editor::edit(memo.content);
}

void removeMemo(std::vector<Memo>& memos, int index) {
		std::remove(memos[index].path.c_str());
		memos.erase(memos.begin()+index);
}

void saveMemos(std::vector<Memo> memos) {
	std::ofstream file;
	file.open(SAVEFILE);

	for(auto memo : memos) 
		file << memo.path << '\n';	
	file.close();

	for(auto memo : memos) {
		file.open(memo.path);
		file << memo.title << '\n';
		for(auto line : memo.content) {
			file << line << '\n';
		}
		file.close();
	}
}

void loadMemos(std::vector<Memo>& memos) {
	std::ifstream file;
	file.open(SAVEFILE);

	if(!file.good()) 
		return;

	std::string temp;
	std::vector<std::string> paths;
	while(std::getline(file, temp)) 
		paths.push_back(temp);
	file.close();

	Memo memo;
	for(auto path : paths) {
		memo.path = path;
		file.open(memo.path);
		std::getline(file, memo.title);
		while(std::getline(file, temp)) 
			memo.content.push_back(temp);

		memos.push_back(memo);
		memo.content.clear();
		file.close();
	}
} 










