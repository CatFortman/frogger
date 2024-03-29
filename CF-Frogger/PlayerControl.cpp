#include "PlayerControl.h"
#include "Plane.h"
#include "Category.h"

namespace GEX 
{
	struct playerMover{
	
		playerMover(float vx, float vy) : velocity(vx, vy){}
		void operator() (Cat& plane, sf::Time) const {
			plane.accelerate(velocity);
		}

		sf::Vector2f velocity;
	};

	PlayerControl::PlayerControl() :
		_missionStatus(MissionStatus::Active)
	{
		initalizeKeyBindings();
		initalizeActionBindings();
	}

	void PlayerControl::handleEvent(const sf::Event& events, CommandQueue & commands)
	{
		if (events.type == sf::Event::KeyPressed)
		{
			// check if key is bound to action
			// and that its not handled as a real event
			auto found = _keyBindings.find(events.key.code);
			if (found != _keyBindings.end() && !isRealTimeAction(found->second))
			{
				commands.push(_actionBindings[found->second]);
			}
		}
	}

	void PlayerControl::handleRealTimeInput(CommandQueue& commands)
	{

		for (auto pair : _keyBindings)
		{
			if (sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second))
				commands.push(_actionBindings[pair.second]);
		}
	}
	bool PlayerControl::isRealTimeAction(Action action)
	{
		switch (action)
		{
		case Action::moveDown:
		case Action::moveLeft:
		case Action::moveRight:
		case Action::moveUp:
			return true;
		default:
			break;
		}
		return false;
	}

	void PlayerControl::initalizeKeyBindings()
	{
		_keyBindings[sf::Keyboard::A] = Action::moveLeft;
		_keyBindings[sf::Keyboard::D] = Action::moveRight;
		_keyBindings[sf::Keyboard::W] = Action::moveUp;
		_keyBindings[sf::Keyboard::S] = Action::moveDown;
	}

	void PlayerControl::initalizeActionBindings()
	{
		const float playerSpeed = 200.f;
		const float rotation = .30f;

		_actionBindings[Action::moveLeft].action		= derivedAction<Cat>(playerMover(-playerSpeed, 0));
		_actionBindings[Action::moveRight].action       = derivedAction<Cat>(playerMover(playerSpeed, 0));
		_actionBindings[Action::moveUp].action			= derivedAction<Cat>(playerMover(0, -playerSpeed));
		_actionBindings[Action::moveDown].action		= derivedAction<Cat>(playerMover(0, playerSpeed));

		for (auto& pair : _actionBindings)
			pair.second.category = Category::playerCharacter;
	}

	void PlayerControl::setMissionStatus(MissionStatus status)
	{
		_missionStatus = status;
	}

	MissionStatus PlayerControl::getMissionStatus() const
	{
		return _missionStatus;
	}
}

