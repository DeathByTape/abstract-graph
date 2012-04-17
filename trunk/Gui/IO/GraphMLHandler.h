/**
 * Application: AbstractGraph
 * Module: GUI
 * File: IO/GraphMLHandler.h
 * Description: XML handler for GraphML XML formats
 *
 * @author Dennis J. McWherter, Jr.
 * @version $Id$
 */

#ifndef IO_GRAPHMLHANDLER_H__
#define IO_GRAPHMLHANDLER_H__

#include <map>
#include <QXmlDefaultHandler>

// GraphiX Library
#include "graphix.h"

// Glue
#include "Glu/AlgorithmsGlu.h"

class GraphMLHandler : public QXmlDefaultHandler
{
public:
  /**
   * Constructor
   *
   * @param scene   The scene to update
   * @param glu     The glue between scene and algorithms
   */
  GraphMLHandler(GRAPHIX::Scene& scene, AlgorithmsGlu& glu);
  
  /**
   * Destructor
   */
  virtual ~GraphMLHandler();
  
  /**
   * Override of startElement
   */
  virtual bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts);
  
  /**
   * Override of endElement
   */
  virtual bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);

protected:
  /**
   * Parse graph elements
   *
   * @param atts    Attributes
   * @return  True on success, false otherwise
   */
  virtual bool parseGraphElm(const QXmlAttributes& atts);
  
  /**
   * Parse graph nodes
   *
   * @param atts    Attributes
   * @return  True on success, false otherwise
   */
  virtual bool parseNode(const QXmlAttributes& atts);
  
  /**
   * Parse graph edges
   *
   * @param atts    Attributes
   * @return  True on success, false otherwise
   */
  virtual bool parseEdge(const QXmlAttributes& atts);
  
private:
  GRAPHIX::Scene& scene;
  AlgorithmsGlu& glu;
  bool done;
  std::map<QString, int> idMap;
};

#endif /* IO_GRAPHMLHANDLER_H__ */
