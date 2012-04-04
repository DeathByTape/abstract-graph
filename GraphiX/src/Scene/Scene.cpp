/**
 * Application: GraphiX Library
 * Module: Library
 * File: Scene.cpp
 * Description: Scene implementation
 *
 * @author Dennis J. McWherter, Jr.
 * @version $Id$
 */

#include "Gfx/Shape.h"
#include "Gfx/Circle.h"

#include "Scene/Scene.h"

#include "Control/NodeCreationMode.h"
#include "Control/EdgeCreationMode.h"

#define GRAPHIX_DEFAULT_PICK_BUFFER_SIZE 16

namespace GRAPHIX
{
Scene::Scene()
{
  bufferSize = GRAPHIX_DEFAULT_PICK_BUFFER_SIZE;
  pickBuffer = new unsigned[bufferSize];
  
  modes[NODECREATION] = new NodeCreationMode(&shapes, &selected);
  modes[EDGECREATION] = new EdgeCreationMode(&shapes, &selected);
  
  // Default in Node Creation Mode for now
  currentMode = modes[NODECREATION];
  
  // Only supporting 2D for now
  glDisable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(139.0/255.0, 137.0/255.0, 137.0/255.0, 0.0);
}

Scene::~Scene()
{
  destroy();
}
  
Scene::Scene(const Scene& rhs)
{
  copy(rhs);
}

Scene& Scene::operator=(const Scene& rhs)
{
  if(this != &rhs) {
    destroy();
    copy(rhs);
  }
  return *this;
}

unsigned* Scene::updateScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(139.0/255.0, 137.0/255.0, 137.0/255.0, 0.0);
  glLoadIdentity();
  
  std::vector<Shape*>::const_iterator it;
  for(it = shapes.begin() ; it != shapes.end() ; ++it) {
    (*it)->draw();
  }
  return NULL;
}
  
void Scene::addShape(SHAPES shape, int xW, int yW)
{
  double x = 0, y = 0;
  float radius = .1f;
  Shape* newShape = NULL;
  
  updateViewport();
  windowToGL(xW, yW, x, y);
  
  if(pickScene(xW, viewport[3]-yW) > 0)
    return;
  
  switch(shape)
  {
    case CIRCLE:
      newShape = new Circle(x, y, radius);
      break;
    default:
      break;
  }
  
  if(newShape != NULL)
    shapes.push_back(newShape);
}

void Scene::registerClick(int xW, int yW)
{
  updateViewport();
  double x = 0, y = 0;
  windowToGL(xW, yW, x, y);
  unsigned hits = pickScene(xW, viewport[3]-yW);
  currentMode->handleClick(x, y, hits, pickBuffer);
}

void Scene::updateMode(GRAPHIX::MODES mode)
{
  currentMode = modes[mode];
  currentMode->removeAllHighlight();
}

void Scene::updateGLSize(int w, int h)
{
  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, h, 0, 0, 0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
               
void Scene::updateViewport()
{
  glGetIntegerv(GL_VIEWPORT, viewport);
}

unsigned Scene::pickScene(float x, float y)
{
  updateViewport();
  glSelectBuffer(bufferSize, pickBuffer);
  
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);
  
  // Prepare OpenGL for picking
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  gluPickMatrix(x, y, 25.0, 25.0, viewport);
  glOrtho(0, viewport[2], viewport[3], 0, 0, 0);
  
  // Perform the picking operation
  glMatrixMode(GL_MODELVIEW);
  
  currentMode->pickLogic();
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  
  unsigned hits = glRenderMode(GL_RENDER);
  
  glMatrixMode(GL_MODELVIEW);
  
  return hits;
}
               
void Scene::windowToGL(int winX, int winY, double& x, double& y) const
{
  double model[16], projection[16];
  double z;
  
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  
  gluUnProject(winX, winY, 0, model, projection, viewport, &x, &y, &z);
  
  y *= -1; // Window to OpenGL so invert y
}

void Scene::copy(const Scene& rhs)
{
  bufferSize = rhs.bufferSize;
  pickBuffer = new unsigned[bufferSize];
  
  for(unsigned i = 0 ; i < bufferSize ; ++i)
    pickBuffer[i] = rhs.pickBuffer[i];
  
  for(unsigned i = 0 ; i < rhs.shapes.size() ; ++i) {
    Shape* shape = rhs.shapes[i];
    Shape* newShape = NULL;
    switch(shape->getType())
    {
      case CIRCLE:
      {
        Circle* circle = static_cast<Circle*>(shape);
        shapes.push_back((newShape = new Circle(*circle)));
      }
        break;
      default:
        break;
    }
    if(newShape != NULL && newShape->isSelected())
      selected.push_back(newShape);
  }
  
  std::map<MODES, Mode*>::const_iterator it;
  for(it = rhs.modes.begin() ; it != rhs.modes.end() ; ++it) {
    MODES modeType = (*it).first;
    Mode* modePtr  = (*it).second;
    
    switch(modePtr->getMode()) {
      case NODECREATION:
      {
        NodeCreationMode* theMode = static_cast<NodeCreationMode*>(modePtr);
        modes[modeType] = new NodeCreationMode(*theMode);
      }
        break;
      case EDGECREATION:
      {
        EdgeCreationMode* theMode = static_cast<EdgeCreationMode*>(modePtr);
        modes[modeType] = new EdgeCreationMode(*theMode);
      }
        break;
      default:
        break;
    }
  }
}
  
void Scene::destroy()
{
  if(pickBuffer != NULL)
    delete pickBuffer;
  
  for(unsigned i = 0 ; i < shapes.size() ; ++i)
    delete shapes[i]; 
  
  std::map<MODES, Mode*>::iterator it;
  for(it = modes.begin() ; it != modes.end() ; ++it)
    delete (*it).second;
  
  shapes.clear();
  selected.clear();
  modes.clear();
}
  
void Scene::resizePickBuffer()
{
  // NOTE: Shrinking not yet implemented
  // but plans to do so if space complexity
  // becomes an issue
  unsigned newSize = bufferSize << 1;
  unsigned* newArr = new unsigned[newSize];
  
  for(unsigned i = 0 ; i < bufferSize ; ++i)
    newArr[i] = pickBuffer[i];
  
  delete pickBuffer;
  
  pickBuffer = newArr;
  bufferSize = newSize;
}
}
