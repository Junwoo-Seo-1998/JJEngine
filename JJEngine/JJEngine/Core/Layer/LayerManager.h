#pragma once
#include <memory>
#include <queue>
class Application;
class Layer;

class LayerManager
{
	friend Application;
public:
	LayerManager() = default;
	~LayerManager();

	void PushLayer(std::shared_ptr<Layer> layer);
	void PushOverlay(std::shared_ptr<Layer> overlay);
	void PopLayer(std::shared_ptr<Layer> layer);
	void PopOverlay(std::shared_ptr<Layer> overlay);



	std::vector<std::shared_ptr<Layer>>& GetLayers() { return m_Layers; }
	std::vector<std::shared_ptr<Layer>>& GetOverLays() { return m_Overlays; }
private:
	void ClearDeleteQueue();
	void PopLayerDirect(std::shared_ptr<Layer> layer);
	void PopOverlayDirect(std::shared_ptr<Layer> overlay);
private:
	std::vector<std::shared_ptr<Layer>> m_Layers;
	std::vector<std::shared_ptr<Layer>> m_Overlays;


	std::queue< std::shared_ptr<Layer>> m_LayerDeleteQueue;
	std::queue< std::shared_ptr<Layer>> m_OverlayDeleteQueue;
};