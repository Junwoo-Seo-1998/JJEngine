#include "EditorLayer.h"

#include "Core/Utils/Log.h"

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	Log::Info("Editor Layer Added");
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate()
{
	//get something from scene
}

void EditorLayer::OnImGuiRender()
{

}
