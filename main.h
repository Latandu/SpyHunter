void CheckForBMPErrors(SDL_Surface* charset, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer);

void DeltaTime(int& t2, double& delta, int& t1, int death, int pause, double& worldTime, float& score, double& distance, double etiSpeed);

void EventHandler(SDL_Event& event, int& quit, int& death, double& etiSpeed, float& score, float& xMovement, double& worldTime, int& pause);

void MovementHandler(const Uint8* keystate, int death, int pause, float& xMovement, double& etiSpeed);

void CollisionHandler(float xMovement, int& death, double screenLeftX, double screenRightX, double screenLeftXBefore, double screenRightXBefore, int currentRect);

void KeyEventHandler(SDL_Event& event, int& quit, int& death, double& etiSpeed, float& score, float& xMovement, double& worldTime, int& pause);

void StringRectangle(SDL_Surface* screen, int czerwony, int niebieski, int death, char  text[128], SDL_Surface* charset, int pause, double worldTime, double fps, float score);

int InitializationHandler(int& rc, SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& screen, SDL_Texture*& scrtex, bool& retflag);

void TextureHandler(float xMovement, int& death, SDL_Surface* screen, int czarny, int zielony, SDL_Surface* eti, int distance, double& screenLeftX, double& screenLeftXBefore,
	double& screenRightX, double& screenRightXBefore, int& distanceBefore, int& distanceRight, int& distanceRightBefore, int& currentRect);
