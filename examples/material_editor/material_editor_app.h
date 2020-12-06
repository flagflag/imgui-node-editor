#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <imgui_node_editor.h>

#include "utilities/builders.h"
#include "utilities/widgets.h"

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

enum class PinType
{
	Flow,
	Bool,
	Int,
	Float,
	String,
	Object,
	Function,
	Delegate,
};

enum class PinKind
{
	Output,
	Input
};

enum class NodeType
{
	Blueprint,
	Simple,
	Tree,
	Comment,
	Houdini
};

struct Node;

struct Pin
{
	ed::PinId   ID;
	::Node* Node;
	std::string Name;
	PinType     Type;
	PinKind     Kind;

	Pin(int id, const char* name, PinType type) :
		ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
	{
	}
};

struct Node
{
	ed::NodeId ID;
	std::string Name;
	std::vector<Pin> Inputs;
	std::vector<Pin> Outputs;
	ImColor Color;
	NodeType Type;
	ImVec2 Size;

	std::string State;
	std::string SavedState;

	Node(int id, const char* name, ImColor color = ImColor(255, 255, 255)) :
		ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
	{
	}
};

struct Link
{
	ed::LinkId ID;

	ed::PinId StartPinID;
	ed::PinId EndPinID;

	ImColor Color;

	Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId) :
		ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
	{
	}
};

struct NodeIdLess
{
	bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
	{
		return lhs.AsPointer() < rhs.AsPointer();
	}
};

class GlobalInterface
{
public:
	virtual ImTextureID LoadTexture(const char* path) = 0;

	virtual void DestroyTexture(ImTextureID id) = 0;

	virtual int GetTextureWidth(ImTextureID id) = 0;

	virtual int GetTextureHeight(ImTextureID id) = 0;
};

class MaterialEditorApp
{
public:
	explicit MaterialEditorApp(GlobalInterface* globalInterface);

	const char* GetName() const { return "MaterialEditor"; }

	void Initialize();

	void Finalize();

	void RunFrame();

protected:
	int GetNextId();

	ed::LinkId GetNextLinkId();

	void TouchNode(ed::NodeId id);

	float GetTouchProgress(ed::NodeId id);

	void UpdateTouch();

	Node* FindNode(ed::NodeId id);

	Link* FindLink(ed::LinkId id);

	Pin* FindPin(ed::PinId id);

	bool IsPinLinked(ed::PinId id);

	bool CanCreateLink(Pin* a, Pin* b);


// Spawn Node
	void BuildNode(Node* node);

	Node* SpawnInputActionNode();

	Node* SpawnBranchNode();

	Node* SpawnDoNNode();

	Node* SpawnOutputActionNode();

	Node* SpawnPrintStringNode();

	Node* SpawnMessageNode();

	Node* SpawnSetTimerNode();

	Node* SpawnLessNode();

	Node* SpawnWeirdNode();

	Node* SpawnTraceByChannelNode();

	Node* SpawnTreeSequenceNode();

	Node* SpawnTreeTaskNode();

	Node* SpawnTreeTask2Node();

	Node* SpawnComment();

	Node* SpawnHoudiniTransformNode();

	Node* SpawnHoudiniGroupNode();

	void BuildNodes();

//
	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);

	ImColor GetIconColor(PinType type);

	void DrawPinIcon(const Pin& pin, bool connected, int alpha);

	void ShowStyleEditor(bool* show = nullptr);

	void ShowLeftPane(float paneWidth);


// Render
	void RenderNodes();

	void RenderNodes_BlueprintAndSimple(util::BlueprintNodeBuilder& builder);

	void RenderNodes_Tree(util::BlueprintNodeBuilder& builder);

	void RenderNodes_Houdini(util::BlueprintNodeBuilder& builder);

	void RenderNodes_Comment(util::BlueprintNodeBuilder& builder);

	void RenderNodes_LinkAction();

	void RenderContextMenu();

private:
	GlobalInterface* globalInterface_;

	ed::EditorContext* m_Editor;

	const int            pinIconSize_ = 24;
	std::vector<Node>    nodes_;
	std::vector<Link>    links_;
	ImTextureID          headerBackground_ = nullptr;
	//static ImTextureID          s_SampleImage = nullptr;
	ImTextureID          saveIcon_ = nullptr;
	ImTextureID          restoreIcon_ = nullptr;

	const float          touchTime_ = 1.0f;
	std::map<ed::NodeId, float, NodeIdLess> nodeTouchTime_;

	int nextId_ = 1;

//
	ed::NodeId contextNodeId_ = 0;
	ed::LinkId contextLinkId_ = 0;
	ed::PinId  contextPinId_ = 0;
	bool createNewNode_ = false;
	Pin* newNodeLinkPin_ = nullptr;
	Pin* newLinkPin_ = nullptr;

	float leftPaneWidth_ = 400.0f;
	float rightPaneWidth_ = 800.0f;
};
