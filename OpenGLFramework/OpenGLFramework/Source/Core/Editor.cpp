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

  Editor::Editor(Application *aApp) : mApp(aApp), mWindowFlags(0)
  {
    mWindowFlags = ImGuiWindowFlags_NoTitleBar;
    mWindowFlags |= ImGuiWindowFlags_NoResize;
    mWindowFlags |= ImGuiWindowFlags_NoMove;


    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

    ImGui::SetNextWindowSize(
      ImVec2(mApp->GetWindowWidth(), mApp->GetWindowHeight()),
      ImGuiCond_Once);
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

      if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
      {
        mListHoveredNode = node->GetID();
        mOpenContextMenu = true;

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
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(60, 60, 70, 200));

    int sceneFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;

    ImGui::BeginChild("Scene", ImVec2(0, 0), true, sceneFlags);

    ImGui::PushItemWidth(120.0f);

    ImVec2 offset = ImGui::GetCursorScreenPos() - mScrolling;

    ImDrawList *drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(2);

    // draw grid
    if (mShowGrid)
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

      ImVec2 a = offset + input->GetOutputSlotPos(link->mOutputSlot);
      ImVec2 b = offset + input->GetInputSlotPos(link->mInputSlot);

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
      ImGui::Text("%s", node->GetName());
      ImGui::EndGroup();


      // save the size of what we have emitted and 
      // whether any of the widgets are being used



    }



    ImGui::EndChild();







    ImGui::End();
  }

}