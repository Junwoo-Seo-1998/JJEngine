/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "Scene.h"

#include "Time.h"
#include "box2d/box2d.h"
#include "Component/BoxCollider2DComponent.h"
#include "Component/CameraComponent.h"
#include "Component/RigidBody2DComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Entity/Entity.hpp"
#include "Component/TransformComponent.h"
#include "Graphics/Renderer/Renderer2D.h"
#include "Utils/Assert.h"
#include "Utils/UUIDGenerator.h"
#include "Utils/Math/MatrixMath.h"

static b2BodyType RigidBody2DTypeToBox2D(RigidBody2DComponent::BodyType bodyType)
{
	switch (bodyType)
	{
	case RigidBody2DComponent::BodyType::Static: return b2_staticBody;
	case RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
	case RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
	}
	ENGINE_ASSERT(false, "Shouldn't be here");
	return b2_staticBody;
}

Scene::Scene()
	:m_scene_name("unnamed-scene")
{
}

Scene::Scene(const std::string& scene_name)
	:m_scene_name(scene_name)
{
}

Scene::~Scene()
{
}

void Scene::Awake()
{
}

void Scene::OnEnable()
{
}

void Scene::Start()
{
}

void Scene::Update()
{
}

void Scene::PostUpdate()
{
}

void Scene::OnDisable()
{
}

void Scene::OnDestroy()
{
}

void Scene::UpdateEditor(EditorCamera& camera)
{
	Renderer2D::BeginScene(camera);
	auto view = m_Registry.view<SpriteRendererComponent>();
	for (auto e : view)
	{
		Entity entity{ e, this };
		auto& spriteComp = entity.GetComponent<SpriteRendererComponent>();
		if (spriteComp.texture)
			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), spriteComp.texture, spriteComp.color);
		else
			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), spriteComp.color);
	}
	Renderer2D::EndScene();
}

void Scene::StartRuntime()
{
	m_2DWorld = std::make_unique<b2World>(b2Vec2{ 0.f, -9.8f });
	auto view = m_Registry.view<RigidBody2DComponent>();
	for (auto e : view)
	{
		Entity entity{ e, this };
		auto& trasform = entity.Transform();
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

		b2BodyDef body_def;
		body_def.type = RigidBody2DTypeToBox2D(rb2d.Type);
		body_def.position.Set(trasform.Position.x, trasform.Position.y);
		body_def.angle = trasform.Rotation.z;

		b2Body* body = m_2DWorld->CreateBody(&body_def);
		body->SetFixedRotation(rb2d.FixedRotation);
		rb2d.RuntimeBody = body;

		if(entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

			b2PolygonShape polygon_shape;
			polygon_shape.SetAsBox(trasform.Scale.x * bc2d.Size.x, trasform.Scale.y * bc2d.Size.y);

			b2FixtureDef fixture_def;
			fixture_def.shape = &polygon_shape;
			fixture_def.density = bc2d.Density;
			fixture_def.friction = bc2d.Friction;
			fixture_def.restitution = bc2d.Restitution;
			fixture_def.restitutionThreshold = bc2d.RestitutionThreshold;
			bc2d.RuntimeFixture = body->CreateFixture(&fixture_def);
		}
	}
}

