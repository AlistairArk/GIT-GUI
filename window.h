#pragma once

#include <QMainWindow>

class QLabel;
class QWidget;

class HelloWindow: public QMainWindow {
public:
	HelloWindow();

private:
	void createWindowContent();
	void createMenuAndStatusBar();

private: // widgets we own
	QLabel* _label;
	QWidget* _mainWidget;
};
