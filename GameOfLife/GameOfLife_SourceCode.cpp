#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

//struct for an organism
struct organism {
	organism( std::string type, char name, int row, int column, int max_energy, int curr_energy)
	: type(type), name(name), row(row), column(column), max_energy(max_energy), curr_energy(curr_energy)
	{}

	organism() {};
	

	void operator() (std::string type, char name, int row, int column, int max_energy, int curr_energy){}

	std::string type;
	char name;
	int row;
	int column;
	int max_energy;
	int curr_energy;
};

//struct plant is an organism
struct plant : organism {
	plant(std::string type, char name, int growth, int status, int row, int column, int max_energy, int curr_energy)
	: organism (type, name, row, column, max_energy, curr_energy), growth(growth), status(status)
	{}
	 
	plant() {};

	void operator() (std::string type, char name, int growth, int status, int row, int column, int max_energy, int curr_energy){};

	int growth;
	int status;
};

//struct animal (contains herbivores and omnivores) is an organism
struct animal : organism {
	animal(std::string type, char name, std::vector<char> chain, int row, int column, int max_energy, int curr_energy)
	: organism ( type, name, row, column, max_energy, curr_energy ), chain(chain)
	{}

	animal() {};
	
	void operator() (std::string type, char name, std::vector<char> chain, int row, int column, int max_energy, int curr_energy){}

	std::vector<char> chain;
};


// A vector that stores all the lines in the game board
using game_board = std::vector<std::string>;

// Asks user to add their own game board or use the ./input/map.txt as default
std::string add_board ( std::istream& input){
	std::string file;
	std::cout << "Enter a game board file or type 'd' to use the default: ";
	std::cin >> file;
	if (file == "d"|| file == "D" || file == ".input/map.txt")
		return "./input/map.txt";
	else 
		return file;
}

// Asks user to add their species or use the ./input/species.txt as default
std::string add_species ( std::istream& input){
	std::string file;
	std::cout << "Enter a species list file or type 'd' to use the default: ";
	std::cin >> file;
	if (file == "d"|| file == "D" || file == ".input/species.txt")
		return "./input/species.txt";
	else 
		return file;
}

// Checks to make sure the inputted file is readable and creates a game_board vector for it
game_board load_game( std::string const& filename )
{

  std::ifstream gamefile;
  gamefile.open(filename);

  if(!gamefile)
    throw std::runtime_error("Can't open " + filename);
  
  else{
    game_board game;
    std::ifstream in( filename );
    std::string msg;
    while ( std::getline(in, msg) && !msg.empty())
      game.push_back(msg);
    return game;
  }
}

//Displays the vector game board for the user to see the starting positions of each organism
void display_game ( game_board& game ){
	for( auto line = game.begin(); line != game.end(); line++)
		std::cout << *line << std::endl;
	std::cout << "\n";
}

//Prompts the user to input the number of iterations they would like to have the game accomplish before prompting again in the next turn
int input_it(std::istream& in){
	int num;
	std::cout << "Enter the number of iterations as an integer or 0 to end the game: ";
	while (!(in >> num )){
		std::cout << "Invalid input - Enter the number of iterations as an INTEGER or 0 to end game: ";
		in.clear();
		in.ignore(123, '\n');
	}
	return num;
}


//function to create a map, storing each individual on the board as thier respective organism(which will be the value) and assigning it an id number (key)
std::map< std::tuple<char, int> , plant> plant_map( game_board& game, game_board& species){
	std::map<std::tuple<char, int> , plant> database;
	int org_id = 1;
	for( auto line = species.begin(); line != species.end(); line++){
		std::istringstream in( *line );
		std::string t;
		char n;
		in >> t >> n;
		int count = 0;
		for( auto row = game.begin(); row != game.end(); row++){
			std::string row_s = *row;
			int r = std::distance(game.begin(), row);
			for(int i = 0; i < row_s.size(); i++){
				if (row_s[i] == n){
					if (t == "plant"){
						int g, max;
						in >> g >> max;
						plant p1(t, n, g, g, r, i, max, max);
						std::tuple<char, int> p ('p', org_id);
						auto pair = std::pair<std::tuple<char, int>, plant>(p, p1);
						database.insert(pair);
						org_id++;
					}else{
						continue;
					}
				}else{
					continue;
				}
			}
		}
	}
	return database;
}


