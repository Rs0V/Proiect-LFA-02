#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define PRINT_SWITCH 0
#define STR std::string
#define STR_LIST std::vector<STR>
#define DICT std::map<STR, std::map<STR, STR_LIST>>
#define WORDS std::map<STR, STR_LIST>

std::ifstream fin("input.in");

std::ostream& operator<<(std::ostream& os, const STR_LIST& str_list) {
	for (STR str : str_list)
		os << str;
	return os;
}

STR char2str(const char c) {
	STR str;
	str.push_back(c);
	return str;
}

int str2int(const STR& str) {
	bool cast = false;
	int number = 0;
	for (char c : str) {
		if (c >= '0' && c <= '9') {
			cast = true;
			if (c == '0' && number > 0 || c > '0')
				number = (number + (c - '0')) * 10;
		}
	}
	number /= 10;
	return cast ? number : -1;
}

STR_LIST split_string(const STR& str) {
	STR_LIST list;
	list.push_back("");
	for (char c : str)
		if (c == ' ')
			list.push_back("");
		else
			list.back().push_back(c);
	return list;
}

void print_all_nwords(int nr_letters, STR& curr_state, STR_LIST& end_states, DICT& dict, STR_LIST& path) {
	if (nr_letters == 0) {
		for (STR end_state : end_states) {
			if (curr_state == end_state) {
				std::cout << path << "\n";
				break;
			}
		}
	}
	else {
		for (auto& letter_map : dict[curr_state]) {
			path.push_back(letter_map.first);
			for (auto& state : letter_map.second)
				print_all_nwords(nr_letters - 1, state, end_states, dict, path);
			path.pop_back();
		}
	}
}

int main() {
	int n;
	STR_LIST input_lines;
	STR begin_state;
	STR_LIST end_states;

	while (!fin.eof()) {
		STR line;
		std::getline(fin, line);
		input_lines.push_back(line);
	}

	n = str2int(input_lines.back());
	input_lines.pop_back();
	DICT dict;

	for (int i = 0; i < input_lines.size() - 1; ++i) {
		STR_LIST vstr = split_string(input_lines[i]);
		auto state_iter = dict.find(vstr[0]);

		if (state_iter != dict.end()) {
			auto letter_iter = state_iter->second.find(vstr[1]);
			if (letter_iter != state_iter->second.end())
				letter_iter->second.push_back(vstr[2]);
			else
				state_iter->second.insert({ vstr[1], STR_LIST(1,vstr[2]) });
		}
		else {
			std::map<STR, STR_LIST> letter_map;
			letter_map.insert({ vstr[1], STR_LIST(1, vstr[2]) });
			dict.insert({ vstr[0], letter_map });
		}

		int bs_nr = str2int(begin_state);
		int state0_nr = str2int(vstr[0]);
		int state1_nr = str2int(vstr[2]);

		if (bs_nr == -1)
			bs_nr = 999999999;
		if (bs_nr > state0_nr)
			begin_state = vstr[0], bs_nr = state0_nr;
		if (bs_nr > state1_nr)
			begin_state = vstr[2];
	}

	end_states = split_string(input_lines.back());
	STR_LIST path;

	for (int i = 1; i <= n; ++i)
		print_all_nwords(i, begin_state, end_states, dict, path);

	return 0;
}