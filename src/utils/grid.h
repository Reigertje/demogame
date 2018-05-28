#ifndef GRID_H
#define GRID_H

#include <vector>

template<class T> 
class Grid {

public:

	Grid();

	Grid(int width, int height);

	Grid(int width, int height, T initialValue);

	int getWidth() const;

	int getHeight() const;

	bool isOutOfBound(int x, int y) const;

	std::vector<T>& operator[](int x);

	const std::vector<T>& operator[](int x) const;

private:

	int width_;

	int height_;

	std::vector<std::vector<T>> grid_;

};

template <class T>
Grid<T>::Grid() {
	width_ = 0;
	height_ = 0;
}

template <class T>
Grid<T>::Grid(int width, int height) {
	width_ = width;
	height_ = height;
	grid_.resize(width_);
	for (int x = 0; x < width_; ++x) {
		grid_[x].resize(height_);
	}
}

template <class T>
Grid<T>::Grid(int width, int height, T initialValue) {
	width_ = width;
	height_ = height;
	grid_.resize(width_);
	for (int x = 0; x < width_; ++x) {
		grid_[x].resize(height_, initialValue);
		/*for (int y = 0; y < height_; ++y) {
			grid_[x][y] = initialValue;
		}*/
	}
}

template <class T>
int Grid<T>::getWidth() const {
	return width_;
}

template <class T>
int Grid<T>::getHeight() const {
	return height_;
}

template <class T>
bool Grid<T>::isOutOfBound(int x, int y) const {
	return x < 0 || x >= width_ || y < 0 || y >= height_;
}

template <class T>
std::vector<T>& Grid<T>::operator[](int x) {
	return grid_[x];
}

template <class T>
const std::vector<T>& Grid<T>::operator[](int x) const {
	return grid_[x];
}

#endif