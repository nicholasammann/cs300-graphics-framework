#pragma once

#include <string>

#include "../ImGui/imgui.h"


namespace elba
{

  class Node
  {
  public:
    Node(int aID, std::string aName, int aInputCount, int aOutputCount);

    ImVec2 GetInputSlotPos(int aSlotNum);
    ImVec2 GetOutputSlotPos(int aSlotNum);

    int GetID() const;
    std::string GetName() const;

    int mID;

    std::string mName;

    ImVec2 mPos;
    ImVec2 mSize;

    int mNumInputs;
    int mNumOutputs;

  };


  class NodeLink
  {
  public:
    NodeLink(int aInputIndex, int aInputSlot, int aOutputIndex, int aOutputSlot);

    int mInputIndex;
    int mInputSlot;

    int mOutputIndex;
    int mOutputSlot;

  };

}