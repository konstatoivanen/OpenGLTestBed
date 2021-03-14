#include "PrecompiledHeader.h"
#include "DebugEngine.h"
#include "Context/Structs/Implementers.h"
#include "Context/Structs/EntityViews.h"
#include "Rendering/MeshUtility.h"
#include "Core/Application.h"

static void CreateMeshRenderable(PKECS::EntityDatabase* entityDb, const float3& position, const float3& rotation, Ref<Mesh> mesh, Weak<Material> material)
{
	auto egid = PKECS::EGID(entityDb->ReserveEntityId(), (uint)PKECS::ENTITY_GROUPS::ACTIVE);
	auto implementer = entityDb->ResereveImplementer<PKECS::Implementers::MeshRenderableImplementer>();
	auto transformView = entityDb->ReserveEntityView<PKECS::EntityViews::TransformView>(egid);
	auto baseView = entityDb->ReserveEntityView<PKECS::EntityViews::BaseRenderable>(egid);
	auto meshView = entityDb->ReserveEntityView<PKECS::EntityViews::MeshRenderable>(egid);

	transformView->bounds = static_cast<PKECS::Components::Bounds*>(implementer);
	transformView->transform = static_cast<PKECS::Components::Transform*>(implementer);
	baseView->bounds = static_cast<PKECS::Components::Bounds*>(implementer);
	baseView->handle = static_cast<PKECS::Components::RenderableHandle*>(implementer);
	meshView->materials = static_cast<PKECS::Components::Materials*>(implementer);
	meshView->mesh = static_cast<PKECS::Components::MeshReference*>(implementer);
	meshView->transform = static_cast<PKECS::Components::Transform*>(implementer);

	implementer->localAABB = CGMath::CreateBoundsCenterExtents(CG_FLOAT3_ZERO, CG_FLOAT3_ONE);
	implementer->isCullable = true;
	implementer->isVisible = false;
	implementer->position = position;
	implementer->rotation = glm::quat(rotation);
	implementer->scale = CG_FLOAT3_ONE;
	implementer->sharedMaterials.push_back(material);
	implementer->sharedMesh = mesh;
	implementer->type = PKECS::Components::RenderHandleType::MeshRenderer;
	implementer->viewSize = 1.0f;
}

static void CreatePointLight(PKECS::EntityDatabase* entityDb, const float3& position, const color& color, float radius)
{
	auto egid = PKECS::EGID(entityDb->ReserveEntityId(), (uint)PKECS::ENTITY_GROUPS::ACTIVE);
	auto implementer = entityDb->ResereveImplementer<PKECS::Implementers::PointLightImplementer>();
	auto transformView = entityDb->ReserveEntityView<PKECS::EntityViews::TransformView>(egid);
	auto baseView = entityDb->ReserveEntityView<PKECS::EntityViews::BaseRenderable>(egid);
	auto lightView = entityDb->ReserveEntityView<PKECS::EntityViews::PointLightRenderable>(egid);

	transformView->bounds = static_cast<PKECS::Components::Bounds*>(implementer);
	transformView->transform = static_cast<PKECS::Components::Transform*>(implementer);
	baseView->bounds = static_cast<PKECS::Components::Bounds*>(implementer);
	baseView->handle = static_cast<PKECS::Components::RenderableHandle*>(implementer);
	lightView->pointLight = static_cast<PKECS::Components::PointLight*>(implementer);
	lightView->transform = static_cast<PKECS::Components::Transform*>(implementer);

	implementer->localAABB = CGMath::CreateBoundsCenterExtents(CG_FLOAT3_ZERO, CG_FLOAT3_ONE * 0.5f * radius);
	implementer->isCullable = true;
	implementer->isVisible = false;
	implementer->position = position;
	implementer->color = color;
	implementer->radius = radius;
	implementer->type = PKECS::Components::RenderHandleType::PointLight;
	implementer->viewSize = 1.0f;
}

