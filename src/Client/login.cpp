#include "login.h"

Login::Login(SDL_Renderer* rend, TTF_Font* f, float r_f, Socket* socket) {
	renderer = rend;
	font = f;
	render_factor = r_f;
	sock = socket;
}

void Login::print_username(std::string username) {
	SDL_Color foreground = { 0, 0, 0 };
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, username.c_str(), foreground);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect dest;
	dest.x = 227 * render_factor;
	dest.y = 252 * render_factor;
	dest.w = text_surface->w * render_factor;
	dest.h = text_surface->h * render_factor;
	SDL_RenderCopy(renderer, text, NULL, &dest);
	SDL_DestroyTexture(text);
	SDL_FreeSurface(text_surface);
}

void Login::print_password(std::string password) {
	SDL_Color foreground = { 0, 0, 0 };
	SDL_Surface* text_surf = TTF_RenderText_Solid(font, password.c_str(), foreground);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surf);
	SDL_Rect dest;
	dest.x = 227 * render_factor;
	dest.y = 290 * render_factor;
	dest.w = text_surf->w * render_factor;
	dest.h = text_surf->h * render_factor;
	SDL_RenderCopy(renderer, text, NULL, &dest);
	SDL_DestroyTexture(text);
	SDL_FreeSurface(text_surf);
}

void Login::print_response(std::string response) {
	SDL_Color foreground = { 255, 255, 255 };
	SDL_Surface* text_surf = TTF_RenderText_Solid(font, response.c_str(), foreground);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surf);
	SDL_Rect dest;
	dest.x = 207 * render_factor;
	dest.y = 360 * render_factor;
	dest.w = text_surf->w * render_factor;
	dest.h = text_surf->h * render_factor;
	SDL_RenderCopy(renderer, text, NULL, &dest);
	SDL_DestroyTexture(text);
	SDL_FreeSurface(text_surf);
}

void Login::render() {
	if (input.size()) {
		print_username(input);
	}
	if (input_p.size()) {
		print_password(input_p);
	}
	if (response_s.size()) {
		print_response(response_s);
	}
}

void Login::login() {
	char buffer[4];
	// Mando Usuario
	sock->int_to_big_endian(input.size(), buffer);
	sock->s_write(buffer, 4);
	sock->s_write(input.c_str(), input.size());

	// Mando Contrase�a
	sock->int_to_big_endian(input_password.length(), buffer);
	sock->s_write(buffer, 4);
	sock->s_write(input_password.c_str(), input_password.length());

	sock->s_read(buffer, 4);
	if (sock->big_endian_to_int(buffer) == 0) {
		response_s.assign("Logged in, Waiting for other Players\n");
		logged_in = true;
	} else if (sock->big_endian_to_int(buffer) == 1) {
		response_s.assign("Wrong User/Password\n");
	} else if (sock->big_endian_to_int(buffer) == 2) {
		response_s.assign("Server Full\n");
	} else {
		fprintf(stderr, "Respuesta no reconocida %d\n", sock->big_endian_to_int(buffer));
	}
}

bool Login::input_login(SDL_Event* event) {
	static const unsigned char* keys = SDL_GetKeyboardState(NULL);

	switch (event->type) {
	case SDL_QUIT:
		return false;
	case SDL_TEXTINPUT:
		if (current_input == "password") {
			input_p += '*';
			input_password += event->text.text;
		} else {
			input += event->text.text;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (in_bound_user(x, y)) {
			current_input = "username";
		} else if (in_bound_pass(x, y)) {
			current_input = "password";
		}
		break;
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_RETURN) {
			if (current_input == "password") {
				login();
			} else {
				response_s.assign(" Missing password");
			}
		} else if (event->key.keysym.sym == SDLK_BACKSPACE && current_input == "username") {
			if (input.length() > 0) {
				input.pop_back();
			}
		} else if (event->key.keysym.sym == SDLK_BACKSPACE && current_input == "password") {
			if (input_p.length() > 0) {
				input_p.pop_back();
				input_password.pop_back();
			}
		} else if (event->key.keysym.sym == SDLK_TAB && current_input == "username") {
			current_input = "password";
		}
		break;

	default:
		break;
	}

	return true;
}

bool Login::in_bound_user(int x, int y) {
	if (x > 167 * render_factor &&
		x < 452 * render_factor &&
		y > 245 * render_factor &&
		y < 279 * render_factor)
		return true;
	return false;
}

bool Login::in_bound_pass(int x, int y) {
	if (x > 167 * render_factor &&
		x < 452 * render_factor &&
		y > 283 * render_factor &&
		y < 319 * render_factor)
		return true;
	return false;
}
