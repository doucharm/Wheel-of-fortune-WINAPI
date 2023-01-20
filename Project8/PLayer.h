#pragma once
class Player
{
private:
	int Money;
public:

	 Player()
	 {
		Money = 0;
	 }
	 void Guess_Correct_Letter(int occurence, int value)
	 {
		 Money += occurence * value;
	 }
	 void Phrase_Guessed()
	 {
		 Money = Money + 1200;
	 }
	 int Get_Cash()
	 {
		 return Money;
	 }
	 void Lose_Money()
	 {
		 Money = (Money / 20);
		 Money*=10;
	 }
	 void Phrase_Failed()
	 {
		 Money = Money - 500;
	 }
};