void Scene::UpdateRuntime()
{

	{
		constexpr int velocityIters = 6;
		constexpr int positionIters = 2;
		m_2DWorld->Step(Time::GetDelta(), velocityIters, positionIters);
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view)
		{
			Entity entity{ e, this };
			auto& trasform = entity.Transform();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			const auto& position = body->GetPosition();
			trasform.Position.x = position.x;
			trasform.Position.y = position.y;
			trasform.Rotation.z = body->GetAngle();

		}
	}

	Entity camera = GetMainCameraEntity();
	if(!camera)
	{
		Log::Warn("There is no Camera In This Scene!");
		return;
	}

	{
		auto& camTransform = camera.Transform();
		auto& camComp = camera.GetComponent<CameraComponent>();
		glm::mat4 viewProj = camComp.GetMatrix() *
			MatrixMath::BuildCameraMatrixWithDirection(camTransform.Position, camTransform.GetForward(), camTransform.GetUp());
		Renderer2D::BeginScene(viewProj);
		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto e : group)
		{
			Entity entity{ e, this };
			auto& spriteComp = entity.GetComponent<SpriteRendererComponent>();
			if (spriteComp.texture)
				Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), spriteComp.texture, spriteComp.color);
			else
				Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), spriteComp.color);
		}

		Renderer2D::EndScene();
	}
}

void Scene::StopRuntime()
{
	m_2DWorld.release();
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity{ m_Registry.create(),this };

	auto& uuidComponent = entity.AddComponent<UUIDComponent>();
	uuidComponent.UUID = UUIDGenerator::Generate();

	entity.AddComponent<TransformComponent>();
	if(!name.empty())
		entity.AddComponent<NameComponent>(name);

	entity.AddComponent<RelationshipComponent>();
	m_entity_map[uuidComponent.UUID] = entity;

	//for editor view
	SortEntityMap();

	return entity;
}

Entity Scene::CreateEntityWithUUID(UUIDType uuid, const std::string& name, bool sort)
{
	auto entity = Entity{ m_Registry.create(), this };

	auto& uuidComponent = entity.AddComponent<UUIDComponent>();
	uuidComponent.UUID = uuid;

	entity.AddComponent<TransformComponent>();
	if (!name.empty())
		entity.AddComponent<NameComponent>(name);

	entity.AddComponent<RelationshipComponent>();
	ENGINE_ASSERT(m_entity_map.find(uuidComponent.UUID) == m_entity_map.end(), "There is an Entity already with same uuid");
	m_entity_map[uuidComponent.UUID] = entity;

	//for editor view
	if (sort)
		SortEntityMap();

	return entity;
}

Entity Scene::GetEntity(UUIDType uuid) const
{
	ENGINE_ASSERT(m_entity_map.find(uuid) != m_entity_map.end(), "There is no entity with given UUID");
	return m_entity_map.at(uuid);
}

Entity Scene::TryGetEntity(UUIDType uuid) const
{
	if(const auto found = m_entity_map.find(uuid); found != m_entity_map.end())
		return found->second;
	return Entity{};
}

Entity Scene::TryGetEntity(const std::string& name)
{
	const auto entities = m_Registry.view<NameComponent>();
	for(auto entity : entities)
	{
		if (entities.get<NameComponent>(entity).Name == name)
			return { entity, this };
	}
	return {};
}

Entity Scene::GetMainCameraEntity()
{
	auto view = m_Registry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto& comp = view.get<CameraComponent>(entity);
		if (comp.IsMainCamera)
		{
			return { entity, this };
		}
	}
	return {};
}

std::string Scene::GetSceneName() const {
	return m_scene_name;
}

std::filesystem::path Scene::GetScenePath() const {
	return scenePath;
}
void Scene::SetScenePath(std::filesystem::path path) {
	scenePath = path;
}

entt::registry& Scene::GetRegistry()
{
	return m_Registry;
}

const EntityMap& Scene::GetEntityMap() const
{
	return m_entity_map;
}

void Scene::ResizeViewport(unsigned width, unsigned height)
{
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	//resize non fixed aspect ratio
	auto view = m_Registry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto& cameraComponent = view.get<CameraComponent>(entity);
		cameraComponent.Aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	}
}

void Scene::SortEntityMap()
{
	m_Registry.sort<UUIDComponent>([&](const auto left, const auto right)
	{
		auto leftEntity = m_entity_map.find(left.UUID);
		auto rightEntity = m_entity_map.find(right.UUID);
		return leftEntity->second < rightEntity->second;
	});
}
