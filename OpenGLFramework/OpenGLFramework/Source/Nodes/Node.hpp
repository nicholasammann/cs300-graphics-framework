#pragma once

#include <string>

#include "../ImGui/imgui.h"


namespace elba
{

  class Node
  {
  public:
    Node(int aID, std::string aName, ImVec2 aPos, ImVec4 aColor, int aInputCount, int aOutputCount)
      : mID(aID), mName(aName), mPos(aPos), mColor(aColor), 
        mNumInputs(aInputCount), mNumOutputs(aOutputCount)
    {

    }

    ImVec2 GetInputSlotPos(int aSlotNum)
    {
      ImVec2 pos(mPos.x, mPos.y + mSize.y * ((float(aSlotNum + 0.5f) / (float)mNumInputs + 0.5f)));
      return pos;
    }

    ImVec2 GetOutputSlotPos(int aSlotNum)
    {
      ImVec2 pos(mPos.x + mSize.x, mPos.y + mSize.y * ((float(aSlotNum + 0.5f) / (float)mNumOutputs + 0.5f)));
      return pos;
    }

    int GetID() const
    {
      return mID;
    }

    std::string GetName() const
    {
      return mName;
    }

    int mID;

    std::string mName;

    ImVec2 mPos;
    ImVec2 mSize;

    int mNumInputs;
    int mNumOutputs;

    ImVec4 mColor;

  };


  class NodeLink
  {
  public:
    NodeLink(int aInputIndex, int aInputSlot, int aOutputIndex, int aOutputSlot)
      : mInputIndex(aInputIndex), mInputSlot(aInputSlot),
        mOutputIndex(aOutputIndex), mOutputSlot(aOutputSlot)
    {

    }

    int mInputIndex;
    int mInputSlot;

    int mOutputIndex;
    int mOutputSlot;

  };

}