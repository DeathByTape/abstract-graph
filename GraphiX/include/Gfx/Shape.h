/**
 * Application: GraphiX Library
 * Module: Library
 * File: Shape.h
 * Description: Base class for shapes
 *
 * @author Dennis J. McWherter, Jr.
 * @version $Id$
 */

#ifndef GRAPHIX_GFX_SHAPE_H__
#define GRAPHIX_GFX_SHAPE_H__

#include "graphix_incl.h"
#include "Color.h"

namespace GRAPHIX
{
/**
 * Shapes that can be generated
 */
enum GRAPHIX_EXPORT SHAPES
{
  CIRCLE,
  LINE
};

/**
 * Base shape class for all
 * primitive shapes in the GraphiX scene
 */
class GRAPHIX_EXPORT Shape
{
public:
  /**
   * Constructor
   *
   * @param x   X coordinate
   * @param y   Y coordinate
   */
  Shape(float x, float y);
  
  /**
   * Constructor
   *
   * @param x   X coordinate
   * @param y   Y coordinate
   * @param c
   */
  Shape(float x, float y, Color c);

  /**
   * Destructor
   */
  virtual ~Shape();

  /**
   * Set X and Y coords respectively
   *
   * @param val   The new value
   */
  void setX(float val);
  void setY(float val);

  /**
   * Get X and Y coordinates respectively
   */
  float getX() const;
  float getY() const;
  
  /**
   * Set a new object color
   *
   * @param c   The new color
   */
  virtual void setColor(const Color& c);
  
  /**
   * Set a new object color
   *
   * @param r   Red color
   * @param g   Green color
   * @param b   Blue color
   * @param a   Alpha channel
   */
  virtual void setColor(float r, float g, float b, float a = 0.f);
  
  /**
   * Set the color's highlight (i.e. outline)
   *
   * @param color   The color to set the highlight
   */
  virtual void setHighlight(const Color& h);
  
  /**
   * Get the object's color
   *
   * @return  The object's color
   */
  virtual Color getColor() const;
  
  /**
   * Get the object's border color
   *
   * @return  The object's highlight color
   */
  virtual Color getHighlight() const;
  
  /**
   * Toggle selected
   */
  virtual void toggleSelected();
  
  /**
   * Check if shape is selected
   *
   * @return  True if selected, false otherwise
   */
  virtual bool isSelected() const;

  /**
   * Method to aid in picking
   *
   * @param idx   The name to give to a shape on the scene
   */
  virtual void pick(unsigned idx) const;
  
  /**
   * Draw a shape (pure virtual)
   */
  virtual void draw() const = 0;
  
  /**
   * Determine the shape type (pure virtual)
   */
  virtual SHAPES getType() const = 0;
  
private:
  float xCoord, yCoord;
  Color color;
  Color highlight;
  bool selected;
};
}
#endif /* GRAPHIX_GFX_SHAPE_H__ */

