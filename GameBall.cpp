#include "StdAfx.h"
#include "GameBall.h"
#include "Game.h"
#include "ServiceLocator.h"
static GameObjectManager _gameObjectManager;
GameBall::GameBall() :
	_velocity(230.0f),
	_elapsedTimeSinceStart(0.0f),
	_runYet(false)

{
	//ServiceLocator::GetAudio()->PlaySound("audio/kaboom.wav");
	Load("images/ball.png");
	assert(IsLoaded());

	GetSprite().setOrigin(15,15);
	float random_integer = std::rand() % 360 + 1; //Gives random number between 1 and 360.
	_angle = random_integer;
	//_angle = (float)(sf::Randomizer::Random(0,360);
}


GameBall::~GameBall()
{
}


void GameBall::Update(float elapsedTime) //Parameter is the time since last frame in seconds. VERY small number.
{
	//ServiceLocator::GetAudio()->PlaySound("audio/Soundtrack.ogg");

	if(_runYet == false)
		_runYet = true;
	else
	{
		_elapsedTimeSinceStart += elapsedTime;

		// Delay game from starting until 3 seconds have passed
		if(_elapsedTimeSinceStart < 3.0f)
			return;

		float moveAmount = _velocity  * elapsedTime;
	
	
		float moveByX = LinearVelocityX(_angle) * moveAmount;
		float moveByY = LinearVelocityY(_angle) * moveAmount;

	
		//collide with the left side of the screen
		if(GetPosition().x + moveByX <= 0 + GetWidth()/2 || GetPosition().x + GetHeight()/2 + moveByX >= Game::SCREEN_WIDTH)
		{
			//Ricochet!
			_angle = 360.0f - _angle;
			if(_angle > 260.0f && _angle < 280.0f) _angle += 20.0f;
			if(_angle > 80.0f && _angle < 100.0f) _angle += 20.0f;
			moveByX = -moveByX;
		}
	


		PlayerPaddle* player1 = dynamic_cast<PlayerPaddle*>(Game::GetGameObjectManager().Get("Paddle1"));
		if(player1 != NULL)
		{
			sf::Rect<float> p1BB = player1->GetBoundingRect();
	
			if(p1BB.intersects(GetBoundingRect()))       //(GetPosition().x + moveByX + (GetSprite().GetSize().x /2),GetPosition().y + (GetSprite().GetSize().y /2) + moveByY))
			{ 
				_angle =  360.0f - (_angle - 180.0f);
				if(_angle > 360.0f) _angle -= 360.0f;

				moveByY = -moveByY;
		
				/*//iki kadang gawe ngebug sudut mantul bolane, tak koment 
				float playerVelocity = player1->GetVelocity();
		
				if(playerVelocity < 0)
				{
					// moving left
					_angle -= 20.0f;
					if(_angle < 0 ) _angle = 360.0f - _angle;
				}
				else if(playerVelocity > 0)
				{
					_angle += 20.0f;
					if(_angle > 360.0f) _angle = _angle - 360.0f;
				}*/

				ServiceLocator::GetAudio()->PlaySound("audio/kaboom.wav");
				_velocity += 5.0f;
			}


	
			//if(GetPosition().y - GetSprite().GetSize().y/2 - moveByY <= 0 || GetPosition().y + GetSprite().GetSize().y/2 + moveByY >= Game::SCREEN_HEIGHT)
			//Player 2 Scores
			if(GetPosition().y + GetHeight()/2 + moveByY >= Game::SCREEN_HEIGHT)
			{
				Game::scoreP2+=1;
				ServiceLocator::GetAudio()->PlaySound("audio/goal.wav"); //player2 goal
				// move to middle of the screen for now and randomize angle
				GetSprite().setPosition(Game::SCREEN_WIDTH/2, Game::SCREEN_HEIGHT/2);
				_angle = (rand()%360)+1;
				//_velocity = 230.0f;
				_elapsedTimeSinceStart = 0.0f;
			}

		}

		PlayerPaddle2* player2 = dynamic_cast<PlayerPaddle2*>(Game::GetGameObjectManager().Get("Paddle2"));
		if (player2 != NULL)
		{
			sf::Rect<float> p2BB = player2->GetBoundingRect();

			if (p2BB.intersects(GetBoundingRect()))       //(GetPosition().x + moveByX + (GetSprite().GetSize().x /2),GetPosition().y + (GetSprite().GetSize().y /2) + moveByY))
			{
				_angle = 360.0f - (_angle - 180.0f);
				if (_angle > 360.0f) _angle -= 360.0f;



				moveByY = -moveByY;

				

				/*//iki kadang gawe ngebug sudut mantul bolane, tak koment
				float playerVelocity = player2->GetVelocity();

				if (playerVelocity < 0)
				{
					// moving left
					_angle -= 20.0f;
					if (_angle < 0) _angle = 360.0f - _angle;
				}
				else if (playerVelocity > 0)
				{
					_angle += 20.0f;
					if (_angle > 360.0f) _angle = _angle - 360.0f;
				}*/

				ServiceLocator::GetAudio()->PlaySound("audio/kaboom.wav");
				_velocity += 5.0f;
			}



			//if(GetPosition().y - GetSprite().GetSize().y/2 - moveByY <= 0 || GetPosition().y + GetSprite().GetSize().y/2 + moveByY >= Game::SCREEN_HEIGHT)
			//Player 1 Scores
			if (GetPosition().y - GetHeight() / 2 - moveByY < 0 + GetWidth() / 2)
			{
				Game::scoreP1+=1;
				ServiceLocator::GetAudio()->PlaySound("audio/goal.wav");
				// move to middle of the screen for now and randomize angle
				GetSprite().setPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
				_angle = (rand() % 360) + 1;
				_elapsedTimeSinceStart = 0.0f;

			}

		}
		GetSprite().move(moveByX, moveByY);
		GetSprite().move(moveByX, moveByY);
	}
}

void GameBall::updVelo(float mod) {
	if (mod == 0) {
		_velocity = 100.0f;
	}
	else if (mod == 1) {
		_velocity = 230.0f;
	}
	else if (mod == 2) {
		_velocity = 430.0f;
	}
}

float GameBall::LinearVelocityX(float angle)
{
	angle -= 90;
    if (angle < 0) angle = 360 + angle;
		return (float)std::cos( angle * ( 3.1415926 / 180.0f ));
}

float GameBall::LinearVelocityY(float angle)
{
	angle -= 90;
    if (angle < 0) angle = 360 + angle;
		return (float)std::sin( angle * ( 3.1415926 / 180.0f ));
}
