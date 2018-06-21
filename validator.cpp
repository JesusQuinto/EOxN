#include "validator.h"
#include "zodiacgraph/scenehandle.h"
#include "zodiacgraph/nodehandle.h"
#include "zodiacgraph/scene.h"
#include "mainctrl.h"

#include "zodiacgraph/node.h"
#include  "zodiacgraph/scene.h"
#include "nodectrl.h"
#include "mainctrl.h"

#include <QDebug>

Validator::Validator(QObject *parent, MainCtrl *mainCtr)
    : QObject(parent),
      m_mainCtrl(mainCtr)
{}


void Validator::hasCicle()
{
       zodiac::SceneHandle *scene =  m_mainCtrl->getSocialScene();
       QList<zodiac::NodeHandle> nodes = scene->getNodes();

       amountNodes = nodes.size();
       nroNodos = 0;
       visitados.clear();

        _dfs(nodes.first());

      qDebug() << "visitado:" << visitados<< endl;
}

void Validator::_dfs(zodiac::NodeHandle node)
{
  if(visitados[node.getId().toString()] == true)
      return;

  visitados[node.getId().toString()] = true;
  nroNodos++;

  if(nroNodos == amountNodes)
      return;

  for (auto plug: node.getPlugs())
  {
      if(plug.getConnectedPlugs().size() > 0)
      {
          zodiac::NodeHandle otherNode
                  = plug.getConnectedPlugs().first().getNode();
          Validator::_dfs(otherNode);
      }
  }
}


