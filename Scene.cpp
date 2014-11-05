#include <cstdlib>      // std::srand, std::rand
#include <ctime>        // std::time
#include <vector>       // std::vector
#include <algorithm>    // std::random_shuffle
#include <QDebug>
#include "Scene.h"

struct Cell
{
    int x1;
    int y1;
    int x2;
    int y2;
} s[100];

enum { Up, Down, Left, Right };

class Fruit
{
public:
    int x1, y1;
    int x2, y2;

    void drawApple()
    {
        glColor3f( 0.0f, 1.0f, 0.0f );
        glRectf( x1, y1, x2, y2 );
    }
} apples[10];

void newApples( std::vector<int> &xArr,
                std::vector<int> &yArr,
                Fruit *apples)
{
    std::random_shuffle( xArr.begin(), xArr.end() );
    std::random_shuffle( yArr.begin(), yArr.end() );

    for ( int i = 0; i < 10; ++i ) {
        apples[i].x1 = xArr[i];
        apples[i].y1 = yArr[i];
        apples[i].x2 = apples[i].x1 + 25;
        apples[i].y2 = apples[i].y1 - 25;
    }
}

Scene::Scene( QWidget *parent ) :
    QGLWidget( parent ),
    N( 30 ),
    M( 20 ),
    scale( 25 ),
    w( scale * N ),
    h( scale * M ),
    dir( 0 ),
    num( 4 ),
    isPause( true )
{
    this->setFocusPolicy( Qt::StrongFocus );

    // Initialize seed
    std::srand( std::time( NULL ) );

    for ( int x = -750/2; x < 750/2; x += 25 ) {
        xArr.push_back( x );
    }

    for ( int y = -500/2; y < 500/2; y += 25 ) {
        yArr.push_back( y );
    }

    newApples( xArr, yArr, apples );

    initSnake();

    // Set timer
    connect( &timer, SIGNAL( timeout() ),
             this, SLOT( slotUpdate() ) );
}

void Scene::initializeGL()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

void Scene::paintGL()
{
    // Clear the window with current clearing color
    glClear( GL_COLOR_BUFFER_BIT );

    for( int i = 0; i < 10; ++i ) {
        apples[i].drawApple();
    }

    drawField();
    drawSnake();
}

void Scene::resizeGL( int w, int h )
{
    // Set Viewport to window dimensions
    glViewport( 0, 0, w, h );

    // Reset coordinate system
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( -w/2, w/2, -h/2, h/2, 1.0, -1.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void Scene::keyPressEvent( QKeyEvent *event )
{
    switch( event->key() ) {
        case Qt::Key_Up:
            if ( dir != Down ) {
                dir = Up;
            }
            break;
        case Qt::Key_Down:
            if ( dir != Up ) {
                dir = Down;
            }
            break;
        case Qt::Key_Right:
            if ( dir != Left ) {
                dir = Right;
            }
            break;
        case Qt::Key_Left:
            if ( dir != Right ) {
                dir = Left;
            }
            break;
        case Qt::Key_Space:
            if ( isPause ) {
                isPause = false;
                timer.start( 100 );
            } else {
                isPause = true;
                timer.stop();
            }
            break;
    }

    updateGL();
}

void Scene::initSnake()
{
    s[0].x1 = 0;
    s[0].y1 = -150;
    s[0].x2 = s[0].x1 + scale;
    s[0].y2 = s[0].y1 - scale;

    s[1].x1 = 0;
    s[1].y1 = s[0].y1 - scale;
    s[1].x2 = s[1].x1 + scale;
    s[1].y2 = s[1].y1 - scale;

    s[2].x1 = 0;
    s[2].y1 = s[1].y1 - scale;
    s[2].x2 = s[2].x1 + scale;
    s[2].y2 = s[2].y1 - scale;

    s[3].x1 = 0;
    s[3].y1 = s[2].y1 - scale;
    s[3].x2 = s[3].x1 + scale;
    s[3].y2 = s[3].y1 - scale;

    num = 4;
    dir = 0;

    updateGL();
}

void Scene::drawField()
{
    glColor3f( 0.0f, 1.0f, 0.0 );

    glBegin( GL_LINES );
    {
        for( int i = -w/2; i < w/2; i += scale ) {
            glVertex2f( i, -h/2 ); glVertex2f( i, h/2 );
        }

        for( int j = -h/2; j < h/2; j += scale ) {
            glVertex2f( -w/2, j ); glVertex2f( w/2, j );
        }
    }
    glEnd();
}

void Scene::drawSnake()
{
    glColor3f( 0.0f, 0.0f, 1.0 );

    for( int i = 0; i < num; ++i ) {
        glRectf( s[i].x1, s[i].y1, s[i].x2, s[i].y2 );
        //        glRectf( s[i].x * scale, s[i].y * scale,
        //                 ( s[i].x + 1 ) * scale,
        //                 ( s[i].y + 1 ) * scale );
    }
}

void Scene::tick()
{
    // Save head
    Cell oldHead;
    oldHead = s[0];

    // Head
    // Up
    if( dir == Up ) {
        s[0].y1 += scale;
        s[0].y2 += scale;
    }

    // Down
    if( dir == Down ) {
        s[0].y1 -= scale;
        s[0].y2 -= scale;
    }

    // Left
    if( dir == Left ) {
        s[0].x1 -= scale;
        s[0].x2 -= scale;
    }

    // Right
    if( dir == Right ) {
        s[0].x1 += scale;
        s[0].x2 += scale;
    }

    // Tail
    for( int i = num - 1; i > 1; --i ) {
        s[i] = s[i-1];
    }
    s[1] = oldHead;

    // On yourself
    for ( int i = 1; i < num; ++i ) {
        if ( ( s[0].x1 == s[i].x1 ) &&
             ( s[0].y1 == s[i].y1 ) ) {
            emit signalSetStatus( QString( "Lose" ) );
            initSnake();
            return;
        }
    }

    // On border
    if ( ( s[0].x1 < -w/2 ) ||
         ( s[0].x2 > w/2 ) ||
         ( s[0].y2 < -h/2 ) ||
         ( s[0].y1 > h/2 ) ) {
        emit signalSetStatus( QString( "Lose" ) );
        initSnake();
        return;
    }

    int lastX1 = s[num-1].x1;
    int beforeLastX1 = s[num-1].x1;
    for ( int i = 0; i < 10; ++i ) {
        if ( ( s[0].x1 == apples[i].x1 ) && ( s[0].y1 == apples[i].y1 ) ) {
            // Where to add
            if ( lastX1 == beforeLastX1 ) {
                s[num].x1 = s[num-1].x1;
                s[num].x2 = s[num-1].x2;
                if ( dir == Up ) {
                    s[num].y1 = s[num-1].y1 - scale;
                    s[num].y2 = s[num-1].y2 - scale;
                } else {
                    s[num].y1 = s[num-1].y1 + scale;
                    s[num].y2 = s[num-1].y2 + scale;
                }
            } else {
                s[num].y1 = s[num-1].y1;
                s[num].y2 = s[num-1].y2;
                if ( dir == Right ) {
                    s[num].x1 = s[num-1].x1 - scale;
                    s[num].x2 = s[num-1].x2 - scale;
                } else {
                    s[num].x1 = s[num-1].x1 + scale;
                    s[num].x2 = s[num-1].x2 + scale;
                }
            }
            ++num;
            newApples( xArr, yArr, apples );
            emit signalSetStatus( QString::number( num ) );
            if ( num == 20 ) {
                initSnake();
            }
            return;
        }
    }
}

void Scene::slotUpdate()
{
    tick();
    updateGL();
}
