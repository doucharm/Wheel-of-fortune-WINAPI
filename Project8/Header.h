#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<math.h>
using namespace std;
class String_Reveal
{
public:
	std::string line;
	vector<bool>is_revealed;
	void All_Correct()
	{
		for (int i = 0; i < is_revealed.size(); i++)
		{
			is_revealed.at(i) = true;
		}
	}
};
void Get_Question_and_Answer(string* question, string* answer)
{
	srand(time(0));
	fstream question_file;
	question_file.open("question_bank.txt");
	string pom = "";
	int k1 = rand() % 4;
	int i = 0;
	while (!question_file.eof())
	{
		string pom1;
		streamsize m = 100;
		getline(question_file, pom1);

		if (i == k1)
		{
			pom = pom1.substr(0, pom1.find(':'));
			pom1 = pom1.substr(pom1.find(":") + 1);
			vector<string>Answers;
			stringstream ss(pom1);
			string pom2;
			while (getline(ss, pom2, ';'))
			{
				Answers.push_back(pom2);
			}
			int k2 = rand() % Answers.size();
			*question = pom;
			*answer = Answers.at(k2);
		}
		i++;
	}



}
String_Reveal Get_Guess_String(string answer)
{
	String_Reveal guessing_string;
	guessing_string.line = answer;
	for (int i = 0; i < answer.length(); i++)
	{
		if (answer[i] == ' ')
		{
			guessing_string.is_revealed.push_back(true);
		}
		else guessing_string.is_revealed.push_back(false);
	}
	return guessing_string;
}

int Count_Occurences(string line, char letter)
{
	int number = 0;
	for (int i = 0; i < line.length(); i++)
	{
		if (letter == line[i])
		{
			number++;
		}
	}
	return number;
}
void Check_Guess_and_Display(String_Reveal* guess_line, char guess)
{
	for (int i = 0; i < guess_line->line.length(); i++)
	{
		if (guess == guess_line->line[i])
		{
			guess_line->is_revealed.at(i) = true;
		}
	}
}
int Get_Procent(String_Reveal answered)
{
	int count = 0;
	for (int i = 0; i < answered.line.length(); i++)
	{
		if (answered.is_revealed.at(i) == true)
		{
			count++;
		}
	}
	return count * 100 / answered.line.length();
}

