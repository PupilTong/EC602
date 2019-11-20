//#define Assignment8
#ifdef Assignment8
// Copyright  -- your info here --
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "timer.h"

using namespace std;

class Arrangements {
private:
	vector<char> _names;
	bool _needRegenerateNames = true;
	int _seats=0;
	//static vector<string> _cache;
	//static vector<char> _namesCache;
	//static int _lengthCache;	
	void _VerifyInput(int n) {
		if (!_needRegenerateNames&& _names.size() < n)throw n;
	}
	void  _Dinner(vector<string>& r, int length) {
		int size = r.size();
		for (int i = 0; i < size; i++) {
			if (r.at(i).back() == _names.at(length - 2)) {
				string t = r.at(i);
				t.insert(t.end() - 1, _names.at(t.size()));
				r.push_back(t);
				t = r.at(i);
				t.insert(t.begin(), _names.at(t.size()));
				r.push_back(t);
			}
			r.at(i) = r.at(i) + _names.at(length - 1);
		}
		string t0, t1;
		for (int i = 1; i < length - 1; i++) {
			t0 += _names.at(i);
		}
		t1 = t0 + _names.at(length - 1) + _names.at(0);
		t0.insert(t0.begin(), _names.at(length - 1));
		t0.insert(t0.begin(),_names.at(0));
		r.push_back(t0);
		r.push_back(t1);
	}
	void  _Panel(vector<string>& r, int length) {
		if (length == 2) {
			string t = "";
			r.push_back(t + _names.at(0) + _names.at(1));
			r.push_back(t + _names.at(1) + _names.at(0));
			return ;
		}
		else {
			int size = r.size();
			for (int i = 0; i < size; i++) {
				if (r.at(i).back() == _names.at(length - 2)) {
					string t = r.at(i);
					t.insert(t.end() - 1, _names.at(t.size()));
					r.push_back(t);
				}
				r.at(i) = r.at(i) + _names.at(length - 1);
			}
		}
	}	
	vector<string>& _GetDinner(vector<string>& r, int n) {
		if (_names.size() < n) {
			for (int i = _names.size(); i < n; i++)_names.push_back('a' + i);
		}
		if (n == 0)return r;
		if (n == 1) {
			string t = "" + _names.at(0);
			r.push_back(t);
		}
		for (int i = 1; i <= n - 1; i++) {
			_Panel(r, i);
		}
		_Dinner(r, n);
		if (_needRegenerateNames)_names.clear();
		return r;
	}
	vector<string>& _GetPanel(vector<string>& r, int n, int currentRLength = 0) {
		if (_names.size() < n) {
			for (int i = _names.size(); i < n; i++)_names.push_back('0' + i);
		}
		if (n == 0)return r;
		if (n == 1) {
			string t = "" + _names.at(0);
			r.push_back(t);
		}
		for (int i = currentRLength + 1; i <= n; i++) {
			_Panel(r, i);
		}
		if (_needRegenerateNames)_names.clear();
		return r;
	}
public:
	Arrangements() {
		_names.empty();
	}
	explicit Arrangements(string thenames) {
		for (auto c : thenames) {
			_names.push_back(c);
		}
		_needRegenerateNames = false;
	}
	vector<string> panel_shuffles(int n) {
		_VerifyInput(n);
		vector<string> r;
		/*
		r = this->_cache;
		if (n < this->_lengthCache) {
			int i = panel_count(n);
			while (i < r.size())r.erase(r.begin() + i);
			for (int i = 0; i < r.size();i++) {
				r.at(i).resize(n);
			}
		}
		else {
			_GetPanel(r, n , _lengthCache);
		}
		
		//replace different names
		map<char, char> t;
		for (int i = 0; i < min(_namesCache.size(), _names.size()); i++) {
			if (_names.at(i) != _namesCache.at(i)) {
				t[_namesCache.at(i)] = _names.at(i);
			}
		}
		for (auto s : r) {
			for (int i = 0; i < s.length(); i++) {
				if (t.find(s.at(i)) != t.end()) {
					string cha = "" + t[s.at(i)];
					s.at(i)= t[s.at(i)];
				}
			}
		}
		//
		if (n < 30) {
			_cache = r;
			_lengthCache = n;
			_namesCache = _names;
		}*/
		_GetPanel(r, n);
		return r;
	}
	vector<string> dinner_shuffles(int n) {
		_VerifyInput(n);
		vector<string> r;
		_GetDinner(r, n);
		return r;
	}
	double panel_count(int n) {
		_VerifyInput(n);
		__int128 f, s, t;
		f = s = 1;
		if (n == 0)return 0;
		if (n < 2)return 1;
		for (int i = 0; i < n-1; i++) {
			t = f;
			f = s;
			s = f + t;
		}
		return s;
	}
	double dinner_count(int n) {
		_VerifyInput(n);
		__int128 f, s, t;
		f = s = 1;
		if (n <= 2)return n;
		for (int i = 0; i < n-2; i++) {
			t = f;
			f = s;
			s = f + t;
		}
		return s + 2*f+ 2 ;
	}
	~Arrangements(){}
};
/*
vector<string>  Arrangements::_cache;
vector<char> Arrangements::_namesCache;
int Arrangements::_lengthCache = 0;
*/




// TESTING: leave this line and below as is.

void show_result(vector<string> v) {
	sort(v.begin(), v.end());
	for (auto c : v)
		cout << c << "\n";
	cout << "\n";
}

