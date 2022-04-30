#include <iostream>
#include <random>
#include <chrono>

size_t guess(size_t correct_number, int guesses, int minrange, int maxrange);
void play_game();
void play_again();
size_t validate_guess(int minrange, int maxrange);

size_t random_number(int minrange, int maxrange)
{
	/* std::random_device -> gets 8 bit of random integers
	*  std::seed_seq -> sets our current seed with some variables
	*  std::chrono::steady_clock... -> gets the current time and the gets the earliest time it could measure between now and uses it to generate PRNG
	*  std::mt199737 -> use mersene twister algorithm with seed to generate PRNG
	*  std::uniform_int_distribution number(1,100) convert PRNG into numbers between 1 to 100 to guess.
	*/

	std::random_device rd; 
	std::seed_seq ss{ rd(), rd(), static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()), rd(), rd() };
	std::mt19937 mt{ ss }; 

	std::uniform_int_distribution number(minrange, maxrange);

	return number(mt);
}

void play_again()
{
	char answer{ ' ' };
	std::cout << "would u like to play again? (y/n):"; std::cin >> answer;

	switch (answer)
	{
	case 'y':
		play_game();
	case 'n':
		std::cout << "Thank you for playing, see you next time!";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
		break;
	default:
		// check for partial extractions and ignore them and clear input buffer
		std::cout << "Invalid Input, please enter y or n\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		play_again();
	}
}

void play_game()
{
	int guesses{ 8 }; // number of guesses
	int minrange{ 1 }; // minimum range, for example 1 to x
	int maxrange{ 100 }; // maximum range for example x to 100

	std::cout << "Lets play a game. Im thinking of a number between " << minrange << " and " << maxrange << ", you have " << guesses << " guesses to guess what number im thinking off.\n";
	size_t correct_number{ random_number(minrange, maxrange) };
	guess(correct_number, guesses, minrange, maxrange);
	play_again();
}

int main()
{
	play_game();
	return EXIT_SUCCESS;
}

size_t guess(size_t correct_number, int guesses, int minrange, int maxrange)
{
	for (int i{1}; i <= guesses; i++)
	{
		std::cout << "Guess #" << i << ": ";
		size_t guess_answer{ validate_guess(minrange, maxrange) };

			if (guess_answer > correct_number)
			{
				std::cout << "your guess was too high.\n";
			}
			else if (guess_answer < correct_number)
			{
				std::cout << "your guess was too low.\n";
			}
			else if (guess_answer == correct_number)
			{
				std::cout << "you win.\n";
				return 0;
			}
	}
	std::cout << "you lost\n";
	std::cout << "the correct number was: " << correct_number << '\n';
	return 0;
}

size_t validate_guess(int minrange, int maxrange)
{
		size_t guess_answer{ 0 };
		std::cin >> guess_answer;

		if (std::cin.good())
		{
			if (guess_answer > maxrange)
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cerr << "Invalid guess, guess was higher than the allowed maximum range\n";
				validate_guess(minrange, maxrange);
			}
			else if (guess_answer < minrange)
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cerr << "Invalid guess, guess was lower than the allowed minimum range\n";
				validate_guess(minrange, maxrange);
			}
			else if ((guess_answer < maxrange) && (guess_answer > minrange))
			{
				// check for partial extractions and ignore them
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				return guess_answer;
			}
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cerr << "Invalid guess, please try again.\n";
			validate_guess(minrange, maxrange);
		}
}