//function to create a map, storing each individual on the board as thier respective organism(which will be the value) and assigning it an id number (key)
std::map<std::tuple<char, int>, animal> animal_map( game_board& game, game_board& species){
	std::map<std::tuple<char, int>, animal> database;
	int org_id = 1;
	for( auto line = species.begin(); line != species.end(); line++){
		std::istringstream in( *line );
		std::string t;
		char n;
		in >> t >> n;
		std::vector<char> c;
		int max;
		if (t != "plant"){
			char lbrack, next;
			in >> lbrack;
			in >> next;
			while (next != ']'){
				if (next != ','){
				char prey = next;
				c.push_back(prey);
				}else{
					in.ignore();
				}
				in >> next;
			}
			in >> max;
		}
		for( auto row = game.begin(); row != game.end(); row++){
			std::string row_s = *row;
			int r = std::distance(game.begin(), row);
			for(int i = 0; i < row_s.size(); i++){
				if (row_s[i] == n){
					if (t == "plant"){
						continue;
					}else{
						animal a1(t, n, c, r, i, max, max);
						std::tuple<char, int> a ('a', org_id);
						auto pair = std::pair<std::tuple<char, int>, animal>(a, a1);
						database.insert(pair);
						org_id++;
					}
				}else{
					continue;
				}
			}
		}
	}
	return database;
}


//Function that checks what organism is at this key
void grow(std::map< std::tuple<char, int> , plant>& pmap, std::map<std::tuple<char, int>, animal>& amap, game_board& game){
	auto p = pmap.begin();
	while (p != pmap.end()){
		int growth = p->second.growth;
		if (p->second.status < growth){
			p->second.status++;
		}else{
			int row = p->second.row;
			int col = p->second.column;
			auto a = amap.begin();
			bool open = true;
			while(a != amap.end()){
	 			if (a->second.row == row && a->second.column == col){
					open = false;
					break;
				}
				a++;
			}
			if (open == true){
				char name = p->second.name;
				game[row][col] = name;
			}
		p++;
		}
	}
}

std::tuple<int, int> randomdirection( std::vector<std::tuple<int,int>> v){
	int i = rand() % v.size();
	return v[i];
}

void consume(std::map< std::tuple<char, int> , plant>& pmap, std::map<std::tuple<char, int>, animal>& amap, game_board& game, int r, int c, std::tuple<char, int> id){
	int diff = (amap[id].max_energy - amap[id].curr_energy);
	auto p = pmap.begin();
	while (p != pmap.end()){
		if ((p->second.row == r) && (p->second.column == c)){
			if(p->second.status < p->second.growth){
				continue;
			}else{
				p->second.status = 0;
				if(p->second.max_energy > diff){
					amap[id].curr_energy = amap[id].max_energy;
				}else{
					amap[id].curr_energy = amap[id].curr_energy + p->second.max_energy;
				}
			}
		}
		p++;
	}
	auto a = amap.begin();
	while (a != amap.end()){
		if((a->second.row == r) && (a->second.column == c)){
			if(a->second.curr_energy > diff){
				amap[id].curr_energy = amap[id].max_energy;
			}else{
				amap[id].curr_energy = amap[id].curr_energy + a->second.curr_energy;
			}
			amap.erase(a);
		}
		a++;
	}
}

