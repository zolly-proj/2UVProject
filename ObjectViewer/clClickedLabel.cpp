#include "clClickedLabel.h"

clClickedLabel::clClickedLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    
}

clClickedLabel::~clClickedLabel() {}

void clClickedLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
