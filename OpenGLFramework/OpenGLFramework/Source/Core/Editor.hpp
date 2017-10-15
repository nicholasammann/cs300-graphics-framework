#pragma once

namespace elba
{

  class Application;

  class Node;
  class NodeLink;

  class Editor
  {
  public:

    Editor(Application *aApp);
    
    void Update();

  private:

    Application *mApp;

    int mWindowFlags;

    ImVector<Node*> mNodes;
    ImVector<NodeLink*> mNodeLinks;
    int mCurrentNode;
    int mListHoveredNode;
    int mSceneHoveredNode;
    bool mOpenContextMenu;
    bool mShowGrid;

    ImVec2 mScrolling;

  };

}