#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>

class Scene : public QGLWidget
{
    Q_OBJECT

public:
    Scene( QWidget *parent = 0 );

signals:
    void signalSetStatus( const QString &status );

public slots:
    void initSnake();

private slots:
    void slotUpdate();

private:
    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h );

    void keyPressEvent( QKeyEvent *event );

    void drawField();
    void drawSnake();
    void tick();

private:
    QTimer timer;
    const int N;
    const int M;
    const int scale;
    int w;
    int h;
    int dir;
    int num;
    std::vector<int> xArr;
    std::vector<int> yArr;
    bool isPause;
};

#endif // SCENE_H
