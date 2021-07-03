#include <iostream>
#include <set>
#include <fstream>
#include <random>
#include <algorithm>
#include <cctype>

using namespace std;

set<string> getWords(ifstream & infile, unsigned int length)
{
	string s;
	set<string> set;

	while (infile >> s)
	{
		if (s.size() >= length)
		{
			transform(s.begin(), s.end(), s.begin(), [](char c){ return tolower(c);});
			set.insert(s);
		}
	}

	return set;
}

int main()
{
	constexpr int n = 6; // 6 letters + 1 essential one

	set<string> words;
	vector<char> letters;

	while (words.size() == 0)
	{
		letters.clear();

		// set up dictionary
		ifstream infile {"words.txt"};
		if (!infile)
		{
			cerr << "RIP" << endl;
			return 1;
		}
		words = getWords(infile, 4);

		// set up random letters
		random_device rd;
		default_random_engine g{rd()};
		{
			vector<char> alphabet;
			for (char c {'a'}; c <= 'z'; ++c)
				alphabet.push_back(c);

			shuffle(alphabet.begin(), alphabet.end(), g);
			letters.insert(letters.end(), alphabet.begin(), alphabet.begin() + n + 1);
		}

		// filter dictionary using random letters
		erase_if(words,
				 [&letters](auto const & word) {
					 // must consist of our selected letters
					 for (auto const & letter : word)
					 {
						 if (find(letters.begin(), letters.end(), letter) == letters.end())
							 return true;
					 }

					 // must contain the special letter
					 return (find(word.begin(), word.end(), letters[0]) == word.end());
				 });
	}

	// start the game
	for (const auto & letter : letters)
		cout << letter << " ";
	cout << endl << letters[0] << " must be in every word" << endl;

	unsigned int numAnswers {words.size()};
	if (numAnswers == 1)
		cout << "There is 1 word." << endl;
	else
		cout << "There are " << numAnswers << " words." << endl;

	// guess loop
	string guess;
	while (cin >> guess)
	{
		if (auto it = std::find(words.begin(), words.end(), guess); it != words.end())
		{
			words.erase(it);
			if (numAnswers == 1)
			{
				cout << "You win!";
				break;
			}
			cout << "Yay only " << --numAnswers << " " << (numAnswers == 1 ? "word" : "words") << " left!" << endl;
		}
		else
			cout << "Nope" << endl;
	}

	for (auto & word : words)
		cout << word << endl;

	return 0;
}
