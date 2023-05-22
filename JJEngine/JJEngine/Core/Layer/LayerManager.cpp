#include "LayerManager.h"
#include "Layer.h"
LayerManager::~LayerManager()
{
}

void LayerManager::PushLayer(std::shared_ptr<Layer> layer)
{
	m_Layers.emplace_back(layer);
	layer->OnAttach();
}

void LayerManager::PushOverlay(std::shared_ptr<Layer> overlay)
{
	m_Overlays.emplace_back(overlay);
	overlay->OnAttach();
}

void LayerManager::PopLayer(std::shared_ptr<Layer> layer)
{
	m_LayerDeleteQueue.push(layer);
	layer->OnDetach();
}

void LayerManager::PopOverlay(std::shared_ptr<Layer> overlay)
{
	m_OverlayDeleteQueue.push(overlay);
	overlay->OnDetach();
}

void LayerManager::ClearDeleteQueue()
{
	while (m_LayerDeleteQueue.empty() == false)
	{
		std::shared_ptr<Layer> layer = m_LayerDeleteQueue.front();
		m_LayerDeleteQueue.pop();

		PopLayerDirect(layer);
	}
	while (m_OverlayDeleteQueue.empty() == false)
	{
		std::shared_ptr<Layer> layer = m_OverlayDeleteQueue.front();
		m_OverlayDeleteQueue.pop();

		PopOverlayDirect(layer);
	}
}

void LayerManager::PopLayerDirect(std::shared_ptr<Layer> layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		layer->OnDetach();
		m_Layers.erase(it);
	}
}

void LayerManager::PopOverlayDirect(std::shared_ptr<Layer> overlay)
{
	auto it = std::find(m_Overlays.begin(), m_Overlays.end(), overlay);
	if (it != m_Overlays.end())
	{
		overlay->OnDetach();
		m_Overlays.erase(it);
	}
}