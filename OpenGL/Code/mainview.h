#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MainView(QWidget *parent = 0);
    ~MainView();

    void updateRotation(int x, int y, int z);
    void updateModel(QString name);
    void updateShader(QString name);
    void updateScale(float scale);

    /* Add your public members below */

    /* End of public members */

    QVector3D convertHSLtoRGB(float H, float S, float L);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private:
    QOpenGLDebugLogger *debugLogger;

    void createShaderPrograms();
    void createBuffers();
    void updateBuffers();
    void updateUniforms();

    // Raytracer scene functions
    QVector3D rotate3D(QVector3D point, float angle);
    void renderCube(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos, GLint texptr_local, float scale_factor);
    void renderAnimation();

    /* Add your private members below */
    void loadModel(QString filename, GLuint bufferObject);

    // Shader programs, GLint for uniforms/buffer objects, other variables
    QOpenGLShaderProgram *mainShaderProg;

    QTimer timer; // timer used for animation

    Model *cubeModel;
    GLuint cubeBO;

    unsigned numTris;

    GLuint vao;
    GLuint coors;
    GLuint colours;
    GLuint normal_buffer;

    QMatrix4x4 model;
    float planet_rotations[10];
    QMatrix4x4 view;
    QMatrix4x4 projection;

    QMatrix3x3 normal_matrix;

    QVector3D colour;
    QVector4D material;
    QVector3D light;

    GLint viewptr;
    GLint modelptr;
    GLint projectionptr;

    GLint normal_matrixptr;

    GLint colourptr;
    GLint lightcolourptr;
    GLint materialptr;
    GLint lightptr;

    // Rotation parameters requirements
    QMatrix4x4 model_reset;
    int rotation_x;
    int rotation_y;
    int rotation_z;
    double scale;

    int rotation_x_start;
    int rotation_y_start;

    void determineRotationMatrix ();

    // Texture stuff
    GLuint texptr;
    GLuint texture_coordinates;
    GLint texture_uniform_ptr;

    GLuint texptr_2;
    GLuint texptr_3;
    GLuint texptr_4;
    GLuint texptr_5;
    GLuint texptr_6;
    GLuint texptr_7;
    GLuint texptr_8;
    GLuint texptr_9;
    GLuint texptr_10;

    void loadTexture (QString file, GLuint texptr);
    QVector<quint8> imageToBytes(QImage image);

    /* End of private members */

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
