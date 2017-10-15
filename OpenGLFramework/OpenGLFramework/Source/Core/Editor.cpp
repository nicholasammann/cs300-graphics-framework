#include <string>

#include "../ImGui/imgui.h"

#include "../Nodes/Node.hpp"

#include "Application.hpp" 
#include "Editor.hpp"

const float NODE_SLOT_RADIUS = 4.0f;
const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

namespace elba
{

  ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) 
  { 
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
  }

  ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
  { 
    return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
  }

  Editor::Editor(Application *aApp) 
    : mApp(aApp), mWindowFlags(0), 
    mListHoveredNode(-1), mSceneHoveredNode(-1), 
    mOpenContextMenu(false), mScrolling(0, 0)
  {
    //mWindowFlags = ImGuiWindowFlags_NoTitleBar;
    mWindowFlags = ImGuiWindowFlags_NoResize;
    mWindowFlags |= ImGuiWindowFlags_NoMove;


    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

    ImGui::SetNextWindowSize(
      ImVec2(mApp->GetWindowWidth(), mApp->GetWindowHeight()),
      ImGuiCond_Once);


    mNodes.push_back(new Node(0, "MainTex", ImVec2(40, 50), ImColor(255, 100, 100), 1, 1));
    mNodes.push_back(new Node(1, "BumpMap", ImVec2(40, 150), ImColor(200, 100, 200), 1, 1));
    mNodes.push_back(new Node(2, "Combine", ImVec2(270, 80), ImColor(0, 200, 100), 2, 2));
    mNodeLinks.push_back(new NodeLink(0, 0, 2, 0));
    mNodeLinks.push_back(new NodeLink(1, 0, 2, 1));
  }

  void Editor::Update()
  {
    bool open = true;

    ImGui::Begin("Start", &open, mWindowFlags);


    // Node List
    ImGui::BeginChild("NodeList", ImVec2(100, 0));

    ImGui::Text("Nodes");
    ImGui::Separator();

    for (int i = 0; i < mNodes.size(); ++i)
    {
      Node *node = mNodes[i];

      ImGui::PushID(node->GetID());

      if (ImGui::Selectable(node->GetName().c_str(), node->GetID() == mCurrentNode))
      {
        mCurrentNode = node->GetID();
      }

      if (ImGui::IsItemHovered())
      {
        mListHoveredNode = node->GetID();
        mOpenContextMenu |= ImGui::IsMouseClicked(1);

      }

      ImGui::PopID();

    }

    ImGui::EndChild();

    ImGui::SameLine();

    // text header above scene
    ImGui::BeginGroup();
    ImGui::Text("Hold middle mouse button to scroll");
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::Checkbox("Show Grid", &mShowGrid);
    
    
    // scene
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

    int sceneFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;

    ImGui::BeginChild("Scene", ImVec2(0, 0), true, sceneFlags);

    ImGui::PushItemWidth(120.0f);

    ImVec2 offset = ImGui::GetCursorScreenPos() - mScrolling;

    ImDrawList *drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(2);

    // draw grid
    if (false)
    {
      ImU32 gridCol = ImColor(200, 200, 200, 40);
      float gridSize = 64.0f;
      ImVec2 winPos = ImGui::GetCursorScreenPos();
      ImVec2 canvasSize = ImGui::GetWindowSize();

      // vertical lines
      for (float x = fmodf(offset.x, gridSize); x < canvasSize.x; x += gridSize)
      {
        ImVec2 a = ImVec2(x, 0.0f) + winPos;
        ImVec2 b = ImVec2(x, canvasSize.y) + winPos;
        drawList->AddLine(a, b, gridCol);
      }

      // horizontal lines
      for (float y = fmodf(offset.y, gridSize); y < canvasSize.y; y += gridSize)
      {
        ImVec2 a = ImVec2(0.0f, y) + winPos;
        ImVec2 b = ImVec2(canvasSize.x, y) + winPos;
        drawList->AddLine(a, b, gridCol);
      }
    }


    // draw node links
    drawList->ChannelsSetCurrent(0);

    for (int i = 0; i < mNodeLinks.size(); ++i)
    {
      NodeLink *link = mNodeLinks[i];

      Node *input = mNodes[link->mInputIndex];
      Node *output = mNodes[link->mOutputIndex];

      ImVec2 a = offset + input->GetOutputSlotPos(link->mInputSlot);
      ImVec2 b = offset + output->GetInputSlotPos(link->mOutputSlot);

      drawList->AddBezierCurve(
        a, 
        a + ImVec2(50, 0), 
        b + ImVec2(-50, 0), 
        b, 
        ImColor(200, 200, 100), 3.0f
      );
    }



    // draw nodes
    for (int i = 0; i < mNodes.size(); ++i)
    {
      Node *node = mNodes[i];

      ImGui::PushID(node->GetID());

      ImVec2 node_rect_min = offset + node->mPos;


      // display node contents first
      drawList->ChannelsSetCurrent(1);
      bool old_any_active = ImGui::IsAnyItemActive();

      ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
      
      ImGui::BeginGroup();
      ImGui::Text("%s", node->GetName().c_str());
      ImGui::EndGroup();


      // save the size of what we have emitted and 
      // whether any of the widgets are being used
      bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());

      node->mSize = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
      ImVec2 node_rect_max = node_rect_min + node->mSize;


      // draw node box
      drawList->ChannelsSetCurrent(0);

      ImGui::SetCursorScreenPos(node_rect_min);
      ImGui::InvisibleButton("node", node->mSize);
      
      if (ImGui::IsItemHovered())
      {
        mSceneHoveredNode = node->GetID();
        mOpenContextMenu |= ImGui::IsMouseClicked(1);
      }

      bool node_moving_active = ImGui::IsItemActive();

      if (node_widgets_active || node_moving_active)
      {
        mCurrentNode = node->GetID();
      }

      if (node_moving_active && ImGui::IsMouseDragging(0))
      {
        node->mPos = node->mPos + ImGui::GetIO().MouseDelta;
      }

      // check if node is hovered or selected
      ImU32 node_bg_color = (mListHoveredNode == node->GetID() ||
                             mListHoveredNode == node->GetID() ||
                            (mListHoveredNode == -1 && mCurrentNode) ? 
                             ImColor(75, 75, 75) : ImColor(60, 60, 60));

      drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
      drawList->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);

      for (int i = 0; i < node->mNumInputs; ++i)
      {
        drawList->AddCircleFilled(offset + node->GetInputSlotPos(i), NODE_SLOT_RADIUS, ImColor(150, 150, 150, 150));
      }

      for (int i = 0; i < node->mNumOutputs; ++i)
      {
        drawList->AddCircleFilled(offset + node->GetOutputSlotPos(i), NODE_SLOT_RADIUS, ImColor(150, 150, 150, 150));
      }

      ImGui::PopID();

    }

    drawList->ChannelsMerge();


    // context menu
    if (ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
    {
      mCurrentNode = mListHoveredNode = mSceneHoveredNode = -1;
      mOpenContextMenu = true;
    }

    if (mOpenContextMenu)
    {
      ImGui::OpenPopup("ContextMenu");

      if (mListHoveredNode != -1)
      {
        mCurrentNode = mListHoveredNode;
      }

      if (mSceneHoveredNode != -1)
      {
        mCurrentNode = mSceneHoveredNode;
      }
    }

    // draw context menu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

    if (ImGui::BeginPopup("ContextMenu"))
    {
      Node *node = mCurrentNode != -1 ? mNodes[mCurrentNode] : nullptr;
      
      ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;

      if (node)
      {
        ImGui::Text("Node %s", node->GetName());
        ImGui::Separator();

        if (ImGui::MenuItem("Rename..", nullptr, false, false)) {}
        if (ImGui::MenuItem("Delete..", nullptr, false, false)) {}
        if (ImGui::MenuItem("Copy..", nullptr, false, false)) {}
      }
      else
      {
        Node *newNode = new Node(mNodes.size(), "New Node", scene_pos, ImColor(100, 200, 200), 2, 2);
        
        if (ImGui::MenuItem("Add"))
        {
          mNodes.push_back(newNode); 
        }

        if (ImGui::MenuItem("Paste", nullptr, false, false)) { }
      }

      ImGui::EndPopup();
    }

    ImGui::PopStyleVar();


    // Scrolling

    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
    {
      mScrolling = mScrolling - ImGui::GetIO().MouseDelta;
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();


    ImGui::End();
  }

}