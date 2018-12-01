class WindowManager {
	private:
		unsigned int _width;
		unsigned int _height;

		void setWindowSize(unsigned int width, unsigned int height) {
			_width = width;
			_height = height;
		}

	public:
		static WindowManager& getManager() {
			static WindowManager manager;
			return manager;
		}

		static void init(unsigned int width, unsigned int height) {
			getManager().setWindowSize(width, height);
		}

		inline unsigned int getWindowWidth() {	return _width; }
		inline unsigned int getWindowHeight() { return _height; }
};