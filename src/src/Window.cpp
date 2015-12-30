#include "Window.h"

#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Window::Window(const std::string& name, Vector2 size) {
	_size = size;
	_name = name;
	_currState = Uninitalized;
}

Window::Window() {
	_name = _name;
	_size = _size;
	_currState = Uninitalized;
}

Window::~Window() {

}

///////////////////////////////////////////////////////////////////////////////
// State Methods
///////////////////////////////////////////////////////////////////////////////
void Window::init() {
	if (_currState != Uninitalized) {
		CORE_ERROR("[%s] Already initalized. Cannot init", _name.c_str());
		return;
	}
	setState(Paused);
	CORE_INFO("[%s] Initalized", _name.c_str());
}

void Window::reset() {
	if (_currState != Running) {
		CORE_WARNING("[%s] Not running. Resetting anyways", _name.c_str());
	}
	CORE_INFO("[%s] Reset", _name.c_str());
}

void Window::pause() {
	if (_currState != Running) {
		CORE_ERROR("[%s] Not running. Cannot pause", _name.c_str());
		return;
	}
	setState(Paused);
	CORE_INFO("[%s] Paused", _name.c_str());
}

void Window::resume() {
	if (_currState != Paused) {
		CORE_ERROR("[%s] Not paused and attempted to resume", _name.c_str());
		return;
	}
	setState(Running);
	CORE_INFO("[%s] Resuming", _name.c_str());
}

void Window::close() {
	if (_currState != Paused) {
		CORE_WARNING("[%s] CLOSING WHILE NOT PAUSED.", _name.c_str());
	}
	CORE_INFO("[%s] Marked for removal", _name.c_str());
}

void Window::setState(WindowState state) {
	CORE_INFO("[%s] State change (%s -> %s)", _name.c_str(),
		getStateString(_currState).c_str(), getStateString(state).c_str());
	_currState = state;
}

// Return a String literal of our current State
const std::string Window::getStateString() {
	return getStateString(_currState);
}

// If a State is specified instead return the String literal of that state
const std::string Window::getStateString(WindowState state) {
	if (state == Running) {
		return "Running";
	} else if (state == Paused) {
		return "Paused";
	} else if (state == Uninitalized) {
		return "Uninitalized";
	} else {
		return "INVALID";
	}
}

///////////////////////////////////////////////////////////////////////////////
// Updating Methods
///////////////////////////////////////////////////////////////////////////////
void Window::update(int diff) {

}

///////////////////////////////////////////////////////////////////////////////
// Event Methods
///////////////////////////////////////////////////////////////////////////////
void Window::handleEvent(sf::Event& e) {
	if (e.type == sf::Event::Closed) {
		CORE_WARNING("[%s] Given sf::Event::Closed. Discarding",_name.c_str());
	} else if (e.type == sf::Event::KeyPressed) {
		keyEvent(e);
	} else if (e.type == sf::Event::MouseButtonReleased) {
		mouseEvent(e);
	} else if (e.type == sf::Event::Resized) {
		resizeEvent(e);
	}
}

void Window::keyEvent(sf::Event& e) {

}

void Window::mouseEvent(sf::Event& e) {

}

void Window::resizeEvent(sf::Event& e) {

}

void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const {

}

void Window::render(sf::RenderWindow& target) {

}