void show_partial_result(string testname, vector<string> res, int n) {
	if (res.empty()) return;

	sort(res.begin(), res.end());

	std::vector<uint64_t> locs{ 0, res.size() / 3,
							   2 * res.size() / 3, res.size() - 1 };
	std::cout << "\n" << testname << " " << n << "\n";
	for (auto i : locs) {
		std::cout << " res.at(" << i
			<< ") = " << res.at(i) << "\n";
	}
}


const int COUNTLIM = 100;
const int COUNTLIM_SMALL = 30;

void standard_tests();
void interactive_main();

int main(int argc, char const** argv) {
	if (argc > 1 and (string(*(argv + 1)) == string("int")))
		interactive_main();
	else
		standard_tests();
}

void standard_tests() {
	int n;

	cout.precision(15);

	// Basic test
	Arrangements standard;

	cout << "\nPanel Shuffles for 4 panelists.\n";
	show_result(standard.panel_shuffles(4));

	cout << "\nDinner Shuffles for 4 guests.\n";
	show_result(standard.dinner_shuffles(4));

	// Test other names
	Arrangements numbers("123456789");
	Arrangements symbols("!@#$%^&*()_+");

	std::array<Arrangements*, 3> v{ &standard, &numbers, &symbols };

	cout << "\nPanel Shuffles for 6 panelists, 3 sets of names.\n";
	for (auto arr : v)
		show_result(arr->panel_shuffles(6));

	cout << "\nDinner Shuffles for 6 guests, 3 sets of names.\n";
	for (auto arr : v)
		show_result(arr->dinner_shuffles(6));

	// Count tests
	Arrangements large(string(COUNTLIM, 'a'));

	Timer t_pc("panel count", true);
	n = 1;
	cout << "\nPanel Shuffle Count Table (0.1 seconds)\n";
	cout << "     N  panel(N)\n";
	
	while (n < COUNTLIM and t_pc.time() < 0.1) {
		t_pc.start();
		double pc = large.panel_count(n);
		t_pc.stop();
		cout << std::setw(6) << n << " "
			<< std::setw(6) << pc << "\n";
		n++;
	}


	Timer t_dc("dinner count", true);
	n = 1;
	cout << "\nDinner Shuffle Count Table (0.1 seconds)\n";
	cout << "     N  dinner(N)\n";

	while (n < COUNTLIM and t_dc.time() < 0.1) {
		t_dc.start();
		double dc = large.dinner_count(n);
		t_dc.stop();
		cout << std::setw(6) << n << " "
			<< std::setw(6) << dc << "\n";
		n++;
	}
	
	Timer t_panel("panel", true);
	n = 4;
	cout << "\nHow many panel shuffles can be created in 0.5 seconds?\n";
	
	while (t_panel.time() < 0.5) {
		double last = t_panel.time();
		t_panel.start();
		vector<string> res = standard.panel_shuffles(n);
		t_panel.stop();
		show_partial_result("panel", res, n);
		cout << "time " << t_panel.time() - last << "\n";
		n++;
	}
	
	int largest_panel = n - 1;
	
	Timer t_dinner("dinner timing", true);
	n = 4;
	cout << "\nHow many dinner shuffles can be created in 0.5 seconds?\n";

	while (t_dinner.time() < 0.5) {
		double last = t_dinner.time();
		t_dinner.start();
		vector<string> res = standard.dinner_shuffles(n);
		t_dinner.stop();
		show_partial_result("dinner", res, n);
		cout << "time " << t_dinner.time() - last << "\n";
		n++;
	}
	cout << "\nLargest panel shuffles performed: "
		<< largest_panel << "\n";
	cout << "\nLargest dinner shuffles performed: " << n - 1 << "\n";
	
	// Error checking
	Arrangements small("abcd");
	cout << "\nError Handling Tests\n";

	try {
		small.panel_count(5);
	}
	catch (int n) {
		cout << n;
	}
	try {
		small.panel_shuffles(6);
	}
	catch (int n) {
		cout << n;
	}
	try {
		small.dinner_count(7);
	}
	catch (int n) {
		cout << n;
	}
	try {
		small.dinner_shuffles(89);
	}
	catch (int n) {
		cout << n;
	}
	try {
		large.dinner_shuffles(122);
	}
	catch (int n) {
		cout << n;
	}
	try {
		numbers.dinner_shuffles(9);
	}
	catch (int n) {
		cout << n;
	}
	try {
		numbers.dinner_shuffles(10);
	}
	catch (int n) {
		cout << n;
	}
	cout << "\n";
	
}


void interactive_main() {
	std::string asktype, symbols;
	int number;
	cout << "Type quit to exit.\n";
	cout << "Commands:\npc names n\nps names n\ndc names n\nds names n\n";
	cout.precision(15);

	while (true) {
		std::cin >> asktype;
		if (asktype == "quit") break;
		std::cin >> symbols;
		Arrangements h(symbols);
		std::cin >> number;
		if (asktype == "pc") {
			std::cout << "panel_count(" << number << ") = ";
			std::cout << h.panel_count(number) << "\n";
		}
		else if (asktype == "ps") {
			std::cout << "panel_shuffles(" << number << ") = ";
			for (auto e : h.panel_shuffles(number))
				std::cout << e << " ";
			std::cout << "\n";
		}
		else if (asktype == "dc") {
			std::cout << "dinner_count(" << number << ") = ";
			std::cout << h.dinner_count(number) << "\n";
		}
		else if (asktype == "ds") {
			std::cout << "dinner_shuffles(" << number << ") = ";
			for (auto e : h.dinner_shuffles(number))
				std::cout << e << " ";
			std::cout << "\n";
		}
	}
}
#endif