DebugEngine::DebugEngine(AssetDatabase* assetDatabase, Time* time, PKECS::EntityDatabase* entityDb)
{
	m_entityDb = entityDb;
	m_assetDatabase = assetDatabase;
	m_time = time;

	//meshCube = MeshUtilities::GetBox(CG_FLOAT3_ZERO, { 10.0f, 0.5f, 10.0f });
	meshSphere = MeshUtilities::GetSphere(CG_FLOAT3_ZERO, 1.0f);
	materialMetal = assetDatabase->Find<Material>("M_Metal_Panel");
	materialGravel = assetDatabase->Find<Material>("M_Gravel");
	cornellBoxMaterial = assetDatabase->Find<Material>("M_Metal_Panel1");
	cornellBox = assetDatabase->Find<Mesh>("cornell_box");

	auto minpos = float3(-40, -10, -40);
	auto maxpos = float3(40, 10, 40);

	for (auto i = 0; i < 256; ++i)
	{
		CreateMeshRenderable(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), CGMath::RandomEuler(), meshSphere, materialMetal);
	}

	for (auto i = 0; i < 256; ++i)
	{
		CreateMeshRenderable(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), CGMath::RandomEuler(), meshSphere, materialGravel);
	}

	for (auto i = 0; i < 4; ++i)
	{
		CreatePointLight(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), CGMath::HueToRGB(CGMath::RandomFloat()) * CGMath::RandomRangeFloat(2.5f, 10.0f), CGMath::RandomRangeFloat(5, 40));
	}
}

DebugEngine::~DebugEngine()
{
	meshSphere = nullptr;
}

void DebugEngine::Step(Input* input)
{
	if (input->GetKeyDown(KeyCode::ESCAPE))
	{
		Application::Get().Close();
		return;
	}

	if (input->GetKeyDown(KeyCode::T))
	{
		materialMetal.lock()->GetShader().lock()->ListProperties();
	}

	if (input->GetKeyDown(KeyCode::R))
	{
		auto shader = materialMetal.lock()->GetShader();
		m_assetDatabase->Reload<Shader>(shader);
		PK_CORE_LOG("Reimported shader: %s", shader.lock()->GetFileName().c_str());
	}

	if (input->GetKey(KeyCode::C))
	{
		m_time->Reset();
	}

	m_time->LogFrameRate();
}

void DebugEngine::Step(int condition)
{
	auto lights = m_entityDb->Query<PKECS::EntityViews::PointLightRenderable>((int)PKECS::ENTITY_GROUPS::ACTIVE);
	auto time = Application::GetService<Time>()->GetTime();

	for (auto i = 0; i < lights.count; ++i)
	{
		lights[i].transform->position.y = sin(time + ((float)i / lights.count)) * 10;
	}

	auto meshes = m_entityDb->Query<PKECS::EntityViews::MeshRenderable>((int)PKECS::ENTITY_GROUPS::ACTIVE);

	for (auto i = 0; i < meshes.count; ++i)
	{
		meshes[i].transform->position.y = sin(time + (10 * (float)i / meshes.count)) * 10;
	}
}

void DebugEngine::Step(GizmoRenderer* gizmos)
{
	auto aspect = Application::GetWindow().GetAspect();
	auto proj = CGMath::GetPerspective(50.0f, aspect, 0.1f, 50.0f);
	auto view = CGMath::GetMatrixInvTRS(CG_FLOAT3_ZERO, glm::quat(float3(0, Application::GetService<Time>()->GetTime(), 0)), CG_FLOAT3_ONE);
	auto vp = proj * view;

	gizmos->SetColor(CG_COLOR_GREEN);
	gizmos->DrawFrustrum(vp);
	
	FrustrumPlanes frustrum;
	CGMath::ExtractFrustrumPlanes(vp, &frustrum, true);

	auto cullables = m_entityDb->Query<PKECS::EntityViews::BaseRenderable>((int)PKECS::ENTITY_GROUPS::ACTIVE);

	for (auto i = 0; i < cullables.count; ++i)
	{
		const auto& bounds = cullables[i].bounds->worldAABB;
		gizmos->SetColor(CGMath::IntersectPlanesAABB(frustrum.planes, 6, bounds) ? CG_COLOR_GREEN : CG_COLOR_BLACK);
		gizmos->DrawWireBounds(bounds);
	}
}
