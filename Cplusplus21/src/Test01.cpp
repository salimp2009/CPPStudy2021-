#include <stdio.h>
#include <iostream>

//  testing static values
class Player
{
private:
	inline static int NumOfPlayers = 0;

public:
	
	Player():value{0}
	{ 
		++NumOfPlayers; 
		printf("Num of players= %i \n", NumOfPlayers);

		//std::cout << "Num of players" << NumOfPlayers<<"\n";
	}
	~Player() 
	{ 
		if (NumOfPlayers > 0) --NumOfPlayers; 
		printf("Num of players= %i \n", NumOfPlayers);
		//std::cout << "Num of players" << NumOfPlayers << "\n";
	}

	static  int GetNumOfPlayers()

	{
		printf("Num of players= %i \n", NumOfPlayers);
		return NumOfPlayers;
	}

private:
	int value;

};




int main()
{
	
	Player::GetNumOfPlayers();
	Player myplayer{}, myplayer2{};
	Player::GetNumOfPlayers();

	return 0;

}