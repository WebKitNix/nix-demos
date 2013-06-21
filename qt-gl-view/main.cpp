#include <QApplication>
#include "qglview.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QGLView view;
    view.show();
    return app.exec();
}
