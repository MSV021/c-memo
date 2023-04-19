#include "editor.hpp"
#include "Memo.hpp"
#include <fstream> 
#include <vector> 
#include <string> 

#include <ncurses.h> 

const std::string SAVEFILE = "save.bin";

std::string getnstring(int);
void setSelectMode(void); 
void setWriteMode(void);

int promptOptions(std::vector<std::string>);
void saveMemos(std::vector<Memo>);
void loadMemos(std::vector<Memo>&);
void createNewMemo(std::vector<Memo>&);
void editMemo(Memo&);

int main() {
	initscr();
	cbreak(); 
	keypad(stdscr, TRUE);

	static std::vector<Memo> memos;
	loadMemos(memos);

	std::vector<std::string> options;
	for(auto memo : memos) 
		options.push_back(memo.title);
	options.push_back("Create New Memo");
	options.push_back("Exit");

	int selectedOption;
	while(true) {
		selectedOption = promptOptions(options);	
		if(selectedOption < memos.size()) { 
			editMemo(memos[selectedOption]);
			saveMemos(memos);		
		}
		else if(selectedOption == memos.size()) {
			createNewMemo(memos);
			options.insert(options.begin()+memos.size()-1, memos.back().title);
		}
		else 
			break;
	}

	endwin();
	return 0;
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

void printOptions(std::vector<std::string> options, int currentOption) {
	clear();
	for(int i = 0; i < options.size(); i++) {
		if(i == currentOption)
			attrset(A_REVERSE); 
			
		addstr(options[i].c_str());
		addch('\n');

		attrset(A_NORMAL);
	}
}

int promptOptions(std::vector<std::string> options) {
	setSelectMode();

	int currentOption = 0;
	while(true) {
		printOptions(options, currentOption);

		int key;
		while((key = getch()) != KEY_UP && key != KEY_DOWN && key != '\n') 
			;

		switch(key) {
			case KEY_UP: 
				if(currentOption == 0) 
					currentOption = options.size()-1;
				else 
					currentOption--;
				break;
			case KEY_DOWN: 
				if(currentOption == options.size()-1) 
					currentOption = 0;
				else 
					currentOption++;
				break;
			case '\n': 
				return currentOption;
		}
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










