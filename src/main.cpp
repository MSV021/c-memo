#include "editor.hpp"
#include "Memo.hpp"
#include <cstdio> 
#include <fstream> 
#include <vector> 
#include <string> 
#include <ctime> 

#include <ncurses.h> 
#define KEY_ESC 27

const std::string SAVEFILE = "memos.txt";

std::string getnstring(int);
void printLine(void);

void setSelectMode(void); 
void setWriteMode(void);

void printMenu(std::vector<std::string>, int, bool);
void printTitle(void);
void printHelp(void);
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
	bool showHelp = true;
	int selectedOption = 0;
	while(true) {
		clear();
		printMenu(options, selectedOption, showHelp); 

		setSelectMode();
		key = getch();
		switch(key) {
			case 'c': 
				createNewMemo(memos);
				options.insert(options.end(), memos.back().title);
				break;
			case 'h': 
				showHelp = !showHelp;
				break;
			case KEY_ESC: 
				endwin();
				return 0;
		}

		if(options.empty()) 
			continue;

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
			case 'x': 
				removeMemo(memos, selectedOption);
				options.erase(options.begin()+selectedOption);
				if(selectedOption == memos.size() && selectedOption != 0) 
					selectedOption--;
				saveMemos(memos);
				break;
			case '\n': 
				editMemo(memos[selectedOption]);
				saveMemos(memos);
				break;
		}
	}
}

std::string getnstring(int limit) {
	char temp[limit];
	getnstr(temp, limit);

	return std::string(temp);
}

void printLine() {
	printw("----------\n");
}

void setSelectMode() {
	noecho();
	curs_set(0);
}

void setWriteMode() {
	echo();
	curs_set(1);
}

void printTitle() {
	attrset(A_BOLD);
	printw("C-MEMO\n");
	attrset(A_NORMAL);
	printLine();
}

void printHelp() {
	attrset(A_LOW | A_ITALIC);
	printw("Quick Help\n");
	printw("[C]   -> create a new one\n");
	printw("[X]   -> delete highlighted memo\n");
	printw("[H]   -> hide quick help\n");
	printw("[ESC] -> quit c-memo\n");
	attrset(A_NORMAL);
}

void printMenu(std::vector<std::string> options, int currentOption, bool showHelp) {
	printTitle();

	if(options.empty()) {
		attrset(A_ITALIC);
		printw("Your memos will go here!\n");
		attrset(A_NORMAL);
	}

	for(int i = 0; i < options.size(); i++) {
		if(i == currentOption)
			attrset(A_REVERSE); 
			
		addstr(options[i].c_str());
		addch('\n');

		attrset(A_NORMAL);
	}	

	printLine();
	if(showHelp) 
		printHelp();
}

void createNewMemo(std::vector<Memo>& memos) {
		clear();
		printw("Enter new memo's title (max %d characters):\n", Memo::maxTitleLen);

		setWriteMode();
		std::string title = getnstring(Memo::maxTitleLen);
		
		if(title.empty()) {
			std::time_t now = std::time(0);
			char* dt = ctime(&now);
			title = dt;
			title.pop_back();
		}

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