void move(std::map< std::tuple<char, int> , plant>& pmap, std::map<std::tuple<char, int>, animal>& amap, game_board& game){
	int last_r = game.size() - 1;
	int last_c = game[0].size()-1;
	auto a = amap.begin();
	while (a != amap.end()){
		int r = a->second.row;
		int c = a->second.column;
		auto ch = a->second.chain;
		std::vector<std::tuple<int, int>> dir;
		//down
		if ( r != last_r) {
			if ((game[r+1][c] == ' ') || (std::count(ch.begin(), ch.end(), game[r+1][c]))){
				int r1 = r+1;
				std::tuple<int, int> l(r1, c);
				dir.push_back(l);
			}
		}
		//up
		if (r != 0) {
			if ((game[r-1][c] == ' ') || (std::count(ch.begin(), ch.end(), game[r-1][c]))){
				int r1 = r-1;
				std::tuple<int,int> l(r1, c);
				dir.push_back(l);
			}
		}
		//right c+1
		if (c != last_c) {
			if ((game[r][c+1] == ' ') || (std::count(ch.begin(), ch.end(), game[r][c+1]))){ 
				int c1 = c+1;
				std::tuple<int,int> l(r, c1);
				dir.push_back(l);
			}
		}
		//left c-1
		if (c != 0) {
			if ((game[r][c-1] == ' ') || (std::count(ch.begin(), ch.end(), game[r][c-1]))){
				int c1 = c-1;
				std::tuple<int,int> l(r, c1);
				dir.push_back(l);
			}
		}

		if (dir.empty()){
			continue;
		}else{
			a->second.curr_energy = (a->second.curr_energy) - 1;
			auto newloc = randomdirection(dir);
			auto id = a->first;
			int nr = std::get<0>(newloc);
			int nc = std::get<1>(newloc);
			game[r][c] = ' ';
			consume(pmap, amap, game, nr, nc, id);
			a->second.row = nr;
			a->second.column = nc;
			if (a->second.curr_energy == 0){
				amap.erase(a);
			}else{
				char name = a->second.name;
				game[nr][nc] = name;
			}
		}
		a++;
	}
}

void save(std::istream& in, std::map< std::tuple<char, int> , plant>& pmap, std::map<std::tuple<char, int>, animal>& amap){
	std::cout << "Would you like to save your game state? ";
	std::cout << "\nType 'Y' to save or any other key to cancel: ";
	char s;
	in >> s;
	if ((s == 'y') || (s == 'Y')){
		std::string filename;
		std::cout << "Name file: ";
		in >> filename;
		std::ofstream file(filename);
		auto p = pmap.begin();
		file << "Type Name Growth Row Col Energy" << std::endl;
		while (p != pmap.end()){
			file << p->second.type << " " << p->second.name << " " << p->second.status << " " << p->second.row << " " << p->second.column << " " << p->second.curr_energy << std::endl;
			p++;
		}
		file << "\n";
		file << "Type Name Row Col Energy" << std::endl;
		auto a = amap.begin();
		while (a != amap.end()){
			file << a->second.type << " " << a->second.name << " " << a->second.row << " " << a->second.column << " " << a->second.curr_energy << std::endl;
			a++;
		} 
		file.close();
		std::cout << filename << " was successfully saved!" << std::endl;
	}else{
		std::cout << "Your game was not saved." << std::endl;
	}
}


int main() {
	std::cout << "Welcome to Divya's Game of Life!"<< std::endl;
	//handles the game board and species list 
	std::string game_file = add_board(std::cin);
	std::string species_file = add_species(std::cin);
  auto game1 = load_game(game_file);
	auto species = load_game(species_file);
	std::cout << "\nYour game board:\n" << std::endl;
	display_game(game1);
	std::cout << "\nYour species list:\n" << std::endl;
	display_game(species);

	//creates a map of all the plants on the board
	auto pmap = plant_map(game1, species);
	//creates a map of all the animals on the board
	auto amap = animal_map(game1, species);
	auto a = amap.begin();

	//Continues to ask for number of iterations as long as the user doesn't input 0, which will end the game
	int iterations = input_it(std::cin);
	while (iterations){
		int i;
		for (i = 0; i < iterations; i++){
			grow(pmap, amap, game1);
			move(pmap, amap, game1);
		}
		std::cout << "\nUpdated game board after " << iterations << " iterations:\n" << std::endl;
		display_game(game1);
		iterations = input_it(std::cin);
		if (iterations == 0){
			save(std::cin, pmap, amap);
		}
	}
	std::cout << "Thanks for playing! Run to play again!";
}
