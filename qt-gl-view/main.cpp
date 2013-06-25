#include <QApplication>
#include "qglview.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QString url;
    if (argc > 1)
        url = QString(argv[1]);

    QGLView view(url);
    view.show();
    return app.exec();
}
