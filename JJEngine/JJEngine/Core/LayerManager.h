/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 01/03/2023
End Header-------------------------------------------------------- */
#pragma once
#include <string>
#include <memory>
#include <vector>

//temp class
class Layer {
public:
	Layer() = default;
};
class LayerManager {
	struct NamedLayer{
		double value{};
		std::string name{};
		std::shared_ptr<Layer> layerPtr{};
	};
	std::vector<NamedLayer> sortedLayers{};
public:
	const std::string GetOrders() const;
	const int GetOrder(const std::string)const;
	const std::string GetLayerName(const int order)const;
	
	void PushLayer(int order, std::string layerName, std::shared_ptr<Layer> layer); // sorting
	void PopLayer(std::string layerName);
	void PopLayer(int order);

	void OverrideLayer(std::string layerName); // layer overriding

};
